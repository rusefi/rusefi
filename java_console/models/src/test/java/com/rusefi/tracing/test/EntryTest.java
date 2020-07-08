package com.rusefi.tracing.test;

import com.rusefi.tracing.Entry;
import com.rusefi.tracing.Phase;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class EntryTest {
    @Test
    public void testToString() {
        Entry e = new Entry("hello", Phase.E, 0.1, 0, 0);

        assertEquals("{\"name\":\"hello\",\"ph\":\"E\",\"tid\":0,\"pid\":0,\"ts\":0.1}", e.toString());

    }
}
