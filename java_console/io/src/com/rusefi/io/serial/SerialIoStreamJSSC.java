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

    public static void setupPort(SerialPort serialPort, int baudRate) throws SerialPortException {
        serialPort.setRTS(false);
        serialPort.setDTR(false);
        serialPort.setParams(baudRate, 8, 1, 0);//Set params.
        int mask = SerialPort.MASK_RXCHAR;
        //Set the prepared mask
        serialPort.setEventsMask(mask);
        serialPort.setFlowControlMode(0);
    }

    public static SerialIoStreamJSSC open(String port, int baudRate, Logger logger) {
        logger.info("[SerialIoStreamJSSC]");
        SerialPort serialPort = new SerialPort(port);
        try {
            FileLog.MAIN.logLine("Opening " + port + " @ " + baudRate);
            boolean opened = serialPort.openPort();//Open serial port
            if (!opened)
                FileLog.MAIN.logLine(port + ": not opened!");
            SerialIoStreamJSSC.setupPort(serialPort, baudRate);
        } catch (SerialPortException e) {
            FileLog.MAIN.logLine("ERROR " + e.getMessage());
            return null;
        }
        FileLog.MAIN.logLine("PortHolder: Sleeping a bit");
        try {
            // todo: why is this delay here? add a comment
            Thread.sleep(200);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
        SerialIoStreamJSSC stream = new SerialIoStreamJSSC(serialPort, logger);
        return stream;
    }

    @Override
    public void close() {
        isClosed = true;
        try {
            FileLog.MAIN.logLine("CLOSING PORT... "  + serialPort.getPortName());
            serialPort.removeEventListener();
            serialPort.closePort();
            FileLog.MAIN.logLine("PORT CLOSED: " + serialPort.getPortName());
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
            JSSCPortReader reader = new JSSCPortReader(serialPort, listener, this);
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
