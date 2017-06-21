package com.rusefi.binaryprotocol;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;

/**
 * (c) Andrey Belomutskiy
 * 6/21/2017.
 */
public enum BinaryProtocolHolder {
    INSTANCE;

    // todo: fix this? less horrible but still weird!
    private BinaryProtocol instance;

    public static BinaryProtocolHolder getInstance() {
        return INSTANCE;
    }

    public BinaryProtocol get() {
        return instance;
    }

    public static BinaryProtocol create(final Logger logger, IoStream stream) {
        BinaryProtocol result = new BinaryProtocol(logger, stream);
        getInstance().instance = result;
        return result;
    }
}
