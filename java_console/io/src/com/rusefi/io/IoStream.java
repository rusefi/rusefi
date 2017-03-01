package com.rusefi.io;

import com.opensr5.io.DataListener;

import java.io.IOException;

/**
 * Physical controller communication level
 *
 * (c) Andrey Belomutskiy
 *
 * 5/11/2015.
 */
public interface IoStream {
    /**
     * @param listener would be invoked from unknown implementation-dependent thread
     */
    void setDataListener(DataListener listener);

    /**
     * this blocking method would transmit the outgoing bytes
     */
    void write(byte[] bytes) throws IOException;

    void close();

    void purge();

    boolean isClosed();
}
