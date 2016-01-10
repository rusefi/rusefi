package com.rusefi.autotune.test;

import com.rusefi.autotune.FuelAutoTune;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

/**
 * 1/5/2016
 * (c) Andrey Belomutskiy 2013-2016
 */
public class FuelAutoTuneTest {

    @Test
    public void testAutoTune() {
        List<FuelAutoTune.stDataOnline> dataPoints = new ArrayList<>();
        dataPoints.add(FuelAutoTune.stDataOnline.valueOf(13, 1200, 80));

        {
            FuelAutoTune.Result r = FuelAutoTune.process(false, dataPoints, 0.1, 13);
            printNotDefault(r.getKgbcRES(), 1);
        }

        dataPoints.add(FuelAutoTune.stDataOnline.valueOf(13, 1200, 80));
        dataPoints.add(FuelAutoTune.stDataOnline.valueOf(14, 1300, 60));
        dataPoints.add(FuelAutoTune.stDataOnline.valueOf(15, 1400, 70));
        dataPoints.add(FuelAutoTune.stDataOnline.valueOf(16, 1500, 90));

        for (int i = 0; i < 2000; i++)
            dataPoints.add(FuelAutoTune.stDataOnline.valueOf(16, 1500 + i, 90));

        {
            FuelAutoTune.Result r = FuelAutoTune.process(false, dataPoints, 0.01, 13);
            printNotDefault(r.getKgbcRES(), 1);
        }

        for (int i = 0; i < 2000; i++)
            dataPoints.add(FuelAutoTune.stDataOnline.valueOf(15, 1500 + i, 90));

        {
            FuelAutoTune.Result r = FuelAutoTune.process(false, dataPoints, 0.01, 13);
            printNotDefault(r.getKgbcRES(), 1);
        }


        // todo: validate results

    }

    /**
     * this method prints all values which do not equal default value
     */
    private static void printNotDefault(double[][] array, double defaultValue) {
        for (int i = 0; i < array.length; i++) {
            printNotDefault(array[i], i, defaultValue);
        }
    }

    private static void printNotDefault(double[] array, int index, double defaultValue) {
        for (int i = 0; i < array.length; i++) {
            if (array[i] != defaultValue)
                System.out.println("Found value: " + index + " " + i + ": " + array[i]);
        }
    }
}
