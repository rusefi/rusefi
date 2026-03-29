package com.rusefi.ui.todo_better_module;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorNames;

import javax.swing.*;
import javax.swing.plaf.LayerUI;
import java.awt.*;

/**
 * todo: start using this code for real, not just in Sandbox!
 *
 * Overlay layer that draws a real-time tracer dot on top of TuningTableView.
 * All visual constants and sensor logic are centralized for easy maintenance.
 */
public class TracerLayerUI extends LayerUI<JPanel> {

    // --- Visual Appearance Constants ---
    private static final Color TRACER_GLOW_COLOR = new Color(255, 128, 0, 150);
    private static final Color TRACER_CORE_COLOR = new Color(255, 128, 0);
    private static final Color TRACER_BORDER_COLOR = Color.WHITE;

    private static final int GLOW_SIZE = 16;
    private static final int CORE_SIZE = 10;
    private static final int GLOW_OFFSET = GLOW_SIZE / 2;
    private static final int CORE_OFFSET = CORE_SIZE / 2;

    // todo: sensors should be defined dynamically based on table/curve axis configuration
    private static final String xAxisName = Sensor.RPMGauge.getNativeName();
    private static final String yAxisName = SensorNames.MAPGauge;

    private final Double[] xBins;
    private final Double[] yBins;

    private JComponent owner;
    private double currentXValue = Double.NaN;
    private double currentYValue = Double.NaN;

    private final SensorCentral.SensorListener sensorListener = value -> {
        currentXValue = SensorCentral.getInstance().getValue(xAxisName);
        currentYValue = SensorCentral.getInstance().getValue(yAxisName);
        if (owner != null) {
            owner.repaint();
        }
    };

    public TracerLayerUI(Double[] xBins, Double[] yBins) {
        this.xBins = xBins;
        this.yBins = yBins;

        // Listen to live ECU data and trigger repaint
        SensorCentral sc = SensorCentral.getInstance();
        sc.addListener(xAxisName, sensorListener);
        sc.addListener(yAxisName, sensorListener);
    }

    // todo: this is raising a big question of reliable lifecycle management pattern
    public void dispose() {
        SensorCentral sc = SensorCentral.getInstance();
        sc.removeListener(xAxisName, sensorListener);
        sc.removeListener(yAxisName, sensorListener);
        this.owner = null;
    }

    /**
     * Sets the component that will be repainted when sensor data arrives.
     */
    public void setOwner(JComponent owner) {
        this.owner = owner;
    }

    @Override
    public void paint(Graphics g, JComponent c) {
        // 1. Paint the original table/panel first
        super.paint(g, c);

        // 2. Abort if data is invalid or missing
        if (Double.isNaN(currentXValue) || Double.isNaN(currentYValue)) return;

        // 3. Find the JTable within the panel hierarchy
        JTable table = findTable(c);
        if (table == null || xBins == null || yBins == null) return;

        drawTracer(g, table, c);
    }

    private void drawTracer(Graphics g, JTable table, JComponent c) {
        // Calculate raw pixel coordinates relative to the table
        double posX = getInterpolatedPixels(currentXValue, xBins, table, true);
        double posY = getInterpolatedPixels(currentYValue, yBins, table, false);

        if (posX < 0 || posY < 0) return;

        // Map coordinates to the JLayer's coordinate system
        Point tableLoc = table.getLocationOnScreen();
        Point layerLoc = c.getLocationOnScreen();
        int finalX = (int) (tableLoc.x - layerLoc.x + posX);
        int finalY = (int) (tableLoc.y - layerLoc.y + posY);

        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        // Draw Glow
        g2.setColor(TRACER_GLOW_COLOR);
        g2.fillOval(finalX - GLOW_OFFSET, finalY - GLOW_OFFSET, GLOW_SIZE, GLOW_SIZE);

        // Draw Core
        g2.setColor(TRACER_CORE_COLOR);
        g2.fillOval(finalX - CORE_OFFSET, finalY - CORE_OFFSET, CORE_SIZE, CORE_SIZE);

        // Draw Border
        g2.setColor(TRACER_BORDER_COLOR);
        g2.drawOval(finalX - CORE_OFFSET, finalY - CORE_OFFSET, CORE_SIZE, CORE_SIZE);

        g2.dispose();
    }

    private double getInterpolatedPixels(double value, Double[] bins, JTable table, boolean isX) {
        if (bins.length < 2) return -1;

        // Find interval
        int i = 0;
        while (i < bins.length - 1 && value > bins[i+1]) i++;

        // Linear interpolation factor
        double factor = (value - bins[i]) / (bins[i+1] - bins[i]);
        factor = Math.max(0, Math.min(1, factor));

        // Determine cell indices (RPM is X, Load is Y inverted)
        int cell1 = isX ? i + 1 : (bins.length - 1 - i);
        int cell2 = isX ? i + 2 : (bins.length - 2 - i);

        if (cell1 < 0 || cell2 < 0) return -1;

        Rectangle r1 = isX ? table.getCellRect(0, cell1, true) : table.getCellRect(cell1, 0, true);
        Rectangle r2 = isX ? table.getCellRect(0, cell2, true) : table.getCellRect(cell2, 0, true);

        double p1 = isX ? r1.getCenterX() : r1.getCenterY();
        double p2 = isX ? r2.getCenterX() : r2.getCenterY();

        return p1 + factor * (p2 - p1);
    }

    private JTable findTable(Container container) {
        for (Component comp : container.getComponents()) {
            if (comp instanceof JTable) return (JTable) comp;
            if (comp instanceof Container) {
                JTable table = findTable((Container) comp);
                if (table != null) return table;
            }
        }
        return null;
    }
}
