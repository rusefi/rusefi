package com.rusefi.io.tcp;

import com.opensr5.ConfigurationImage;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.LinkManager;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Fields.*;

/**
 * This class makes rusEfi console a proxy for other tuning software, this way we can have two tools connected via same
 * serial port simultaneously
 *
 * @author Andrey Belomutskiy
 *         11/24/15
 */

public class BinaryProtocolServer implements BinaryProtocolCommands {
    private static final int DEFAULT_PROXY_PORT = 2390;
    private static final String TS_OK = "\0";

    public AtomicInteger unknownCommands = new AtomicInteger();

    public void start(LinkManager linkManager) {
        start(linkManager, DEFAULT_PROXY_PORT);
    }

    public void start(LinkManager linkManager, int port) {
        FileLog.MAIN.logLine("BinaryProtocolServer on " + port);
        Runnable runnable = new Runnable() {
            @SuppressWarnings("InfiniteLoopStatement")
            @Override
            public void run() {
                ServerSocket serverSocket;
                try {
                    serverSocket = new ServerSocket(port, 1);
                } catch (IOException e) {
                    FileLog.MAIN.logException("Error binding server socket", e);
                    return;
                }

                try {
                    while (true) {
                        // Wait for a connection
                        final Socket clientSocket = serverSocket.accept();
                        FileLog.MAIN.logLine("Binary protocol proxy port connection");
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    runProxy(linkManager, clientSocket);
                                } catch (IOException e) {
                                    FileLog.MAIN.logLine("proxy connection: " + e);
                                }
                            }
                        }, "proxy connection").start();
                    }
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        };
        new Thread(runnable, "BinaryProtocolServer").start();
    }

    @SuppressWarnings("InfiniteLoopStatement")
    private void runProxy(LinkManager linkManager, Socket clientSocket) throws IOException {
        DataInputStream in = new DataInputStream(clientSocket.getInputStream());

        while (true) {
            byte first = in.readByte();
            if (first == COMMAND_PROTOCOL) {
                //System.out.println("Ignoring plain F command");
                System.out.println("Got plain F command");
                OutputStream outputStream = clientSocket.getOutputStream();
                outputStream.write(TS_PROTOCOL.getBytes());
                outputStream.flush();
                continue;
            }

            int length = first * 256 + in.readByte();

            System.out.println("Got [" + length + "] length promise");

            if (length == 0)
                throw new IOException("Zero length not expected");

            byte[] packet = new byte[length];
            int size = in.read(packet);
            if (size != packet.length)
                throw new IllegalStateException();

            DataInputStream dis = new DataInputStream(new ByteArrayInputStream(packet));
            byte command = (byte) dis.read();
            System.out.println("Got [" + (char) command + "/" + command + "] command");

            int crc = in.readInt();
            if (crc != IoHelper.getCrc32(packet))
                throw new IllegalStateException("CRC mismatch");

            TcpIoStream stream = new TcpIoStream(linkManager, clientSocket);
            if (command == COMMAND_HELLO) {
                stream.sendPacket((TS_OK + Fields.TS_SIGNATURE).getBytes(), FileLog.LOGGER);
            } else if (command == COMMAND_PROTOCOL) {
//                System.out.println("Ignoring crc F command");
                stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes(), FileLog.LOGGER);
            } else if (command == Fields.TS_GET_FIRMWARE_VERSION) {
                stream.sendPacket((TS_OK + "rusEFI proxy").getBytes(), FileLog.LOGGER);
            } else if (command == COMMAND_CRC_CHECK_COMMAND) {
                handleCrc(linkManager, stream);
            } else if (command == COMMAND_PAGE) {
                stream.sendPacket(TS_OK.getBytes(), FileLog.LOGGER);
            } else if (command == COMMAND_READ) {
                handleRead(linkManager, dis, stream);
            } else if (command == Fields.TS_CHUNK_WRITE_COMMAND) {
                handleWrite(linkManager, packet, dis, stream);
            } else if (command == Fields.TS_BURN_COMMAND) {
                stream.sendPacket(new byte[]{TS_RESPONSE_BURN_OK}, FileLog.LOGGER);
            } else if (command == Fields.TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY) {
                // todo: relay command
                stream.sendPacket(TS_OK.getBytes(), FileLog.LOGGER);
            } else if (command == Fields.TS_OUTPUT_COMMAND) {
                int offset = swap16(dis.readShort());
                int count = swap16(dis.readShort());
                System.out.println("TS_OUTPUT_COMMAND offset=" + offset + "/count=" + count);

                byte[] response = new byte[1 + count];
                response[0] = (byte) TS_OK.charAt(0);
                BinaryProtocolState binaryProtocolState = linkManager.getBinaryProtocolState();
                byte[] currentOutputs = binaryProtocolState.getCurrentOutputs();
                if (currentOutputs != null)
                    System.arraycopy(currentOutputs, 1 + offset , response, 1, count);
                stream.sendPacket(response, FileLog.LOGGER);
            } else {
                unknownCommands.incrementAndGet();
                new IllegalStateException().printStackTrace();
                FileLog.MAIN.logLine("Error: unknown command " + (char) command + "/" + command);
            }
        }
    }

    private static void handleWrite(LinkManager linkManager, byte[] packet, DataInputStream dis, TcpIoStream stream) throws IOException {
        dis.readShort(); // page
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        FileLog.MAIN.logLine("TS_CHUNK_WRITE_COMMAND: offset=" + offset + " count=" + count);
        BinaryProtocolState bp = linkManager.getBinaryProtocolState();
        bp.setRange(packet, 7, offset, count);
        stream.sendPacket(TS_OK.getBytes(), FileLog.LOGGER);
    }

    private static void handleRead(LinkManager linkManager, DataInputStream dis, TcpIoStream stream) throws IOException {
        short page = dis.readShort();
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        if (count <= 0) {
            FileLog.MAIN.logLine("Error: negative read request " + offset + "/" + count);
        } else {
            System.out.println("read " + page + "/" + offset + "/" + count);
            BinaryProtocolState bp = linkManager.getBinaryProtocolState();
            byte[] response = new byte[1 + count];
            response[0] = (byte) TS_OK.charAt(0);
            Objects.requireNonNull(bp, "bp");
            ConfigurationImage configurationImage = bp.getControllerConfiguration();
            Objects.requireNonNull(configurationImage, "configurationImage");
            System.arraycopy(configurationImage.getContent(), offset, response, 1, count);
            stream.sendPacket(response, FileLog.LOGGER);
        }
    }

    private static void handleCrc(LinkManager linkManager, TcpIoStream stream) throws IOException {
        System.out.println("CRC check");
        BinaryProtocolState bp = linkManager.getBinaryProtocolState();
        byte[] content = bp.getControllerConfiguration().getContent();
        int result = IoHelper.getCrc32(content);
        ByteArrayOutputStream response = new ByteArrayOutputStream();
        response.write(TS_OK.charAt(0));
        new DataOutputStream(response).writeInt(result);
        stream.sendPacket(response.toByteArray(), FileLog.LOGGER);
    }
}