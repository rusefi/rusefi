package com.rusefi.io.serial;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.fazecast.jSerialComm.SerialPortThreadFactory;
import com.opensr5.io.DataListener;
import com.rusefi.NamedThreadFactory;
import com.rusefi.binaryprotocol.test.Bug3923;
import com.rusefi.core.OsUtil;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

public abstract class SerialIoStream extends AbstractIoStream {
    static final Logging log = getLogging(SerialIoStream.class);
    private static final int PROBE_WRITE_TIMEOUT_MS = 500;

    @Nullable // null in case of port open error, for instance lack of permissions on Unix
    protected final SerialPort sp;
    private final NativePort nativePort;
    protected final String port;
    private boolean withListener;

    static {
        log.info("Using com.fazecast.jSerialComm " + SerialPort.getVersion());
        // NamedThreadFactory has daemon=false by default and we like that!
        SerialPortThreadFactory.set(new NamedThreadFactory("ECU SerialIoStream jSerialComm"));
    }

    public SerialIoStream(@NotNull SerialPort sp, String port) {
        this(sp == null ? null : new NativePort(sp), port);
    }

    SerialIoStream(NativePort nativePort, String port) {
        this.nativePort = nativePort;
        this.sp = nativePort == null ? null : nativePort.serialPort;
        this.port = Objects.requireNonNull(port);
    }

    @Nullable
    protected static SerialPort openSerial(String port) {
        NativePort nativePort = openSerial(port, false, new NativePort(SerialPort.getCommPort(port)));
        return nativePort == null ? null : nativePort.serialPort;
    }

    @Nullable
    static NativePort openSerial(String port, boolean probe, NativePort nativePort) {
        nativePort.setBaudRate(BaudRateHolder.INSTANCE.baudRate);
        boolean openedOk = nativePort.openPort();
        if (!openedOk) {
            log.error("Error opening " + port + " maybe no permissions?");
            // todo: leverage jSerialComm method once we start using version 2.9+
            return null;
        }
        if (probe && nativePort.supportsWriteTimeout()) {
            // On Windows, even a nominally nonblocking jSerialComm write can wait
            // in native code. WRITE_BLOCKING makes its finite TX budget effective.
            // Do not enable it on POSIX: the TX budget is ignored and writes use tcdrain.
            // Leave reads nonblocking: buffered input still uses the data listener.
            // This is not a deadline for native open/flush/close or a broken driver.
            boolean configured = false;
            try {
                configured = nativePort.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0,
                    PROBE_WRITE_TIMEOUT_MS);
            } finally {
                if (!configured) {
                    nativePort.closePort();
                }
            }
            if (!configured) {
                log.error("Cannot configure serial probe write timeout on " + port);
                return null;
            }
        }
        // Discard any stale bytes left in the RX/TX queues by a prior
        // open/close cycle (e.g. scanner probe, prior console connection).
        nativePort.flushIOBuffers();
        return nativePort;
    }

    @Override
    public void close() {
        if (isClosed())
            return;
        log.info(port + ": Closing port...");
        super.close();
        nativePort.closePort();
        log.info(port + ": Closed port.");
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        if (Bug3923.obscene)
            log.info("Writing " + bytes.length + " byte(s)");
        if (nativePort == null) {
            throw new IOException("Port was never opened");
        }

        int written = nativePort.writeBytes(bytes);

        // If we failed to write all the bytes, the ECU probably disconnected
        if (written != bytes.length) {
            throw new IOException("write failed: wrote " + written + " but expected " + bytes.length);
        }
    }

    /** Narrow native-call seam: the real opener and writes use these same operations. */
    static class NativePort {
        final SerialPort serialPort;

        NativePort(SerialPort serialPort) {
            this.serialPort = serialPort;
        }

        void setBaudRate(int baudRate) {
            serialPort.setBaudRate(baudRate);
        }

        boolean openPort() {
            return serialPort.openPort();
        }

        boolean supportsWriteTimeout() {
            return OsUtil.isWindows();
        }

        boolean setComPortTimeouts(int mode, int readTimeoutMs, int writeTimeoutMs) {
            return serialPort.setComPortTimeouts(mode, readTimeoutMs, writeTimeoutMs);
        }

        void flushIOBuffers() {
            serialPort.flushIOBuffers();
        }

        void closePort() {
            serialPort.closePort();
        }

        int writeBytes(byte[] bytes) {
            return serialPort.writeBytes(bytes, bytes.length);
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
