package com.rusefi.io;

import com.rusefi.core.EngineState;

/**
 * (c) Andrey Belomutskiy
 * 6/20/2015.
 */
public interface LinkDecoder {
    LinkDecoder VOID = new LinkDecoder() {};

    default String unpack(String packet) {
        return packet;
    }
}
