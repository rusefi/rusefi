package com.rusefi;

import com.irnems.FileLog;
import com.irnems.core.MessagesCentral;
import com.irnems.core.Sensor;
import com.irnems.core.EngineTimeListener;
import com.irnems.core.SensorCentral;
import com.irnems.file.TableGenerator;
import com.irnems.models.Point3D;
import com.irnems.models.Range;
import com.irnems.models.XYData;
import com.rusefi.test.EcuStimulatorSandbox;
import com.rusefi.ui.ChartHelper;
import com.rusefi.ui.RpmModel;
import com.rusefi.ui.widgets.PotCommand;
import com.rusefi.ui.widgets.RpmCommand;
import com.rusefi.io.LinkManager;
import net.ericaro.surfaceplotter.DefaultSurfaceModel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;

/**
 * Date: 3/24/13
 * (c) Andrey Belomutskiy
 *
 * @see EcuStimulatorSandbox
 */
public class EcuStimulator {
    //    private static final String TITLE = "Spark Advance";
    private static final String TITLE = "Fuel Table";

    private static final String DELIMITER = ",";
    private static final long SLEEP_TIME = 300;

    private static final double EL_INCREMENT = 0.1;

    private static final int RPM_MIN = 400;
    private static final int RPM_MAX = 6000;
    private static final int RPM_INCREMENT = 250;
    private static final Sensor DWELL_SENSOR = Sensor.DWELL0;

    private static final String CSV_FILE_NAME = "table" + RPM_INCREMENT + "_" + EL_INCREMENT + ".csv";

    private static final int MEASURES = 7;
    //    private static final String C_FILE_NAME = "advance_map.c";
//    private static final String C_PREFIX = "ad_";
    private static final String C_FILE_NAME = "fuel_map.c";
    private static final String C_PREFIX = "fuel_";

    public static Range RPM_RANGE = new Range(0, RPM_MAX); // x-coord
    private StimulationInputs inputs = new StimulationInputs(this);
    //
    private XYData data = new XYData();
    private DefaultSurfaceModel model = ChartHelper.createDefaultSurfaceModel(data, RPM_RANGE, new Range(1, 5));

    private final JPanel content = new JPanel(new BorderLayout());

    private static EcuStimulator instance = new EcuStimulator();

    private final JLabel statusLabel = new JLabel();

    private EcuStimulator() {
        JPanel panel = ChartHelper.create3DControl(data, model, TITLE);
        content.add(statusLabel, BorderLayout.NORTH);
        content.add(panel, BorderLayout.CENTER);
        content.add(inputs.getContent(), BorderLayout.WEST);
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

        final BufferedWriter csv;
        try {
            csv = new BufferedWriter(new FileWriter(CSV_FILE_NAME));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        ResultListener listener = new ResultListener() {
            @Override
            public void onResult(int rpm, double maf, float advance, double dwell) {
                data.addPoint(new Point3D(rpm, maf, (float) dwell));
                model.plot().execute();

                String msg = putValue("rpm", rpm) +
                        putValue("engine_load", maf) +
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

        buildTable(listener, DWELL_SENSOR);

        try {
            csv.close();
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        TableGenerator.writeAsC(data, C_PREFIX, C_FILE_NAME);
    }

    private void buildTable(ResultListener listener, Sensor dwellSensor) {
        for (int rpm = RPM_MIN; rpm <= RPM_MAX; rpm += RPM_INCREMENT)
            runSimulation(rpm, listener, dwellSensor);
    }

    private void runSimulation(int rpm, ResultListener resultListener, final Sensor dwellSensor) {
        for (double engineLoad = inputs.getEngineLoadMin(); engineLoad <= inputs.getEngineLoadMax(); engineLoad += EL_INCREMENT) {
            //setPotVoltage(maf, Sensor.MAF);
            setPotVoltage(engineLoad, null);
            setRpm(rpm);
            /**
             * Let's give the firmware some time to react
             */
            sleepRuntime(SLEEP_TIME);

            statusLabel.setText("RPM " + rpm + ", el " + engineLoad);

            /**
             * We are making a number of measurements and then we take the middle one
             */
            MultipleMeasurements r = waitForMultipleResults(dwellSensor);
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

            if (dwell > 40)
                throw new IllegalStateException("Unexpected value, how comes? " + dwell);

            log("Stimulator result: " + rpm + "@" + engineLoad + ": " + dwell);

//            double dwell = Launcher.getAdcModel().getValue(Sensor.DWELL0);
//            double advance = Launcher.getAdcModel().getValue(Sensor.ADVANCE);

            resultListener.onResult(rpm, engineLoad, (float) advance, dwell);
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
        //double vRef = SensorCentral.getInstance().getValue(Sensor.VREF) * PotCommand.VOLTAGE_CORRECTION;
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
        void onResult(int rpm, double engineLoad, float advance, double dwell);
    }

    public MultipleMeasurements waitForMultipleResults(final Sensor dwellSensor) {
        final MultipleMeasurements result = new MultipleMeasurements();

        final CountDownLatch latch = new CountDownLatch(MEASURES);

        EngineTimeListener listener = new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                if (latch.getCount() == 0)
                    return;
                double dwell = getValue(dwellSensor);
                double advance = 0;//getValue(Sensor.ADVANCE0);
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
