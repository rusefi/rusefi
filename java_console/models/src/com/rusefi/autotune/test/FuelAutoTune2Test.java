package com.rusefi.autotune.test;

import com.rusefi.autotune.FuelAutoTune2;
import com.rusefi.autotune.Result;
import com.rusefi.autotune.AfrDataPoint;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

import static com.rusefi.autotune.test.FuelAutoTuneTest.createVeTable;

/**
 * 2/23/2016
 * (c) Andrey Belomutskiy 2013-2017
 */
public class FuelAutoTune2Test {

    @Test
    public void testAutoTune() {
        List<AfrDataPoint> dataPoints = new ArrayList<>();
        dataPoints.add(AfrDataPoint.valueOf(13, 1200, 80));

        {
            System.out.println("Running with one datapoint already at target afr");
            Result r = FuelAutoTune2.INSTANCE.process(false, dataPoints, 0.1, 13, createVeTable(1));
            FuelAutoTuneTest.printNotDefault(r.getKgbcRES(), 1);
        }

        dataPoints.add(AfrDataPoint.valueOf(13, 1200, 80));
        dataPoints.add(AfrDataPoint.valueOf(14, 1300, 60));
        dataPoints.add(AfrDataPoint.valueOf(15, 1400, 70));
        dataPoints.add(AfrDataPoint.valueOf(16, 1500, 90));

        for (int i = 0; i < 2000; i++)
            dataPoints.add(AfrDataPoint.valueOf(16, 1500 + i, 90));

        {
            System.out.println("Running with more datapoints");
            Result r = FuelAutoTune2.INSTANCE.process(false, dataPoints, 0.01, 13, createVeTable(1));
            FuelAutoTuneTest.printNotDefault(r.getKgbcRES(), 1);
        }

        for (int i = 0; i < 2000; i++)
            dataPoints.add(AfrDataPoint.valueOf(15, 1500 + i, 90));

        {
            System.out.println("Running with more datapoints");
            Result r = FuelAutoTune2.INSTANCE.process(false, dataPoints, 0.01, 13, createVeTable(1));
            FuelAutoTuneTest.printNotDefault(r.getKgbcRES(), 1);
        }


        // todo: validate results

    }
}
