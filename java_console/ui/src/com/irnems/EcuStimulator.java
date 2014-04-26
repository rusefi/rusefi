package com.irnems;

import com.irnems.core.MessagesCentral;
import com.irnems.core.Sensor;
import com.irnems.core.EngineTimeListener;
import com.irnems.core.SensorCentral;
import com.irnems.file.TableGenerator;
import com.irnems.models.Point3D;
import com.irnems.models.Range;
import com.irnems.models.XYData;
import com.irnems.ui.ChartHelper;
import com.irnems.ui.RpmModel;
import com.irnems.ui.widgets.PotCommand;
import com.irnems.ui.widgets.RpmCommand;
import com.rusefi.io.LinkManager;
import net.ericaro.surfaceplotter.DefaultSurfaceModel;

import javax.swing.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;

/**
 * Date: 3/24/13
 * (c) Andrey Belomutskiy
 */
public class EcuStimulator {
    //    private static final String TITLE = "Spark Advance";
    private static final String TITLE = "Fuel Table";

    private static final String D = ",";
    private static final long SLEEP_TIME = 300;

    private static final float MAF_MIN = 1.2f;
    private static final float MAF_MAX = 4.5f;
    private static final double MAF_INCREMENT = 0.1;

    private static final int RPM_MIN = 400;
    private static final int RPM_MAX = 6000;
    private static final int RPM_INCREMENT = 250;
    private static final Sensor DWELL_SENSOR = Sensor.DWELL0;

    private static final String TABLE_FILE_NAME = "table" + RPM_INCREMENT + "_" + MAF_INCREMENT + ".csv";

    private static final int MEASURES = 7;
    //    private static final String C_FILE_NAME = "advance_map.c";
//    private static final String C_PREFIX = "ad_";
    private static final String C_FILE_NAME = "fuel_map.c";
    private static final String C_PREFIX = "fuel_";

    public static Range RPM_RANGE = new Range(0, RPM_MAX); // x-coord
    static Range mafRange = new Range(MAF_MIN, MAF_MAX); // y-coord
    private static XYData data = new XYData();
    private static DefaultSurfaceModel model = ChartHelper.createDefaultSurfaceModel(data, RPM_RANGE, mafRange);
    public static JPanel panel = ChartHelper.create3DControl(data, model, TITLE);

    public static void buildTable() {
        data.clear();

//        setPotVoltage(2.2, Sensor.MAF);
//        if (1 == 1)
//            return;

        final BufferedWriter bw;
        try {
            bw = new BufferedWriter(new FileWriter(TABLE_FILE_NAME));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        ResultListener listener = new ResultListener() {
            @Override
            public void onResult(int rpm, double maf, float advance, double dwell) {
                data.addPoint(new Point3D(rpm, maf, (float) dwell));
                model.plot().execute();

                String msg = putValue("rpm", rpm) +
                        putValue("maf", maf) +
                        putValue("advance", advance) +
                        putValue("dwell", dwell);
                MessagesCentral.getInstance().postMessage(EcuStimulator.class, msg);

                try {
                    bw.write(msg + "\r\n");
                    bw.flush();
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        };

        buildTable(bw, listener, DWELL_SENSOR);

        try {
            bw.close();
        } catch (FileNotFoundException e) {
            throw new IllegalStateException(e);
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        TableGenerator.writeAsC(data, C_PREFIX, C_FILE_NAME);
    }

    private static void buildTable(BufferedWriter bw, ResultListener listener, Sensor dwellSensor) {
        for (int rpm = RPM_MIN; rpm <= RPM_MAX; rpm += RPM_INCREMENT)
            runSimulation(rpm, listener, dwellSensor);
    }

    private static void runSimulation(int rpm, ResultListener resultListener, final Sensor dwellSensor) {
        for (double maf = MAF_MIN; maf <= MAF_MAX; maf += MAF_INCREMENT) {
            //setPotVoltage(maf, Sensor.MAF);
            setPotVoltage(maf, null);
            setRpm(rpm);
            sleepRuntime(SLEEP_TIME);

            /**
             * We are making a number of measurements and then we take the middle one
             */
            final List<Double> dwells = new ArrayList<Double>(MEASURES);
            final List<Double> advances = new ArrayList<Double>(MEASURES);

            final CountDownLatch latch = new CountDownLatch(MEASURES);

            EngineTimeListener listener = new EngineTimeListener() {
                @Override
                public void onTime(double time) {
                    if (latch.getCount() == 0)
                        return;
                    double dwell = getValue(dwellSensor);
                    double advance = 0;//getValue(Sensor.ADVANCE0);
                    dwells.add(dwell);
                    advances.add(advance);
                    latch.countDown();
                }
            };
            LinkManager.engineState.timeListeners.add(listener);
            try {
                latch.await();
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
            LinkManager.engineState.timeListeners.remove(listener);

            // sorting measurements, taking middle value
            Collections.sort(dwells);
            Collections.sort(advances);

            double dwellDiff = Math.abs(dwells.get(0) - dwells.get(MEASURES - 1));
            if (dwellDiff > 1)
                System.out.println("dwells " + dwells);

            double dwell = dwells.get(MEASURES / 2);
            double advance = advances.get(MEASURES / 2);

            if (dwell > 40) {
                throw new UnsupportedOperationException();
            }

            log("Stimulator result: " + rpm + "@" + maf + ": " + dwell);

//            double dwell = Launcher.getAdcModel().getValue(Sensor.DWELL0);
//            double advance = Launcher.getAdcModel().getValue(Sensor.ADVANCE);

            resultListener.onResult(rpm, maf, (float) advance, dwell);
        }
    }

    private static void sleepRuntime(long sleepTime) {
        try {
            Thread.sleep(sleepTime);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    private static double getValue(Sensor sensor) {
        return SensorCentral.getInstance().getValue(sensor);
    }

    private static void setRpm(int rpm) {
        int actual;
        int attempt = 0;
        do {
            RpmCommand.requestRpmChange(rpm);
            sleepRuntime(50);
            actual = RpmModel.getInstance().getValue();
        } while (attempt++ < 10 && Math.abs(rpm - actual) >= 100);
        log("Result: " + actual + " while setting " + rpm);
    }

    public static void setPotVoltage(double targetVoltage, Sensor sensor) {
        if (sensor != null)
            log("Current targetVoltage: " + getValue(sensor) + ", setting " + targetVoltage);
        int attempt = 0;
        //double vRef = SensorCentral.getInstance().getValue(Sensor.VREF) * PotCommand.VOLTAGE_CORRECTION;
        double vRef = 4.7;
        int resistance = PotCommand.getPotResistance(targetVoltage, vRef);
        if (resistance <= 0) {
            log("Invalid resistance " + resistance + ". Invalid targetVoltage " + targetVoltage + "?");
            return;
        }

        if (sensor == null) {
            PotCommand.requestPotChange(1, resistance);
            sleepRuntime(1000);
        } else {

            double actual;
            do {
                PotCommand.requestPotChange(1, resistance);
                sleepRuntime(50);
                actual = getValue(sensor);
                log("Got: " + actual + " on attempt=" + attempt + " while setting " + targetVoltage + " for " + sensor);
            } while (attempt++ < 10 && Math.abs(targetVoltage - actual) > 0.2);
            log("Result: " + actual + " while setting " + targetVoltage);
        }
    }

    private static void log(String message) {
        MessagesCentral.getInstance().postMessage(EcuStimulator.class, message);
        FileLog.MAIN.logLine(message);
    }

    private static String putValue(String msg, double value) {
        return msg + D + value + D;
    }

    private static String putValue(String msg, int value) {
        return msg + D + value + D;
    }

    interface ResultListener {
        void onResult(int rpm, double maf, float advance, double dwell);
    }
}
