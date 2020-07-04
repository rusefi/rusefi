package com.rusefi.io.tcp;

import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ResponseBuffer;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;

import java.io.IOException;
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
    private final LinkManager linkManager;
    private final Logger logger;

    private BinaryProtocol bp;

    public TcpConnector(LinkManager linkManager, String port, Logger logger) {
        this.linkManager = linkManager;
        this.logger = logger;
        try {
            this.port = getTcpPort(port);
            this.hostname = getHostname(port);
        } catch (InvalidTcpPort e) {
            throw new IllegalStateException("Unexpected", e);
        }
    }

    public BinaryProtocol getBinaryProtocol() {
        return bp;
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

    public static String getHostname(String port) {
        String[] portParts = port.split(":");
        return (portParts.length == 1 ? LOCALHOST : portParts[0].length() > 0 ? portParts[0] : LOCALHOST);
    }

    /**
     * this implementation is blocking
     */
    @Override
    public void connectAndReadConfiguration(ConnectionStateListener listener) {
        logger.info("Connecting to host=" + hostname + "/port=" + port);
        TcpIoStream tcpIoStream;

        try {
            Socket socket = new Socket(hostname, port);
            tcpIoStream = new TcpIoStream(logger, linkManager, socket);
        } catch (IOException e) {
            listener.onConnectionFailed();
            logger.error("Failed to connect to " + hostname + "/port=" + port);
            return;
        }

        final ResponseBuffer rb = new ResponseBuffer(new ResponseBuffer.ResponseListener() {
            @Override
            public void onResponse(String line) {
                linkManager.getEngineState().processNewData(line + "\r\n", LinkManager.ENCODER);
            }
        });

        DataListener listener1 = new DataListener() {
            @Override
            public void onDataArrived(byte[] freshData) {
                rb.append(new String(freshData), LinkManager.ENCODER);
            }
        };
//        ioStream.setInputListener(listener1);

        bp = new BinaryProtocol(linkManager, logger, tcpIoStream);

        boolean result = bp.connectAndReadConfiguration(listener1);
        if (result) {
            listener.onConnectionEstablished();
        } else {
            listener.onConnectionFailed();
        }
    }

    @Override
    public void restart() {
    }

    @Override
    public void send(String command, boolean fireEvent) throws InterruptedException {
        if (bp == null) {
            logger.info("Not connected");
            return;
        }

        bp.doSend(command, fireEvent);
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
