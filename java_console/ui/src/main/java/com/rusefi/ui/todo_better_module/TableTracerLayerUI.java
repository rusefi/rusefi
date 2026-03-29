package com.rusefi.ui.todo_better_module;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorNames;

import javax.swing.*;
import javax.swing.plaf.LayerUI;
import javax.swing.table.TableModel;
import java.awt.*;
import java.lang.reflect.Field;

public class TableTracerLayerUI extends LayerUI<JPanel> {
     //Private constants for UI styling
    private static final Color TRACER_COLOR = new Color(255, 128, 0);
    private static final int DOT_SIZE = 12;

     //Private constants for sensor mapping
    private static final String RPM_SENSOR = SensorNames.RPMValue;
    private static final String LOAD_SENSOR = SensorNames.MAPGauge;

    private JComponent owner;
    private double currentRpm = Double.NaN;
    private double currentLoad = Double.NaN;

    public TableTracerLayerUI() {
        SensorCentral.SensorListener listener = value -> {
             //Updated to use private static final sensors
            currentRpm = SensorCentral.getInstance().getValue(RPM_SENSOR);
            currentLoad = SensorCentral.getInstance().getValue(LOAD_SENSOR);
            if (owner != null) owner.repaint();
        };
        SensorCentral.getInstance().addListener(RPM_SENSOR, listener);
        SensorCentral.getInstance().addListener(LOAD_SENSOR, listener);
    }

    public void setOwner(JComponent owner) {
        this.owner = owner;
    }

    @Override
    public void paint(Graphics g, JComponent c) {
        super.paint(g, c);
        if (Double.isNaN(currentRpm) || Double.isNaN(currentLoad)) return;

        JTable table = findTable(c);
        if (table == null || !table.isShowing()) return;

        TableModel model = table.getModel();
        Double[] xBins = getPrivateField(model, "xBins");
        Double[] yBins = getPrivateField(model, "yBins");

        if (xBins != null && yBins != null) {
            drawTracer(g, table, c, xBins, yBins);
        }
    }

    private Double[] getPrivateField(Object obj, String fieldName) {
        try {
            Field field = obj.getClass().getDeclaredField(fieldName);
            field.setAccessible(true);
            return (Double[]) field.get(obj);
        } catch (Exception e) {
            return null;
        }
    }

    private void drawTracer(Graphics g, JTable table, JComponent layer, Double[] xBins, Double[] yBins) {
        double x = getInterpolatedPixels(currentRpm, xBins, table, true);
        double y = getInterpolatedPixels(currentLoad, yBins, table, false);

        if (x < 0 || y < 0) return;

        Point p = SwingUtilities.convertPoint(table, new Point((int) x, (int) y), layer);

        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g2.setColor(TRACER_COLOR);
        g2.fillOval(p.x - DOT_SIZE/2, p.y - DOT_SIZE/2, DOT_SIZE, DOT_SIZE);
        g2.setColor(Color.WHITE);
        g2.drawOval(p.x - DOT_SIZE/2, p.y - DOT_SIZE/2, DOT_SIZE, DOT_SIZE);
        g2.dispose();
    }

    private double getInterpolatedPixels(double value, Double[] bins, JTable table, boolean isX) {
        if (bins.length < 2) return -1;

        int i = 0;
        if (value <= bins[0]) i = 0;
        else if (value >= bins[bins.length - 1]) i = bins.length - 2;
        else while (i < bins.length - 1 && value > bins[i+1]) i++;

        double diff = bins[i+1] - bins[i];
        double factor = (diff == 0) ? 0 : (value - bins[i]) / diff;
        factor = Math.max(0, Math.min(1, factor));

        int colCount = table.getColumnCount();
        int rowCount = table.getRowCount();

        int cell1, cell2;
        if (isX) {
            cell1 = i + 1;
            cell2 = i + 2;
        } else {
            cell1 = bins.length - 1 - i;
            cell2 = bins.length - 2 - i;
        }

        if (cell1 < 0 || (isX && cell1 >= colCount) || (!isX && cell1 >= rowCount)) return -1;

        Rectangle r1 = isX ? table.getCellRect(0, cell1, true) : table.getCellRect(cell1, 0, true);
        double p1 = isX ? r1.getCenterX() : r1.getCenterY();

        if (cell2 < 0 || (isX && cell2 >= colCount) || (!isX && cell2 >= rowCount)) return p1;

        Rectangle r2 = isX ? table.getCellRect(0, cell2, true) : table.getCellRect(cell2, 0, true);
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
