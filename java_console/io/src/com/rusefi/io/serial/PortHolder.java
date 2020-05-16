package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.io.CommunicationLoggingHolder;
import com.rusefi.io.ConnectionStateListener;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
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
    private static final DataListener dataListener = freshData -> LinkManager.engineState.processNewData(new String(freshData), LinkManager.ENCODER);

    public ConnectionStateListener listener;
    private final Object portLock = new Object();

    private BinaryProtocol bp;

    protected PortHolder() {
    }

    @Nullable
    private IoStream serialPort;

    public String port;

    boolean connectAndReadConfiguration() {
        if (port == null)
            return false;

        CommunicationLoggingHolder.communicationLoggingListener.onPortHolderMessage(getClass(), "Opening port: " + port);

        IoStream stream = SerialIoStreamJSerialComm.openPort(port);
        synchronized (portLock) {
            this.serialPort = stream;
            portLock.notifyAll();
        }

        bp = BinaryProtocolHolder.getInstance().create(FileLog.LOGGER, stream);

        boolean result = bp.connectAndReadConfiguration(dataListener);
        if (listener != null) {
            if (result) {
                listener.onConnectionEstablished();
            } else {
                listener.onConnectionFailed();
            }
        }
        return result;
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
}
