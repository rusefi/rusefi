package com.rusefi.io.tcp;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.LinkManager;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Fields.TS_PROTOCOL;
import static com.rusefi.config.generated.Fields.TS_RESPONSE_BURN_OK;

/**
 * This class makes rusEfi console a proxy for other tuning software, this way we can have two tools connected via same
 * serial port simultaneously
 *
 * @author Andrey Belomutskiy
 *         11/24/15
 */

public class BinaryProtocolServer implements BinaryProtocolCommands {
    private static final int PROXY_PORT = 2390;
    private static final String TS_OK = "\0";

    public static void start() {
        FileLog.MAIN.logLine("BinaryProtocolServer on " + PROXY_PORT);
        Runnable runnable = new Runnable() {
            @SuppressWarnings("InfiniteLoopStatement")
            @Override
            public void run() {
                ServerSocket serverSocket;
                try {
                    serverSocket = new ServerSocket(PROXY_PORT, 1);
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
                                    runProxy(clientSocket);
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
    private static void runProxy(Socket clientSocket) throws IOException {
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


            TcpIoStream stream = new TcpIoStream(clientSocket.getInputStream(), clientSocket.getOutputStream());
            if (command == COMMAND_HELLO) {
                stream.sendPacket((TS_OK + Fields.TS_SIGNATURE).getBytes(), FileLog.LOGGER);
            } else if (command == COMMAND_PROTOCOL) {
//                System.out.println("Ignoring crc F command");
                stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes(), FileLog.LOGGER);
            } else if (command == Fields.TS_GET_FIRMWARE_VERSION) {
                stream.sendPacket((TS_OK + "rusEFI proxy").getBytes(), FileLog.LOGGER);
            } else if (command == COMMAND_CRC_CHECK_COMMAND) {
                handleCrc(stream);
            } else if (command == COMMAND_PAGE) {
                stream.sendPacket(TS_OK.getBytes(), FileLog.LOGGER);
            } else if (command == COMMAND_READ) {
                handleRead(dis, stream);
            } else if (command == Fields.TS_CHUNK_WRITE_COMMAND) {
                handleWrite(packet, dis, stream);
            } else if (command == Fields.TS_BURN_COMMAND) {
                stream.sendPacket(new byte[]{TS_RESPONSE_BURN_OK}, FileLog.LOGGER);
            } else if (command == Fields.TS_OUTPUT_COMMAND) {
                int offset = swap16(dis.readShort());
                int count = swap16(dis.readShort());
                System.out.println("TS_OUTPUT_COMMAND offset=" + offset + "/count=" + count);

                byte[] response = new byte[1 + count];
                response[0] = (byte) TS_OK.charAt(0);
                BinaryProtocol bp = LinkManager.getCurrentStreamState();
                byte[] currentOutputs = bp.currentOutputs;
                if (currentOutputs != null)
                    System.arraycopy(currentOutputs, 1 + offset , response, 1, count);
                stream.sendPacket(response, FileLog.LOGGER);
            } else {
                new IllegalStateException().printStackTrace();
                FileLog.MAIN.logLine("Error: unknown command " + command);
            }
        }
    }

    private static void handleWrite(byte[] packet, DataInputStream dis, TcpIoStream stream) throws IOException {
        dis.readShort(); // page
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        FileLog.MAIN.logLine("TS_CHUNK_WRITE_COMMAND: offset=" + offset + " count=" + count);
        BinaryProtocol bp = LinkManager.getCurrentStreamState();
        bp.setRange(packet, 7, offset, count);
        stream.sendPacket(TS_OK.getBytes(), FileLog.LOGGER);
    }

    private static void handleRead(DataInputStream dis, TcpIoStream stream) throws IOException {
        short page = dis.readShort();
        int offset = swap16(dis.readShort());
        int count = swap16(dis.readShort());
        if (count <= 0) {
            FileLog.MAIN.logLine("Error: negative read request " + offset + "/" + count);
        } else {
            System.out.println("read " + page + "/" + offset + "/" + count);
            BinaryProtocol bp = LinkManager.getCurrentStreamState();
            byte[] response = new byte[1 + count];
            response[0] = (byte) TS_OK.charAt(0);
            System.arraycopy(bp.getControllerConfiguration().getContent(), offset, response, 1, count);
            stream.sendPacket(response, FileLog.LOGGER);
        }
    }

    private static void handleCrc(TcpIoStream stream) throws IOException {
        System.out.println("CRC check");
        BinaryProtocol bp = LinkManager.getCurrentStreamState();
        byte[] content = bp.getControllerConfiguration().getContent();
        int result = IoHelper.getCrc32(content);
        ByteArrayOutputStream response = new ByteArrayOutputStream();
        response.write(TS_OK.charAt(0));
        new DataOutputStream(response).writeInt(result);
        stream.sendPacket(response.toByteArray(), FileLog.LOGGER);
    }
}