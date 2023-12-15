package com.rusefi.io.can;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertThrows;

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

    @Test
    public void decodeUnevenInfoOffset() {
        byte[] destination = new byte[7];
        HexUtil.hexToBytes("1abcd", destination, 2);
        assertArrayEquals(new byte[]{0, 0, 1, (byte) 0xab, (byte) 0xcd, 0, 0}, destination);
    }

    @Test
    public void unexpected() {
      assertThrows(NumberFormatException.class, () -> {
        HexUtil.hexToBytes("0xff");});
    }

    @Test
    public void asString() {
        assertEquals("AB01", HexUtil.asString(new byte[]{(byte) 0xab, 1}));
    }
}
