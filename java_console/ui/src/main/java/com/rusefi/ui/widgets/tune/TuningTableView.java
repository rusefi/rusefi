package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.StringFormatter;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.util.Optional;

public class TuningTableView {
    private final JTable table = new JTable();
    private final Surface3DView surface3DView = new Surface3DView();
    private final CardLayout cardLayout = new CardLayout();
    private final JPanel tableContainer = new JPanel(cardLayout);
    private final JPanel content = new JPanel();
    private final String title;
    private double minValue = Double.MAX_VALUE;
    private double maxValue = -Double.MAX_VALUE;

    public TuningTableView(String title) {
        this.title = title;
        table.getTableHeader().setReorderingAllowed(false);
        table.setDefaultRenderer(Object.class, new GradientRenderer());

        tableContainer.add(new JScrollPane(table), "table");
        tableContainer.add(surface3DView, "3d");

        JCheckBox view3d = new JCheckBox("3D view");
        view3d.addActionListener(e -> {
            cardLayout.show(tableContainer, view3d.isSelected() ? "3d" : "table");
        });

        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        content.add(new JLabel(title));
        content.add(view3d);
        content.add(tableContainer);
    }

    public void displayTable(IniFileModel iniFile, String tableName, ConfigurationImage zImage, ConfigurationImage axisImage) {
        TableModel iniTable = iniFile.getTable(tableName);
        if (iniTable == null) {
            return;
        }

        Optional<IniField> zBinsField = iniFile.findIniField(iniTable.getZBinsConstant());
        if (!zBinsField.isPresent() || !(zBinsField.get() instanceof ArrayIniField)) {
            return;
        }

        ArrayIniField ltft = (ArrayIniField) zBinsField.get();
        int precision = IniField.parseDigits(ltft.getDigits());

        // Extract data from outputs buffer using the field's offset
        Double[][] dataValues = ConfigurationImageGetterSetter.getArrayValues(ltft, zImage);

        // X and Y bins (RPM and load) are on page 1
        Double[] xBins = extractAxisBins(iniFile, iniTable.getXBinsConstant(), axisImage);
        Double[] yBins = extractAxisBins(iniFile, iniTable.getYBinsConstant(), axisImage);

        calculateMinMax(dataValues);

        table.setModel(new TuningTableModel(dataValues, xBins, yBins, precision));
        surface3DView.setData(dataValues, xBins, yBins, minValue, maxValue);
    }

    public void displayTable(IniFileModel iniFile, String tableName, ConfigurationImage image) {
        displayTable(iniFile, tableName, image, image);
    }

    @Deprecated
    public void displayTable(CalibrationsInfo info, byte[] zBinsBuffer, ConfigurationImage image, String tableName) {
        displayTable(info.getIniFile(), tableName, new ConfigurationImage(zBinsBuffer), image);
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

    private Double[] extractAxisBins(IniFileModel iniFile,
                                     String binConstant,
                                     ConfigurationImage image) {
        Optional<IniField> binsField = iniFile.findIniField(binConstant);
        if (!binsField.isPresent() || !(binsField.get() instanceof ArrayIniField)) {
            return null;
        }

        ArrayIniField field = (ArrayIniField) binsField.get();
        Double[][] values = ConfigurationImageGetterSetter.getArrayValues(field, image);

        if (values.length == 0 || values[0].length == 0) {
            return null;
        }

        return (values.length == 1) ? values[0] :
                (values[0].length == 1) ? extractColumn(values) :
                        values[0];
    }

    private Double[] extractColumn(Double[][] values) {
        Double[] column = new Double[values.length];
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
        private final Double[] xBins;
        private final Double[] yBins;
        private final int precision;

        public TuningTableModel(Double[][] data, Double[] xBins, Double[] yBins, int precision) {
            this.data = data;
            this.xBins = xBins;
            this.yBins = yBins;
            this.precision = precision;
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
                return StringFormatter.niceToString((Number) value, precision);
            }
            return String.valueOf(value);
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
                String strValue = (String) value;
                if (!strValue.isEmpty()) {
                    try {
                        double val = Double.parseDouble(strValue);
                        applyGradient(c, val);
                    } catch (NumberFormatException ignored) {
                    }
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

            // Pastel Blue (173, 216, 230) to Pastel Red (255, 182, 193)
            int r1 = 173, g1 = 216, b1 = 230; // Min value color
            int r2 = 255, g2 = 182, b2 = 193; // Max value color

            int red = (int) (r1 + ratio * (r2 - r1));
            int green = (int) (g1 + ratio * (g2 - g1));
            int blue = (int) (b1 + ratio * (b2 - b1));
            Color background = new Color(red, green, blue);
            c.setBackground(background);
            // Ensure text is readable - pastel colors are generally light, so black text should be fine
            double brightness = (red * 0.299 + green * 0.587 + blue * 0.114) / 255.0;
            c.setForeground(brightness < 0.5 ? Color.WHITE : Color.BLACK);
        }
    }
}
