package com.rusefi.io.can;

import org.junit.Test;

import static org.junit.Assert.assertArrayEquals;

public class HexUtilTest {
    @Test
    public void decodeSimple() {
        byte[] pa = HexUtil.hexToBytes("abcd");
        assertArrayEquals(new byte[]{(byte) 0xab, (byte) 0xcd}, pa);
    }

    @Test
    public void decodeUneven() {
        byte[] pa = HexUtil.hexToBytes("1abcd");
        assertArrayEquals(new byte[]{1, (byte) 0xab, (byte) 0xcd}, pa);
    }

    @Test(expected = NumberFormatException.class)
    public void unexpected() {
        HexUtil.hexToBytes("0xff");
    }
}
