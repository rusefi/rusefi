package com.rusefi.io;

import com.opensr5.io.DataListener;
import com.opensr5.io.WriteStream;

import java.io.IOException;

/**
 * Physical bi-directional controller communication level
 *
 * (c) Andrey Belomutskiy
 *
 * 5/11/2015.
 */
public interface IoStream extends WriteStream {
    /**
     * @param listener would be invoked from unknown implementation-dependent thread
     */
    void setInputListener(DataListener listener);

    boolean isClosed();

    void close();

    /**
     * purges pending input and output
     */
    void purge();
}
