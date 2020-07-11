package com.rusefi.io.serial;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;

/**
 * https://github.com/Fazecast/jSerialComm looks to be alive as of 2020
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 06/03/2019
 */
public class SerialIoStreamJSerialComm implements IoStream {
    private boolean isClosed;
    private SerialPort sp;
    private final String port;
    private final Logger logger;
    private final IncomingDataBuffer dataBuffer;

    /**
     * @see #openPort(String, Logger)
     */
    private SerialIoStreamJSerialComm(SerialPort sp, String port, Logger logger) {
        this.sp = sp;
        this.port = port;
        this.logger = logger;
        this.dataBuffer = IncomingDataBuffer.createDataBuffer(this, logger);
    }

    @Override
    public void setInputListener(DataListener listener) {
        sp.addDataListener(new SerialPortDataListener() {
            @Override
            public int getListeningEvents() {
                return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
            }

            @Override
            public void serialEvent(SerialPortEvent event) {
                if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
                    return;
                int bytesAvailable = sp.bytesAvailable();
                if (bytesAvailable <= 0)
                    return; // sometimes negative value is returned at least on Mac
                byte[] newData = new byte[bytesAvailable];
                int numRead = sp.readBytes(newData, newData.length);
                byte[] data = new byte[numRead];
                System.arraycopy(newData, 0, data, 0, numRead);
                listener.onDataArrived(data);
                //System.out.println("Read " + numRead + " bytes.");
            }
        });

    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    @Override
    public void close() {
        logger.info(port + ": Closing port...");
        isClosed = true;
        sp.closePort();
        logger.info(port + ": Closed port.");
    }

    @Override
    public void write(byte[] bytes) {
        sp.writeBytes(bytes, bytes.length);
    }

    /**
     * Just open physical serial and not much more
     * @see PortHolder#connectAndReadConfiguration()
     */
    public static IoStream openPort(String port, Logger logger) {
        logger.info("[SerialIoStreamJSerialComm] openPort " + port);
        SerialPort serialPort = SerialPort.getCommPort(port);
        serialPort.setBaudRate(BaudRateHolder.INSTANCE.baudRate);
        serialPort.openPort(0);
//        FileLog.LOGGER.info("[SerialIoStreamJSerialComm] opened " + port);
        return new SerialIoStreamJSerialComm(serialPort, port, logger);
    }
}
