package com.rusefi;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/4/2015.
 */
public class BitState {
    private int bitIndex;

    public void incrementBitIndex(ConfigFieldImpl cf, ConfigFieldImpl next) {
        if (!cf.isBit()) {
            bitIndex = 0;
            return;
        }
        incrementBitIndex(cf);
    }

    public void incrementBitIndex(ConfigFieldImpl cf) {
        if (bitIndex == 32)
            throw new TooManyBitsInARow("Too many bits in a row: " + cf.getName());
        bitIndex++;
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
