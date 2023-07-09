package com.rusefi.can232;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class Can232ParsingTest {
    @Test
    public void extT() {
        String string = "T0EF5000020200";

        CanPacket p = CanPacket.parse(string);

        assertEquals(0xEF50000, p.getId());
        assertEquals(2, p.getPayload().length);
    }
}
