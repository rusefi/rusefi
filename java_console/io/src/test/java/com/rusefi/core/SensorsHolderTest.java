package com.rusefi.core;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class SensorsHolderTest {
    private SensorsHolder holder;

    @BeforeEach
    void setUp() {
        holder = new SensorsHolder();
    }

    @Test
    void getValueForUnknownSensorReturnsNaN() {
        double value = holder.getValue("unknownSensor");
        assertTrue(Double.isNaN(value));
    }

    @Test
    void getValueForUnknownSensorEnumReturnsNaN() {
        double value = holder.getValue(Sensor.RPMGauge);
        assertTrue(Double.isNaN(value));
    }

    @Test
    void setValueThenGetValue() {
        holder.setValue(42.5, "testSensor");
        assertEquals(42.5, holder.getValue("testSensor"), 0.001);
    }

    @Test
    void setValueWithSensorEnumThenGetValue() {
        holder.setValue(1500.0, Sensor.RPMGauge);
        assertEquals(1500.0, holder.getValue(Sensor.RPMGauge), 0.001);
    }

    @Test
    void setValueReturnsTrueForNewValue() {
        boolean isUpdated = holder.setValue(100.0, "newSensor");
        assertTrue(isUpdated);
    }

    @Test
    void setValueReturnsTrueForDifferentValue() {
        holder.setValue(100.0, "sensor");
        boolean isUpdated = holder.setValue(200.0, "sensor");
        assertTrue(isUpdated);
    }

    @Test
    void setValueReturnsFalseForSameValue() {
        holder.setValue(100.0, "sensor");
        boolean isUpdated = holder.setValue(100.0, "sensor");
        assertFalse(isUpdated);
    }

    @Test
    void caseInsensitiveLookup() {
        holder.setValue(55.0, "TPSValue");

        assertEquals(55.0, holder.getValue("TPSValue"), 0.001);
        assertEquals(55.0, holder.getValue("tpsvalue"), 0.001);
        assertEquals(55.0, holder.getValue("TPSVALUE"), 0.001);
        assertEquals(55.0, holder.getValue("TpsValue"), 0.001);
    }

    @Test
    void caseInsensitiveOverwrite() {
        holder.setValue(10.0, "Sensor");
        holder.setValue(20.0, "SENSOR");

        assertEquals(20.0, holder.getValue("sensor"), 0.001);
    }

    @Test
    void multipleIndependentSensors() {
        holder.setValue(100.0, "sensor1");
        holder.setValue(200.0, "sensor2");
        holder.setValue(300.0, "sensor3");

        assertEquals(100.0, holder.getValue("sensor1"), 0.001);
        assertEquals(200.0, holder.getValue("sensor2"), 0.001);
        assertEquals(300.0, holder.getValue("sensor3"), 0.001);
    }

    @Test
    void setValueWithNaNIsValid() {
        holder.setValue(Double.NaN, "sensor");
        assertTrue(Double.isNaN(holder.getValue("sensor")));
    }

    @Test
    void setValueWithInfinityIsValid() {
        holder.setValue(Double.POSITIVE_INFINITY, "sensor");
        assertEquals(Double.POSITIVE_INFINITY, holder.getValue("sensor"));
    }

    @Test
    void setValueWithNegativeValueIsValid() {
        holder.setValue(-50.0, "sensor");
        assertEquals(-50.0, holder.getValue("sensor"), 0.001);
    }

    @Test
    void setValueWithZeroIsValid() {
        holder.setValue(0.0, "sensor");
        assertEquals(0.0, holder.getValue("sensor"), 0.001);
    }
}
