package com.opensr5.io;

import java.io.IOException;

/**
 * (c) Andrey Belomutskiy
 * 3/1/2017
 */
public interface WriteStream {
    /**
     * this blocking method would transmit the outgoing bytes
     */
    void write(byte[] bytes) throws IOException;
}
