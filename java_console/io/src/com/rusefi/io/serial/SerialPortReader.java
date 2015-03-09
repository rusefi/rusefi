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
    private SerialPort serialPort;
    private DataListener listener;

    public SerialPortReader(SerialPort serialPort, DataListener listener) {
        this.serialPort = serialPort;
        this.listener = listener;
    }

    public void serialEvent(SerialPortEvent spe) {
        if (spe.isRXCHAR() || spe.isRXFLAG()) {
            try {
                handleRx(spe);
            } catch (SerialPortException e) {
                e.printStackTrace(System.err);
            }
        } else {
            FileLog.rlog("less expected SerialPortReader serialEvent " + spe);
        }
    }

    private void handleRx(SerialPortEvent spe) throws SerialPortException {
        if (spe.getEventValue() > 0) {
            byte[] buffer = serialPort.readBytes(spe.getEventValue());
            listener.onDataArrived(buffer);
            //           System.out.println("arrived [" + str + "]");
        }
    }

}
