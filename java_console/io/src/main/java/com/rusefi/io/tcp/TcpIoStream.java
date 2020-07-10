package com.rusefi.io.tcp;

import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.ByteReader;
import com.rusefi.io.IoStream;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 5/11/2015.
 */
public class TcpIoStream implements IoStream {
    private final InputStream input;
    private final OutputStream output;
    private final Logger logger;
    private boolean isClosed;
    private final IncomingDataBuffer dataBuffer;

    public TcpIoStream(Logger logger, Socket socket) throws IOException {
        InputStream input = new BufferedInputStream(socket.getInputStream());
        OutputStream output = socket.getOutputStream();
        this.logger = logger;
        if (input == null)
            throw new NullPointerException("input");
        if (output == null)
            throw new NullPointerException("output");
        this.output = output;
        this.input = input;
        this.dataBuffer = IncomingDataBuffer.createDataBuffer(this, logger);
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    @Override
    public void close() {
        isClosed = true;
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        output.write(bytes);
        output.flush();
    }

    @Override
    public void setInputListener(final DataListener listener) {

        ByteReader.runReaderLoop(listener, input::read, logger);
    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }
}
