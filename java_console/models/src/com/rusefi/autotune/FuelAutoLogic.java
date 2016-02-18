package com.rusefi.autotune;

import java.util.Collection;

/**
 * (c) Andrey Belomutskiy 2013-2016
 * 2/18/2016.
 */
public interface FuelAutoLogic {
    // void MainWindow::calckGBC(double STEP)
    FuelAutoTune.Result process(boolean smooth, Collection<FuelAutoTune.stDataOnline> dataECU, double STEP, double targetAFR, float[][] kgbcINIT);
}
