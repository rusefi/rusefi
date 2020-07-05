package com.rusefi.io.tcp;

import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.io.ByteReader;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;

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
    private final LinkManager linkManager;
    private boolean isClosed;

    public TcpIoStream(Logger logger, LinkManager linkManager, Socket socket) throws IOException {
        this(logger, linkManager, new BufferedInputStream(socket.getInputStream()), socket.getOutputStream());
    }

    private TcpIoStream(Logger logger, LinkManager linkManager, InputStream input, OutputStream output) {
        this.logger = logger;
        this.linkManager = linkManager;
        if (input == null)
            throw new NullPointerException("input");
        if (output == null)
            throw new NullPointerException("output");
        this.output = output;
        this.input = input;
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
        ByteReader reader = buffer -> input.read(buffer);

        ByteReader.runReaderLoop(listener, reader, logger);
    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }
}
