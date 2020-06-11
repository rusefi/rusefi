package com.rusefi.io;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/20/2015.
 */
public interface LinkDecoder {
    LinkDecoder VOID = new LinkDecoder() {};

    default String unpack(String packet) {
        return packet;
    }
}
