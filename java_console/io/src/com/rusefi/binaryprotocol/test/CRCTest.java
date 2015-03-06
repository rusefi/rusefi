package com.rusefi.binaryprotocol.test;

import com.rusefi.binaryprotocol.CRC;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class CRCTest {
    @Test
    public void testCrc() {
        {
            byte[] a = {'A'};
            assertEquals(0xD3D99E8B, CRC.crc32(a, a.length));
        }
        {
            byte[] a = {'S'};
            assertEquals(0x2060EFC3, CRC.crc32(a, a.length));
        }
    }
}
