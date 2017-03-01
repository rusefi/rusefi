package com.rusefi.io;

import com.opensr5.io.DataListener;
import com.opensr5.io.WriteStream;

import java.io.IOException;

/**
 * Physical controller communication level
 *
 * (c) Andrey Belomutskiy
 *
 * 5/11/2015.
 */
public interface IoStream extends WriteStream {
    /**
     * @param listener would be invoked from unknown implementation-dependent thread
     */
    void setDataListener(DataListener listener);

    void close();

    void purge();

    boolean isClosed();
}
