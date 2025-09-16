package com.rusefi.libopenblt.transport;

import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.net.Socket;
import java.util.Arrays;

public class XcpNet implements IXcpTransport {
    private final Object mLock = new Object();

    private final String mHostname;
    private final int mPort;

    private TransportImpl mTransport;

    private void checkTransport() {
        if (mTransport == null) {
            throw new IllegalStateException("Transport is invalid");
        }
    }

    public XcpNet(String hostname, int port) {
        mHostname = hostname;
        mPort = port;
    }

    @Override
    public void connect() throws IOException {
        synchronized (mLock) {
            if (mTransport != null) {
                throw new IllegalStateException("Cannot connect when already connected");
            }
            mTransport = new TransportImpl(mHostname, mPort);
        }
    }

    @Override
    public void disconnect() throws IOException {
        synchronized (mLock) {
            if (mTransport != null) {
                mTransport.close();
                mTransport = null;
            }
        }
    }

    @Override
    public void close() throws IOException {
        disconnect();
    }

    private static class TransportImpl implements Closeable {
        private final Socket mSocket;

        private final InputStream mReader;
        private final OutputStream mWriter;

        TransportImpl(@NotNull String hostname, int port) throws IOException {
            mSocket = new Socket(hostname, port);
            mReader = mSocket.getInputStream();
            mWriter = mSocket.getOutputStream();
        }

        @Override
        public void close() throws IOException {
            mSocket.close();
        }

        void write(byte[] data) throws IOException {
            byte[] dataWithHeader = new byte[data.length + 1];

            dataWithHeader[0] = (byte)data.length;
            System.arraycopy(data, 0, dataWithHeader, 1, data.length);

            mWriter.write(dataWithHeader);
            mWriter.flush();
        }

        byte[] read(int len, int timeoutMs) throws IOException {
            byte[] readBuffer = new byte[len + 1];

            mSocket.setSoTimeout(timeoutMs);
            int actualRead = mReader.read(readBuffer);

            if (actualRead != readBuffer.length) {
                throw new IllegalStateException("Unexpected bytes read, requested " + readBuffer.length + " but got " + actualRead);
            }

            if (len != readBuffer[0]) {
                throw new IllegalStateException("Unexpected length of return packet, expected " + len + " but target returned " + (readBuffer[0] & 0xFF));
            }

            return Arrays.copyOfRange(readBuffer, 1, 1 + len);
        }
    }

    @Override
    public byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) throws IOException {
        synchronized (mLock) {
            checkTransport();

            mTransport.write(request);
            return mTransport.read(expectResponseBytes, timeoutMs);
        }
    }
}
