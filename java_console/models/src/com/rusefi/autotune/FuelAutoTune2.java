package com.rusefi.autotune;

import java.util.Collection;

/**
 * (c) Andrey Belomutskiy 2013-2018
 * 2/18/2016.
 */
public enum FuelAutoTune2 implements FuelAutoLogic {
    INSTANCE;

    private static final int SIZE = 16;


    @Override
    public Result process(boolean smooth, Collection<AfrDataPoint> dataECU, double GRAD, double targetAFR, double[][] VEcur) {
        double result[][] = new double[SIZE][SIZE];

        // proverka na statichnost' rezhimnoy tochki
        boolean fl_static = true;
        for (AfrDataPoint dataPoint : dataECU) {
            // TODO
            // proverka idet po trem poslednim dannym v dataECU
            // proverka po rpmIndex

            // proverka po engineLoadIndex

            // esli tochka ne statichna to fl_static = false
        }
        if (!fl_static)
            return null;
        // end
        AfrDataPoint s = dataECU.iterator().next();
        double delta = (s.getAfr() - targetAFR) / targetAFR; // privedennoe otklonenie po toplivu


        for (int r = 0; r < SIZE; r++) {          //rpmIndex
            for (int e = 0; e < SIZE; e++) {      //engineLoadIndex
                result[r][e] = (float) (VEcur[r][e] + VEcur[r][e] * delta * GRAD / Math.min(Math.max(Math.abs(s.getEngineLoadIndex() - e), Math.abs(s.getRpmIndex() - r)), 1));
            }
        }


        return new Result(result);
    }
}