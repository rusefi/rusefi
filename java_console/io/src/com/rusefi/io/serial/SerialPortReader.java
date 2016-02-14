package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.io.DataListener;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

/**
 * Date: 12/25/12
 * (c) Andrey Belomutskiy
 */
public class SerialPortReader implements SerialPortEventListener {
    private final SerialPort serialPort;
    private DataListener listener;

    public SerialPortReader(final SerialPort serialPort, final DataListener listener) {
        this.serialPort = serialPort;
        this.listener = listener;
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (serialPort.isOpened()) {
                        byte[] data;
                        synchronized (serialPort) {
                            data = serialPort.readBytes();
                        }
                        if (data != null) {
                            listener.onDataArrived(data);
                        } else {
                            Thread.sleep(100);
                        }
                    }
                } catch (SerialPortException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    throw new IllegalStateException(e);
                }
            }
        }, "Reader_" + serialPort).start();
    }

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
            FileLog.MAIN.logLine("less expected SerialPortReader serialEvent " + spe.getEventType());
        }
    }

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
}
