package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.io.CommunicationLoggingHolder;
import com.rusefi.io.ConnectionStateListener;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;

/**
 * This class holds the reference to the actual Serial port object
 * <p/>
 * 7/25/13
 * (c) Andrey Belomutskiy
 */
public class PortHolder {
    /**
     * Nasty code: this field is not final, we have UI which overrides this default!
     */
    public static int BAUD_RATE = 115200;
    private static PortHolder instance = new PortHolder();
    private final Object portLock = new Object();

    private BinaryProtocol bp;

    private PortHolder() {
    }

    @Nullable
    private IoStream serialPort;

    boolean openPort(String port, DataListener dataListener, ConnectionStateListener listener) {
        CommunicationLoggingHolder.communicationLoggingListener.onPortHolderMessage(SerialManager.class, "Opening port: " + port);
        if (port == null)
            return false;
        boolean result = open(port, dataListener);
        if (listener != null) {
            if (result) {
                listener.onConnectionEstablished();
            } else {
                listener.onConnectionFailed();
            }
        }
        return result;
    }

    /**
     * @return true if everything fine
     */
    private boolean open(String port, final DataListener listener) {
        EstablishConnection establishConnection = new EstablishConnection(port).invoke();
        if (!establishConnection.isConnected())
            return false;
        synchronized (portLock) {
            PortHolder.this.serialPort = establishConnection.stream;
            portLock.notifyAll();
        }
        IoStream stream = establishConnection.getStream();

        bp = BinaryProtocolHolder.create(FileLog.LOGGER, stream);

        return bp.connectAndReadConfiguration(listener);
    }

    private static boolean isWindows10() {
        // todo: this code is fragile! What about Windows 11, 12 etc!? this is a problem for the later day :(
        return System.getProperty(FileLog.OS_VERSION).startsWith("10");
    }

    public void close() {
        synchronized (portLock) {
            if (serialPort != null) {
                try {
                    serialPort.close();
                    serialPort = null;
                } finally {
                    portLock.notifyAll();
                }
            }
        }
    }

    /**
     * this method blocks till a connection is available
     */
    public void packAndSend(final String command, boolean fireEvent) throws InterruptedException {
        if (bp == null) {
            Window[] windows = JDialog.getWindows();
            Window window = windows.length == 0 ? null : windows[0];
            JOptionPane.showMessageDialog(window, "No connectivity",
                    "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(-1);
        }

        bp.doSend(command, fireEvent);
    }

    public static PortHolder getInstance() {
        return instance;
    }

    public static class EstablishConnection {
        private boolean isConnected;
        private String port;
        private IoStream stream;

        public EstablishConnection(String port) {
            this.port = port;
        }

        // todo: remove dead code - always true?
        public boolean isConnected() {
            return isConnected;
        }

        public IoStream getStream() {
            return stream;
        }

        public EstablishConnection invoke() {
            stream = SerialIoStreamJSerialComm.open(port, BAUD_RATE, FileLog.LOGGER);
/*
todo: remove dead code
            if (stream == null) {
                isConnected = false;
                return this;
            }
             */

            isConnected = true;
            return this;
        }
    }
}
