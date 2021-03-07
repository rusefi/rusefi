package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.file.TableGenerator;
import com.rusefi.models.Point3D;
import com.rusefi.models.Range;
import com.rusefi.models.XYData;
import com.rusefi.test.EcuStimulatorSandbox;
import com.rusefi.ui.ChartHelper;
import com.rusefi.ui.RpmModel;
import com.rusefi.ui.widgets.PotCommand;
import com.rusefi.ui.widgets.RpmCommand;
import net.ericaro.surfaceplotter.DefaultSurfaceModel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;

/**
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see EcuStimulatorSandbox
 */
public class EcuStimulator {
    private static final String DELIMITER = ",";
    private static final long SLEEP_TIME = 300;
    private static final double EPS = 0.001;

    public boolean isDisplayingFuel = true;

    private static final int MEASURES = 7;
    //    private static final String C_FILE_NAME = "advance_map.c";
//    private static final String C_PREFIX = "ad_";

    private static final String C_PREFIX = "fuel_";

    public static Range RPM_RANGE = new Range(0, StimulationInputs.DEFAULT_RPM_MAX); // x-coord
    private StimulationInputs inputs = new StimulationInputs(this);
    //
    private XYData data = new XYData();
    private DefaultSurfaceModel model = ChartHelper.createDefaultSurfaceModel(data, RPM_RANGE, new Range(1, 5));

    private final JPanel content = new JPanel(new BorderLayout());

    private static EcuStimulator instance = new EcuStimulator();

    private final JLabel statusLabel = new JLabel();

    private EcuStimulator() {
        JPanel panel = ChartHelper.create3DControl(data, model, isDisplayingFuel ? "Fuel Table" : "Timing");
        content.add(statusLabel, BorderLayout.NORTH);
        content.add(panel, BorderLayout.CENTER);
        content.add(inputs.getContent(), BorderLayout.SOUTH);
    }

    public static EcuStimulator getInstance() {
        return instance;
    }

    public JPanel getPanel() {
        return content;
    }

    public void buildTable() {
        data.clear();

//        setPotVoltage(2.2, Sensor.MAF);
//        if (1 == 1)
//            return;

        String csvFileName = "table_" + inputs.getRpmStep() + "_" + inputs.getEngineLoadStep() + Logger.getDate() + ".csv";
        FileLog.MAIN.logLine("Wring to " + csvFileName);

        final BufferedWriter csv;
        try {
            csv = new BufferedWriter(new FileWriter(csvFileName));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        ResultListener listener = new ResultListener() {
            @Override
            public void onResult(int rpm, double engineLoad, double advance, double dwell) {
                data.addPoint(new Point3D(rpm, engineLoad, isDisplayingFuel ? (float) dwell : (float) advance));
                model.plot().execute();

                String msg = putValue("rpm", rpm) +
                        putValue("engine_load", engineLoad) +
                        putValue("advance", advance) +
                        putValue("dwell", dwell);
                MessagesCentral.getInstance().postMessage(EcuStimulator.class, msg);

                try {
                    csv.write(msg + "\r\n");
                    csv.flush();
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        };

        ChartHelper.setYRange(new Range(inputs.getEngineLoadMin(), inputs.getEngineLoadMax()), model);

        //buildTable(listener, DWELL_SENSOR);

        try {
            csv.close();
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        TableGenerator.writeAsC(data, C_PREFIX, "map" + Logger.getDate() + ".c");
    }

    private void buildTable(ResultListener listener, Sensor dwellSensor) {
        for (double rpm = inputs.getRpmFrom(); rpm <= inputs.getRpmTo() + EPS; rpm += inputs.getRpmStep()) {
            for (double engineLoad = inputs.getEngineLoadMin(); engineLoad <= inputs.getEngineLoadMax() + EPS; engineLoad += inputs.getEngineLoadStep()) {
                for (int clt = inputs.getCltFrom(); clt <= inputs.getCltTo(); clt += 100) {
                    testPoint((int) rpm, engineLoad, clt, listener, dwellSensor);
                }
            }
        }
    }

    private void testPoint(int rpm, double engineLoad, int clt, ResultListener resultListener, Sensor dwellSensor) {
        //setPotVoltage(maf, Sensor.MAF);
        setPotVoltage(engineLoad, null);
        setRpm(rpm);
        /**
         * Let's give the firmware some time to react
         */
        sleepRuntime(SLEEP_TIME);

        statusLabel.setText("RPM " + rpm + ", el " + engineLoad + ", CLT " + clt);

        /**
         * We are making a number of measurements and then we take the middle one
         */
        MultipleMeasurements r = waitForMultipleResults(dwellSensor, null);
        List<Double> dwells = r.getDwells();
        List<Double> advances = r.getAdvances();

        // sorting measurements, taking middle value
        Collections.sort(dwells);
        Collections.sort(advances);

        double dwellDiff = Math.abs(dwells.get(0) - dwells.get(MEASURES - 1));
        if (dwellDiff > 1)
            System.out.println("dwells " + dwells);

        double dwell = dwells.get(MEASURES / 2);
        double advance = advances.get(MEASURES / 2);

//        if (dwell > 40)
//            throw new IllegalStateException("Unexpected value, how come? " + dwell);

        log("Stimulator result: " + rpm + "@" + engineLoad + ": " + dwell + ", adv=" + advance);

//            double dwell = Launcher.getAdcModel().getValue(Sensor.DWELL0);
//            double advance = Launcher.getAdcModel().getValue(Sensor.ADVANCE);

        resultListener.onResult(rpm, engineLoad, (float) advance, dwell);
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
            RpmCommand.requestRpmChange(rpm, null);
            sleepRuntime(50);
            actual = RpmModel.getInstance().getValue();
        } while (attempt++ < 10 && Math.abs(rpm - actual) >= 100);
        log("Result: " + actual + " while setting " + rpm);
    }

    public static void setPotVoltage(double targetVoltage, Sensor sensor) {
        if (sensor != null)
            log("Current targetVoltage: " + getValue(sensor) + ", setting " + targetVoltage);
        int attempt = 0;
        double vRef = getVRef();

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

    private static double getVRef() {
        // todo: make this adjustable via the UI
        //double vRef = SensorCentral.getConfig().getValue(Sensor.VREF) * PotCommand.VOLTAGE_CORRECTION;
        return 4.7;
    }

    private static void log(String message) {
        MessagesCentral.getInstance().postMessage(EcuStimulator.class, message);
        FileLog.MAIN.logLine(message);
    }

    private static String putValue(String msg, double value) {
        return msg + DELIMITER + value + DELIMITER;
    }

    private static String putValue(String msg, int value) {
        return msg + DELIMITER + value + DELIMITER;
    }

    public JButton createButton() {
        final JButton button = new JButton("stimulate stock ECU");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            buildTable();
                        } catch (Throwable e) {
                            e.printStackTrace();
                            System.exit(-20);
                        }
                        SwingUtilities.invokeLater(new Runnable() {
                            @Override
                            public void run() {
                                button.setText("Done");
                            }
                        });
                    }
                }, "Ecu Stimulator").start();
            }
        });
        return button;
    }

    public StimulationInputs getInputs() {
        return inputs;
    }

    interface ResultListener {
        void onResult(int rpm, double engineLoad, double advance, double dwell);
    }

    public MultipleMeasurements waitForMultipleResults(final Sensor dwellSensor, final Sensor advanceSensor) {
        final MultipleMeasurements result = new MultipleMeasurements();

        final CountDownLatch latch = new CountDownLatch(MEASURES);

        /*
        EngineTimeListener listener = new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                if (latch.getCount() == 0)
                    return;
                double dwell = getValue(dwellSensor);
                double advance = getValue(advanceSensor);
                advance = Sensor.processAdvance(advance);
                result.dwells.add(dwell);
                result.advances.add(advance);
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
         */
        return result;
    }

    private class MultipleMeasurements {
        private List<Double> dwells = new ArrayList<>(MEASURES);
        private List<Double> advances = new ArrayList<>(MEASURES);

        public List<Double> getDwells() {
            return dwells;
        }

        public List<Double> getAdvances() {
            return advances;
        }
    }
}
