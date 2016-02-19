package com.rusefi.autotune;

import java.util.Collection;

/**
 * (c) Andrey Belomutskiy 2013-2016
 2/18/2016.
 */
public enum FuelAutoTune2 implements FuelAutoLogic {
    INSTANCE;


    @Override
    public FuelAutoTune.Result process(boolean smooth, Collection<FuelAutoTune.stDataOnline> dataECU, double STEP, double targetAFR, float[][] kgbcINIT) {
        return null;
    }
}
