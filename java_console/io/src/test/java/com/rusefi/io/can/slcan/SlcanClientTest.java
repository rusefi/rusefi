package com.rusefi.io.can.slcan;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class SlcanClientTest {
    @Test
    public void testParseFrame() {
        assertNotNull(SlcanClient.Frame.parse("t1230"));
        assertNotNull(SlcanClient.Frame.parse("T123456780"));
        assertNotNull(SlcanClient.Frame.parse("r1230"));
        assertNotNull(SlcanClient.Frame.parse("R123456780"));

        // Valid frame with data
        SlcanClient.Frame f = SlcanClient.Frame.parse("t1232AABB");
        assertNotNull(f);
        assertEquals(0x123, f.id);
        assertEquals(2, f.dlc);
        assertArrayEquals(new byte[]{(byte)0xAA, (byte)0xBB}, f.data);

        // Invalid frames
        assertNull(SlcanClient.Frame.parse("V123"));
        assertNull(SlcanClient.Frame.parse(""));
        assertNull(SlcanClient.Frame.parse("x1230"));
    }
}
