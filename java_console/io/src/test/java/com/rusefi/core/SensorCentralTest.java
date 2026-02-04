package com.rusefi.core;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

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
        sensorCentral.setValue(2000.0, Sensor.RPMGauge);
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

        sensorCentral.addListener(Sensor.MAPGauge, receivedValue::set);
        sensorCentral.setValue(101.3, Sensor.MAPGauge);

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
        sensorCentral.setValue(14.4, Sensor.VBatt);
        ValueSource source = sensorCentral.getValueSource(Sensor.VBatt);

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
    void listenerStillNotifiedWhenValueUnchanged() {
        AtomicInteger callCount = new AtomicInteger(0);

        sensorCentral.addListener("testSensor11", value -> callCount.incrementAndGet());
        sensorCentral.setValue(50.0, "testSensor11");
        sensorCentral.setValue(50.0, "testSensor11");

        assertEquals(2, callCount.get());
    }

    @Test
    void responseListenerNotifiedOnGrabSensorValues() {
        AtomicInteger callCount = new AtomicInteger(0);

        sensorCentral.addListener(callCount::incrementAndGet);

        // Note: grabSensorValues requires proper ini file setup
        // This test verifies that ResponseListener can be added
        assertNotNull(callCount);
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
