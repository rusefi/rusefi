package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.Logger;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.DataListener;
import com.rusefi.io.LinkManager;
import jssc.SerialPort;
import jssc.SerialPortException;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.concurrent.*;

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

    public PortHolderListener portHolderListener = PortHolderListener.VOID;
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

    public boolean open(String port, final DataListener listener) {
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

        bp = new BinaryProtocol(FileLog.LOGGER, serialPort);

        bp.switchToBinaryProtocol();
        bp.readImage(BinaryProtocol.IMAGE_SIZE);
        if (bp.isClosed)
            return false;

        if (!LinkManager.COMMUNICATION_QUEUE.isEmpty()) {
            System.out.println("Current queue: " + LinkManager.COMMUNICATION_QUEUE.size());
        }
        Runnable textPull = new Runnable() {
            @Override
            public void run() {
                while (!bp.isClosed) {
//                    FileLog.rlog("queue: " + LinkManager.COMMUNICATION_QUEUE.toString());
                    if (LinkManager.COMMUNICATION_QUEUE.isEmpty()) {
                        LinkManager.COMMUNICATION_EXECUTOR.submit(new Runnable() {
                            @Override
                            public void run() {
                                String text = bp.requestText();
                                if (text != null)
                                    listener.onDataArrived((text + "\r\n").getBytes());
                            }
                        });
                    }
                    sleep();
                }
                FileLog.MAIN.logLine("Stopping text pull");
            }
        };
        Thread tr = new Thread(textPull);
        tr.setName("text pull");
        tr.start();

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
        return true;
    }

    private void sleep() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
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
        FileLog.MAIN.logLine("Sending [" + command + "]");
        portHolderListener.onPortHolderMessage(PortHolder.class, "Sending [" + command + "]");

        Future f = LinkManager.COMMUNICATION_EXECUTOR.submit(new Runnable() {
            @Override
            public void run() {
                bp.sendTextCommand(command);
            }

            @Override
            public String toString() {
                return "Runnable for " + command;
            }
        });

        try {
            f.get(Timeouts.COMMAND_TIMEOUT_SEC, TimeUnit.SECONDS);
        } catch (ExecutionException e) {
            throw new IllegalStateException(e);
        } catch (TimeoutException e) {
            bp.getLogger().error("timeout, giving up: " + e);
            return;
        }
        /**
         * this here to make CommandQueue happy
         */
        MessagesCentral.getInstance().postMessage(PortHolder.class, CommandQueue.CONFIRMATION_PREFIX + command);


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
