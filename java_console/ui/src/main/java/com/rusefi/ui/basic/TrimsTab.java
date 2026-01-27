package com.rusefi.ui.basic;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.PageReader;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.BinaryProtocolExecutor;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class TrimsTab {
    private final JPanel content = new JPanel(new BorderLayout());
    private final JTable jTable = new JTable();
    private final ConnectivityContext connectivityContext;
    private final AtomicReference<Optional<PortResult>> ecuPortToUse;

    public TrimsTab(ConnectivityContext connectivityContext,
                    AtomicReference<Optional<PortResult>> ecuPortToUse) {
        this.connectivityContext = connectivityContext;
        this.ecuPortToUse = ecuPortToUse;

        // Prevent column reordering
        jTable.getTableHeader().setReorderingAllowed(false);

        content.add(new JScrollPane(jTable), BorderLayout.CENTER);

        content.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && content.isShowing()) {
                loadTrims();
            }
        });
    }

    private void loadTrims() {
        Optional<PortResult> portResult = ecuPortToUse.get();
        if (!portResult.isPresent()) {
            return;
        }

        new Thread(() -> {
            try {
                Optional<CalibrationsInfo> info = CalibrationsHelper.readCurrentCalibrations(
                        portResult.get().port,
                        UpdateOperationCallbacks.DUMMY,
                        connectivityContext
                );

                if (info.isPresent()) {
                    String port = portResult.get().port;
                    CalibrationsInfo calibInfo = info.get();

                    Optional<byte[]> ltftData = BinaryProtocolExecutor.executeWithSuspendedPortScanner(
                        port,
                        UpdateOperationCallbacks.DUMMY,
                        (binaryProtocol) -> {

                            // TODO: get this from the .ini file
                            int iniPageNumber = 3;
                            int page3Size = calibInfo.getIniFile().getMetaInfo().getPageSize(iniPageNumber - 1);
                            ConfigurationImageWithMeta page3 = PageReader.readPageData(binaryProtocol, iniPageNumber, page3Size,
                                calibInfo.getIniFile().getMetaInfo().getSignature());

                            if (page3 != null && !page3.isEmpty()) {
                                return Optional.of(Objects.requireNonNull(page3.getConfigurationImage()).getContent());
                            }
                            return Optional.empty();
                        },
                        Optional.empty(),
                        connectivityContext,
                        "read LTFT"
                    );

                    if (ltftData.isPresent()) {
                        SwingUtilities.invokeLater(() -> {
                            displayTrims(calibInfo, ltftData.get());
                        });
                    }
                }
            } finally {
            }
        }).start();
    }

    private void displayTrims(CalibrationsInfo info, byte[] zBinsBuffer) {
        // Page 1 contains the axis bins (RPM and load)
        ConfigurationImage page1Image = info.getImage().getConfigurationImage();

        TableModel iniTable = info.getIniFile().getTable("ltftBank1Tbl");
        if (iniTable == null) {
            return;
        }

        Optional<IniField> zBinsField = info.getIniFile().findIniField(iniTable.getZBinsConstant());
        if (!zBinsField.isPresent() || !(zBinsField.get() instanceof ArrayIniField)) {
            return;
        }

        ArrayIniField ltft = (ArrayIniField) zBinsField.get();

        // Extract LTFT data from outputs buffer using the field's offset
        ConfigurationImage outputsImage = new ConfigurationImage(zBinsBuffer);
        String[][] ltftValues = ltft.getValues(ConfigurationImageGetterSetter.getValue(ltft, outputsImage));

        // X and Y bins (RPM and load) are on page 1
        String[] rpmBins = extractAxisBins(info.getIniFile(), iniTable.getXBinsConstant(), page1Image, "X");
        String[] loadBins = extractAxisBins(info.getIniFile(), iniTable.getYBinsConstant(), page1Image, "Y");

        jTable.setModel(new TrimsTableModel(ltftValues, rpmBins, loadBins));
    }

    private String[] extractAxisBins(IniFileModel iniFile,
                                     String binConstant,
                                     ConfigurationImage image,
                                     String axisName) {
        Optional<IniField> binsField = iniFile.findIniField(binConstant);
        if (!binsField.isPresent() || !(binsField.get() instanceof ArrayIniField)) {
            return null;
        }

        ArrayIniField field = (ArrayIniField) binsField.get();
        String[][] values = field.getValues(ConfigurationImageGetterSetter.getValue(field, image));

        if (values.length == 0 || values[0].length == 0) {
            return null;
        }

        String[] bins = (values.length == 1) ? values[0] :
                        (values[0].length == 1) ? extractColumn(values) :
                        values[0];

        return bins;
    }

    private String[] extractColumn(String[][] values) {
        String[] column = new String[values.length];
        for (int i = 0; i < values.length; i++) {
            column[i] = values[i][0];
        }
        return column;
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
                return formatNumber(xBins[column - 1]);
            }
            return "Col " + column;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            if (columnIndex == 0) {
                if (yBins != null && rowIndex < yBins.length) {
                    return formatNumber(yBins[rowIndex]);
                }
                return "Row " + rowIndex;
            }
            return formatNumber(data[rowIndex][columnIndex - 1]);
        }

        private String formatNumber(String value) {
            try {
                double num = Double.parseDouble(value);
                // Format to 1 decimal place to match INI specification
                return String.format("%.1f", num);
            } catch (NumberFormatException e) {
                return value;
            }
        }
    }
}
