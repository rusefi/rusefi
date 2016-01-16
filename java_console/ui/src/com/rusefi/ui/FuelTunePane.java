package com.rusefi.ui;

import com.romraider.Settings;
import com.romraider.maps.Scale;
import com.romraider.maps.Table3D;
import com.rusefi.BinarySearch;
import com.rusefi.FileLog;
import com.rusefi.autotune.FuelAutoTune;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.config.BaseConfigField;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * (c) Andrey Belomutskiy 2013-2016
 * 1/9/2016
 *
 * @see FuelAutoTune
 */
public class FuelTunePane {
    private final JPanel content = new JPanel(new BorderLayout());

    private final List<FuelDataPoint> incomingDataPoints = new ArrayList<>();
    private final float veLoadBins[] = new float[Fields.FUEL_LOAD_COUNT];
    private final float veRpmBins[] = new float[Fields.FUEL_RPM_COUNT];
    private final Table3D veTable = new Table3D();
    private final Table3D changeMap = new Table3D();

    public FuelTunePane() {
        final JLabel incomingBufferSize = new JLabel();

        JButton runLogic = new JButton("one iteration");
        runLogic.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                doJob();
            }
        });

        JPanel topPanel = new JPanel(new FlowLayout());
        topPanel.add(incomingBufferSize);
        topPanel.add(runLogic);

        Timer timer = new Timer(300, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int size;
                synchronized (incomingDataPoints) {
                    size = incomingDataPoints.size();
                }
                incomingBufferSize.setText(size + " records in buffer");
            }
        });
        timer.start();

        content.add(topPanel, BorderLayout.NORTH);


        JPanel rightPanel = new JPanel(new GridLayout(2, 1));
        rightPanel.add(changeMap);
        rightPanel.add(new JLabel("bottom"));

        JPanel middlePanel = new JPanel(new GridLayout(1, 2));
        middlePanel.add(veTable);
        middlePanel.add(rightPanel);

        content.add(middlePanel, BorderLayout.CENTER);
        initTable(veTable);
        initTable(changeMap);
    }

    private void initTable(Table3D table) {
        // todo: which one is which?
        table.setSizeX(Fields.FUEL_LOAD_COUNT);
        table.setSizeY(Fields.FUEL_RPM_COUNT);
        table.getXAxis().setDataSize(Fields.FUEL_LOAD_COUNT);
        table.getYAxis().setDataSize(Fields.FUEL_RPM_COUNT);

        table.getXAxis().setAxisParent(table);
        table.getYAxis().setAxisParent(table);

        table.setBorder(BorderFactory.createLineBorder(Color.red));
        table.addScale(new Scale());
        table.getXAxis().addScale(new Scale());
        table.getYAxis().addScale(new Scale());
    }

    private void doJob() {
        float veTable[][] = new float[Fields.FUEL_LOAD_COUNT][Fields.FUEL_RPM_COUNT];
        loadMap(veTable, Fields.VETABLE.getOffset());

        List<FuelAutoTune.stDataOnline> data = new ArrayList<>();
        synchronized (incomingDataPoints) {
            for (FuelDataPoint point : incomingDataPoints)
                data.add(point.asDataOnline());
            incomingDataPoints.clear();
        }

        // todo: move this away from AWT thread
        FuelAutoTune.Result a = FuelAutoTune.process(false, data, 0.1, 14.7, veTable);


        changeMap.setStorageAddress(0);
        changeMap.setStorageType(Settings.STORAGE_TYPE_FLOAT);
        changeMap.populateTable(new byte[4 * 16 * 16], 0);

        for (int i = 0; i < 16; i++) {
            for (int rpmIndex = 0; rpmIndex < 16; rpmIndex++) {
                changeMap.get3dData()[i][rpmIndex].setBinValue(a.getKgbcRES()[i][rpmIndex]);
            }
        }

        changeMap.drawTable();

    }

    public void showContent() {
        final SensorCentral sc = SensorCentral.getInstance();
        sc.addListener(Sensor.RPM, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                int rpm = (int) value;
                double engineLoad = sc.getValue(Sensor.MAP);
                double afr = sc.getValue(Sensor.AFR);
                // todo: add UI for pre-conditions
                double deltaTps = sc.getValue(Sensor.TPS_DELTA);
                double clt = sc.getValue(Sensor.CLT);
//                if (clt < 80)
//                    return;
                FuelDataPoint newPoint = new FuelDataPoint(rpm, engineLoad, afr);
                synchronized (incomingDataPoints) {
                    incomingDataPoints.add(newPoint);
                }
            }
        });

        loadArray(veLoadBins, Fields.VETABLE.getOffset() + Fields.FUEL_RPM_COUNT * Fields.FUEL_LOAD_COUNT * 4);
        loadArray(veRpmBins, Fields.VETABLE.getOffset() + Fields.FUEL_RPM_COUNT * Fields.FUEL_LOAD_COUNT * 4 + Fields.FUEL_LOAD_COUNT * 4);

        BinaryProtocol bp = BinaryProtocol.instance;

        byte[] content = bp.getController().getContent();
        veTable.setStorageAddress(Fields.VETABLE.getOffset());
        veTable.setStorageType(Settings.STORAGE_TYPE_FLOAT);
        veTable.populateTable(content, 0);
        veTable.drawTable();

//        UiUtils.trueLayout(content.getParent());
    }

    private void loadMap(float[][] map, int offset) {
        for (int engineLoadIndex = 0; engineLoadIndex < map.length; engineLoadIndex++) {
            loadArray(map[engineLoadIndex], offset + engineLoadIndex * 4 * Fields.FUEL_RPM_COUNT);
        }
    }

    private void loadArray(float[] array, int offset) {
        BinaryProtocol bp = BinaryProtocol.instance;
        if (bp == null) {
            FileLog.MAIN.logLine("bp not ready");
            return;
        }
        for (int i = 0; i < array.length; i++)
            array[i] = BaseConfigField.getByteBuffer(bp.getController(), offset + 4 * i).getFloat();
        System.out.println("Loaded " + Arrays.toString(array));
    }

    public Component getContent() {
        return content;
    }

    private class FuelDataPoint {
        private final int rpm;
        private final double engineLoad;
        private final double afr;
        private final int rpmIndex;
        private final int engineLoadIndex;

        public FuelDataPoint(int rpm, double engineLoad, double afr) {
            this.rpm = rpm;
            this.engineLoad = engineLoad;
            this.afr = afr;
            // too low values are returning '-1' indeces
            rpmIndex = Math.max(0, BinarySearch.binarySearch(rpm, veRpmBins));
            engineLoadIndex = Math.max(0, BinarySearch.binarySearch(engineLoad, veLoadBins));
        }

        public FuelAutoTune.stDataOnline asDataOnline() {
            return new FuelAutoTune.stDataOnline(afr, rpmIndex, engineLoadIndex);
        }
    }
}
