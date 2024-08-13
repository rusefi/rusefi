package com.rusefi.io.tcp;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.serial.AbstractIoStream;

import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static com.devexperts.logging.Logging.getLogging;

public interface ByteReader {
    Logging log = getLogging(ByteReader.class);

    static void runReaderLoop(String loggingPrefix, DataListener listener, ByteReader reader, AbstractIoStream ioStream) {
        /*
         * Threading of the whole input/output does not look healthy at all!
         *
         * @see #COMMUNICATION_EXECUTOR
         */
        Executor threadExecutor = Executors.newSingleThreadExecutor(BinaryProtocolServer.getThreadFactory(loggingPrefix + "TCP reader"));

        threadExecutor.execute(() -> {
            log.info(loggingPrefix + "Running TCP connection loop");

            byte[] inputBuffer = new byte[Fields.BLOCKING_FACTOR * 2];
            while (!ioStream.isClosed()) {
                try {
                    int result = reader.read(inputBuffer);
                    if (result == -1)
                        throw new IOException("TcpIoStream: End of input?");
                    listener.onDataArrived(Arrays.copyOf(inputBuffer, result));
                } catch (IOException e) {
                    log.error("TcpIoStream: End of connection " + e);
                    ioStream.close();
                    return;
                }
            }
        });
    }

    int read(byte[] buffer) throws IOException;
}
