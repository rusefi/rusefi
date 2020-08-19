package com.rusefi.io.tcp;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.CompatibleFunction;
import com.rusefi.Listener;
import com.rusefi.NamedThreadFactory;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.*;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.server.rusEFISSLContext;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Fields.*;

/**
 * This class makes rusEfi console a proxy for other tuning software, this way we can have two tools connected via same
 * serial port simultaneously
 *
 * See BinaryProtocolServerSandbox
 * @author Andrey Belomutskiy
 * 11/24/15
 */

public class BinaryProtocolServer implements BinaryProtocolCommands {
    public static final String TEST_FILE = "test_log.mlg.Z";
    private static final Logging log = getLogging(BinaryProtocolServer.class);
    private static final int DEFAULT_PROXY_PORT = 2390;
    public static final String TS_OK = "\0";

    private final static boolean MOCK_SD_CARD = true;
    private static final int SD_STATUS_OFFSET = 246;
    private static final int FAST_TRANSFER_PACKET_SIZE = 2048;

    public AtomicInteger unknownCommands = new AtomicInteger();

    public static final ServerSocketFunction SECURE_SOCKET_FACTORY = rusEFISSLContext::getSSLServerSocket;

    public static final ServerSocketFunction PLAIN_SOCKET_FACTORY = port -> {
        ServerSocket serverSocket = new ServerSocket(port);
        log.info("ServerSocket " + port + " created");
        return serverSocket;
    };

    private static ConcurrentHashMap<String, ThreadFactory> THREAD_FACTORIES_BY_NAME = new ConcurrentHashMap<>();

    public void start(LinkManager linkManager) {
        try {
            start(linkManager, DEFAULT_PROXY_PORT, Listener.empty(), new Context());
        } catch (IOException e) {
            log.error("Error starting local proxy", e);
        }
    }

    public void start(LinkManager linkManager, int port, Listener serverSocketCreationCallback, Context context) throws IOException {
        log.info("BinaryProtocolServer on " + port);

        CompatibleFunction<Socket, Runnable> clientSocketRunnableFactory = clientSocket -> () -> {
            try {
                runProxy(linkManager, clientSocket, context);
            } catch (IOException e) {
                log.info("proxy connection: " + e);
            }
        };

        tcpServerSocket(port, "BinaryProtocolServer", clientSocketRunnableFactory, serverSocketCreationCallback);
    }

    /**
     * Starts a new thread
     *
     * @param port                         server port to accept connections
     * @param threadName
     * @param socketRunnableFactory        method to invoke on a new thread for each new client connection
     * @param serverSocketCreationCallback this callback is invoked once we open the server socket
     * @return
     */
    public static ServerSocketReference tcpServerSocket(int port, String threadName, CompatibleFunction<Socket, Runnable> socketRunnableFactory, Listener serverSocketCreationCallback) throws IOException {
        return tcpServerSocket(socketRunnableFactory, port, threadName, serverSocketCreationCallback, PLAIN_SOCKET_FACTORY);
    }

    public static ServerSocketReference tcpServerSocket(CompatibleFunction<Socket, Runnable> clientSocketRunnableFactory, int port, String threadName, Listener serverSocketCreationCallback, ServerSocketFunction nonSecureSocketFunction) throws IOException {
        ThreadFactory threadFactory = getThreadFactory(threadName);

        Objects.requireNonNull(serverSocketCreationCallback, "serverSocketCreationCallback");
        ServerSocket serverSocket = nonSecureSocketFunction.apply(port);

        ServerSocketReference holder = new ServerSocketReference(serverSocket);

        serverSocketCreationCallback.onResult(null);
        Runnable runnable = () -> {
            while (!holder.isClosed()) {
                // Wait for a connection
                final Socket clientSocket;
                try {
                    clientSocket = serverSocket.accept();
                } catch (IOException e) {
                    log.info("Client socket closed right away " + e);
                    continue;
                }
                log.info("Accepting binary protocol proxy port connection on " + port);
                threadFactory.newThread(clientSocketRunnableFactory.apply(clientSocket)).start();
            }
        };
        threadFactory.newThread(runnable).start();
        return holder;
    }

    @NotNull
    public static ThreadFactory getThreadFactory(String threadName) {
        synchronized (THREAD_FACTORIES_BY_NAME) {
            ThreadFactory threadFactory = THREAD_FACTORIES_BY_NAME.get(threadName);
            if (threadFactory == null) {
                threadFactory = new NamedThreadFactory(threadName);
                THREAD_FACTORIES_BY_NAME.put(threadName, threadFactory);
            }
            return threadFactory;
        }
    }

    @SuppressWarnings("InfiniteLoopStatement")
    private void runProxy(LinkManager linkManager, Socket clientSocket, Context context) throws IOException {
        TcpIoStream stream = new TcpIoStream("[proxy] ", clientSocket);

        IncomingDataBuffer in = stream.getDataBuffer();

        while (true) {
            AtomicBoolean handled = new AtomicBoolean();
            Handler protocolCommandHandler = () -> {
                handleProtocolCommand(clientSocket);
                handled.set(true);
            };

            int length = getPacketLength(in, protocolCommandHandler, context.getTimeout());
            if (handled.get()) {
                continue;
            }

            if (log.debugEnabled())
                log.debug("Got [" + length + "] length promise");

            Packet packet = readPromisedBytes(in, length);
            byte[] payload = packet.getPacket();

            if (payload.length == 0)
                throw new IOException("Empty packet");

            byte command = payload[0];

            log.info("Got command " + BinaryProtocol.findCommand(command));

            if (command == Fields.TS_HELLO_COMMAND) {
                new HelloCommand(Fields.TS_SIGNATURE).handle(stream);
            } else if (command == COMMAND_PROTOCOL) {
                stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes());
            } else if (command == Fields.TS_GET_FIRMWARE_VERSION) {
                stream.sendPacket((TS_OK + "rusEFI proxy").getBytes());
            } else if (command == COMMAND_CRC_CHECK_COMMAND) {
                handleCrc(linkManager, stream);
            } else if (command == Fields.TS_PAGE_COMMAND) {
                stream.sendPacket(TS_OK.getBytes());
            } else if (command == Fields.TS_READ_COMMAND) {
                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
                handleRead(linkManager, dis, stream);
            } else if (command == Fields.TS_CHUNK_WRITE_COMMAND) {
                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
                handleWrite(linkManager, payload, dis, stream);
            } else if (command == Fields.TS_BURN_COMMAND) {
                stream.sendPacket(new byte[]{TS_RESPONSE_BURN_OK});
            } else if (command == Fields.TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY) {
                System.err.println("NOT IMPLEMENTED TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY relay");
                // todo: relay command
                stream.sendPacket(TS_OK.getBytes());
            } else if (command == TS_SD_R_COMMAND) {
                handleSD_R_command(stream, packet, payload);
            } else if (command == TS_SD_W_COMMAND) {
                handleSD_W_command(stream, packet, payload);
            } else if (command == Fields.TS_OUTPUT_COMMAND) {
                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
                int offset = swap16(dis.readShort());
                int count = swap16(dis.readShort());
                log.info("TS_OUTPUT_COMMAND offset=" + offset + "/count=" + count);

                byte[] response = new byte[1 + count];
                response[0] = (byte) TS_OK.charAt(0);
                BinaryProtocolState binaryProtocolState = linkManager.getBinaryProtocolState();
                byte[] currentOutputs = binaryProtocolState.getCurrentOutputs();
                if (MOCK_SD_CARD)
                    currentOutputs[SD_STATUS_OFFSET] = 1 + 4;
                if (currentOutputs != null)
                    System.arraycopy(currentOutputs, offset, response, 1, count);
                stream.sendPacket(response);
            } else if (command == Fields.TS_GET_TEXT) {
                // todo: relay command
                System.err.println("NOT IMPLEMENTED TS_GET_TEXT relay");
                stream.sendPacket(TS_OK.getBytes());
            } else {
                unknownCommands.incrementAndGet();
                new IllegalStateException().printStackTrace();
                log.info("Error: unexpected " + BinaryProtocol.findCommand(command));
            }
        }
    }

    private void handleSD_W_command(TcpIoStream stream, Packet packet, byte[] payload) throws IOException {
        log.info("TS_SD: 'w' " + IoStream.printHexBinary(packet.packet));
        if (payload[1] == 0 && payload[2] == TS_SD_PROTOCOL_FETCH_INFO) {

            if (payload[6] == TS_SD_PROTOCOL_DO) {
                log.info("TS_SD: do command, command=" + payload[payload.length - 1]);
                sendOkResponse(stream);
            } else if (payload[6] == TS_SD_PROTOCOL_READ_DIR) {
                log.info("TS_SD: read directory command " + payload[payload.length - 1]);
                sendOkResponse(stream);
            } else if (payload[6] == TS_SD_PROTOCOL_REMOVE_FILE) {
                String pattern = new String(payload, 7, 4);
                log.info("TS_SD: remove file command " + Arrays.toString(packet.packet) + " " + pattern);

                sendOkResponse(stream);
            } else if (payload[6] == TS_SD_PROTOCOL_FETCH_COMPRESSED) {
                log.info("TS_SD: read compressed file command " + Arrays.toString(packet.packet));
                ByteBuffer bb = ByteBuffer.wrap(payload, 7, 8);
                bb.order(ByteOrder.BIG_ENDIAN);
                int sectorNumber = bb.getInt();
                int sectorCount = bb.getInt();
                log.info("TS_SD: sectorNumber=" + sectorNumber + ", sectorCount=" + sectorCount);
                sendOkResponse(stream);
            } else {
                log.info("TS_SD: Got unexpected w fetch " + IoStream.printHexBinary(packet.packet));
            }
        } else {
            log.info("TS_SD: Got unexpected w " + IoStream.printHexBinary(packet.packet));
        }
    }

    private void handleSD_R_command(TcpIoStream stream, Packet packet, byte[] payload) throws IOException {
        log.info("TS_SD: 'r' " + IoStream.printHexBinary(packet.packet));
        if (payload[1] == 0 && payload[2] == TS_SD_PROTOCOL_RTC) {
            log.info("TS_SD: RTC read command");
            byte[] response = new byte[9];
            stream.sendPacket(response);
        } else if (payload[1] == 0 && payload[2] == TS_SD_PROTOCOL_FETCH_INFO) {
            ByteBuffer bb = ByteBuffer.wrap(payload, 5, 2);
            bb.order(ByteOrder.BIG_ENDIAN);
            int bufferLength = bb.getShort();
            log.info("TS_SD: fetch buffer command, length=" + bufferLength);

            byte[] response = new byte[1 + bufferLength];

            response[0] = TS_RESPONSE_OK;

            if (bufferLength == 16) {
                response[1] = 1 + 4; // Card present + Ready
                response[2] = 0; // Y - error code

                response[3] = 2; // higher byte of '512' sector size
                response[4] = 0; // lower byte

                response[5] = 0;
                response[6] = 0x20; // 0x20 00 00 of 512 is 1G virtual card
                response[7] = 0;
                response[8] = 0;

                response[9] = 0;
                response[10] = 1; // number of files
            } else if (bufferLength == 0x202){
                // SD read directory command
                //

                setFileEntry(response, 0, "hello123mlq", (int) new File(TEST_FILE).length());
                setFileEntry(response, 1, "he      mlq", 1024);
                setFileEntry(response, 2, "_333o123mlq", 1000000);

            } else {
                log.info("TS_SD: Got unexpected r fetch " + IoStream.printHexBinary(packet.packet));
                return;
            }
            log.info("TS_SD: sending " + IoStream.printHexBinary(response));
            stream.sendPacket(response);
        } else if (payload[1] == 0 && payload[2] == TS_SD_PROTOCOL_FETCH_DATA) {
            ByteBuffer bb = ByteBuffer.wrap(payload, 3, 4);
            bb.order(ByteOrder.BIG_ENDIAN);
            int blockNumber = bb.getShort();
            int suffix = bb.getShort();
            log.info("TS_SD: fetch data command blockNumber=" + blockNumber + ", requesting=" + suffix);



            File f = new File(BinaryProtocolServer.TEST_FILE);
            FileInputStream fis = new FileInputStream(f);
            int size = (int) f.length();


            int offset = blockNumber * FAST_TRANSFER_PACKET_SIZE;
            int len = Math.max(0, Math.min(size - offset, FAST_TRANSFER_PACKET_SIZE));

            byte[] response = new byte[1 + 2 + len];
            response[0] = TS_RESPONSE_OK;
            response[1] = payload[3];
            response[2] = payload[4];

            if (len > 0) {
                fis.skip(offset);
                log.info("TS_SD reading " + offset + " " + len + " of " + size);
                fis.read(response, 3, len);
            }

            stream.sendPacket(response);
        } else {
            log.info("TS_SD: Got unexpected r " + IoStream.printHexBinary(packet.packet));
        }
    }

    private static void setFileEntry(byte[] response, int index, String fileName, int fileSize) {
        int offset = 1 + 32 * index;
        System.arraycopy(fileName.getBytes(), 0, response, offset, 11);
        response[offset + 11] = 1; // file
        //  12-15 = undefined

        response[offset + 14] = 0x11;
        response[offset + 15] = 0x13; // time

        response[offset + 16] = 0x24;
        response[offset + 17] = 0x25; // 0x2425 = FAT16 date format September 4, 1998

        for (int i = 18; i < 22; i++)
            response[offset + i] = (byte) (i + 10 * index); // sector number

        for (int i = 24; i < 28; i++) {
            response[offset + i] = (byte) (i + index);
        }

        IoHelper.putInt(response, offset + 28, IoHelper.swap32(fileSize));
    }

    private static void sendOkResponse(TcpIoStream stream) throws IOException {
        byte[] response = new byte[1];
        response[0] = TS_RESPONSE_OK;
        stream.sendPacket(response);
    }

    public static int getPacketLength(IncomingDataBuffer in, Handler protocolCommandHandler) throws IOException {
        return getPacketLength(in, protocolCommandHandler, Timeouts.BINARY_IO_TIMEOUT);
    }

    public static int getPacketLength(IncomingDataBuffer in, Handler protocolCommandHandler, int ioTimeout) throws IOException {
        byte first = in.readByte(ioTimeout);
        if (first == COMMAND_PROTOCOL) {
            protocolCommandHandler.handle();
            return 0;
        }
        return first * 256 + in.readByte(ioTimeout);
    }

    public static Packet readPromisedBytes(DataInputStream in, int length) throws IOException {
        if (length < 0)
            throw new IllegalArgumentException(String.format("Negative %d %x", length, length));
        byte[] packet = new byte[length];
        int size = in.read(packet);
        if (size != packet.length)
            throw new IOException(size + " promised but " + packet.length + " arrived");
        int crc = in.readInt();
        if (crc != IoHelper.getCrc32(packet))
            throw new IOException("CRC mismatch");
        return new Packet(packet, crc);
    }

    public static Packet readPromisedBytes(IncomingDataBuffer in, int length) throws IOException {
        if (length <= 0)
            throw new IOException("Unexpected packed length " + length);
        byte[] packet = new byte[length];
        in.read(packet);
        int crc = in.readInt();
        int fromPacket = IoHelper.getCrc32(packet);
        if (crc != fromPacket)
            throw new IOException("CRC mismatch crc=" + Integer.toString(crc, 16) + " vs packet=" + Integer.toString(fromPacket, 16) + " len=" + packet.length + " data: " + IoStream.printHexBinary(packet));
        in.onPacketArrived();
        return new Packet(packet, crc);
    }

    public interface Handler {
        void handle() throws IOException;
    }

    public static void handleProtocolCommand(Socket clientSocket) throws IOException {
        log.info("Got plain F command");
        OutputStream outputStream = clientSocket.getOutputStream();
        outputStream.write(TS_PROTOCOL.getBytes());
        outputStream.flush();
    }

    private void handleWrite(LinkManager linkManager, byte[] packet, DataInputStream dis, TcpIoStream stream) throws IOException {
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        log.info("TS_CHUNK_WRITE_COMMAND: offset=" + offset + " count=" + count);
        BinaryProtocolState bp = linkManager.getBinaryProtocolState();
        bp.setRange(packet, 7, offset, count);
        stream.sendPacket(TS_OK.getBytes());
    }

    private void handleRead(LinkManager linkManager, DataInputStream dis, TcpIoStream stream) throws IOException {
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        if (count <= 0) {
            log.info("Error: negative read request " + offset + "/" + count);
        } else {
            if (log.debugEnabled())
                log.debug("read " + offset + "/" + count);
            BinaryProtocolState bp = linkManager.getBinaryProtocolState();
            byte[] response = new byte[1 + count];
            response[0] = (byte) TS_OK.charAt(0);
            Objects.requireNonNull(bp, "bp");
            ConfigurationImage configurationImage = bp.getControllerConfiguration();
            Objects.requireNonNull(configurationImage, "configurationImage");
            System.arraycopy(configurationImage.getContent(), offset, response, 1, count);
            stream.sendPacket(response);
        }
    }

    private void handleCrc(LinkManager linkManager, TcpIoStream stream) throws IOException {
        log.info("CRC check");
        BinaryProtocolState bp = linkManager.getBinaryProtocolState();
        byte[] content = bp.getControllerConfiguration().getContent();
        int result = IoHelper.getCrc32(content);
        ByteArrayOutputStream response = new ByteArrayOutputStream();
        response.write(TS_OK.charAt(0));
        new DataOutputStream(response).writeInt(result);
        stream.sendPacket(response.toByteArray());
    }

    public static class Packet {
        private final byte[] packet;
        private final int crc;

        public Packet(byte[] packet, int crc) {
            this.packet = packet;
            this.crc = crc;
        }

        public byte[] getPacket() {
            return packet;
        }

        public int getCrc() {
            return crc;
        }
    }

    public static class Context {
        public int getTimeout() {
            return Timeouts.BINARY_IO_TIMEOUT;
        }
    }
}