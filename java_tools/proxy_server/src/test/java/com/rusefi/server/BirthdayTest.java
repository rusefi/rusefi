package com.rusefi.server;

import org.junit.jupiter.api.Test;

import static com.rusefi.Timeouts.MINUTE;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.server.Birthday.humanReadableFormat;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class BirthdayTest {
    @Test
    public void testDurationString() {
        assertEquals("1s", humanReadableFormat(SECOND));
        assertEquals("1m", humanReadableFormat(MINUTE));
    }
}
