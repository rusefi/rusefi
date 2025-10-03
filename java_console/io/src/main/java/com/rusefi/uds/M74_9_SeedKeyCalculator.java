package com.rusefi.uds;

import com.devexperts.logging.Logging;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Itelma M74.9
 */
public class M74_9_SeedKeyCalculator {
    private static Logging log = getLogging(M74_9_SeedKeyCalculator.class);

    public static final int BOOTLOADER_SECRET = 0xB24F5249;
    public static final int SECRET = 0x57649392;

    public static int Uds_Security_CalcKey(int secret, int seed, int rnd) {
        rnd = rnd & 0xFF;
        int originalSeed = seed;
        if (rnd < 220) {
            rnd += 35;
        } else {
            rnd = 255;
        }

        for (int i = 0; i < rnd; i++) {
            if (seed < 0)
                seed = secret ^ seed << 1;
            else
                seed <<= 1;
        }
        log.info(String.format("seed %x secret %x rnd %x makes %x", originalSeed, secret, rnd, seed));
        return seed;
    }
}
