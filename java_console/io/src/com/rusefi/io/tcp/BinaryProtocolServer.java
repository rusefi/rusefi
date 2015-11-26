package com.rusefi.io.tcp;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IoHelper;

import java.io.DataInputStream;
import java.io.IOException;
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
    private static final String TS_SIGNATURE = "\0MShift v0.01";

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
            short length = in.readShort();

            System.out.println("Got [" + length + "] length promise");

            if (length == 0)
                throw new IOException("Zero length not expected");

            byte[] packet = new byte[length];
            in.read(packet);

            byte command = packet[0];
            System.out.println("Got [" + (char) command + "] command");

            int crc = in.readInt();
            if (crc != IoHelper.crc32(packet))
                throw new IllegalStateException("CRC mismatch");

            if (command == BinaryProtocol.COMMAND_HELLO) {
                TcpIoStream stream = new TcpIoStream(clientSocket.getOutputStream(), null);
                BinaryProtocol.sendCrcPacket(TS_SIGNATURE.getBytes(), FileLog.LOGGER, stream);
            }


        }

    }
}