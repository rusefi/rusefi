package com.rusefi.io.tcp;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.CompatibleFunction;
import com.rusefi.Listener;
import com.rusefi.NamedThreadFactory;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

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

public class BinaryProtocolServer {
    public static final String TEST_FILE = "test_log.mlg.Z";
    private static final Logging log = getLogging(BinaryProtocolServer.class);
    private static final int DEFAULT_PROXY_PORT = 2390;
    public static final String TS_OK = "\0";

    private final static boolean MOCK_SD_CARD = true;
    private static final int SD_STATUS_OFFSET = 246;
    private static final int FAST_TRANSFER_PACKET_SIZE = 2048;

    static {
        log.configureDebugEnabled(false);
    }

    public AtomicInteger unknownCommands = new AtomicInteger();

    public static final ServerSocketFunction SECURE_SOCKET_FACTORY = rusEFISSLContext::getSSLServerSocket;

    private final static ConcurrentHashMap<String, ThreadFactory> THREAD_FACTORIES_BY_NAME = new ConcurrentHashMap<>();

    public void start(LinkManager linkManager) {
        try {
            start(linkManager, DEFAULT_PROXY_PORT, Listener.empty(), new Context());
        } catch (IOException e) {
            log.warn("Error starting local proxy: " + e);
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

        tcpServerSocket(port, "BinaryProtocolServer", clientSocketRunnableFactory, serverSocketCreationCallback, StatusConsumer.ANONYMOUS);
    }

    /**
     * Starts a new thread
     *
     * @param port                         server port to accept connections
     * @param threadName
     * @param socketRunnableFactory        method to invoke on a new thread for each new client connection
     * @param serverSocketCreationCallback this callback is invoked once we open the server socket
     * @param statusConsumer
     * @return
     */
    public static ServerSocketReference tcpServerSocket(int port, String threadName, CompatibleFunction<Socket, Runnable> socketRunnableFactory, Listener serverSocketCreationCallback, StatusConsumer statusConsumer) throws IOException {
        return tcpServerSocket(socketRunnableFactory, port, threadName, serverSocketCreationCallback, p -> {
            ServerSocket serverSocket = new ServerSocket(p);
            statusConsumer.append("ServerSocket " + p + " created. Feel free to point TS at IP Address 'localhost' port " + p);
            return serverSocket;
        });
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
                Runnable clientRunnable = clientSocketRunnableFactory.apply(clientSocket);
                Objects.requireNonNull(clientRunnable, "Runnable for " + clientSocket);
                threadFactory.newThread(clientRunnable).start();
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
            Integer length = getPendingPacketLengthOrHandleProtocolCommand(clientSocket, context, in);
            if (length == null)
                continue;

            byte[] payload = getPacketContent(in, length);

            byte command = payload[0];

            log.info("Got command " + BinaryProtocol.findCommand(command));

            if (command == Fields.TS_HELLO_COMMAND) {
                new HelloCommand(Fields.TS_SIGNATURE).handle(stream);
            } else if (command == Fields.TS_GET_PROTOCOL_VERSION_COMMAND_F) {
                stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes());
            } else if (command == Fields.TS_GET_FIRMWARE_VERSION) {
                stream.sendPacket((TS_OK + "rusEFI proxy").getBytes());
            } else if (command == Fields.TS_CRC_CHECK_COMMAND) {
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

    @NotNull
    public static byte[] getPacketContent(IncomingDataBuffer in, Integer length) throws IOException {
        if (log.debugEnabled())
            log.debug("Got [" + length + "] length promise");

        Packet packet = readPromisedBytes(in, length);
        byte[] payload = packet.getPacket();

        if (payload.length == 0)
            throw new IOException("Empty packet");
        return payload;
    }

    /**
     * @return null if we have handled GET_PROTOCOL_VERSION_COMMAND command
     */
    @Nullable
    public static Integer getPendingPacketLengthOrHandleProtocolCommand(Socket clientSocket, Context context, IncomingDataBuffer in) throws IOException {
        AtomicBoolean handled = new AtomicBoolean();
        Handler protocolCommandHandler = () -> {
            handleProtocolCommand(clientSocket);
            handled.set(true);
        };

        int length = getPacketLength(in, protocolCommandHandler, context.getTimeout());
        if (handled.get()) {
            return null;
        }
        return length;
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
        if (first == Fields.TS_GET_PROTOCOL_VERSION_COMMAND_F) {
            protocolCommandHandler.handle();
            return 0;
        }
        byte secondByte = in.readByte(ioTimeout);
        return IoHelper.getInt(first, secondByte);
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
        if (log.debugEnabled())
            log.debug("Got plain GetProtocol F command");
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