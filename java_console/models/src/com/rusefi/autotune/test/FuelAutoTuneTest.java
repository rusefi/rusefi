package com.rusefi.autotune.test;

import com.rusefi.autotune.FuelAutoTune;
import com.rusefi.autotune.Result;
import com.rusefi.autotune.AfrDataPoint;
import com.rusefi.config.Fields;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

/**
 * 1/5/2016
 * (c) Andrey Belomutskiy 2013-2017
 */
public class FuelAutoTuneTest {

    @Test
    public void testAutoTune() {
        List<AfrDataPoint> dataPoints = new ArrayList<>();
        dataPoints.add(AfrDataPoint.valueOf(13, 1200, 80));

        {
            Result r = FuelAutoTune.INSTANCE.process(false, dataPoints, 0.1, 13, createVeTable(1));
            printNotDefault(r.getKgbcRES(), 1);
        }

        dataPoints.add(AfrDataPoint.valueOf(13, 1200, 80));
        dataPoints.add(AfrDataPoint.valueOf(14, 1300, 60));
        dataPoints.add(AfrDataPoint.valueOf(15, 1400, 70));
        dataPoints.add(AfrDataPoint.valueOf(16, 1500, 90));

        for (int i = 0; i < 2000; i++)
            dataPoints.add(AfrDataPoint.valueOf(16, 1500 + i, 90));

        {
            Result r = FuelAutoTune.INSTANCE.process(false, dataPoints, 0.01, 13, createVeTable(1));
            printNotDefault(r.getKgbcRES(), 1);
        }

        for (int i = 0; i < 2000; i++)
            dataPoints.add(AfrDataPoint.valueOf(15, 1500 + i, 90));

        {
            Result r = FuelAutoTune.INSTANCE.process(false, dataPoints, 0.01, 13, createVeTable(1));
            printNotDefault(r.getKgbcRES(), 1);
        }


        // todo: validate results

    }

    /**
     * this method prints all values which do not equal default value
     */
    static void printNotDefault(float[][] array, double defaultValue) {
        for (int i = 0; i < array.length; i++) {
            printNotDefault(array[i], i, defaultValue);
        }
    }

    private static void printNotDefault(float[] array, int index, double defaultValue) {
        for (int i = 0; i < array.length; i++) {
            if (array[i] != defaultValue)
                System.out.println("Found value: x=" + index + " y=" + i + ": " + array[i]);
        }
    }

    static float[][] createVeTable(int value) {
        float veMap[][] = new float[Fields.FUEL_LOAD_COUNT][Fields.FUEL_RPM_COUNT];
        for (int engineLoadIndex = 0; engineLoadIndex < Fields.FUEL_LOAD_COUNT; engineLoadIndex++) {
            for (int rpmIndex = 0; rpmIndex < Fields.FUEL_RPM_COUNT; rpmIndex++) {
                veMap[engineLoadIndex][rpmIndex] = value;
            }
        }
        return veMap;
    }

}
