package com.rusefi.io;

import java.io.IOException;

/**
 * (c) Andrey Belomutskiy
 *
 * 5/11/2015.
 */
public interface IoStream {
    void close();

    void write(byte[] bytes) throws IOException;

    void purge();

    void addEventListener(DataListener listener);
}
