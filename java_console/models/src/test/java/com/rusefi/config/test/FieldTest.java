package com.rusefi.config.test;

import com.rusefi.config.FieldCommandResponse;
import com.rusefi.config.StringFormatter;
import com.rusefi.core.Pair;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class FieldTest {
    @Test
    public void testParse() {
        {
            assertNull(FieldCommandResponse.parseResponse("notint @768 is 21"));
        }
        {
            Pair<Integer, ?> p = FieldCommandResponse.parseResponse("int @768 is 21");
            assertNotNull(p);
            assertEquals(new Integer(768), p.first);
            assertEquals(21, p.second);
        }
        {
            Pair<Integer, ?> p = FieldCommandResponse.parseResponse("float @808 is 1.00");
            assertNotNull(p);
            assertEquals(new Integer(808), p.first);
            assertEquals(1.0, p.second);
        }
    }

    @Test
    public void testPrecisionDependingOnScale() {
        assertEquals("0.123", StringFormatter.niceToString(0.12302, 3));
        assertEquals("0.1232", StringFormatter.niceToString(0.12317, 4));

        assertEquals("1234567.123", StringFormatter.niceToString(1234567.123, 3));
        assertEquals("10000.0", StringFormatter.niceToString(10000.00002, 4));
        assertEquals("0.002", StringFormatter.niceToString(0.002, 4));
        assertEquals("12.302", StringFormatter.niceToString(12.302, 4));
        assertEquals("12.302", StringFormatter.niceToString(12.302, 3));
        assertEquals("12.31", StringFormatter.niceToString(12.312, 2));
        assertEquals("123.02", StringFormatter.niceToString(123.02, 4));
        assertEquals("60", StringFormatter.niceToString(60.0, 0));
        assertEquals("60.0", StringFormatter.niceToString(60.0, 1));
    }

}

