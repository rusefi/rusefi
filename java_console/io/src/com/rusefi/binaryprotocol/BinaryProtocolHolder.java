package com.rusefi.binaryprotocol;

/**
 * (c) Andrey Belomutskiy
 * 6/21/2017.
 */
public enum BinaryProtocolHolder {
    INSTANCE;

    // todo: fix this, this is HORRIBLE!
    @Deprecated
    public static BinaryProtocol instance;

    public static BinaryProtocolHolder getInstance() {
        return INSTANCE;
    }

    public BinaryProtocol get() {
        return instance;
    }
}
