package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;
import org.jetbrains.annotations.Nullable;

/**
 * Date: 12/25/12
 * (c) Andrey Belomutskiy
 */
public class JSSCPortReader {
    private static final int[] SLEEP_DURATIONS = {2, 20, 50, 100};
    private final SerialPort serialPort;
    private DataListener listener;

    public JSSCPortReader(final SerialPort serialPort, final DataListener listener, IoStream serialIoStreamJSSC) {
        this.serialPort = serialPort;
        this.listener = listener;
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (serialPort.isOpened()) {
                        byte[] data = progressiveSleepRead(serialPort);
                        if (data != null)
                            listener.onDataArrived(data);
                    }
                } catch (SerialPortException e) {
                    if (!serialIoStreamJSSC.isClosed()) {
                        e.printStackTrace();
                    }
                }
            }
        }, "Reader_" + serialPort).start();
    }

    /**
     * This method starts with shorter sleeps (which we do not know to what extent Windows handle) and then we sleep
     * a bit longer
     * @param serialPort port to read from
     * @return fresh data or null
     */
    @Nullable
    private static byte[] progressiveSleepRead(SerialPort serialPort) throws SerialPortException {
        for (int sleepDuration : SLEEP_DURATIONS) {
            byte[] data;
            synchronized (serialPort) {
                data = serialPort.readBytes();
            }
            if (data != null)
                return data;
            try {
                Thread.sleep(sleepDuration);
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
        }

        return null;
    }

    private SerialPortEventListener serialPortEventListener = new SerialPortEventListener() {


    public void serialEvent(SerialPortEvent spe) {
        if (spe.isRXCHAR() || spe.isRXFLAG()) {
// event-based serial read implementation does not work well on Windows 10 for some reason
// https://sourceforge.net/p/rusefi/tickets/264/
//            try {
//                handleRx(spe);
//            } catch (SerialPortException e) {
//                e.printStackTrace(System.err);
//            }
        } else if (spe.getEventType() != SerialPortEvent.TXEMPTY) {
            FileLog.MAIN.logLine("less expected JSSCPortReader serialEvent " + spe.getEventType());
        }
    }
    };

//    private void handleRx(SerialPortEvent spe) throws SerialPortException {
//        if (spe.getEventValue() > 0) {
//            byte[] buffer = serialPort.readBytes(spe.getEventValue());
//            listener.onDataArrived(buffer);
//            //           System.out.println("arrived [" + str + "]");
//        }
//    }

    public void readInitial() throws SerialPortException {
        int input = serialPort.getInputBufferBytesCount();
        FileLog.MAIN.logLine(input + " bytes in input buffer");
        while (serialPort.getInputBufferBytesCount() > 0) {
            byte[] data = serialPort.readBytes();
            if (data != null)
                listener.onDataArrived(data);
        }
    }

    public SerialPortEventListener getSerialPortEventListener() {
        return serialPortEventListener;
    }
}
