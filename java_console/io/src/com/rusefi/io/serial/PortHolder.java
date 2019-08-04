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
import java.util.Objects;

/**
 * This class holds the reference to the actual Serial port object
 * <p/>
 * 7/25/13
 * (c) Andrey Belomutskiy
 */
public class PortHolder {
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
        IoStream stream;
        // todo: BUG: Mac version 10 also 'is windows10 == true' at the moment :)
        boolean windows10 = isWindows10();
        FileLog.MAIN.logLine("Is windows10: " + windows10);
        if (windows10) {
            // this implementation is way simpler but seems to kind of work, keeping just in case
            stream = SerialIoStreamJSerialComm.open(port, BAUD_RATE, FileLog.LOGGER);
        } else {
            stream = SerialIoStreamJSSC.open(port, BAUD_RATE, FileLog.LOGGER);
        }
        if (stream == null)
            return false;

        synchronized (portLock) {
            this.serialPort = stream;
            portLock.notifyAll();
        }

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
}
