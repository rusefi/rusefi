package com.rusefi;

/**
 * Consecutive 'bit' fields are packed into 32-bit words; a run longer than one word simply
 * continues in the next word, see ConfigFieldImpl#getSize word rollover logic.
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/4/2015.
 */
public class BitState {
    public static final int WORD_SIZE_BITS = 32;

    private int bitIndex;

    public void incrementBitIndex(ConfigField cf, ConfigField next) {
        if (!cf.isBit()) {
            bitIndex = 0;
            return;
        }
        incrementBitIndex(cf);
    }

    public void incrementBitIndex(ConfigField cf) {
        bitIndex++;
        if (bitIndex == WORD_SIZE_BITS) {
            bitIndex = 0;
        }
    }

    public void reset() {
        bitIndex = 0;
    }

    public void set(int value) {
        bitIndex = value;
    }

    public int get() {
        return bitIndex;
    }
}
