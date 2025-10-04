package com.rusefi.io.can;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

// see also test_lua_e65.cpp
public class BmwChecksumTest {
    @Test
    public void test() {
        assertEquals(0x35, run(new byte[]{0, 0x06, 0x27, 0x11, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF}));
    }

    private int run(byte[] bytes) {

        int canId = 0x6F4;
        int checksum = canId;
        int offset = 1;
        for (int i = offset; i < bytes.length; i++) {
            checksum = checksum + bytes[i];
        }
        checksum = (checksum >> 8) + (checksum & 0xff);
        return checksum;
    }
}
