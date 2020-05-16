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
        IoStream stream = EstablishConnection.create(port);
        synchronized (portLock) {
            PortHolder.this.serialPort = stream;
            portLock.notifyAll();
        }

        bp = BinaryProtocolHolder.getInstance().create(FileLog.LOGGER, stream);

        return bp.connectAndReadConfiguration(listener);
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
        public static IoStream create(String port) {
            return SerialIoStreamJSerialComm.open(port, BAUD_RATE, FileLog.LOGGER);
        }
    }
}
