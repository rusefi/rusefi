package com.rusefi.sensor_logs;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.function.Function;

public class BinarySensorLogSandbox {
    public static void main(String[] args) {
        Map<String, Double> values = new HashMap<>();

        Function<String, Double> valueProvider = sensor -> {
            Double result = values.getOrDefault(sensor, 0.0);
            Objects.requireNonNull(result, "value for sensor");
            return result;
        };

        BinarySensorLog l = new BinarySensorLog(valueProvider,
                Arrays.asList(
/*                "SECONDS,
                "RPMValue,
                "VEHICLESPEEDKPH,
                "TPSVALUE,
                "tpsAccelFuel,
                "THROTTLEPEDALPOSITION,
                "veValue,
//                "etbTarget,
                "etb1DutyCycle,
                "totalTriggerErrorCounter,
                "LAMBDAVALUE,
//                "TARGET_AFR,
                "FIRMWAREVERSION,
                "COOLANT
 */
                ));

        values.put("SECONDS", 1.0);
        values.put("RPMGauge", 0.0);
        values.put("vehicleSpeedKph", 60.0);
        values.put("firmwareVersionGauge", 20200101.0);
        values.put("CLTGauge", 29.0);

        l.writeSensorLogLine();

        for (int i = 2; i < 10; i++) {
            values.put("SECONDS", (double) i);
            values.put("RPMGauge", 180.0 + i);
            values.put("firmwareVersionGauge", 20200101.0);
            values.put("CLTGauge", 39.0);
            l.writeSensorLogLine();
        }

        l.close();
    }
}
