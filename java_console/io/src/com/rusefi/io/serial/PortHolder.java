package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.DataListener;
import com.rusefi.io.LinkManager;
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
    public static int BAUD_RATE = 115200;
    private static PortHolder instance = new PortHolder();
    private final Object portLock = new Object();

    // todo: rename class & move field to not-serial-specific class
    public static PortHolderListener portHolderListener = PortHolderListener.VOID;
    private BinaryProtocol bp;

    private PortHolder() {
    }

    @Nullable
    private SerialPort serialPort;

    boolean openPort(String port, DataListener dataListener, LinkManager.LinkStateListener listener) {
        this.portHolderListener.onPortHolderMessage(SerialManager.class, "Opening port: " + port);
        if (port == null)
            return false;
        boolean result = open(port, dataListener);
        if (result) {
            listener.onConnectionEstablished();
        } else {
            listener.onConnectionFailed();
        }
        return result;
    }

    private boolean open(String port, final DataListener listener) {
        SerialPort serialPort = new SerialPort(port);
        try {
            FileLog.MAIN.logLine("Opening " + port + " @ " + BAUD_RATE);
            boolean opened = serialPort.openPort();//Open serial port
            if (!opened)
                FileLog.MAIN.logLine("not opened!");
            setupPort(serialPort, BAUD_RATE);
//            serialPort.addEventListener(new SerialPortReader(serialPort, portHolderListener));
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

        synchronized (portLock) {
            this.serialPort = serialPort;
            portLock.notifyAll();
        }

        bp = new BinaryProtocol(FileLog.LOGGER, new SerialIoStream(serialPort, FileLog.LOGGER));

        return bp.connect(listener);

//
//        try {
//            FileLog.rlog("PortHolder: test command");
//            /**
//             * Let's make sure we have not connected to Tuner Studio port?
//             * @see EngineState#TS_PROTOCOL_TAG
//             */
//            doWriteCommand("test");
//        } catch (SerialPortException e) {
//            return false;
//        }
    }

    public static void setupPort(SerialPort serialPort, int baudRate) throws SerialPortException {
        serialPort.setParams(baudRate, 8, 1, 0);//Set params.
        int mask = SerialPort.MASK_RXCHAR;
        //Set the prepared mask
        serialPort.setEventsMask(mask);
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
    public void packAndSend(final String command) throws InterruptedException {
        bp.doSend(command);


//        long now = System.currentTimeMillis();
//
//        synchronized (portLock) {
//            while (serialPort == null) {
//                if (System.currentTimeMillis() - now > 3 * MINUTE)
//                    portHolderListener.onPortHolderMessage(PortHolder.class, "Looks like connection is gone :(");
//                portLock.wait(MINUTE);
//            }
//            // we are here only when serialPort!=null, that means we have a connection
//            try {
//                doWriteCommand(command);
//            } catch (SerialPortException e) {
//                throw new IllegalStateException(e);
//            }
//        }
    }

    private void doWriteCommand(@NotNull String command) throws SerialPortException {
        if (serialPort == null)
            throw new NullPointerException("serialPort");
        serialPort.writeBytes((command + "\n").getBytes());
    }

    public static PortHolder getInstance() {
        return instance;
    }
}
