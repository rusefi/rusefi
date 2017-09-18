package com.rusefi.ui.util;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * This class is used to figure out if we have multiple instances of rusEfi console running
 *
 * (c) Andrey Belomutskiy
 * 5/4/2015
 */
public class JustOneInstance {
    private static final int PORT = 29212;

    public static boolean isAlreadyRunning() {
        try {
            Socket clientSocket = new Socket("localhost", PORT);
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
