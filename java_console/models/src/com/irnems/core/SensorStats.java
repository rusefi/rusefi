package com.irnems.core;

/**
 * 7/26/13
 * (c) Andrey Belomutskiy
 */
public class SensorStats {
    public static void start(final Sensor source, final Sensor destination) {

        SensorCentral.getInstance().addListener(source, new SensorCentral.AdcListener() {

            int counter;
            double min = Double.MAX_VALUE;
            double max = Double.MIN_VALUE;

            @Override
            public void onAdcUpdate(SensorCentral model, double value) {
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

}
