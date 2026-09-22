package com.rusefi.common;

import com.rusefi.RusefiTestBase;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorSubscription;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.function.DoublePredicate;

import static org.junit.Assert.assertTrue;

/** coverage for the ADC DMA completion on F4/F7 hardware (issue #10303). */
public class AdcHardwareTest extends RusefiTestBase {
    private static final String FAST_ADC_PERIOD = "fastAdcPeriod";
    private static final String FAST_ADC_CONVERSION_COUNT = "fastAdcConversionCount";
    private static final String SLOW_ADC_SCAN_COUNT = "slowAdcScanCount";
    private static final int UINT16_MODULUS = 1 << 16;

    @Test
    public void testFastAdcConversionCountProgressesAcrossFreshSamples() {
        List<Double> samples = awaitFreshSamples(FAST_ADC_CONVERSION_COUNT, 4);

        for (int i = 1; i < samples.size(); i++) {
            int previous = unsigned16(samples.get(i - 1));
            int current = unsigned16(samples.get(i));
            int delta = (current - previous + UINT16_MODULUS) % UINT16_MODULUS;
            assertTrue("Fast ADC conversion count did not advance between fresh samples "
                    + previous + " and " + current, delta > 0);
        }
    }

    @Test
    public void testFastAdcCompletionInitializesPeriod() {
        // This field starts at zero and is assigned only by the completion callback.
        // Ongoing callback activity is checked separately by the conversion counter.
        List<Double> samples = awaitFreshSamples(FAST_ADC_PERIOD, 3, sample -> sample > 0);

        for (double sample : samples) {
            assertTrue("Fast ADC completion published an invalid period: " + sample,
                    Double.isFinite(sample) && sample > 0);
        }
    }

    @Test
    public void testSlowAdcCompleteScanCountProgressesAcrossFreshSamples() {
        List<Double> samples = awaitFreshSamples(SLOW_ADC_SCAN_COUNT, 4);

        for (int i = 1; i < samples.size(); i++) {
            int previous = unsigned16(samples.get(i - 1));
            int current = unsigned16(samples.get(i));
            int delta = (current - previous + UINT16_MODULUS) % UINT16_MODULUS;
            assertTrue("Slow ADC complete scan count did not advance between fresh samples "
                    + previous + " and " + current, delta > 0);
        }
    }

    private static int unsigned16(double sample) {
        assertTrue("ADC counter is outside uint16 range: " + sample,
                Double.isFinite(sample) && sample >= 0 && sample < UINT16_MODULUS);
        return (int) sample;
    }

    private static List<Double> awaitFreshSamples(String sensorName, int sampleCount) {
        return awaitFreshSamples(sensorName, sampleCount, sample -> true);
    }

    private static List<Double> awaitFreshSamples(String sensorName, int sampleCount, DoublePredicate accepted) {
        SensorCentral sensors = SensorCentral.getInstance();
        CountDownLatch samplesReady = new CountDownLatch(sampleCount);
        List<Double> samples = new ArrayList<>();
        SensorCentral.ResponseListenerToken listener = sensors.addListener(() -> {
            double sample = sensors.getValue(sensorName);
            if (Double.isFinite(sample) && accepted.test(sample)) {
                synchronized (samples) {
                    if (samples.size() < sampleCount) {
                        samples.add(sample);
                        samplesReady.countDown();
                    }
                }
            }
        }, new SensorSubscription(sensorName));

        try {
            assertTrue("Timed out waiting for fresh " + sensorName + " samples",
                    samplesReady.await(15, TimeUnit.SECONDS));
            synchronized (samples) {
                return new ArrayList<>(samples);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new IllegalStateException("Interrupted waiting for " + sensorName, e);
        } finally {
            listener.remove();
        }
    }
}
