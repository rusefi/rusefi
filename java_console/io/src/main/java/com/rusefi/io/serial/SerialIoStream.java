package com.rusefi.io.serial;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.fazecast.jSerialComm.SerialPortThreadFactory;
import com.opensr5.io.DataListener;
import com.rusefi.NamedThreadFactory;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.test.Bug3923;
import com.rusefi.io.IoStream;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

public abstract class SerialIoStream extends AbstractIoStream {
    static final Logging log = getLogging(SerialIoStream.class);

    @Nullable // null in case of port open error, for instance lack of permissions on Unix
    protected final SerialPort sp;
    protected final String port;
    private boolean withListener;

    static {
        log.info("Using com.fazecast.jSerialComm " + SerialPort.getVersion());
        // NamedThreadFactory has daemon=false by default and we like that!
        SerialPortThreadFactory.set(new NamedThreadFactory("ECU SerialIoStream jSerialComm"));
    }

    public SerialIoStream(@NotNull SerialPort sp, String port) {
        this.sp = sp;
        this.port = Objects.requireNonNull(port);
    }

    @Nullable
    protected static SerialPort openSerial(String port) {
        SerialPort serialPort = SerialPort.getCommPort(port);
        serialPort.setBaudRate(BaudRateHolder.INSTANCE.baudRate);
        boolean openedOk = serialPort.openPort();
        if (!openedOk) {
            log.error("Error opening " + port + " maybe no permissions?");
            // todo: leverage jSerialComm method once we start using version 2.9+
            return null;
        }
        return serialPort;
    }

    @Override
    public void close() {
        if (isClosed())
            return;
        log.info(port + ": Closing port...");
        super.close();
        sp.closePort();
        log.info(port + ": Closed port.");
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        if (Bug3923.obscene)
            log.info("Writing " + bytes.length + " byte(s)");
        if (sp == null)
            throw new IOException("Port was never opened");

        int written = sp.writeBytes(bytes, bytes.length);

        // If we failed to write all the bytes, the ECU probably disconnected
        if (written != bytes.length) {
            throw new IOException("write failed: wrote " + written + " but expected " + bytes.length);
        }
    }

    @Override
    public String toString() {
        return getClass().getName() + "@" + port;
    }

    @Override
    public void flush() throws IOException {
  //      sp.flushIOBuffers(); todo uncomment once we migrate to fresh reliable version of connector
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
        if (sp == null)
            return;
        sp.addDataListener(new SerialPortDataListener() {

            @Override
            public int getListeningEvents() {
                return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
//todo: requires jSerialComm newer than 2.7 even if we want it               return SerialPort.LISTENING_EVENT_DATA_AVAILABLE | SerialPort.LISTENING_EVENT_PORT_DISCONNECTED;
            }

            @Override
            public void serialEvent(SerialPortEvent event) {
                if (Bug3923.obscene)
                    log.info("serialEvent " + event.getEventType());

/*
requires jSerialComm newer than 2.7
                if (event.getEventType() == SerialPort.LISTENING_EVENT_PORT_DISCONNECTED) {
                    System.out.println("got event SerialPort.LISTENING_EVENT_PORT_DISCONNECTED");
                    return;
                }
*/
                if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
                    return;
                int bytesAvailable = sp.bytesAvailable();
                if (Bug3923.obscene)
                    log.info("serialEvent bytesAvailable " + bytesAvailable);
                if (bytesAvailable <= 0)
                    return; // sometimes negative value is returned at least on Mac
                byte[] data = new byte[bytesAvailable];
                int numRead = sp.readBytes(data, data.length);

                // Copy in to a smaller array if the read was incomplete
                if (numRead != bytesAvailable) {
                    byte[] dataSmaller = new byte[numRead];
                    System.arraycopy(data, 0, dataSmaller, 0, numRead);
                    data = dataSmaller;
                }

                listener.onDataArrived(data);
            }
        });
    }
}
