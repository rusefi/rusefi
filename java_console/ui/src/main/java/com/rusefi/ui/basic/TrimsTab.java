package com.rusefi.ui.basic;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import java.awt.*;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class TrimsTab {
    private final JPanel content = new JPanel(new BorderLayout());
    private final JTable jTable = new JTable();
    private final JButton loadButton = new JButton("Load Trims");
    private final StatusPanel statusPanel;
    private final ConnectivityContext connectivityContext;
    private final AtomicReference<Optional<PortResult>> ecuPortToUse;

    public TrimsTab(ConnectivityContext connectivityContext,
                    AtomicReference<Optional<PortResult>> ecuPortToUse,
                    StatusPanel statusPanel) {
        this.connectivityContext = connectivityContext;
        this.ecuPortToUse = ecuPortToUse;
        this.statusPanel = statusPanel;

        loadButton.addActionListener(e -> loadTrims());

        content.add(new JScrollPane(jTable), BorderLayout.CENTER);
        JPanel bottomPanel = new JPanel(new BorderLayout());
        bottomPanel.add(loadButton, BorderLayout.WEST);
        bottomPanel.add(statusPanel, BorderLayout.CENTER);
        content.add(bottomPanel, BorderLayout.SOUTH);
    }

    private void loadTrims() {
        Optional<PortResult> portResult = ecuPortToUse.get();
        if (!portResult.isPresent()) {
            statusPanel.logLine("Not connected?");
            return;
        }

        new Thread(() -> {
            SwingUtilities.invokeLater(() -> loadButton.setEnabled(false));
            try {
                Optional<CalibrationsInfo> info = CalibrationsHelper.readCurrentCalibrations(
                        portResult.get().port,
                        statusPanel,
                        connectivityContext
                );

                info.ifPresent(calibrationsInfo -> SwingUtilities.invokeLater(() -> {
                    displayTrims(calibrationsInfo);
                }));
            } finally {
                SwingUtilities.invokeLater(() -> loadButton.setEnabled(true));
            }
        }).start();
    }

    private void displayTrims(CalibrationsInfo info) {
        ConfigurationImage image = info.getImage().getConfigurationImage();
        TableModel iniTable = info.getIniFile().getTable("ltftBank1Tbl");
        if (iniTable == null) {
            statusPanel.logLine("Trims table not found or not an array");
            return;
        }
        Optional<IniField> content = info.getIniFile().findIniField(iniTable.getZBinsConstant());
        Optional<IniField> xBinsField = info.getIniFile().findIniField(iniTable.getXBinsConstant());
        Optional<IniField> yBinsField = info.getIniFile().findIniField(iniTable.getYBinsConstant());

        if (!content.isPresent() || !(content.get() instanceof ArrayIniField)) {
            statusPanel.logLine("Trims table not found or not an array");
            return;
        }

        ArrayIniField ltft = (ArrayIniField) content.get();
        String[][] ltftValues = ltft.getValues(ConfigurationImageGetterSetter.getValue(ltft, image));

        String[] rpmBins = null;
        if (xBinsField.isPresent() && xBinsField.get() instanceof ArrayIniField) {
            ArrayIniField rpm = (ArrayIniField) xBinsField.get();
            String[][] values = rpm.getValues(ConfigurationImageGetterSetter.getValue(rpm, image));
            if (values.length > 0) rpmBins = values[0];
        }

        String[] loadBins = null;
        if (yBinsField.isPresent() && yBinsField.get() instanceof ArrayIniField) {
            ArrayIniField load = (ArrayIniField) yBinsField.get();
            String[][] values = load.getValues(ConfigurationImageGetterSetter.getValue(load, image));
            if (values.length > 0) loadBins = values[0];
        }

        jTable.setModel(new TrimsTableModel(ltftValues, rpmBins, loadBins));
    }

    public Component getContent() {
        return content;
    }

    private static class TrimsTableModel extends AbstractTableModel {
        private final String[][] data;
        private final String[] xBins;
        private final String[] yBins;

        public TrimsTableModel(String[][] data, String[] xBins, String[] yBins) {
            this.data = data;
            this.xBins = xBins;
            this.yBins = yBins;
        }

        @Override
        public int getRowCount() {
            return data.length;
        }

        @Override
        public int getColumnCount() {
            return data[0].length + 1;
        }

        @Override
        public String getColumnName(int column) {
            if (column == 0) return "Load \\ RPM";
            if (xBins != null && column - 1 < xBins.length) {
                return xBins[column - 1];
            }
            return "Col " + column;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            if (columnIndex == 0) {
                if (yBins != null && rowIndex < yBins.length) {
                    return yBins[rowIndex];
                }
                return "Row " + rowIndex;
            }
            return data[rowIndex][columnIndex - 1];
        }
    }
}
