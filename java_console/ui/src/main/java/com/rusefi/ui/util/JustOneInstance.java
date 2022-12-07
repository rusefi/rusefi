package com.rusefi.ui.util;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * This class is used to figure out if we have multiple instances of rusEfi console running
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 5/4/2015
 */
public class JustOneInstance {
    private static final int PORT = 29212;
    private static final int LOCAL_CONNECTION_TIMEOUT_MS = 100;

    public static boolean isAlreadyRunning() {
        if (Boolean.getBoolean("SKIP_ONE_INSTANCE_CHECK"))
            return false;
        try {
            Socket clientSocket = new Socket();
            clientSocket.connect(new InetSocketAddress("localhost", PORT), LOCAL_CONNECTION_TIMEOUT_MS);
            System.out.println("*** Already running!");
            return true;
        } catch (IOException e) {
            return false;
        }
    }

    public static void onStart() {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                ServerSocket serverSocket;
                try {
                    serverSocket = new ServerSocket(PORT, 1);

                    while (true) {
                        // Wait for a connection
                        Socket clientSocket = serverSocket.accept();
                        // System.out.println("*** Got a connection! ");
                        clientSocket.close();
                    }
                } catch (IOException e) {
                }
            }
        };
        new Thread(runnable, "JustOneInstance").start();
    }
}
