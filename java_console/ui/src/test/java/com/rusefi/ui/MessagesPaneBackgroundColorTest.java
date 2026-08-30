package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import java.awt.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class MessagesPaneBackgroundColorTest {
    @Test
    public void parseHex() {
        assertEquals(new Color(0xFF, 0, 0), MessagesPaneBackgroundColor.parseColor("FF0000"));
        assertEquals(new Color(0, 0xAB, 0xCD), MessagesPaneBackgroundColor.parseColor("#00ABCD"));
        assertEquals(new Color(0x12, 0x34, 0x56), MessagesPaneBackgroundColor.parseColor("123456"));
    }

    @Test
    public void parseDecimalTriplet() {
        assertEquals(new Color(255, 128, 0), MessagesPaneBackgroundColor.parseColor("255,128,0"));
        assertEquals(new Color(1, 2, 3), MessagesPaneBackgroundColor.parseColor("1, 2, 3"));
    }

    @Test
    public void parseInvalid() {
        assertNull(MessagesPaneBackgroundColor.parseColor(""));
        assertNull(MessagesPaneBackgroundColor.parseColor("#"));
        assertNull(MessagesPaneBackgroundColor.parseColor("red"));
        assertNull(MessagesPaneBackgroundColor.parseColor("FFF"));
        assertNull(MessagesPaneBackgroundColor.parseColor("GG0000"));
        assertNull(MessagesPaneBackgroundColor.parseColor("1,2"));
        assertNull(MessagesPaneBackgroundColor.parseColor("1,2,3,4"));
        assertNull(MessagesPaneBackgroundColor.parseColor("256,0,0"));
        assertNull(MessagesPaneBackgroundColor.parseColor("-1,0,0"));
    }
}
