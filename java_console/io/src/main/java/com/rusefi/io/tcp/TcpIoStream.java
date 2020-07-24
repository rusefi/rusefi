package com.rusefi.io.tcp;

import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.ByteReader;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.shared.FileUtil;
import org.jetbrains.annotations.NotNull;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 5/11/2015.
 */
public class TcpIoStream extends AbstractIoStream {
    private final InputStream input;
    private final OutputStream output;
    private final Logger logger;
    private final String loggingPrefix;
    @NotNull
    private final Socket socket;
    private final IncomingDataBuffer dataBuffer;

    public TcpIoStream(Logger logger, Socket socket) throws IOException {
        this("", logger, socket);
    }

    public TcpIoStream(String loggingPrefix, Logger logger, Socket socket) throws IOException {
        this.loggingPrefix = loggingPrefix;
        if (socket == null)
            throw new NullPointerException("socket");
        this.socket = socket;
        InputStream input = new BufferedInputStream(socket.getInputStream());
        OutputStream output = socket.getOutputStream();
        this.logger = logger;
        if (output == null)
            throw new NullPointerException("output");
        this.output = output;
        this.input = input;
        this.dataBuffer = IncomingDataBuffer.createDataBuffer(loggingPrefix, this, logger);
    }

    @Override
    public void close() {
        super.close();
        FileUtil.close(socket);
    }

    @Override
    public String getLoggingPrefix() {
        return loggingPrefix;
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        output.write(bytes);
    }

    @Override
    public void flush() throws IOException {
        super.flush();
        output.flush();
    }

    @Override
    public void setInputListener(final DataListener listener) {

        ByteReader.runReaderLoop(loggingPrefix, listener, input::read, logger);
    }
}
