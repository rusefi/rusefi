package com.rusefi.maintenance.libopenblt.transport;

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
            if (!mPort.openPort()) {
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

            mPort.writeBytes(request, request.length);

            byte[] response = new byte[expectResponseBytes];
            int actualRead = mPort.readBytes(response, response.length);

            if (actualRead != expectResponseBytes) {
                throw new IllegalStateException("Unexpected bytes read, requested " + expectResponseBytes + " but got " + actualRead);
            }

            return response;
        }
    }
}
