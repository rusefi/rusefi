package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import jssc.SerialPort;
import jssc.SerialPortException;

import java.io.IOException;

/**
 * (c) Andrey Belomutskiy
 * 5/11/2015.
 */
public class SerialIoStreamJSSC implements IoStream {
    private final SerialPort serialPort;
    private final Logger logger;
    private boolean isClosed;

    public SerialIoStreamJSSC(SerialPort serialPort, Logger logger) {
        this.serialPort = serialPort;
        this.logger = logger;
    }

    @Override
    public void close() {
        isClosed = true;
        try {
            FileLog.MAIN.logLine("CLOSING PORT...");
            serialPort.closePort();
            FileLog.MAIN.logLine("PORT CLOSED");
        } catch (SerialPortException e) {
            logger.error("Error closing port: " + e);
        }
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        try {
            synchronized (serialPort) {
                serialPort.writeBytes(bytes);
            }
        } catch (SerialPortException e) {
            throw new IOException(e);
        }
    }

    @Override
    public void purge() {
        try {
            synchronized (serialPort) {
                serialPort.purgePort(SerialPort.PURGE_RXCLEAR | SerialPort.PURGE_TXCLEAR);
            }
        } catch (SerialPortException e) {
            logger.info("Error while purge: " + e);
            close();
        }
    }

    @Override
    public void setInputListener(DataListener listener) {
        try {
            SerialPortReader reader = new SerialPortReader(serialPort, listener);
            serialPort.addEventListener(reader.getSerialPortEventListener());
            reader.readInitial();
        } catch (SerialPortException e) {
            throw new IllegalStateException(e);
        }
    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }
}
