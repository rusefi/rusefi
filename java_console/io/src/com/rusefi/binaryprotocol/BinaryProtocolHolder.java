package com.rusefi.binaryprotocol;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;

/**
 * At any given moment of time JVM manages one communication stream
 *
 *
 * (c) Andrey Belomutskiy
 * 6/21/2017.
 */
public enum BinaryProtocolHolder {
    INSTANCE;

    private BinaryProtocol currentStream;

    public static BinaryProtocolHolder getInstance() {
        return INSTANCE;
    }

    public BinaryProtocol create(final Logger logger, IoStream stream) {
        BinaryProtocol result = new BinaryProtocol(logger, stream);
        currentStream = result;
        return result;
    }

    public BinaryProtocol getCurrentStreamState() {
        return currentStream;
    }
}
