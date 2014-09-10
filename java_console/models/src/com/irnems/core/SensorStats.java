package com.irnems.core;

import com.rusefi.CyclicBuffer;

/**
 * 7/26/13
 * (c) Andrey Belomutskiy
 */
public class SensorStats {
    public static void start(final Sensor source, final Sensor destination) {

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

                    SensorCentral.getInstance().setValue(width, destination);

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
        final CyclicBuffer sb = new CyclicBuffer(30);

        SensorCentral.getInstance().addListener(source, new SensorCentral.SensorListener() {
                    @Override
                    public void onSensorUpdate(double value) {
                        sb.add(value);

                        if (sb.getSize() == sb.getMaxSize()) {
                            double stdDev = sb.getStandardDeviation();
                            SensorCentral.getInstance().setValue(stdDev, destination);
                        }
                    }
                }
        );
    }

}
