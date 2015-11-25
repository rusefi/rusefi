package com.rusefi.io.tcp;

import com.rusefi.FileLog;

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
                                runProxy(clientSocket);
                            }
                        }, "proxy connection").start();
                    }
                } catch (IOException e) {
                }
            }
        };
        new Thread(runnable, "BinaryProtocolServer").start();
    }

    private static void runProxy(Socket clientSocket) {

    }
}