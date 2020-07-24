package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static com.devexperts.logging.Logging.getLogging;

public interface ByteReader {
    Logging log = getLogging(ByteReader.class);

    static void runReaderLoop(String loggingPrefix, DataListener listener, ByteReader reader, TcpIoStream.DisconnectListener disconnectListener) {
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
            log.info(loggingPrefix + "Running TCP connection loop");

            byte inputBuffer[] = new byte[Fields.BLOCKING_FACTOR * 2];
            while (true) {
                try {
                    int result = reader.read(inputBuffer);
                    if (result == -1)
                        throw new IOException("TcpIoStream: End of input?");
                    listener.onDataArrived(Arrays.copyOf(inputBuffer, result));
                } catch (IOException e) {
                    log.error("TcpIoStream: End of connection " + e);
                    disconnectListener.onDisconnect();
                    return;
                }
            }
        });
    }

    int read(byte[] buffer) throws IOException;
}
