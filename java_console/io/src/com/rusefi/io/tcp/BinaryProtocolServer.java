package com.rusefi.io.tcp;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IoHelper;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * This class makes rusEfi console a proxy for other tuning software, this way we can have two tools connected via same
 * serial port simultaniously
 *
 * @author Andrey Belomutskiy
 *         11/24/15
 */

public class BinaryProtocolServer {
    private static final int PROXY_PORT = 2390;
    private static final String TS_OK = "\0";

    private static final String TS_SIGNATURE = "MShift v0.01";
    private static final String TS_PROTOCOL = "001";

    public static void start() {
        FileLog.MAIN.logLine("BinaryProtocolServer on " + PROXY_PORT);
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                ServerSocket serverSocket;
                try {
                    serverSocket = new ServerSocket(PROXY_PORT, 1);

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
                }
            }
        };
        new Thread(runnable, "BinaryProtocolServer").start();
    }

    private static void runProxy(Socket clientSocket) throws IOException {
        DataInputStream in = new DataInputStream(clientSocket.getInputStream());

        while (true) {
            byte first = in.readByte();
            if (first == BinaryProtocol.COMMAND_PROTOCOL) {
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
            in.read(packet);

            DataInputStream dis = new DataInputStream(new ByteArrayInputStream(packet));
            byte command = (byte) dis.read();
            System.out.println("Got [" + (char) command + "/" + command + "] command");

            int crc = in.readInt();
            if (crc != IoHelper.crc32(packet))
                throw new IllegalStateException("CRC mismatch");


            TcpIoStream stream = new TcpIoStream(clientSocket.getOutputStream(), null);
            if (command == BinaryProtocol.COMMAND_HELLO) {
                BinaryProtocol.sendCrcPacket((TS_OK + TS_SIGNATURE).getBytes(), FileLog.LOGGER, stream);
            } else if (command == BinaryProtocol.COMMAND_PROTOCOL) {
//                System.out.println("Ignoring crc F command");
                BinaryProtocol.sendCrcPacket((TS_OK + TS_PROTOCOL).getBytes(), FileLog.LOGGER, stream);
            } else if (command == BinaryProtocol.COMMAND_CRC_CHECK_COMMAND) {
                short page = dis.readShort();
                short offset = dis.readShort();
                short count = dis.readShort();
                System.out.println("CRC check " + page + "/" + offset + "/" + count);
            }


        }

    }
}