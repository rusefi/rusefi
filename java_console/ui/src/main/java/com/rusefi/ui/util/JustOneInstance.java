package com.rusefi.ui.util;

import com.devexperts.logging.Logging;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.util.function.Consumer;

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
    private static final int LOCAL_RESPONSE_TIMEOUT_MS = 500;
    static final String PING = "PING";
    private static final String OPEN_LOG = "OPEN_LOG ";
    private static final String ACK = "RUSEFI_ACK";

    public static boolean isAlreadyRunning() {
        if (Boolean.getBoolean("SKIP_ONE_INSTANCE_CHECK"))
            return false;
        boolean running = send(PING) != null;
        log.info(running ? "Another instance is running" : "No other instances seem to be running");
        return running;
    }

    /** Sends a log to the running instance; false when none is listening or it does not support forwarding. */
    public static boolean openLogInRunningInstance(File file) {
        return ACK.equals(send(openLogRequest(file)));
    }

    private static String send(String request) {
        return send(PORT, request);
    }

    private static String send(int port, String request) {
        try {
            try (Socket socket = new Socket()) {
                socket.connect(new InetSocketAddress(InetAddress.getLoopbackAddress(), port),
                    LOCAL_CONNECTION_TIMEOUT_MS);
                socket.setSoTimeout(LOCAL_RESPONSE_TIMEOUT_MS);
                PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);
                writer.println(request);
                return new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8))
                    .readLine();
            }
        } catch (IOException e) {
            return null;
        }
    }

    static boolean sendRequest(int port, String request) {
        return ACK.equals(send(port, request));
    }

    static String openLogRequest(File file) {
        String path = file.getAbsolutePath();
        return OPEN_LOG + Base64.getEncoder().encodeToString(path.getBytes(StandardCharsets.UTF_8));
    }

    public static void onStart(Consumer<File> openLog) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                log.info("server socket " + PORT);
                try (ServerSocket serverSocket = new ServerSocket(PORT, 1, InetAddress.getLoopbackAddress())) {
                    while (true) {
                        try (Socket clientSocket = serverSocket.accept()) {
                            handleConnection(clientSocket, openLog);
                        } catch (IOException e) {
                            log.warn("Single-instance request failed: " + e);
                        }
                    }
                } catch (IOException e) {
                    log.warn("Could not start the single-instance server: " + e);
                }
            }
        };
        new Thread(runnable, "JustOneInstance").start();
    }

    static void handleConnection(Socket socket, Consumer<File> openLog) throws IOException {
        socket.setSoTimeout(LOCAL_RESPONSE_TIMEOUT_MS);
        BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
        String request = reader.readLine();
        File file = logFile(request);
        if (file != null) {
            openLog.accept(file);
        }
        new PrintWriter(socket.getOutputStream(), true).println(ACK);
    }

    private static File logFile(String request) {
        if (request == null || !request.startsWith(OPEN_LOG)) {
            return null;
        }
        try {
            byte[] path = Base64.getDecoder().decode(request.substring(OPEN_LOG.length()));
            return new File(new String(path, StandardCharsets.UTF_8));
        } catch (IllegalArgumentException e) {
            log.warn("Ignoring an invalid open-log request");
            return null;
        }
    }
}
