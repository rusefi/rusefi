package com.rusefi.io.serial;

import com.irnems.FileLog;
import com.irnems.core.EngineState;
import com.irnems.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.DataListener;
import jssc.SerialPort;
import jssc.SerialPortException;
import org.jetbrains.annotations.Nullable;

/**
 * This class holds the reference to the actual Serial port object
 * <p/>
 * 7/25/13
 * (c) Andrey Belomutskiy
 */
class PortHolder {
    //    private static final int BAUD_RATE = 8 * 115200;// 921600;
//    private static final int BAUD_RATE = 2 * 115200;
    private static final int BAUD_RATE = 115200;
    private static final int SECOND = 1000;
    private static final int MINUTE = 60 * SECOND;
    private static PortHolder instance = new PortHolder();
    private final Object portLock = new Object();

    public static long startedAt = System.currentTimeMillis();

    private PortHolder() {
    }

    @Nullable
    private SerialPort serialPort;

    void openPort(String port, final EngineState es) {
        MessagesCentral.getInstance().postMessage(SerialManager.class, "Opening port: " + port);
        if (port == null)
            return;
        open(port, new DataListener() {
            public void onStringArrived(String string) {
                //                jTextAreaIn.append(string);
                es.append(string);
            }
        });
    }

    public boolean open(String port, DataListener listener) {
        SerialPort serialPort = new SerialPort(port);
        try {
            FileLog.rlog("Opening " + port + " @ " + BAUD_RATE);
            boolean opened = serialPort.openPort();//Open serial port
            if (!opened)
                FileLog.rlog("opened: " + opened);
            serialPort.setParams(BAUD_RATE, 8, 1, 0);//Set params.
            int mask = SerialPort.MASK_RXCHAR;
            //Set the prepared mask
            serialPort.setEventsMask(mask);
            serialPort.addEventListener(new SerialPortReader(serialPort, listener));
        } catch (SerialPortException e) {
            FileLog.rlog("ERROR " + e.getMessage());
            return false;
        }

        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }

        synchronized (portLock) {
            this.serialPort = serialPort;
            portLock.notifyAll();
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
                    FileLog.rlog("Error while closing: " + e);
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
        FileLog.rlog("Sending [" + command + "]");
        MessagesCentral.getInstance().postMessage(CommandQueue.class, "Sending [" + command + "]");

        long now = System.currentTimeMillis();

        synchronized (portLock) {
            while (serialPort == null) {
                if (System.currentTimeMillis() - now > 3 * MINUTE)
                    MessagesCentral.getInstance().postMessage(PortHolder.class, "Looks like connection is gone :(");
                portLock.wait(MINUTE);
            }
            // we are here only when serialPort!=null, that means we have a connection
            try {
                serialPort.writeString(command + "\r\n");
            } catch (SerialPortException e) {
                throw new IllegalStateException(e);
            }
        }
    }

    public static PortHolder getInstance() {
        return instance;
    }
}
