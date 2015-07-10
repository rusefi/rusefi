package com.rusefi.io.tcp;

import com.rusefi.FileLog;
import com.rusefi.io.DataListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * (c) Andrey Belomutskiy
 * 5/11/2015.
 */
public class TcpIoStream implements IoStream {
    private final OutputStream os;
    private final BufferedInputStream stream;

    public TcpIoStream(OutputStream os, BufferedInputStream stream) {
        this.os = os;
        this.stream = stream;
    }

    @Override
    public void close() {

    }

    @Override
    public void write(byte[] bytes) throws IOException {
        os.write(bytes);
        os.flush();
    }

    @Override
    public void purge() {

    }

    @Override
    public void setDataListener(final DataListener listener) {
        LinkManager.IO_EXECUTOR.execute(new Runnable() {
            @Override
            public void run() {
                Thread.currentThread().setName("TCP connector loop");
                FileLog.MAIN.logLine("Running TCP connection loop");

                byte b[] = new byte[1];
                while (true) {
                    try {
                        int result = stream.read(b);
                        if (result == -1) {
                            System.err.println("End of stream?");
                        }
                        listener.onDataArrived(b);
                    } catch (IOException e) {
                        System.err.println("End of connection");
                        return;
                    }
                }
            }
        });

    }
}
