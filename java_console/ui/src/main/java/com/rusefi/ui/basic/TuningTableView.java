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
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.util.Optional;

public class TuningTableView {
    private final JTable table = new JTable();
    private final JPanel content = new JPanel();
    private final String title;
    private double minValue = Double.MAX_VALUE;
    private double maxValue = -Double.MAX_VALUE;

    public TuningTableView(String title) {
        this.title = title;
        table.getTableHeader().setReorderingAllowed(false);
        table.setDefaultRenderer(Object.class, new GradientRenderer());

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
        Double[][] dataValues = ConfigurationImageGetterSetter.getArrayValues(ltft, outputsImage);

        // X and Y bins (RPM and load) are on page 1
        String[] xBins = extractAxisBins(info.getIniFile(), iniTable.getXBinsConstant(), page1Image);
        String[] yBins = extractAxisBins(info.getIniFile(), iniTable.getYBinsConstant(), page1Image);

        calculateMinMax(dataValues);

        table.setModel(new TuningTableModel(dataValues, xBins, yBins));
    }

    private void calculateMinMax(Double[][] dataValues) {
        double min = Double.MAX_VALUE;
        double max = -Double.MAX_VALUE;
        for (Double[] row : dataValues) {
            for (Double v : row) {
                if (v != null) {
                    min = Math.min(min, v);
                    max = Math.max(max, v);
                }
            }
        }
        this.minValue = min;
        this.maxValue = max;
    }

    private String[] extractAxisBins(IniFileModel iniFile,
                                     String binConstant,
                                     ConfigurationImage image) {
        Optional<IniField> binsField = iniFile.findIniField(binConstant);
        if (!binsField.isPresent() || !(binsField.get() instanceof ArrayIniField)) {
            return null;
        }

        ArrayIniField field = (ArrayIniField) binsField.get();
        String[][] values = field.getValues(ConfigurationImageGetterSetter.getStringValue(field, image));

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
        private final Double[][] data;
        private final String[] xBins;
        private final String[] yBins;

        public TuningTableModel(Double[][] data, String[] xBins, String[] yBins) {
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

        private String formatNumber(Object value) {
            if (value instanceof Number) {
                double num = ((Number) value).doubleValue();
                return String.format("%.1f", num);
            }
            if (value == null) return "";
            String strValue = value.toString();
            try {
                double num = Double.parseDouble(strValue);
                return String.format("%.1f", num);
            } catch (NumberFormatException e) {
                return strValue;
            }
        }
    }

    private class GradientRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            if (column == 0 || isSelected) {
                c.setBackground(isSelected ? table.getSelectionBackground() : table.getBackground());
                return c;
            }

            if (value instanceof String) {
                try {
                    double val = Double.parseDouble((String) value);
                    applyGradient(c, val);
                } catch (NumberFormatException ignored) {
                }
            }
            return c;
        }

        private void applyGradient(Component c, double value) {
            if (maxValue <= minValue) {
                c.setBackground(Color.WHITE);
                c.setForeground(Color.BLACK);
                return;
            }
            double ratio = (value - minValue) / (maxValue - minValue);
            ratio = Math.max(0, Math.min(1, ratio));

            // Dark Blue (0, 0, 128) to Red (255, 0, 0)
            int red = (int) (ratio * 255);
            int blue = (int) ((1 - ratio) * 128);
            Color background = new Color(red, 0, blue);
            c.setBackground(background);
            // Ensure text is readable against dark backgrounds
            double brightness = (red * 0.299 + blue * 0.114) / 255.0;
            c.setForeground(brightness < 0.5 ? Color.WHITE : Color.BLACK);
        }
    }
}
