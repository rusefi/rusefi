package com.rusefi.core;

import com.opensr5.ini.ImmutableIniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

public class SensorCentralTest {
    private SensorCentral sensorCentral;

    @BeforeEach
    void setUp() {
        sensorCentral = SensorCentral.getInstance();
    }

    @Test
    void singletonInstance() {
        SensorCentral instance1 = SensorCentral.getInstance();
        SensorCentral instance2 = SensorCentral.getInstance();
        assertSame(instance1, instance2);
    }

    @Test
    void setValueAndGetValueByString() {
        sensorCentral.setValue(123.0, "testSensor1");
        assertEquals(123.0, sensorCentral.getValue("testSensor1"), 0.001);
    }

    @Test
    void setValueAndGetValueBySensorEnum() {
        sensorCentral.setValue(2000.0, Sensor.RPMGauge.getNativeName());
        assertEquals(2000.0, sensorCentral.getValue(Sensor.RPMGauge), 0.001);
    }

    @Test
    void addListenerAndReceiveNotification() {
        AtomicReference<Double> receivedValue = new AtomicReference<>();

        sensorCentral.addListener("testSensor2", receivedValue::set);
        sensorCentral.setValue(99.0, "testSensor2");

        assertEquals(99.0, receivedValue.get(), 0.001);
    }

    @Test
    void addListenerWithSensorEnumAndReceiveNotification() {
        AtomicReference<Double> receivedValue = new AtomicReference<>();

        sensorCentral.addListener(SensorNames.MAPGauge, receivedValue::set);
        sensorCentral.setValue(101.3, SensorNames.MAPGauge);

        assertEquals(101.3, receivedValue.get(), 0.001);
    }

    @Test
    void listenerNotifiedMultipleTimes() {
        AtomicInteger callCount = new AtomicInteger(0);

        sensorCentral.addListener("testSensor3", value -> callCount.incrementAndGet());
        sensorCentral.setValue(1.0, "testSensor3");
        sensorCentral.setValue(2.0, "testSensor3");
        sensorCentral.setValue(3.0, "testSensor3");

        assertEquals(3, callCount.get());
    }

    @Test
    void removeListenerStopsNotification() {
        AtomicInteger callCount = new AtomicInteger(0);
        SensorCentral.SensorListener listener = value -> callCount.incrementAndGet();

        sensorCentral.addListener("testSensor4", listener);
        sensorCentral.setValue(1.0, "testSensor4");

        sensorCentral.removeListener("testSensor4", listener);
        sensorCentral.setValue(2.0, "testSensor4");

        assertEquals(1, callCount.get());
    }

    @Test
    void listenerTokenRemoval() {
        AtomicInteger callCount = new AtomicInteger(0);

        ISensorCentral.ListenerToken token = sensorCentral.addListener("testSensor5", value -> callCount.incrementAndGet());
        sensorCentral.setValue(1.0, "testSensor5");

        token.remove();
        sensorCentral.setValue(2.0, "testSensor5");

        assertEquals(1, callCount.get());
    }

    @Test
    void multipleListenersForSameSensor() {
        AtomicInteger callCount1 = new AtomicInteger(0);
        AtomicInteger callCount2 = new AtomicInteger(0);

        sensorCentral.addListener("testSensor6", value -> callCount1.incrementAndGet());
        sensorCentral.addListener("testSensor6", value -> callCount2.incrementAndGet());
        sensorCentral.setValue(42.0, "testSensor6");

        assertEquals(1, callCount1.get());
        assertEquals(1, callCount2.get());
    }

    @Test
    void caseInsensitiveListenerLookup() {
        AtomicReference<Double> receivedValue = new AtomicReference<>();

        sensorCentral.addListener("TestSensor7", receivedValue::set);
        sensorCentral.setValue(55.0, "testsensor7");

        assertEquals(55.0, receivedValue.get(), 0.001);
    }

    @Test
    void getValueSourceReturnsFunctionalSource() {
        sensorCentral.setValue(100.0, "testSensor8");
        ValueSource source = sensorCentral.getValueSource("testSensor8");

        assertEquals(100.0, source.getValue(), 0.001);

        sensorCentral.setValue(200.0, "testSensor8");
        assertEquals(200.0, source.getValue(), 0.001);
    }

    @Test
    void getValueSourceBySensorEnum() {
        sensorCentral.setValue(14.4, SensorNames.VBatt);
        ValueSource source = sensorCentral.getValueSource(SensorNames.VBatt);

        assertEquals(14.4, source.getValue(), 0.001);
    }

    @Test
    void setValueReturnsTrueForNewValue() {
        boolean isUpdated = sensorCentral.setValue(100.0, "newTestSensor9");
        assertTrue(isUpdated);
    }

    @Test
    void setValueReturnsFalseForSameValue() {
        sensorCentral.setValue(100.0, "testSensor10");
        boolean isUpdated = sensorCentral.setValue(100.0, "testSensor10");
        assertFalse(isUpdated);
    }

    @Test
    void listenerNotNotifiedWhenValueUnchanged() {
        AtomicInteger callCount = new AtomicInteger(0);

        sensorCentral.addListener("testSensor11", value -> callCount.incrementAndGet());
        sensorCentral.setValue(50.0, "testSensor11");
        sensorCentral.setValue(50.0, "testSensor11"); // same value — should not fire again

        assertEquals(1, callCount.get());
    }

    @Test
    void responseListenerNotifiedOnGrabSensorValues() {
        AtomicInteger callCount = new AtomicInteger(0);
        SensorCentral.ResponseListener listener = callCount::incrementAndGet;

        sensorCentral.addListener(listener);
        try {
            sensorCentral.grabSensorValues(new byte[0], IniFileModelMocks.empty(), null);
            sensorCentral.grabSensorValues(new byte[0], IniFileModelMocks.empty(), null);
            assertEquals(2, callCount.get());
        } finally {
            sensorCentral.removeListener(listener);
        }
    }

    @Test
    void responseListenerRemovalStopsNotification() {
        AtomicInteger callCount = new AtomicInteger(0);
        SensorCentral.ResponseListener listener = callCount::incrementAndGet;

        sensorCentral.addListener(listener);
        sensorCentral.grabSensorValues(new byte[0], IniFileModelMocks.empty(), null);
        sensorCentral.removeListener(listener);
        sensorCentral.grabSensorValues(new byte[0], IniFileModelMocks.empty(), null);

        assertEquals(1, callCount.get());
    }

    @Test
    void grabSensorValuesStoresResponseBytes() {
        byte[] payload = {1, 2, 3, 4, 5};
        sensorCentral.grabSensorValues(payload, IniFileModelMocks.empty(), null);
        assertSame(payload, sensorCentral.getResponse());
    }

    @Test
    void grabSensorValuesPublishesRuntimeDataRateGauge() throws InterruptedException {
        AtomicReference<Double> latestHz = new AtomicReference<>();
        SensorCentral.ListenerToken token = sensorCentral.addListener(
                ImmutableIniFileModel.RUNTIME_DATA_RATE_GAUGE, latestHz::set);
        try {
            // First frame: only one timestamp in window → published value is 0.0.
            sensorCentral.grabSensorValues(new byte[0], IniFileModelMocks.empty(), null);
            assertNotNull(latestHz.get(), "runtimeDataRateGauge should be published on first frame");
            assertEquals(0.0, latestHz.get(), 0.0001);

            // Second frame after a measurable delay: rate should be > 0 and finite.
            Thread.sleep(20);
            sensorCentral.grabSensorValues(new byte[0], IniFileModelMocks.empty(), null);
            double hz = latestHz.get();
            assertTrue(hz > 0.0 && Double.isFinite(hz),
                    "expected positive finite Hz, got " + hz);
        } finally {
            token.remove();
        }
    }

    @Test
    void getOutputChannelMapReturnsStableInstance() {
        Map<String, Double> map1 = sensorCentral.getOutputChannelMap();
        Map<String, Double> map2 = sensorCentral.getOutputChannelMap();
        assertNotNull(map1);
        assertSame(map1, map2);
    }

    @Test
    void onGaugeLabelsResolvedStoresLabels() {
        Map<String, ISensorHolder.ResolvedGaugeLabels> labels = Collections.singletonMap(
                "someGauge", new ISensorHolder.ResolvedGaugeLabels("Title", "Units"));
        sensorCentral.onGaugeLabelsResolved(labels);

        assertSame(labels, sensorCentral.getResolvedGaugeLabels());
        ISensorHolder.ResolvedGaugeLabels resolved = sensorCentral.getResolvedLabels("someGauge");
        assertNotNull(resolved);
        assertEquals("Title", resolved.getTitle());
        assertEquals("Units", resolved.getUnits());

        // Unknown gauge → null (not an exception).
        assertNull(sensorCentral.getResolvedLabels("noSuchGauge"));

        // Reset to empty so other tests aren't affected.
        sensorCentral.onGaugeLabelsResolved(Collections.emptyMap());
    }

    @Test
    void noListenerNoException() {
        // Setting a value with no listeners should not throw
        assertDoesNotThrow(() -> sensorCentral.setValue(42.0, "sensorWithNoListeners"));
    }

    @Test
    void removeNonExistentListenerNoException() {
        // Removing a listener that was never added should not throw
        SensorCentral.SensorListener listener = value -> {};
        assertDoesNotThrow(() -> sensorCentral.removeListener("nonExistentSensor", listener));
    }

}
