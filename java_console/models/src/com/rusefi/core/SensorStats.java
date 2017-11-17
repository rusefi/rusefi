package com.rusefi.core;

import com.rusefi.CyclicBuffer;
import com.rusefi.SensorConversion;
import com.rusefi.waves.EngineReport;

/**
 * 7/26/13
 * (c) Andrey Belomutskiy
 */
public class SensorStats {

    /**
     * here we listen of source sensor
     * and on each 10th update post the difference between min and max values to widthDestination
     */
    public static void startRangeMeasurement(final Sensor source, final Sensor widthDestination) {
        SensorCentral.getInstance().addListener(source, new SensorCentral.SensorListener() {

            int counter;
            double min = Double.MAX_VALUE;
            double max = Double.MIN_VALUE;

            @Override
            public void onSensorUpdate(double value) {
                counter++;

                min = Math.min(value, min);
                max = Math.max(value, max);

                if (counter == 10) {
                    counter = 0;
                    double width = max - min;

                    SensorCentral.getInstance().setValue(width, widthDestination);

                    min = Double.MAX_VALUE;
                    max = Double.MIN_VALUE;
                }
            }
        });
    }

    /**
     * http://en.wikipedia.org/wiki/Standard_deviation
     */
    public static void startStandardDeviation(Sensor source, final Sensor destination) {
        final CyclicBuffer cb = new CyclicBuffer(30);

        SensorCentral.getInstance().addListener(source, new SensorCentral.SensorListener() {
                    @Override
                    public void onSensorUpdate(double value) {
                        cb.add(value);
                        if (cb.getSize() == cb.getMaxSize()) {
                            double stdDev = cb.getStandardDeviation();
                            SensorCentral.getInstance().setValue(stdDev, destination);
                        }
                    }
                }
        );
    }

    public static void startDelta(Sensor input1, final Sensor input2, final Sensor destination) {
        SensorCentral.getInstance().addListener(input1, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                double valueMs = 1.0 * (value - SensorCentral.getInstance().getValue(input2)) / EngineReport.SYS_TICKS_PER_MS;
                SensorCentral.getInstance().setValue(valueMs, destination);
            }
        });
    }

    public static void startConversion(final Sensor source, final Sensor destination, final SensorConversion conversion) {
        SensorCentral.getInstance().addListener(source, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                double converted = conversion.convertValue(value);
                SensorCentral.getInstance().setValue(converted, destination);
            }
        });
    }
}
