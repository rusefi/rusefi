package com.rusefi.autotune;

import java.util.Collection;

import static com.rusefi.autotune.MathUtil.square;
import static com.rusefi.config.Fields.FUEL_LOAD_COUNT;
import static com.rusefi.config.Fields.FUEL_RPM_COUNT;

/**
 * 1/5/2016
 * (c) Andrey Belomutskiy 2013-2017
 */
public enum FuelAutoTune implements FuelAutoLogic {
    INSTANCE;

    // todo: eliminate this
    // Fields.FUEL_RPM_COUNT
    // Fields.FUEL_LOAD_COUNT
    public static final int SIZE = 16;
    public static final double _14_7 = 14.7;

    private static boolean isLogEnabled() {
        return true;
    }

    // void MainWindow::calckGBC(double STEP)
    @Override
    public Result process(boolean smooth, Collection<AfrDataPoint> dataECU, double STEP, double targetAFR, double[][] kgbcINIT) {
        double cellDeviation[][] = new double[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
        double totalDeviation = 0;
        double minSQtotal = Double.MAX_VALUE;
        double kgbcSQsumLastTotal = 1e+16;
        double ksq = 1000; //???? ??????????????????? ??????????
        double ke = 100; //???? ??????????
        double kg = 100; //???? ?????

        // let's could how many data points we have for each cell
        int bkGBC[][] = new int[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
        for (AfrDataPoint data : dataECU) {
            bkGBC[data.PRESS_RT_32()][data.RPM_RT_32()]++;
        }

        double result[][] = MathUtil.deepCopy(kgbcINIT);

        double ktgbcRES[][] = new double[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
        double ktgbcINIT[][] = new double[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];

        MathUtil.setArray2D(ktgbcINIT, 1);
        MathUtil.setArray2D(ktgbcRES, 1);

        int COUNT_THRESHOLD = 20; // minimal number of measurements in cell to be considered

        int minK = 0; // todo: what is this?
        while (minK <= 4) {
            for (int loadIndex = 0; loadIndex < FUEL_LOAD_COUNT; loadIndex++) {
                for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
                    if (bkGBC[loadIndex][rpmIndex] < COUNT_THRESHOLD)
                        continue;
                    //log("Processing " + r + "/c" + c);
                    totalDeviation = getTotalDeviationAfterCellOptimization(smooth, dataECU, STEP, targetAFR, kgbcINIT, cellDeviation, ksq, ke, kg, result, loadIndex, rpmIndex);
                }
            }


            if (totalDeviation < minSQtotal)
                minSQtotal = totalDeviation;
            if (Math.abs(minSQtotal - kgbcSQsumLastTotal) < 1e-10)
                minK++;
            kgbcSQsumLastTotal = totalDeviation;
            //ui->statusBar->showMessage(QString::number(gbcSQsum));
            //updateTableGBC();

        }

        log("return " + minK);
        return new Result(result);
    }

    /**
     * Optimize one table cell
     * @return total deviation of the whole map
     */
    private static double getTotalDeviationAfterCellOptimization(
            boolean smooth, Collection<AfrDataPoint> dataECU, double step,
            double targetAFR, double[][] kgbcINIT, double[][] cellDeviation,
            double ksq, double ke, double kg, double[][] result, int loadIndex, int rpmIndex) {
        double totalDeviation = Double.MAX_VALUE;
        double minTotalDeviation = Double.MAX_VALUE;
        double prevTotalDeviation;

        double currentStep = step;
        int goodValueCounter = 0;
        while (goodValueCounter <= 4) {
            prevTotalDeviation = totalDeviation;

            // todo: since we are only adjusting one cell there is not point to recalculate all deviations
            // todo: potential CPU usage optimization here
            countDeviation(dataECU, cellDeviation, result, kgbcINIT, targetAFR);

            totalDeviation = MathUtil.sumArray(cellDeviation);

            if (smooth) {
                totalDeviation = smooth(totalDeviation, ksq, ke, kg, result);
            }
            ////////////////////////////////////
            if (totalDeviation >= prevTotalDeviation)
                currentStep = -currentStep;
            //???? ?????? ?? ??????? ????? ???, ?? ? ?? ?????????? ??
        /*if(bkGBC[r][c]) */

//                        log("Adjusting " + currentStep);
            result[loadIndex][rpmIndex] += currentStep;

            minTotalDeviation = Math.min(minTotalDeviation, totalDeviation);

            if (Math.abs(minTotalDeviation - prevTotalDeviation) < 1e-10)
                goodValueCounter++;
        }
        return totalDeviation;
    }

    /**
     * Calculated all per-cell deviations of all data points over whole table
     */
    private static void countDeviation(Collection<AfrDataPoint> dataECU, double[][] cellDeviation, double[][] result, double[][] kgbcINIT, double targetAFR) {
        MathUtil.setArray2D(cellDeviation, 0);
        double normalizedTargetAfr = targetAFR / _14_7;

        for (AfrDataPoint dataPoint : dataECU) {
            double normalizedAfr = dataPoint.getAfr() / _14_7;

            double tmp = (normalizedAfr - normalizedTargetAfr *
                    (result[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()]) /
                    (kgbcINIT[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()]));

//            if (isLogEnabled())
//                log("r=" + r + "/c=" + c + ": tmp=" + tmp);

//            cellDeviation[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()] += tmp * tmp; todo: what is this deviation called?
            cellDeviation[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()] += Math.abs(tmp); // todo: what is this deviation called?
        }
    }

    private static double smooth(double kgbcSQsum, double ksq, double ke, double kg, double[][] kgbcRES) {
        double e = 0;
        kgbcSQsum = ksq * kgbcSQsum;

        // todo: add a comment while 'SIZE - 1' here?
        for (int i = 0; i < SIZE - 1; i++) {
            for (int j = 0; j < SIZE; j++) {
                e += square(kgbcRES[i][j] - kgbcRES[i + 1][j]);
                e += square(kgbcRES[j][i] - kgbcRES[j][i + 1]);
            }
        }

        double g= 0;
        for (int i = 0; i < SIZE - 2; i++) {
            for (int j = 0; j < SIZE; j++) {
                double tmp1 = square(kgbcRES[i][j] - 2 * kgbcRES[i + 1][j] + kgbcRES[i + 2][j]);
                g += tmp1;
                g += square(kgbcRES[j][i] - 2 * kgbcRES[j][i + 1] + kgbcRES[j][i + 2]);
            }
        }
        kgbcSQsum += ke * e + kg * g;
        return kgbcSQsum;
    }

    private static void log(String s) {
        System.out.println(s);
    }

}
