package com.rusefi.io.tcp;

import com.opensr5.io.DataListener;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.core.ResponseBuffer;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.OutputStream;
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
    private final String hostname;

    private BinaryProtocol bp;

    public TcpConnector(String port) {
        try {
            this.port = getTcpPort(port);
            this.hostname = getHostname(port);
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

    /*
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
        */
    static class InvalidTcpPort extends Exception {
    }

    public static int getTcpPort(String port) throws InvalidTcpPort {
        try {
            String[] portParts = port.split(":");
            int index = (portParts.length == 1 ? 0 : 1);
            return Integer.parseInt(portParts[index]);
        } catch (NumberFormatException e) {
            throw new InvalidTcpPort();
        }
    }

    private static String getHostname(String port) {
        String[] portParts = port.split(":");
        return (portParts.length == 1 ? LOCALHOST : portParts[0].length() > 0 ? portParts[0] : LOCALHOST);
    }

    /**
     * this implementation is blocking
     */
    @Override
    public void connect(ConnectionStateListener listener) {
        FileLog.MAIN.logLine("Connecting to host=" + hostname + "/port=" + port);
        OutputStream os;
        BufferedInputStream stream;
        try {
            Socket socket = new Socket(hostname, port);
            os = socket.getOutputStream();
            stream = new BufferedInputStream(socket.getInputStream());
//            ioStream = new TcpIoStream(os, stream);
        } catch (IOException e) {
            listener.onConnectionFailed();
            FileLog.MAIN.logLine("Failed to connect to " + hostname + "/port=" + port);
            return;
        }

        final ResponseBuffer rb = new ResponseBuffer(new ResponseBuffer.ResponseListener() {
            @Override
            public void onResponse(String line) {
                LinkManager.engineState.processNewData(line + "\r\n", LinkManager.ENCODER);
            }
        });

        DataListener listener1 = new DataListener() {
            @Override
            public void onDataArrived(byte[] freshData) {
                rb.append(new String(freshData), LinkManager.ENCODER);
            }
        };
//        ioStream.setInputListener(listener1);

        bp = BinaryProtocolHolder.create(FileLog.LOGGER, new TcpIoStream(stream, os));

        boolean result = bp.connectAndReadConfiguration(listener1);
        if (result) {
            listener.onConnectionEstablished();
        } else {
            listener.onConnectionFailed();
        }
    }

    @Override
    public void restart() {
//        FileLog.rlog("Restarting on " + port);
    }

    @Override
    public boolean hasError() {
        return false;
    }

    @Override
    public String unpack(String packet) {
        return packet;
//        return EngineState.unpackString(packet);
    }

    @Override
    public void send(String command, boolean fireEvent) throws InterruptedException {
        if (bp == null) {
            FileLog.MAIN.logLine("Not connected");
            return;
        }

        bp.doSend(command, fireEvent);
//        String command = LinkManager.encodeCommand(text);
//        FileLog.MAIN.logLine("Writing " + command);
//        try {
//            ioStream.write((command + "\n").getBytes());
//        } catch (IOException e) {
//            withError = true;
//            System.err.println("err in send");
//            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
//        }
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
