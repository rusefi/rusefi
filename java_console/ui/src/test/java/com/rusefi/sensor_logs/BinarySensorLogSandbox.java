package com.rusefi.sensor_logs;

import com.rusefi.core.Sensor;

import java.util.Arrays;
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
                Arrays.asList(
/*                Sensor.SECONDS,
                Sensor.RPMValue,
                Sensor.VEHICLESPEEDKPH,
                Sensor.TPSVALUE,
                Sensor.tpsAccelFuel,
                Sensor.THROTTLEPEDALPOSITION,
                Sensor.veValue,
//                Sensor.etbTarget,
                Sensor.etb1DutyCycle,
                Sensor.totalTriggerErrorCounter,
                Sensor.LAMBDAVALUE,
//                Sensor.TARGET_AFR,
                Sensor.FIRMWAREVERSION,
                Sensor.COOLANT
 */
                ));

        values.put(Sensor.SECONDS, 1.0);
        values.put(Sensor.RPMGauge, 0.0);
        values.put(Sensor.vehicleSpeedKph, 60.0);
        values.put(Sensor.firmwareVersionGauge, 20200101.0);
        values.put(Sensor.CLTGauge, 29.0);

        l.writeSensorLogLine();

        for (int i = 2; i < 10; i++) {
            values.put(Sensor.SECONDS, (double) i);
            values.put(Sensor.RPMGauge, 180.0 + i);
            values.put(Sensor.firmwareVersionGauge, 20200101.0);
            values.put(Sensor.CLTGauge, 39.0);
            l.writeSensorLogLine();
        }

        l.close();
    }
}
