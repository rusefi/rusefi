package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.core.EngineState;
import com.rusefi.io.DataListener;
import jssc.SerialPort;
import jssc.SerialPortException;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

/**
 * This class holds the reference to the actual Serial port object
 * <p/>
 * 7/25/13
 * (c) Andrey Belomutskiy
 */
public class PortHolder {
    //    private static final int BAUD_RATE = 8 * 115200;// 921600;
//    private static final int BAUD_RATE = 2 * 115200;
    private static final int BAUD_RATE = 115200;
    private static final int SECOND = 1000;
    private static final int MINUTE = 60 * SECOND;
    private static PortHolder instance = new PortHolder();
    private final Object portLock = new Object();

    public PortHolderListener listener = PortHolderListener.VOID;

    private PortHolder() {
    }

    @Nullable
    private SerialPort serialPort;

    void openPort(String port, DataListener dataListener) {
        listener.onPortHolderMessage(SerialManager.class, "Opening port: " + port);
        if (port == null)
            return;
        open(port, dataListener);
    }

    public boolean open(String port, DataListener listener) {
        SerialPort serialPort = new SerialPort(port);
        try {
            FileLog.MAIN.logLine("Opening " + port + " @ " + BAUD_RATE);
            boolean opened = serialPort.openPort();//Open serial port
            if (!opened)
                FileLog.MAIN.logLine("not opened!");
            serialPort.setParams(BAUD_RATE, 8, 1, 0);//Set params.
            int mask = SerialPort.MASK_RXCHAR;
            //Set the prepared mask
            serialPort.setEventsMask(mask);
            serialPort.addEventListener(new SerialPortReader(serialPort, listener));
        } catch (SerialPortException e) {
            FileLog.rlog("ERROR " + e.getMessage());
            return false;
        }
        FileLog.rlog("PortHolder: Sleeping a bit");
        try {
            // todo: why is this delay here? add a comment
            Thread.sleep(200);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }

        synchronized (portLock) {
            this.serialPort = serialPort;
            portLock.notifyAll();
        }

        try {
            FileLog.rlog("PortHolder: test command");
            /**
             * Let's make sure we have not connected to Tuner Studio port?
             * @see EngineState#TS_PROTOCOL_TAG
             */
            doWriteCommand("test");
        } catch (SerialPortException e) {
            return false;
        }
        return true;
    }

    public void close() {
        synchronized (portLock) {
            if (serialPort != null) {
                try {
                    serialPort.closePort();
                    serialPort = null;
                } catch (SerialPortException e) {
                    FileLog.MAIN.logLine("Error while closing: " + e);
                } finally {
                    portLock.notifyAll();
                }
            }
        }
    }

    /**
     * this method blocks till a connection is available
     */
    public void packAndSend(String command) throws InterruptedException {
        FileLog.MAIN.logLine("Sending [" + command + "]");
        listener.onPortHolderMessage(PortHolder.class, "Sending [" + command + "]");

        long now = System.currentTimeMillis();

        synchronized (portLock) {
            while (serialPort == null) {
                if (System.currentTimeMillis() - now > 3 * MINUTE)
                    listener.onPortHolderMessage(PortHolder.class, "Looks like connection is gone :(");
                portLock.wait(MINUTE);
            }
            // we are here only when serialPort!=null, that means we have a connection
            try {
                doWriteCommand(command);
            } catch (SerialPortException e) {
                throw new IllegalStateException(e);
            }
        }
    }

    private void doWriteCommand(@NotNull String command) throws SerialPortException {
        if (serialPort == null)
            throw new NullPointerException("serialPort");
        serialPort.writeString(command + "\r\n");
    }

    public static PortHolder getInstance() {
        return instance;
    }
}
