package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.AxisModel;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.Point2D;
import java.util.Optional;

/**
 * curve panel consists of two parts
 *
 * on the left we have black canvas representing an X by Y table, with a grid of step steps per AxisModel
 * y axis is displayed in descending order
 * grid scale is displayed as red text labels showing from min to max according to AxisModel
 *
 * actual values are charged with green dots and a bright orange line connecting the value dots. User can drag and drop
 * by green dots, the line should render as per dot movement. it should not be possible to move dot too far left part neighboring line
 * or too far right, enforcing X-coordinate to stay in order
 *
 * on the right we need a vertical jtable showing axis values, with X-asix as first column and Y-asix as second column.
 * table cells should follow the gradient effect as in the TrimsTab
 */
public class CurveWidget {
    private final JPanel content = new JPanel(new BorderLayout());
    private final CurveCanvas canvas = new CurveCanvas();
    private final JTable table = new JTable();

    private CurveModel curveModel;
    private Double[] xValues;
    private Double[] yValues;
    private String xUnits = "";

    public CurveWidget() {
        table.getTableHeader().setReorderingAllowed(false);
        table.setDefaultRenderer(Object.class, new GradientRenderer());

        JPanel rightPanel = new JPanel(new BorderLayout());
        rightPanel.add(table.getTableHeader(), BorderLayout.NORTH);
        rightPanel.add(new JScrollPane(table), BorderLayout.CENTER);

        content.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weighty = 1.0;

        gbc.gridx = 0;
        gbc.weightx = 0.8;
        content.add(canvas, gbc);

        gbc.gridx = 1;
        gbc.weightx = 0.2;
        content.add(rightPanel, gbc);
    }

    public void update(CurveModel curveModel, IniFileModel iniFile, ConfigurationImage ci) {
        this.curveModel = curveModel;
        this.xValues = readArray(curveModel.getxBins(), iniFile, ci);
        this.yValues = readArray(curveModel.getyBins(), iniFile, ci);

        if (iniFile != null) {
            Optional<IniField> field = iniFile.findIniField(curveModel.getxBins());
            if (field.isPresent()) {
                this.xUnits = field.get().getUnits();
            } else {
                this.xUnits = "";
            }
        } else {
            this.xUnits = "";
        }

        canvas.setCurve(curveModel, xValues, yValues);
        table.setModel(new CurveTableModel());
        content.revalidate();
        content.repaint();
    }

    private Double[] readArray(String key, IniFileModel iniFile, ConfigurationImage ci) {
        if (iniFile == null)
            return new Double[0];
        Optional<IniField> field = iniFile.findIniField(key);
        if (field.isPresent() && field.get() instanceof ArrayIniField) {
            ArrayIniField arrayField = (ArrayIniField) field.get();
            if (ci != null) {
                Double[][] arrayValues = ConfigurationImageGetterSetter.getArrayValues(arrayField, ci);
                Double[] result = new Double[arrayField.getRows() * arrayField.getCols()];
                int index = 0;
                for (int r = 0; r < arrayField.getRows(); r++) {
                    for (int c = 0; c < arrayField.getCols(); c++) {
                        result[index++] = arrayValues[r][c];
                    }
                }
                return result;
            }

            // Fallback for when ci is null
            int size = arrayField.getRows() * arrayField.getCols();
            Double[] values = new Double[size];
            for (int i = 0; i < size; i++) {
                values[i] = 0.0; // dummy
            }
            return values;
        }
        return new Double[0];
    }

    public JPanel getContentPane() {
        return content;
    }

    public class CurveCanvas extends JPanel {
        private CurveModel curve;
        private Double[] x;
        private Double[] y;

        private Integer draggingIndex = null;

        public CurveCanvas() {
            setBackground(Color.BLACK);
            MouseAdapter mouseAdapter = new MouseAdapter() {
                @Override
                public void mousePressed(MouseEvent e) {
                    draggingIndex = findIndex(e.getPoint());
                }

                @Override
                public void mouseReleased(MouseEvent e) {
                    draggingIndex = null;
                }

                @Override
                public void mouseDragged(MouseEvent e) {
                    if (draggingIndex != null) {
                        updatePoint(draggingIndex, e.getPoint());
                        repaint();
                        table.repaint();
                    }
                }
            };
            addMouseListener(mouseAdapter);
            addMouseMotionListener(mouseAdapter);
        }

        public void setCurve(CurveModel curve, Double[] x, Double[] y) {
            this.curve = curve;
            this.x = x;
            this.y = y;
            repaint();
        }

        private Integer findIndex(Point p) {
            if (x == null || y == null) return null;
            for (int i = 0; i < x.length; i++) {
                Point pt = worldToCanvas(x[i], y[i]);
                if (pt.distance(p) < 10) {
                    return i;
                }
            }
            return null;
        }

        private void updatePoint(int index, Point p) {
            Point2D world = canvasToWorld(p);
            // enforce X-coordinate to stay in order
            double minX = (index == 0) ? curve.getxAxis().getMin() : x[index - 1];
            double maxX = (index == x.length - 1) ? curve.getxAxis().getMax() : x[index + 1];

            x[index] = Math.max(minX, Math.min(maxX, world.x));
            y[index] = Math.max(curve.getyAxis().getMin(), Math.min(curve.getyAxis().getMax(), world.y));
        }

        @Override
        public void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (curve == null) return;

            Graphics2D g2 = (Graphics2D) g;
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

            drawGrid(g2);
            drawCurve(g2);

            g2.setColor(Color.WHITE);
            String title = curve.getTitle();
            int titleWidth = g2.getFontMetrics().stringWidth(title);
            g2.drawString(title, (getWidth() - titleWidth) / 2, 15);

            if (xUnits != null && !xUnits.isEmpty()) {
                int unitsWidth = g2.getFontMetrics().stringWidth(xUnits);
                g2.drawString(xUnits, (getWidth() - unitsWidth) / 2, getHeight() - 5);
            }
        }

        public void drawGrid(Graphics2D g2) {
            Color gridColor = new Color(200, 200, 200, 100); // light grey, semi-transparent
            g2.setStroke(new BasicStroke(0.5f));
            AxisModel xAxis = curve.getxAxis();
            AxisModel yAxis = curve.getyAxis();

            FontMetrics fm = g2.getFontMetrics();

            // Draw vertical grid lines
            for (int i = 0; i < xAxis.getStep(); i++) {
                double val = xAxis.getMin() + (xAxis.getMax() - xAxis.getMin()) * i / (xAxis.getStep() - 1);
                Point p1 = worldToCanvas(val, yAxis.getMin());
                Point p2 = worldToCanvas(val, yAxis.getMax());

                g2.setColor(gridColor);
                g2.drawLine(p1.x, p1.y, p2.x, p2.y);

                g2.setColor(Color.RED);
                String label = String.format("%.1f", val);
                int labelWidth = fm.stringWidth(label);
                g2.drawString(label, p1.x - labelWidth / 2, p1.y + fm.getAscent() + 2);
            }

            // Draw horizontal grid lines
            for (int i = 0; i < yAxis.getStep(); i++) {
                double val = yAxis.getMin() + (yAxis.getMax() - yAxis.getMin()) * i / (yAxis.getStep() - 1);
                Point p1 = worldToCanvas(xAxis.getMin(), val);
                Point p2 = worldToCanvas(xAxis.getMax(), val);

                g2.setColor(gridColor);
                g2.drawLine(p1.x, p1.y, p2.x, p2.y);

                g2.setColor(Color.RED);
                String label = String.format("%.1f", val);
                int labelWidth = fm.stringWidth(label);
                g2.drawString(label, p1.x - labelWidth - 5, p1.y + fm.getAscent() / 2);
            }
            g2.setStroke(new BasicStroke(1.0f));
        }

        private void drawCurve(Graphics2D g2) {
            if (x == null || y == null || x.length == 0) return;

            g2.setColor(Color.ORANGE);
            for (int i = 0; i < x.length - 1; i++) {
                Point p1 = worldToCanvas(x[i], y[i]);
                Point p2 = worldToCanvas(x[i + 1], y[i + 1]);
                g2.drawLine(p1.x, p1.y, p2.x, p2.y);
            }

            g2.setColor(Color.GREEN);
            for (int i = 0; i < x.length; i++) {
                Point p = worldToCanvas(x[i], y[i]);
                g2.fillOval(p.x - 4, p.y - 4, 8, 8);
            }
        }

        private Point worldToCanvas(double wx, double wy) {
            double xMin = curve.getxAxis().getMin();
            double xMax = curve.getxAxis().getMax();
            double yMin = curve.getyAxis().getMin();
            double yMax = curve.getyAxis().getMax();

            int leftPadding = 50;
            int rightPadding = 20;
            int topPadding = 20;
            int bottomPadding = 40;

            int width = getWidth() - leftPadding - rightPadding;
            int height = getHeight() - topPadding - bottomPadding;

            int cx = leftPadding + (int) ((wx - xMin) / (xMax - xMin) * width);
            // y axis is displayed in descending order
            int cy = topPadding + height - (int) ((wy - yMin) / (yMax - yMin) * height);

            return new Point(cx, cy);
        }

        private Point2D canvasToWorld(Point p) {
            double xMin = curve.getxAxis().getMin();
            double xMax = curve.getxAxis().getMax();
            double yMin = curve.getyAxis().getMin();
            double yMax = curve.getyAxis().getMax();

            int leftPadding = 50;
            int rightPadding = 20;
            int topPadding = 20;
            int bottomPadding = 40;

            int width = getWidth() - leftPadding - rightPadding;
            int height = getHeight() - topPadding - bottomPadding;

            double wx = xMin + (double) (p.x - leftPadding) / width * (xMax - xMin);
            double wy = yMin + (double) (topPadding + height - p.y) / height * (yMax - yMin);

            return new Point2D(wx, wy);
        }

        private class Point2D {
            double x, y;
            Point2D(double x, double y) { this.x = x; this.y = y; }
        }
    }

    private class CurveTableModel extends AbstractTableModel {
        @Override
        public int getRowCount() {
            return xValues == null ? 0 : xValues.length;
        }

        @Override
        public int getColumnCount() {
            return 2;
        }

        @Override
        public String getColumnName(int column) {
            return column == 0 ? "X Axis" : "Y Axis";
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            return columnIndex == 0 ? xValues[rowIndex] : yValues[rowIndex];
        }

        @Override
        public boolean isCellEditable(int rowIndex, int columnIndex) {
            return true;
        }

        @Override
        public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
            try {
                double val = Double.parseDouble(aValue.toString());
                if (columnIndex == 0) {
                    // Check order
                    double min = (rowIndex == 0) ? curveModel.getxAxis().getMin() : xValues[rowIndex - 1];
                    double max = (rowIndex == xValues.length - 1) ? curveModel.getxAxis().getMax() : xValues[rowIndex + 1];
                    xValues[rowIndex] = Math.max(min, Math.min(max, val));
                } else {
                    yValues[rowIndex] = Math.max(curveModel.getyAxis().getMin(), Math.min(curveModel.getyAxis().getMax(), val));
                }
                canvas.repaint();
            } catch (NumberFormatException ignored) {}
        }
    }

    private class GradientRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            if (isSelected) return c;

            if (value instanceof Double) {
                double val = (Double) value;
                double min, max;
                if (column == 0) {
                    min = curveModel.getxAxis().getMin();
                    max = curveModel.getxAxis().getMax();
                } else {
                    min = curveModel.getyAxis().getMin();
                    max = curveModel.getyAxis().getMax();
                }
                applyGradient(c, val, min, max);
            }
            return c;
        }

        private void applyGradient(Component c, double value, double minVal, double maxVal) {
            if (maxVal <= minVal) {
                c.setBackground(Color.WHITE);
                return;
            }
            double ratio = (value - minVal) / (maxVal - minVal);
            ratio = Math.max(0, Math.min(1, ratio));

            int r1 = 173, g1 = 216, b1 = 230; // Min value color
            int r2 = 255, g2 = 182, b2 = 193; // Max value color

            int red = (int) (r1 + ratio * (r2 - r1));
            int green = (int) (g1 + ratio * (g2 - g1));
            int blue = (int) (b1 + ratio * (b2 - b1));
            c.setBackground(new Color(red, green, blue));
            c.setForeground(Color.BLACK);
        }
    }
}
