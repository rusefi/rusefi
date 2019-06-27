package com.rusefi.etb.test;

import com.rusefi.core.Sensor;
import com.rusefi.etb.ClosedLoopControlQualityMetric;
import org.junit.Test;

import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.Assert.assertEquals;

public class ClosedLoopControlQualityMetricTest {
    private static final double EPS = 0;

    @Test
    public void testPastDepth() {
        AtomicInteger targetValue = new AtomicInteger();
        AtomicInteger resultValue = new AtomicInteger();

        ClosedLoopControlQualityMetric m = new ClosedLoopControlQualityMetric(
                targetValue::doubleValue,
                resultValue::doubleValue,
                Sensor.ETB_CONTROL_QUALITY, 3);
        m.create(1000);

        m.add();
        assertEquals(0, m.getStandardDeviation(), EPS);

        targetValue.set(10);
        // result same 0
        m.add();
        assertEquals(0, m.getStandardDeviation(), EPS);
    }
}
