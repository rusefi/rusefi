package com.rusefi.test;

import com.rusefi.CyclicBuffer;
import com.rusefi.DataBuffer;
import org.junit.Test;

import static junit.framework.Assert.assertEquals;

public class CyclicBufferTest {
    @Test
    public void test() {
        CyclicBuffer cb = new CyclicBuffer(3);

        cb.add(1);
        cb.add(3);

        assertEquals(2.0, DataBuffer.average(cb.getValues()));

        cb.add(111);
        cb.add(211);
        assertEquals(108.33333333333333, DataBuffer.average(cb.getValues()));

        assertEquals(147.1144679039647, DataBuffer.getStandardDeviation(cb.getValues()));
    }
}