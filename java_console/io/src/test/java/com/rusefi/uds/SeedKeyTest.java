package com.rusefi.uds;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class SeedKeyTest {
    public static int Uds_Security_CalcKey(int secret, int seed, int rnd) {
        if (rnd < 220)
            rnd += 35;
        else
            rnd = 255;

        for (int i = 0; i < rnd; i++) {
            if (seed < 0)
                seed = secret ^ seed << 1;
            else
                seed <<= 1;
        }
        return seed;
    }

    @Test
    public void testAssert() {
        assertEquals(0xC42F15AE, Uds_Security_CalcKey(0xB24F5249, 0x5D8A2010, 0xF0));
    }
}
