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

    @Nullable
    private BinaryProtocol bp;

    protected PortHolder() {
    }

    public String port;

    boolean connectAndReadConfiguration() {
        if (port == null)
            return false;

        CommunicationLoggingHolder.communicationLoggingListener.onPortHolderMessage(getClass(), "Opening port: " + port);

        IoStream stream = SerialIoStreamJSerialComm.openPort(port);
        synchronized (portLock) {
            bp = BinaryProtocolHolder.getInstance().create(FileLog.LOGGER, stream);
            portLock.notifyAll();
        }

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
            if (bp != null) {
                try {
                    bp.close();
                    bp = null;
                } finally {
                    portLock.notifyAll();
                }
            }
        }
    }

    @Nullable
    public BinaryProtocol getBp() {
        return bp;
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
