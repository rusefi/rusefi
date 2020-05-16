package com.rusefi.binaryprotocol;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;

import java.util.Objects;

/**
 * At any given moment of time JVM manages one communication stream
 *
 * TODO: remove this dead class?
 *
 * (c) Andrey Belomutskiy
 * 6/21/2017.
 */
public enum BinaryProtocolHolder {
    INSTANCE;

    public static BinaryProtocolHolder getInstance() {
        return INSTANCE;
    }

    public static BinaryProtocol create(final Logger logger, IoStream stream) {
        return new BinaryProtocol(logger, stream);
    }

    public static BinaryProtocol getCurrentStreamState() {
        Objects.requireNonNull(LinkManager.connector, "connector");
        return LinkManager.connector.getBinaryProtocol();
    }
}
