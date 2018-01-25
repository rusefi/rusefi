package com.rusefi.io.tcp;

import com.rusefi.FileLog;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * (c) Andrey Belomutskiy
 * 5/11/2015.
 */
public class TcpIoStream implements IoStream {
    private final InputStream input;
    private final OutputStream output;
    private boolean isClosed;

    public TcpIoStream(InputStream input, OutputStream output) {
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
    public void purge() {

    }

    @Override
    public void setInputListener(final DataListener listener) {
        LinkManager.TCP_READ_EXECUTOR.execute(new Runnable() {
            @Override
            public void run() {
                Thread.currentThread().setName("TCP connector loop");
                FileLog.MAIN.logLine("Running TCP connection loop");

                byte b[] = new byte[1];
                while (true) {
                    try {
                        int result = input.read(b);
                        if (result == -1)
                            throw new IOException("TcpIoStream: End of input?");
                        listener.onDataArrived(b);
                    } catch (IOException e) {
                        System.err.println("TcpIoStream: End of connection");
                        return;
                    }
                }
            }
        });

    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }
}
