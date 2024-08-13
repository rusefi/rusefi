package com.rusefi.io.tcp;

import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.core.FileUtil;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.net.Socket;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 5/11/2015.
 */
public class TcpIoStream extends AbstractIoStream {
    private final InputStream input;
    private final OutputStream output;
    private final String loggingPrefix;
    private final DisconnectListener disconnectListener;
    @NotNull
    private final Socket socket;
    private final IncomingDataBuffer dataBuffer;

    public TcpIoStream(String loggingPrefix, Socket socket) throws IOException {
        this(loggingPrefix, socket, DisconnectListener.VOID);
    }

    public TcpIoStream(String loggingPrefix, Socket socket, DisconnectListener disconnectListener) throws IOException {
        this.loggingPrefix = loggingPrefix;
        this.disconnectListener = disconnectListener;
        if (socket == null)
            throw new NullPointerException("socket");
        this.socket = socket;
        input = new BufferedInputStream(socket.getInputStream());
        output = new BufferedOutputStream(socket.getOutputStream());
        dataBuffer = createDataBuffer(loggingPrefix);
    }

    @NotNull
    public static TcpIoStream open(String port) throws IOException {
        int portPart = TcpConnector.getTcpPort(port);
        String hostname = TcpConnector.getHostname(port);
        Socket socket = new Socket(hostname, portPart);

        return new TcpIoStream("[start] ", socket);
    }

    @Override
    public void close() {
        // we need to guarantee only one onDisconnect invocation for retry logic to be healthy
        synchronized (this) {
            if (!isClosed()) {
                super.close();
                disconnectListener.onDisconnect("on close");
            }
        }
        FileUtil.close(socket);
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        super.write(bytes);
        output.write(bytes);
    }

    @Override
    public void flush() throws IOException {
        super.flush();
        output.flush();
    }

    @Override
    public void setInputListener(final DataListener listener) {
        ByteReader.runReaderLoop(loggingPrefix, listener, input::read, this);
    }

    public interface DisconnectListener {
        DisconnectListener VOID = (String message) -> {

        };
        void onDisconnect(String message);
    }
}
