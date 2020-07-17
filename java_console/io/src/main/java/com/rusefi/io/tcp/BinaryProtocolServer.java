package com.rusefi.io.tcp;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.rusefi.Listener;
import com.rusefi.binaryprotocol.*;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.HelloCommand;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Fields.*;

/**
 * This class makes rusEfi console a proxy for other tuning software, this way we can have two tools connected via same
 * serial port simultaneously
 *
 * @author Andrey Belomutskiy
 * 11/24/15
 */

public class BinaryProtocolServer implements BinaryProtocolCommands {
    private static final int DEFAULT_PROXY_PORT = 2390;
    public static final String TS_OK = "\0";
    private final Logger logger;

    public AtomicInteger unknownCommands = new AtomicInteger();

    public BinaryProtocolServer(Logger logger) {
        this.logger = logger;
    }

    public void start(LinkManager linkManager) {
        start(linkManager, DEFAULT_PROXY_PORT, null);
    }

    public void start(LinkManager linkManager, int port, Listener serverSocketCreationCallback) {
        logger.info("BinaryProtocolServer on " + port);

        Function<Socket, Runnable> clientSocketRunnableFactory = clientSocket -> () -> {
            try {
                runProxy(linkManager, clientSocket);
            } catch (IOException e) {
                logger.info("proxy connection: " + e);
            }
        };

        tcpServerSocket(port, "BinaryProtocolServer", clientSocketRunnableFactory, logger, serverSocketCreationCallback);
    }

    /**
     * Starts a new thread
     *
     * @param port                         server port to accept connections
     * @param threadName
     * @param clientSocketRunnableFactory  method to invoke on a new thread for each new client connection
     * @param logger
     * @param serverSocketCreationCallback this callback is invoked once we open the server socket
     */
    public static void tcpServerSocket(int port, String threadName, Function<Socket, Runnable> clientSocketRunnableFactory, final Logger logger, Listener serverSocketCreationCallback) {
        Runnable runnable = new Runnable() {
            @SuppressWarnings("InfiniteLoopStatement")
            @Override
            public void run() {
                ServerSocket serverSocket;
                try {
                    serverSocket = new ServerSocket(port, 1);
                } catch (IOException e) {
                    logger.error(threadName + ": Error binding server socket " + port + ": " + e);
                    throw new IllegalStateException(e);
                }
                if (serverSocketCreationCallback != null)
                    serverSocketCreationCallback.onResult(null);

                try {
                    while (true) {
                        // Wait for a connection
                        final Socket clientSocket = serverSocket.accept();
                        logger.info("Binary protocol proxy port connection");
                        new Thread(clientSocketRunnableFactory.apply(clientSocket), "proxy connection").start();
                    }
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        };
        new Thread(runnable, threadName).start();
    }

    @SuppressWarnings("InfiniteLoopStatement")
    private void runProxy(LinkManager linkManager, Socket clientSocket) throws IOException {
        TcpIoStream stream = new TcpIoStream(logger, clientSocket);

        IncomingDataBuffer in = stream.getDataBuffer();

        while (true) {
            AtomicBoolean handled = new AtomicBoolean();
            Handler protocolCommandHandler = () -> {
                handleProtocolCommand(clientSocket);
                handled.set(true);
            };

            int length = getPacketLength(in, protocolCommandHandler);
            if (handled.get()) {
                continue;
            }

            System.out.println("Got [" + length + "] length promise");

            Packet packet = readPromisedBytes(in, length);
            byte[] payload = packet.getPacket();

            if (payload.length == 0)
                throw new IOException("Empty packet");

            byte command = payload[0];

            System.out.println("Got [" + BinaryProtocol.findCommand(command));

            if (command == Fields.TS_HELLO_COMMAND) {
                new HelloCommand(logger, Fields.TS_SIGNATURE).handle(stream);
            } else if (command == COMMAND_PROTOCOL) {
//                System.out.println("Ignoring crc F command");
                stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes(), logger);
            } else if (command == Fields.TS_GET_FIRMWARE_VERSION) {
                stream.sendPacket((TS_OK + "rusEFI proxy").getBytes(), logger);
            } else if (command == COMMAND_CRC_CHECK_COMMAND) {
                handleCrc(linkManager, stream);
            } else if (command == COMMAND_PAGE) {
                stream.sendPacket(TS_OK.getBytes(), logger);
            } else if (command == COMMAND_READ) {
                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
                handleRead(linkManager, dis, stream);
            } else if (command == Fields.TS_CHUNK_WRITE_COMMAND) {
                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
                handleWrite(linkManager, payload, dis, stream);
            } else if (command == Fields.TS_BURN_COMMAND) {
                stream.sendPacket(new byte[]{TS_RESPONSE_BURN_OK}, logger);
            } else if (command == Fields.TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY) {
                System.err.println("NOT IMPLEMENTED TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY relay");
                // todo: relay command
                stream.sendPacket(TS_OK.getBytes(), logger);
            } else if (command == Fields.TS_OUTPUT_COMMAND) {
                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
                int offset = swap16(dis.readShort());
                int count = swap16(dis.readShort());
                System.out.println("TS_OUTPUT_COMMAND offset=" + offset + "/count=" + count);

                byte[] response = new byte[1 + count];
                response[0] = (byte) TS_OK.charAt(0);
                BinaryProtocolState binaryProtocolState = linkManager.getBinaryProtocolState();
                byte[] currentOutputs = binaryProtocolState.getCurrentOutputs();
                if (currentOutputs != null)
                    System.arraycopy(currentOutputs, 1 + offset, response, 1, count);
                stream.sendPacket(response, logger);
            } else if (command == Fields.TS_GET_TEXT) {
                // todo: relay command
                System.err.println("NOT IMPLEMENTED TS_GET_TEXT relay");
                stream.sendPacket(TS_OK.getBytes(), logger);
            } else {
                unknownCommands.incrementAndGet();
                new IllegalStateException().printStackTrace();
                logger.info("Error: unexpected " + BinaryProtocol.findCommand(command));
            }
        }
    }

    public static int getPacketLength(IncomingDataBuffer in, Handler protocolCommandHandler) throws IOException {
        byte first = in.readByte();
        if (first == COMMAND_PROTOCOL) {
            protocolCommandHandler.handle();
        }
        return first * 256 + in.readByte();
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
        int size = in.read(packet);
        if (size != packet.length)
            throw new IllegalStateException(size + " promised but " + packet.length + " arrived");
        int crc = in.readInt();
        int fromPacket = IoHelper.getCrc32(packet);
        if (crc != fromPacket)
            throw new IllegalStateException("CRC mismatch " + crc + " vs " + fromPacket);
        return new Packet(packet, crc);
    }


    public interface Handler {
        void handle() throws IOException;
    }

    public static void handleProtocolCommand(Socket clientSocket) throws IOException {
        System.out.println("Got plain F command");
        OutputStream outputStream = clientSocket.getOutputStream();
        outputStream.write(TS_PROTOCOL.getBytes());
        outputStream.flush();
    }

    private void handleWrite(LinkManager linkManager, byte[] packet, DataInputStream dis, TcpIoStream stream) throws IOException {
        dis.readShort(); // page
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        logger.info("TS_CHUNK_WRITE_COMMAND: offset=" + offset + " count=" + count);
        BinaryProtocolState bp = linkManager.getBinaryProtocolState();
        bp.setRange(packet, 7, offset, count);
        stream.sendPacket(TS_OK.getBytes(), logger);
    }

    private void handleRead(LinkManager linkManager, DataInputStream dis, TcpIoStream stream) throws IOException {
        short page = dis.readShort();
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        if (count <= 0) {
            logger.info("Error: negative read request " + offset + "/" + count);
        } else {
            System.out.println("read " + page + "/" + offset + "/" + count);
            BinaryProtocolState bp = linkManager.getBinaryProtocolState();
            byte[] response = new byte[1 + count];
            response[0] = (byte) TS_OK.charAt(0);
            Objects.requireNonNull(bp, "bp");
            ConfigurationImage configurationImage = bp.getControllerConfiguration();
            Objects.requireNonNull(configurationImage, "configurationImage");
            System.arraycopy(configurationImage.getContent(), offset, response, 1, count);
            stream.sendPacket(response, logger);
        }
    }

    private void handleCrc(LinkManager linkManager, TcpIoStream stream) throws IOException {
        System.out.println("CRC check");
        BinaryProtocolState bp = linkManager.getBinaryProtocolState();
        byte[] content = bp.getControllerConfiguration().getContent();
        int result = IoHelper.getCrc32(content);
        ByteArrayOutputStream response = new ByteArrayOutputStream();
        response.write(TS_OK.charAt(0));
        new DataOutputStream(response).writeInt(result);
        stream.sendPacket(response.toByteArray(), logger);
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
}