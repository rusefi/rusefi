package com.rusefi.libopenblt.transport;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortThreadFactory;
import com.rusefi.NamedThreadFactory;

import java.io.IOException;

public class XcpSerial implements IXcpTransport{
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
                mPort = null;
            }
        }
    }

    @Override
    public void close() throws IOException {
        disconnect();
    }

    @Override
    public byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) throws IOException {
        synchronized (mLock) {
            checkPort();

            mPort.setComPortTimeouts(
                SerialPort.TIMEOUT_READ_BLOCKING | SerialPort.TIMEOUT_WRITE_BLOCKING,
                timeoutMs, timeoutMs);

            // on serial there is a prefix byte both on request and response
            byte[] buf = new byte[1 + request.length];
            buf[0] = (byte)request.length;
            System.arraycopy(request, 0, buf, 1, request.length);

            int actualWritten = mPort.writeBytes(buf, buf.length);
            if (actualWritten != buf.length) {
                throw new IOException("Cannot write to serial, expected " + buf.length + " turned out " + actualWritten);
            }

            byte[] responseLen = new byte[1];
            int actualRead = mPort.readBytes(responseLen, 1);
            if (actualRead != 1) {
                throw new IOException("Cannot read response");
            }

            byte[] response = new byte[responseLen[0]];
            actualRead = mPort.readBytes(response, response.length);
            if (actualRead != expectResponseBytes ||
                responseLen[0] != expectResponseBytes) {
                throw new IOException("Unexpected bytes read on command " + Integer.toHexString(request[0] & 0xFF) +
                    ", requested " + expectResponseBytes + " but got " + actualRead + ": " + hexlify(response));
            }

            return response;
        }
    }

    static private String hexlify(byte [] bytes) {
        StringBuilder hexString = new StringBuilder();
        for (byte b : bytes) {
            if (hexString.length() > 0) {
                hexString.append(' ');
            }
            String hex = Integer.toHexString(b & 0xFF);
            if (hex.length() == 1) {
                hexString.append('0');
            }
            hexString.append(hex);
        }
        return hexString.toString();
    }
}
