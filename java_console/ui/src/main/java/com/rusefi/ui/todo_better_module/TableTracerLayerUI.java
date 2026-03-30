package com.rusefi.ui.todo_better_module;

import com.rusefi.core.SensorCentral;

import javax.swing.*;
import javax.swing.plaf.LayerUI;
import java.awt.*;
import java.awt.geom.Ellipse2D;

/**
 * A UI Layer responsible for drawing a real-time tracer dot over a JTable.
 * It interpolates sensor data against provided bin scales to pinpoint the exact engine state.
 */
public class TableTracerLayerUI extends LayerUI<JPanel> {
    private static final Color TRACER_HIGHLIGHT_COLOR = new Color(255, 128, 0);
    private static final int TRACER_DOT_DIAMETER = 12;

    private final String xAxisSensorChannel;
    private final String yAxisSensorChannel;
    private final double[] xAxisBinScales;
    private final double[] yAxisBinScales;

    private JComponent parentLayerComponent;
    private double currentXAxisValue = Double.NaN;
    private double currentYAxisValue = Double.NaN;

    /**
     * Listener for X-Axis sensor updates. Triggers a repaint of the decorated component.
     */
    private final SensorCentral.SensorListener xAxisSensorListener = newValue -> {
        currentXAxisValue = newValue;
        requestRepaint();
    };

    /**
     * Listener for Y-Axis sensor updates. Triggers a repaint of the decorated component.
     */
    private final SensorCentral.SensorListener yAxisSensorListener = newValue -> {
        currentYAxisValue = newValue;
        requestRepaint();
    };

    public TableTracerLayerUI(String xAxisChannel, String yAxisChannel, double[] xAxisBins, double[] yAxisBins) {
        this.xAxisSensorChannel = xAxisChannel;
        this.yAxisSensorChannel = yAxisChannel;
        this.xAxisBinScales = xAxisBins;
        this.yAxisBinScales = yAxisBins;

        SensorCentral.getInstance().addListener(xAxisSensorChannel, xAxisSensorListener);
        SensorCentral.getInstance().addListener(yAxisSensorChannel, yAxisSensorListener);
    }

    public void setOwner(JComponent owner) {
        this.parentLayerComponent = owner;
    }

    private void requestRepaint() {
        if (parentLayerComponent != null) {
            parentLayerComponent.repaint();
        }
    }

    @Override
    public void paint(Graphics graphics, JComponent layerComponent) {
        // Paint the underlying components first
        super.paint(graphics, layerComponent);

        if (Double.isNaN(currentXAxisValue) || Double.isNaN(currentYAxisValue)) {
            return;
        }

        JTable targetTable = findTargetTable(layerComponent);
        if (targetTable == null || !targetTable.isShowing()) {
            return;
        }

        Point tracerPixelLocation = calculateTracerPixelCoordinates(targetTable, layerComponent);
        if (tracerPixelLocation != null) {
            drawTracerDot(graphics, targetTable, layerComponent, tracerPixelLocation);
        }
    }

    private void drawTracerDot(Graphics graphics, JTable table, JComponent layer, Point location) {
        Graphics2D graphics2D = (Graphics2D) graphics.create();
        try {
            graphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

            // Restrict drawing to the actual data area to avoid overlapping header/labels
            Shape originalClip = graphics2D.getClip();
            Rectangle tableDataArea = calculateTableDataArea(table, layer);
            graphics2D.setClip(tableDataArea);

            graphics2D.setColor(TRACER_HIGHLIGHT_COLOR);
            double offset = (double) TRACER_DOT_DIAMETER / 2;

            // Using Ellipse2D for smoother sub-pixel positioning if needed in the future
            graphics2D.fill(new Ellipse2D.Double(location.x - offset, location.y - offset, TRACER_DOT_DIAMETER, TRACER_DOT_DIAMETER));

            graphics2D.setClip(originalClip);
        } finally {
            graphics2D.dispose();
        }
    }

    public void unsubscribe() {
        SensorCentral.getInstance().removeListener(xAxisSensorChannel, xAxisSensorListener);
        SensorCentral.getInstance().removeListener(yAxisSensorChannel, yAxisSensorListener);
    }

    /**
     * Recursively searches for the JTable component within the decorated panel.
     */
    private JTable findTargetTable(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JTable) {
                return (JTable) component;
            }
            if (component instanceof Container) {
                JTable nestedTable = findTargetTable((Container) component);
                if (nestedTable != null) {
                    return nestedTable;
                }
            }
        }
        return null;
    }

    /**
     * Calculates the clipping area that only includes the data cells (ignoring the Y-axis label column).
     */
    private Rectangle calculateTableDataArea(JTable table, JComponent layer) {
        // Data usually starts at Column 1 (Column 0 is labels)
        Rectangle firstDataCell = table.getCellRect(0, 1, true);
        Rectangle lastDataCell = table.getCellRect(table.getRowCount() - 1, table.getColumnCount() - 1, true);
        Rectangle mergedDataArea = firstDataCell.union(lastDataCell);

        return SwingUtilities.convertRectangle(table, mergedDataArea, layer);
    }

    /**
     * Computes the screen coordinates for the tracer by interpolating current values against bin scales.
     */
    private Point calculateTracerPixelCoordinates(JTable table, JComponent layer) {
        double fractionalXIndex = findFractionalIndexByValue(xAxisBinScales, currentXAxisValue);
        double fractionalYIndex = findFractionalIndexByValue(yAxisBinScales, currentYAxisValue);

        if (fractionalXIndex < 0 || fractionalYIndex < 0) {
            return null;
        }

        // Inverting Y because row 0 is at the top of the table
        double invertedRowIndex = (yAxisBinScales.length - 1) - fractionalYIndex;

        // Data starts at Column 1. Adjusting indexes for JTable coordinate system.
        int dataColumnOffset = 1;

        int firstColumn = (int) Math.floor(fractionalXIndex) + dataColumnOffset;
        int secondColumn = Math.min(firstColumn + 1, table.getColumnCount() - 1);
        double xInterpolationFraction = fractionalXIndex - Math.floor(fractionalXIndex);

        int firstRow = (int) Math.floor(invertedRowIndex);
        int secondRow = Math.min(firstRow + 1, table.getRowCount() - 1);
        double yInterpolationFraction = invertedRowIndex - firstRow;

        Rectangle cellStart = table.getCellRect(firstRow, firstColumn, true);
        Rectangle cellEnd = table.getCellRect(secondRow, secondColumn, true);

        // Linear interpolation of pixel centers
        double pixelX = cellStart.getCenterX() + xInterpolationFraction * (cellEnd.getCenterX() - cellStart.getCenterX());
        double pixelY = cellStart.getCenterY() + yInterpolationFraction * (cellEnd.getCenterY() - cellStart.getCenterY());

        return SwingUtilities.convertPoint(table, new Point((int) pixelX, (int) pixelY), layer);
    }

    /**
     * Finds the fractional index within the bin array for a given value using linear interpolation.
     */
    private double findFractionalIndexByValue(double[] binScales, double value) {
        if (binScales == null || binScales.length < 2) {
            return -1;
        }

        // Clamp values to stay visible within table bounds
        if (value <= binScales[0]) return 0;
        if (value >= binScales[binScales.length - 1]) return binScales.length - 1;

        for (int i = 0; i < binScales.length - 1; i++) {
            if (value >= binScales[i] && value <= binScales[i + 1]) {
                double binRange = binScales[i + 1] - binScales[i];
                return i + (binRange == 0 ? 0 : (value - binScales[i]) / binRange);
            }
        }
        return -1;
    }
}