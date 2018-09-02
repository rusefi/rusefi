package com.rusefi.autotune;

import java.util.Collection;

/**
 * (c) Andrey Belomutskiy 2013-2017
 * 2/18/2016.
 */
public interface FuelAutoLogic {
    // void MainWindow::calckGBC(double STEP)
    Result process(boolean smooth, Collection<AfrDataPoint> dataECU, double STEP, double targetAFR, float[][] kgbcINIT);
}
