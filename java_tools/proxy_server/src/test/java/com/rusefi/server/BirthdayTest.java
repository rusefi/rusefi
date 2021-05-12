package com.rusefi.server;

import org.junit.Test;

import static com.rusefi.Timeouts.MINUTE;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.server.Birthday.humanReadableFormat;
import static org.junit.Assert.assertEquals;

public class BirthdayTest {
    @Test
    public void testDurationString() {
        assertEquals("1s", humanReadableFormat(SECOND));
        assertEquals("1m", humanReadableFormat(MINUTE));
    }
}
