package com.rusefi.io.tcp;

import com.rusefi.FileLog;
import com.rusefi.core.EngineState;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;

import java.io.*;
import java.net.Socket;
import java.util.Collection;
import java.util.Collections;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class TcpConnector implements LinkConnector {
    public final static int DEFAULT_PORT = 29001;
    public static final String LOCALHOST = "localhost";
    private final int port;
    private BufferedWriter writer;
    private boolean withError;

    public TcpConnector(String port) {
        try {
            this.port = getTcpPort(port);
        } catch (InvalidTcpPort e) {
            throw new IllegalStateException("Unexpected", e);
        }
    }

    public static boolean isTcpPort(String port) {
        try {
            getTcpPort(port);
            return true;
        } catch (InvalidTcpPort e) {
            return false;
        }
    }

    public static String doUnpackConfirmation(String message) {
        String confirmation = message.substring(CommandQueue.CONFIRMATION_PREFIX.length());
        int index = confirmation.indexOf(":");
        if (index < 0) {
            return null;
        }
        String number = confirmation.substring(index + 1);
        int length;
        try {
            length = Integer.parseInt(number);
        } catch (NumberFormatException e) {
            return null;
        }
        if (length != index) {
            return null;
        }
        return confirmation.substring(0, length);
    }

    static class InvalidTcpPort extends Exception {

    }


    public static int getTcpPort(String port) throws InvalidTcpPort {
        try {
            return Integer.parseInt(port);
        } catch (NumberFormatException e) {
            throw new InvalidTcpPort();
        }
    }

    public static int parseIntWithReason(String number, String reason) {
        try {
            return Integer.parseInt(number);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Unexpected [" + number + "] for " + reason, e);
        }
    }

    /**
     * this implementation is blocking
     * @param listener
     */
    @Override
    public void connect(LinkManager.LinkStateListener listener) {
        FileLog.MAIN.logLine("Connecting to " + port);
        BufferedInputStream stream;
        try {
            Socket socket = new Socket(LOCALHOST, port);
            writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            stream = new BufferedInputStream(socket.getInputStream());
        } catch (IOException e) {
            throw new IllegalStateException("Failed to connect to simulator", e);
        }

        final BufferedReader reader = new BufferedReader(new InputStreamReader(stream));

        LinkManager.IO_EXECUTOR.execute(new Runnable() {
            @Override
            public void run() {
                Thread.currentThread().setName("TCP connector loop");
                FileLog.MAIN.logLine("Running TCP connection loop");
                while (true) {
                    try {
                        String line = reader.readLine();
                        LinkManager.engineState.processNewData(line + "\r\n");
                    } catch (IOException e) {
                        System.err.println("End of connection");
                        return;
                    }
                }
            }
        });

    }

    @Override
    public void restart() {
//        FileLog.rlog("Restarting on " + port);
    }

    @Override
    public boolean hasError() {
        return withError;
    }

    @Override
    public String unpack(String packet) {
        return EngineState.unpackString(packet);
    }

    @Override
    public void send(String text) throws InterruptedException {
        String command = LinkManager.encodeCommand(text);
        FileLog.MAIN.logLine("Writing " + command);
        try {
            writer.write(command + "\r\n");
            writer.flush();
        } catch (IOException e) {
            withError = true;
            System.err.println("err in send");
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }
    }

    @Override
    public String unpackConfirmation(String message) {
        return doUnpackConfirmation(message);
    }

    public static Collection<String> getAvailablePorts() {
        return isTcpPortOpened() ? Collections.singletonList("" + DEFAULT_PORT) : Collections.<String>emptyList();
    }

    public static boolean isTcpPortOpened() {
        try {
            Socket s = new Socket(LOCALHOST, DEFAULT_PORT);
            s.close();
            return true;
        } catch (IOException e) {
            System.out.println("Connection refused in getAvailablePorts(): simulator not running");
            return false;
        }
    }
}
