package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.io.CommunicationLoggingHolder;
import com.rusefi.io.ConnectionStateListener;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import org.jetbrains.annotations.Nullable;

/**
 * This class holds the reference to the actual Serial port object
 * <p/>
 * 7/25/13
 * (c) Andrey Belomutskiy
 */
public class PortHolder {
    public static final String OS_VERSION = "os.version";
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
        // numeric winnt version for "Windows 10" is 7
        return System.getProperty(OS_VERSION).startsWith("7");
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
        if (bp == null)
            throw new NullPointerException("bp");
        bp.doSend(command, fireEvent);
    }

    public static PortHolder getInstance() {
        return instance;
    }
}
