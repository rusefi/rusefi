package com.rusefi.ui.util;

import com.devexperts.logging.Logging;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

/**
 * This class is used to figure out if we have multiple instances of rusEfi console running
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 5/4/2015
 */
public class JustOneInstance {
    private final static Logging log = Logging.getLogging(JustOneInstance.class);
    private static final int PORT = 29213;
    private static final int LOCAL_CONNECTION_TIMEOUT_MS = 100;

    public static boolean isAlreadyRunning() {
        if (Boolean.getBoolean("SKIP_ONE_INSTANCE_CHECK"))
            return false;
        try {
            Socket clientSocket = new Socket();
            clientSocket.connect(new InetSocketAddress("localhost", PORT), LOCAL_CONNECTION_TIMEOUT_MS);
            String msg = "*** Already running!";
            System.out.println(msg);
            log.info(msg);
            return true;
        } catch (IOException e) {
            log.info("No other instances seem to be running");
            return false;
        }
    }

    public static void onStart() {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                ServerSocket serverSocket;
                log.info("server socket " + PORT);
                try {
                    serverSocket = new ServerSocket(PORT, 1);

                    while (true) {
                        // Wait for a connection
                        Socket clientSocket = serverSocket.accept();
                        handleConnection(clientSocket);
                    }
                } catch (IOException e) {
                }
            }
        };
        new Thread(runnable, "JustOneInstance").start();
    }

    private static void handleConnection(Socket clientSocket) throws IOException {
        try (OutputStream outputStream = clientSocket.getOutputStream()) {
            PrintWriter networkWriter = new PrintWriter(outputStream, true);
            String msg = new Date() + "Already running " + /* todo [tag:java8] ProcessHandle.current().pid() + */"\r\n";
            log.info(msg);
            networkWriter.println(msg);
        }
    }
}
