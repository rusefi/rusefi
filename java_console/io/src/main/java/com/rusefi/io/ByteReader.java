package com.rusefi.io;

import com.opensr5.Logger;
import com.opensr5.io.DataListener;

import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public interface ByteReader {
    static void runReaderLoop(String loggingPrefix, DataListener listener, ByteReader reader, Logger logger) {
        /**
         * Threading of the whole input/output does not look healthy at all!
         *
         * @see #COMMUNICATION_EXECUTOR
         */
        Executor threadExecutor = Executors.newSingleThreadExecutor(r -> {
            Thread t = new Thread(r, "IO executor thread");
            t.setDaemon(true);  // need daemon thread so that COM thread is also daemon
            return t;
        });

        threadExecutor.execute(() -> {
            Thread.currentThread().setName("TCP connector loop");
            logger.info(loggingPrefix + "Running TCP connection loop");

            byte inputBuffer[] = new byte[256];
            while (true) {
                try {
                    int result = reader.read(inputBuffer);
                    if (result == -1)
                        throw new IOException("TcpIoStream: End of input?");
                    listener.onDataArrived(Arrays.copyOf(inputBuffer, result));
                } catch (IOException e) {
                    logger.error("TcpIoStream: End of connection " + e);
                    return;
                }
            }
        });
    }

    int read(byte[] buffer) throws IOException;
}
