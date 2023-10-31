package com.rusefi.test;

import com.rusefi.DataBuffer;
import com.rusefi.TimeBasedBuffer;
import org.junit.Test;

import java.util.Arrays;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.Assert.*;

public class TimeBasedBufferTest {
    @Test
    public void testRealTruncate() throws InterruptedException {
        TimeBasedBuffer b = new TimeBasedBuffer(100);
        b.add(1);
        System.out.println(b.getSize()); // cannot really test size here since who know how time flies
        b.add(2);
        b.add(1);
        System.out.println(b.getSize()); // cannot really test size here since who know how time flies
        Thread.sleep(200);
        assertEquals(0, b.getSize());
    }

    @Test
    public void testTruncate() {
        AtomicLong time = new AtomicLong();
        TimeBasedBuffer b = new TimeBasedBuffer(100) {
            @Override
            protected long currentTimeMillis() {
                return time.get();
            }
        };

        b.add(1);
        assertEquals(1, b.getSize());
        b.add(2);
        assertEquals(1, b.getSize()); // value for same timestamp is overriden
        time.set(3);
        b.add(3);
        assertEquals(2, b.getSize()); // value for same timestamp is overriden
        double[] v = b.getValues();
        assertArrayEquals(new double[]{2, 3}, v, 0.0);

        assertEquals(0.707106, DataBuffer.getStandardDeviation(b.getValues()), 0.001);

        time.set(150);
        assertEquals(0, b.getSize());
    }
}
