package com.rusefi.binaryprotocol.test;

import com.rusefi.binaryprotocol.IoHelper;
import org.junit.Test;

import java.util.Arrays;

import static org.junit.Assert.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class CRCTest {
    @Test
    public void testCrc() {
        {
            byte[] a = {'A'};
            assertEquals(0xD3D99E8B, IoHelper.getCrc32(a));
        }
        {
            byte[] a = {'S'};
            assertEquals(0x2060EFC3, IoHelper.getCrc32(a));
        }
    }

    @Test
    public void testPackPacket() {
        byte[] command = {'S'};
        byte[] expected = {0, 1, 0x53, 0x20, 0x60, -17, -61};
        byte[] actual = IoHelper.makeCrc32Packet(command);
        assertArrayEquals(Arrays.toString(expected) + " vs " + Arrays.toString(actual), expected, actual);
    }
}
