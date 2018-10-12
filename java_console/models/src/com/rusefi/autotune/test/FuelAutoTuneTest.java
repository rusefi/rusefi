package com.rusefi.autotune.test;

import com.rusefi.autotune.*;
import com.rusefi.config.Fields;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertEquals;

/**
 * 1/5/2016
 * (c) Andrey Belomutskiy 2013-2018
 */
public class FuelAutoTuneTest {
    private static final double EPS = 0.00001;

    @Test
    public void testAlreadyTuned() {
        double value = 0.7;
        double[][] veTable = createVeTable(value);
        List<AfrDataPoint> dataPoints = new ArrayList<>();

        for (int i = 0; i < 200; i++)
            dataPoints.add(AfrDataPoint.valueOf(FuelAutoTune._14_7, 1500 + i, 50));
        Result r = FuelAutoTune.INSTANCE.process(false, dataPoints, 0.1, FuelAutoTune._14_7, veTable);

        assertEquals(0, countNotEqual(r.getKgbcRES(), value));
    }

    @Test
    public void testOptimizeOneCell() {

        double value = 0.7;
        double[][] veTable = createVeTable(value);
        List<AfrDataPoint> allSamePoints = new ArrayList<>();

        for (int i = 0; i < 200; i++)
            allSamePoints.add(AfrDataPoint.valueOf(10, 1500, 50));

        Result r = FuelAutoTune.INSTANCE.process(false, allSamePoints, 0.1, FuelAutoTune._14_7, veTable);

        printNotDefault(r.getKgbcRES(), value);
        assertEquals(0.5, r.getKgbcRES()[6][3], EPS);
        assertEquals(1, countNotEqual(r.getKgbcRES(), value));
    }


    private int countNotEqual(double[][] array2D, double value) {
        int result = 0;
        for (double array[] : array2D) {
            for (double v : array)
                if (v != value)
                    result++;
        }
        return result;
    }


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
    static void printNotDefault(double[][] array, double defaultValue) {
        for (int i = 0; i < array.length; i++) {
            printNotDefault(array[i], i, defaultValue);
        }
    }

    private static void printNotDefault(double[] array, int index, double defaultValue) {
        for (int i = 0; i < array.length; i++) {
            if (array[i] != defaultValue)
                System.out.println("Found value: x=" + index + " y=" + i + ": " + array[i]);
        }
    }

    static double[][] createVeTable(double value) {
        double veMap[][] = new double[Fields.FUEL_LOAD_COUNT][Fields.FUEL_RPM_COUNT];
        MathUtil.setArray2D(veMap, value);
        return veMap;
    }

}
