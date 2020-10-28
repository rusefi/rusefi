package com.rusefi.sensor_logs;

import com.rusefi.core.Sensor;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.function.Function;

public class BinarySensorLogSandbox {
    public static void main(String[] args) {
        Map<Sensor, Double> values = new HashMap<>();

        Function<Sensor, Double> valueProvider = sensor -> {
            Double result = values.getOrDefault(sensor, 0.0);
            Objects.requireNonNull(result, "value for sensor");
            return result;
        };

        BinarySensorLog l = new BinarySensorLog(valueProvider,
                Sensor.TIME_SECONDS,
                Sensor.RPM,
                Sensor.VSS,
                Sensor.TPS,
                Sensor.tpsAccelFuel,
                Sensor.PPS,
                Sensor.veValue,
                Sensor.etbTarget,
                Sensor.etb1DutyCycle,
                Sensor.totalTriggerErrorCounter,
                Sensor.Lambda,
                Sensor.TARGET_AFR,
                Sensor.FIRMWARE_VERSION,
                Sensor.CLT);

        values.put(Sensor.TIME_SECONDS, 1.0);
        values.put(Sensor.RPM, 0.0);
        values.put(Sensor.VSS, 60.0);
        values.put(Sensor.FIRMWARE_VERSION, 20200101.0);
        values.put(Sensor.CLT, 29.0);

        l.writeSensorLogLine();

        for (int i = 2; i < 10; i++) {
            values.put(Sensor.TIME_SECONDS, (double) i);
            values.put(Sensor.RPM, 180.0 + i);
            values.put(Sensor.FIRMWARE_VERSION, 20200101.0);
            values.put(Sensor.CLT, 39.0);
            l.writeSensorLogLine();
        }

        l.close();
    }
}
