package com.rusefi.libopenblt.transport;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortThreadFactory;
import com.rusefi.NamedThreadFactory;
import com.rusefi.util.HexBinary;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class XcpSerial implements IXcpTransport {
    private static final Logging log = getLogging(XcpSerial.class);
    private static final boolean VERBOSE;

    static {
        String KEY = "XcpSerial.VERBOSE";
        VERBOSE = Boolean.getBoolean(KEY);
        log.info(KEY + "=" + VERBOSE);
    }

    private final Object mLock = new Object();

    private final String mPortName;

    private SerialPort mPort;

    private void checkPort() {
        if (mPort == null) {
            throw new IllegalStateException("Port is null");
        }

        if (!mPort.isOpen()) {
            throw new IllegalStateException("Port is not open");
        }
    }

    public XcpSerial(String portName) {
        mPortName = portName;

        // NamedThreadFactory has daemon=false by default and we like that!
        SerialPortThreadFactory.set(new NamedThreadFactory("jSerialComm"));
    }


    @Override
    public void connect() throws IOException {
        synchronized (mLock) {
            if (mPort != null) {
                throw new IllegalStateException("Cannot connect when already connected");
            }

            SerialPort port = SerialPort.getCommPort(mPortName);
            if (!port.openPort()) {
                throw new IOException("Failed to connect to serial port: " + mPortName);
            }

            // Opened successfully, store port
            mPort = port;
        }
    }

    @Override
    public void disconnect() {
        synchronized (mLock) {
            if (mPort != null) {
                mPort.closePort();
                log.info("disconnect " + mPortName);
                mPort = null;
            }
        }
    }

    @Override
    public byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) throws IOException {
        synchronized (mLock) {
            checkPort();

            if (VERBOSE) {
                log.info("setComPortTimeouts=" + timeoutMs);
            }
            mPort.setComPortTimeouts(
                SerialPort.TIMEOUT_READ_BLOCKING | SerialPort.TIMEOUT_WRITE_BLOCKING,
                timeoutMs, timeoutMs);

            // on serial there is a prefix byte both on request and response
            byte[] buf = new byte[1 + request.length];
            buf[0] = (byte)request.length;
            System.arraycopy(request, 0, buf, 1, request.length);

            if (VERBOSE) {
                log.info("writeBytes " + HexBinary.printHexBinary(buf));
            }
            int actualWritten = mPort.writeBytes(buf, buf.length);
            if (VERBOSE) {
                log.info("actualWritten " + actualWritten);
            }
            if (actualWritten != buf.length) {
                throw new IOException("Cannot write to serial, expected " + buf.length + " turned out " + actualWritten);
            }

            byte[] responseLen = new byte[1];
            int actualRead = mPort.readBytes(responseLen, 1);
            if (VERBOSE) {
                log.info("actualRead " + actualRead);
            }
            if (actualRead != 1) {
                throw new IOException("XcpSerial: Cannot read response actual read=" + actualRead + "; request length was " + request.length + ", timeoutMs=" + timeoutMs);
            }
            if (VERBOSE) {
                log.info("actualRead responseLen " + HexBinary.printHexBinary(responseLen));
            }

            byte[] response = new byte[responseLen[0]];
            actualRead = mPort.readBytes(response, response.length);
            if (VERBOSE) {
                log.info("actualRead response " + HexBinary.printHexBinary(response));
            }
            //if (actualRead != expectResponseBytes ||
            //    responseLen[0] != expectResponseBytes) {
            //    throw new IOException("Unexpected bytes read on command " + Integer.toHexString(request[0] & 0xFF) +
            //        ", requested " + expectResponseBytes + " but got " + actualRead + ": " + HexBinary.printHexBinary(response));
            //}

            return response;
        }
    }
}
