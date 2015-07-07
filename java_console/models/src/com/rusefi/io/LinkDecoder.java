package com.rusefi.io;

import com.rusefi.core.EngineState;

/**
 * (c) Andrey Belomutskiy
 * 6/20/2015.
 */
public interface LinkDecoder {
    LinkDecoder TEXT_PROTOCOL_DECODER = new LinkDecoder() {
        @Override
        public String unpack(String packet) {
            return EngineState.unpackString(packet);
        }
    };

    LinkDecoder VOID = new LinkDecoder() {
        @Override
        public String unpack(String packet) {
            return packet;
        }
    };

    String unpack(String packet);
}
