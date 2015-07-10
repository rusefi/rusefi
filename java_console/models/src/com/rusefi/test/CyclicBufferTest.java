package com.rusefi.test;

import com.rusefi.CyclicBuffer;
import org.junit.Test;

import static junit.framework.Assert.assertEquals;

public class CyclicBufferTest {
    @Test
    public void test() {
        CyclicBuffer cb = new CyclicBuffer(3);

        cb.add(1);
        cb.add(3);

        assertEquals(2.0, cb.average());

        cb.add(111);
        cb.add(211);
        assertEquals(108.33333333333333, cb.average());

        assertEquals(147.1144679039647, cb.getStandardDeviation());
    }
}