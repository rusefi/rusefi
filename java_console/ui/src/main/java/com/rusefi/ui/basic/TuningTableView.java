package com.rusefi.ui.basic;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import java.awt.*;
import java.util.Optional;

public class TuningTableView {
    private final JTable table = new JTable();
    private final JPanel content = new JPanel();
    private final String title;

    public TuningTableView(String title) {
        this.title = title;
        table.getTableHeader().setReorderingAllowed(false);

        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        content.add(new JLabel(title));
        content.add(table.getTableHeader());
        content.add(table);
    }

    public void displayTable(CalibrationsInfo info, byte[] zBinsBuffer, ConfigurationImage page1Image, String tableName) {
        TableModel iniTable = info.getIniFile().getTable(tableName);
        if (iniTable == null) {
            return;
        }

        Optional<IniField> zBinsField = info.getIniFile().findIniField(iniTable.getZBinsConstant());
        if (!zBinsField.isPresent() || !(zBinsField.get() instanceof ArrayIniField)) {
            return;
        }

        ArrayIniField ltft = (ArrayIniField) zBinsField.get();

        // Extract data from outputs buffer using the field's offset
        ConfigurationImage outputsImage = new ConfigurationImage(zBinsBuffer);
        String[][] dataValues = ltft.getValues(ConfigurationImageGetterSetter.getValue(ltft, outputsImage));

        // X and Y bins (RPM and load) are on page 1
        String[] xBins = extractAxisBins(info.getIniFile(), iniTable.getXBinsConstant(), page1Image);
        String[] yBins = extractAxisBins(info.getIniFile(), iniTable.getYBinsConstant(), page1Image);

        table.setModel(new TuningTableModel(dataValues, xBins, yBins));
    }

    private String[] extractAxisBins(IniFileModel iniFile,
                                     String binConstant,
                                     ConfigurationImage image) {
        Optional<IniField> binsField = iniFile.findIniField(binConstant);
        if (!binsField.isPresent() || !(binsField.get() instanceof ArrayIniField)) {
            return null;
        }

        ArrayIniField field = (ArrayIniField) binsField.get();
        String[][] values = field.getValues(ConfigurationImageGetterSetter.getValue(field, image));

        if (values.length == 0 || values[0].length == 0) {
            return null;
        }

        return (values.length == 1) ? values[0] :
                (values[0].length == 1) ? extractColumn(values) :
                        values[0];
    }

    private String[] extractColumn(String[][] values) {
        String[] column = new String[values.length];
        for (int i = 0; i < values.length; i++) {
            column[i] = values[i][0];
        }
        return column;
    }

    public JPanel getContent() {
        return content;
    }

    private static class TuningTableModel extends AbstractTableModel {
        private final String[][] data;
        private final String[] xBins;
        private final String[] yBins;

        public TuningTableModel(String[][] data, String[] xBins, String[] yBins) {
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
            // traditionally low values are displayed on the bottom and high line on top
            int reversedRowIndex = data.length - 1 - rowIndex;
            if (columnIndex == 0) {
                if (yBins != null && reversedRowIndex < yBins.length) {
                    return formatNumber(yBins[reversedRowIndex]);
                }
                return "Row " + rowIndex;
            }
            return formatNumber(data[reversedRowIndex][columnIndex - 1]);
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
