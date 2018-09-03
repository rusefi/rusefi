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

    private static boolean isLogEnabled() {
        return true;
    }

//    private static final int TEMP_CORR = 39;


    // void MainWindow::calckGBC(double STEP)
    @Override
    public Result process(boolean smooth, Collection<AfrDataPoint> dataECU, double STEP, double targetAFR, float[][] kgbcINIT) {
        double kgbcSQ[][] = new double[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
        double kgbcSQsum = 0;
        double kgbcSQsumLast;
        double minSQtotal = 1e+15;
        double kgbcSQsumLastTotal = 1e+16;
        double ksq = 1000; //???? ??????????????????? ??????????
        double ke = 100; //???? ??????????
        double kg = 100; //???? ?????

        // let's could how many data points we have for each cell
        int bkGBC[][] = new int[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
        for (AfrDataPoint data : dataECU) {
            bkGBC[data.PRESS_RT_32()][data.RPM_RT_32()]++;
        }

        float result[][] = MathUtil.deepCopy(kgbcINIT);

        double ktgbcRES[][] = new double[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
        double ktgbcINIT[][] = new double[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];

        MathUtil.setArray2D(ktgbcINIT, 1);
        MathUtil.setArray2D(ktgbcRES, 1);

        int COUNT_THRESHOLD = 20; // minimal number of measurements in cell to be considered

        int minK = 0; // todo: what is this?
        while (true) {
            for (int loadIndex = 0; loadIndex < FUEL_LOAD_COUNT; loadIndex++) {
                for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
                    if (bkGBC[loadIndex][rpmIndex] < COUNT_THRESHOLD)
                        continue;

                    //log("Processing " + r + "/c" + c);

                    double minSQ = 1e+16;
                    kgbcSQsum = 1e+16;

                    double step = STEP;
                    double mink = 0;
                    while (true) {
                        ////////////////////////////////////
                        //????????? ?????????? ? ????????
                        MathUtil.setArray2D(kgbcSQ, 0);

                        kgbcSQsumLast = kgbcSQsum;

                        countDeviation(dataECU, kgbcSQ, result, kgbcINIT, targetAFR);

                        kgbcSQsum = MathUtil.sumArray(kgbcSQ);

                        if (smooth) {
                            kgbcSQsum = smooth(kgbcSQsum, ksq, ke, kg, result);
                        }
                        ////////////////////////////////////
                        if (kgbcSQsum >= kgbcSQsumLast)
                            step = -step;
                        //???? ?????? ?? ??????? ????? ???, ?? ? ?? ?????????? ??
                    /*if(bkGBC[r][c]) */

//                        log("Adjusting " + step);
                        result[loadIndex][rpmIndex] += step;
                        if (kgbcSQsum < minSQ)
                            minSQ = kgbcSQsum;

                        if (Math.abs(minSQ - kgbcSQsumLast) < 1e-10)
                            mink++;
                        if (mink > 4) {
//                            updateTablekGBC();
//                            ui -> statusBar -> showMessage(QString::number (kgbcSQsum), 500);
                            log("break " + rpmIndex + "/" + loadIndex);
                            break;
                        }
                    }
                }
            }
            if (kgbcSQsum < minSQtotal)
                minSQtotal = kgbcSQsum;
            if (Math.abs(minSQtotal - kgbcSQsumLastTotal) < 1e-10)
                minK++;
            if (minK > 4) {
                //updateTablekGBC();
                //ui->statusBar->showMessage(QString::number(kgbcSQsum), 500);
                log("return " + minK);
                return new Result(result);
            }
            kgbcSQsumLastTotal = kgbcSQsum;
            //ui->statusBar->showMessage(QString::number(gbcSQsum));
            //updateTableGBC();

        }
    }

    private static void countDeviation(Collection<AfrDataPoint> dataECU, double[][] kgbcSQ, float[][] kgbcRES, float[][] kgbcINIT, double targetAFR) {
        for (AfrDataPoint dataPoint : dataECU) {
            double ALF = targetAFR / 14.7;
            double tmp = (dataPoint.getAfr() / 14.7 - ALF *
                    (kgbcRES[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()]) /
                    (kgbcINIT[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()]));

//            if (isLogEnabled())
//                log("r=" + r + "/c=" + c + ": tmp=" + tmp);

//            kgbcSQ[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()] += tmp * tmp; todo: what is this deviation called?
            kgbcSQ[dataPoint.PRESS_RT_32()][dataPoint.RPM_RT_32()] += Math.abs(tmp); // todo: what is this deviation called?
        }
    }

    private static double smooth(double kgbcSQsum, double ksq, double ke, double kg, float[][] kgbcRES) {
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
