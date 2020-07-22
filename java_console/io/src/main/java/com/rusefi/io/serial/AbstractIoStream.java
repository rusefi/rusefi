package com.rusefi.io.serial;

import com.rusefi.io.IoStream;

public abstract class AbstractIoStream implements IoStream {
    private boolean isClosed;

    @Override
    public void close() {
        isClosed = true;
    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }
}
