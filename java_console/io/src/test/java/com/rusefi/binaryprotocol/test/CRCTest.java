package com.rusefi.binaryprotocol.test;

import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.io.commands.HelloCommand;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertEquals;

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
        byte[] command = HelloCommand.HELLO_COMMAND;
        byte[] expected = {0, 1, 0x53, 0x20, 0x60, -17, -61};
        byte[] actual = IoHelper.makeCrc32Packet(command);
        Assertions.assertArrayEquals(expected, actual, Arrays.toString(expected) + " vs " + Arrays.toString(actual));
    }

    @Test
    public void testStopEngine() {
        // see 'cmd_stop_engine' in .ini file
        byte[] command = {'Z', 0, 0x24, 0, 0};
        byte[] expected = {0x00, 0x05, 0x5a, 0x00, 0x24, 0x00, 0x00, (byte) 0x8b, (byte) 0xc5, (byte) 0xce, 0x4b};
        byte[] actual = IoHelper.makeCrc32Packet(command);
        Assertions.assertArrayEquals(expected, actual, Arrays.toString(expected) + " vs " + Arrays.toString(actual));
    }
}
