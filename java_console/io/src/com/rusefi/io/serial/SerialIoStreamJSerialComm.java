package com.rusefi.io.serial;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.opensr5.io.DataListener;
import com.rusefi.FileLog;
import com.rusefi.io.IoStream;

/**
 * https://github.com/Fazecast/jSerialComm looks to be alive as of 2019
 * <p>
 * (c) Andrey Belomutskiy
 * 06/03/2019
 */
public class SerialIoStreamJSerialComm implements IoStream {
    private boolean isClosed;
    private SerialPort sp;
    private final String port;

    SerialIoStreamJSerialComm(SerialPort sp, String port) {
        this.sp = sp;
        this.port = port;
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
    public void close() {
        FileLog.LOGGER.info(port + ": Closing port...");
        isClosed = true;
        sp.closePort();
        FileLog.LOGGER.info(port + ": Closed port.");
    }

    @Override
    public void purge() {

    }

    @Override
    public void write(byte[] bytes) {
        sp.writeBytes(bytes, bytes.length);
    }

    /**
     * Just open physical serial and not much more
     * @see PortHolder#connectAndReadConfiguration()
     */
    public static IoStream openPort(String port) {
        FileLog.LOGGER.info("[SerialIoStreamJSerialComm] " + port);
        SerialPort serialPort = SerialPort.getCommPort(port);
        serialPort.setBaudRate(BaudRateHolder.INSTANCE.baudRate);
        serialPort.openPort();
        return new SerialIoStreamJSerialComm(serialPort, port);
    }
}
