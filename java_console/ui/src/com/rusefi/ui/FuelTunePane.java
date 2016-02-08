package com.rusefi.ui;

import com.romraider.Settings;
import com.romraider.maps.Scale;
import com.romraider.maps.Table;
import com.romraider.maps.Table3D;
import com.romraider.xml.RomAttributeParser;
import com.rusefi.BinarySearch;
import com.rusefi.ConfigurationImage;
import com.rusefi.FileLog;
import com.rusefi.UploadChanges;
import com.rusefi.autotune.FuelAutoTune;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Field;
import com.rusefi.config.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.config.BaseConfigField;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteOrder;
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
    private final JButton upload = new JButton("Upload");
    private byte[] newVeMap;

    public FuelTunePane() {
        final JLabel incomingBufferSize = new JLabel();

        JButton runLogic = new JButton("one iteration");
        runLogic.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                doJob();
            }
        });
        upload.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                byte[] newVeMap = FuelTunePane.this.newVeMap;
                BinaryProtocol bp = BinaryProtocol.instance;
                if (newVeMap == null || bp == null)
                    return;
                ConfigurationImage ci = bp.getController().clone();
                System.arraycopy(newVeMap, 0, ci.getContent(), Fields.VETABLE.getOffset(), newVeMap.length);
                UploadChanges.scheduleUpload(ci);
            }
        });

        JPanel topPanel = new JPanel(new FlowLayout());
        topPanel.add(incomingBufferSize);
        topPanel.add(runLogic);
        topPanel.add(upload);

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

        upload.setEnabled(false);
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

    private static void loadData(Table table, byte[] content, int offset) {
        table.reset();
        table.setStorageAddress(offset);
        table.setStorageType(Settings.STORAGE_TYPE_FLOAT);
        table.populateTable(content, 0);
        table.drawTable();
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

        newVeMap = toByteArray(a.getKgbcRES());

        loadData(changeMap, newVeMap, 0);
        upload.setEnabled(true);
    }

    private byte[] toByteArray(float[][] output) {
        try {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            for (int loadIndex = 0; loadIndex < Fields.FUEL_LOAD_COUNT; loadIndex++) {
                for (int rpmIndex = 0; rpmIndex < Fields.FUEL_RPM_COUNT; rpmIndex++) {
                    byte[] b4 = RomAttributeParser.floatToByte(output[loadIndex][rpmIndex], Settings.ENDIAN_BIG);
                    baos.write(b4);
                }
            }
            return baos.toByteArray();
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
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
                double deltaTps = sc.getValue(Sensor.deltaTps);
                double clt = sc.getValue(Sensor.CLT);
//                if (clt < 80)
//                    return;
                FuelDataPoint newPoint = new FuelDataPoint(rpm, engineLoad, afr);
                synchronized (incomingDataPoints) {
                    incomingDataPoints.add(newPoint);
                }
            }
        });

        int veLoadOffset = Fields.VETABLE.getOffset() + Fields.FUEL_RPM_COUNT * Fields.FUEL_LOAD_COUNT * 4;
        loadArray(veLoadBins, veLoadOffset);
        int veRpmOffset = Fields.VETABLE.getOffset() + Fields.FUEL_RPM_COUNT * Fields.FUEL_LOAD_COUNT * 4 + Fields.FUEL_LOAD_COUNT * 4;
        loadArray(veRpmBins, veRpmOffset);

        BinaryProtocol bp = BinaryProtocol.instance;

        byte[] content = bp.getController().getContent();
        loadData(veTable.getXAxis(), content, veRpmOffset);
        loadData(veTable.getYAxis(), content, veLoadOffset);
        loadData(veTable, content, Fields.VETABLE.getOffset());

        loadData(changeMap.getXAxis(), content, veRpmOffset);
        loadData(changeMap.getYAxis(), content, veLoadOffset);
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
