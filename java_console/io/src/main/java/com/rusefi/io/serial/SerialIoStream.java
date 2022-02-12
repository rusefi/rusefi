package com.rusefi.io.serial;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.test.Bug3923;
import com.rusefi.io.IoStream;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class SerialIoStream extends AbstractIoStream {
    static Logging log = getLogging(SerialIoStream.class);

    protected final SerialPort sp;
    protected final String port;
    private boolean withListener;

    public SerialIoStream(SerialPort sp, String port) {
        this.sp = sp;
        this.port = port;
    }

    public static IoStream openPort(String port) {
        log.info("[SerialIoStream] openPort " + port);
        SerialPort serialPort = openSerial(port);
//        FileLog.LOGGER.info("[SerialIoStreamJSerialComm] opened " + port);
        return new SerialIoStream(serialPort, port);
    }

    @NotNull
    protected static SerialPort openSerial(String port) {
        SerialPort serialPort = SerialPort.getCommPort(port);
        serialPort.setBaudRate(BaudRateHolder.INSTANCE.baudRate);
        serialPort.openPort(0);
        return serialPort;
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        throw new UnsupportedOperationException();
    }

    @Override
    public void close() {
        log.info(port + ": Closing port...");
        super.close();
        sp.closePort();
        log.info(port + ": Closed port.");
    }

    @Override
    public void write(byte[] bytes) {
        if (Bug3923.obscene)
            log.info("Writing " + bytes.length + " byte(s)");
        sp.writeBytes(bytes, bytes.length);
    }

    @Override
    public void flush() throws IOException {
        sp.flushIOBuffers();
    }

    @Override
    public void setInputListener(DataListener listener) {
        if (withListener) {
            /**
             * it looks like some drivers do not handle change of listener properly
             * AndreyB had this problem at least on a random ELM327 clone with CH340 serial chip
             */
            throw new IllegalStateException("Not possible to change listener");
        }
        withListener = true;
        sp.addDataListener(new SerialPortDataListener() {
            private boolean isFirstEvent = true;

            @Override
            public int getListeningEvents() {
                return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
            }

            @Override
            public void serialEvent(SerialPortEvent event) {
                if (Bug3923.obscene)
                    log.info("serialEvent " + event);
                if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
                    return;
                if (isFirstEvent) {
                    // a hack to have explicit thread name, see https://github.com/Fazecast/jSerialComm/issues/308
                    Thread.currentThread().setName("Serial Port Event Thread");
                    isFirstEvent = false;
                }
                int bytesAvailable = sp.bytesAvailable();
                if (Bug3923.obscene)
                    log.info("serialEvent bytesAvailable " + bytesAvailable);
                if (bytesAvailable <= 0)
                    return; // sometimes negative value is returned at least on Mac
                byte[] newData = new byte[bytesAvailable];
                int numRead = sp.readBytes(newData, newData.length);
                byte[] data = new byte[numRead];
                System.arraycopy(newData, 0, data, 0, numRead);
                listener.onDataArrived(data);
            }
        });
    }
}
