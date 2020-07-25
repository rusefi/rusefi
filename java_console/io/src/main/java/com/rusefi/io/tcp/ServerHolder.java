package com.rusefi.io.tcp;

import java.io.Closeable;

public class ServerHolder implements Closeable {
    private boolean isClosed;

    public void close() {
        isClosed = true;
    }

    public boolean isClosed() {
        return isClosed;
    }
}
