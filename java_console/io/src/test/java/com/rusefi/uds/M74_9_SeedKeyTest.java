package com.rusefi.uds;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class M74_9_SeedKeyTest {
    @Test
    public void testAssert() {
        assertEquals(0xC42F15AE, M74_9_SeedKeyCalculator.Uds_Security_CalcKey(M74_9_SeedKeyCalculator.BOOTLOADER_SECRET, 0x5D8A2010, 0xF0));
        assertEquals(0x001B6F78, M74_9_SeedKeyCalculator.Uds_Security_CalcKey(M74_9_SeedKeyCalculator.SECRET, 0x5DA0B808, 0xA4));
    }
}
