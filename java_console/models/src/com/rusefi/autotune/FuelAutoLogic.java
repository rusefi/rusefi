package com.rusefi.autotune;

import java.util.Collection;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 2/18/2016.
 */
public interface FuelAutoLogic {
    // void MainWindow::calckGBC(double STEP)
    Result process(boolean smooth, Collection<AfrDataPoint> dataECU, double STEP, double targetAFR, double[][] kgbcINIT);
}
