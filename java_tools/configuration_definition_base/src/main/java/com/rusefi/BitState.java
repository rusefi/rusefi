package com.rusefi;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/4/2015.
 */
public class BitState {
    private int bitIndex;

    public void incrementBitIndex(ConfigField cf, ConfigField next) {
        if (!cf.isBit()) {
            bitIndex = 0;
            return;
        }
        bitIndex++;
    }

    public void incrementBitIndex(ConfigField cf) {
        bitIndex++;
    }

    public int getByteOffset() {
        return (bitIndex / 32) * 4;
    }

    public int getBitIndex() {
        return bitIndex % 32;
    }

    public void reset() {
        bitIndex = 0;
    }

    public int get() {
        return bitIndex;
    }

    public static class TooManyBitsInARow extends RuntimeException {
        public TooManyBitsInARow(String s) {
            super(s);
        }
    }
}
