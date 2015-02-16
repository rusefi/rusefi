package com.rusefi.config.test;

import com.rusefi.config.Field;
import com.rusefi.core.Pair;
import org.junit.Test;

import java.util.Iterator;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

public class FieldTest {
    @Test
    public void testParse() {
        {
            assertNull(Field.parseResponse("notint @768 is 21"));
        }
        {
            Pair<Integer, ?> p = Field.parseResponse("int @768 is 21");
            assertNotNull(p);
            assertEquals(new Integer(768), p.first);
            assertEquals(21, p.second);
        }
        {
            Pair<Integer, ?> p = Field.parseResponse("float @808 is 1.00");
            assertNotNull(p);
            assertEquals(new Integer(808), p.first);
            assertEquals(1.0, p.second);
        }
    }
}

