package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.io.CommunicationLoggingHolder;
import com.rusefi.io.ConnectionStateListener;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import jssc.SerialPort;
import jssc.SerialPortException;
import org.jetbrains.annotations.Nullable;

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
        SerialPort serialPort = new SerialPort(port);
        try {
            FileLog.MAIN.logLine("Opening " + port + " @ " + BAUD_RATE);
            boolean opened = serialPort.openPort();//Open serial port
            if (!opened)
                FileLog.MAIN.logLine(port + ": not opened!");
            setupPort(serialPort, BAUD_RATE);
        } catch (SerialPortException e) {
            FileLog.MAIN.logLine("ERROR " + e.getMessage());
            return false;
        }
        FileLog.MAIN.logLine("PortHolder: Sleeping a bit");
        try {
            // todo: why is this delay here? add a comment
            Thread.sleep(200);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }

        SerialIoStreamJSSC stream = new SerialIoStreamJSSC(serialPort, FileLog.LOGGER);
        synchronized (portLock) {
            this.serialPort = stream;
            portLock.notifyAll();
        }

        bp = BinaryProtocolHolder.create(FileLog.LOGGER, stream);

        return bp.connectAndReadConfiguration(listener);
    }

    public static void setupPort(SerialPort serialPort, int baudRate) throws SerialPortException {
        serialPort.setRTS(false);
        serialPort.setDTR(false);
        serialPort.setParams(baudRate, 8, 1, 0);//Set params.
        int mask = SerialPort.MASK_RXCHAR;
        //Set the prepared mask
        serialPort.setEventsMask(mask);
        serialPort.setFlowControlMode(0);
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
