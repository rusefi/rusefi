package com.rusefi.ui.todo_better_module;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.AxisModel;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import javax.swing.plaf.LayerUI;
import java.awt.*;

public class CurveTracerLayerUI extends LayerUI<JPanel> {
    private static final Color TRACER_COLOR = new Color(255, 128, 0);
    private static final int DOT_SIZE = 14;

    private JComponent owner;
    private final UIContext uiContext;
    private final String curveId;
    private final Sensor targetSensor;
    private final SensorCentral.SensorListener listener;
    private double currentXValue = Double.NaN;

    public CurveTracerLayerUI(UIContext uiContext, String curveId, Sensor targetSensor) {
        this.uiContext = uiContext;
        this.curveId = curveId;
        this.targetSensor = targetSensor;

        this.listener = value -> {
            currentXValue = SensorCentral.getInstance().getValue(this.targetSensor);
            if (owner != null) owner.repaint();
        };

        SensorCentral.getInstance().addListener(targetSensor.getNativeName(), listener);
    }

    public void setOwner(JComponent owner) {
        this.owner = owner;
    }

    @Override
    public void paint(Graphics g, JComponent c) {
        super.paint(g, c);

        if (Double.isNaN(currentXValue)) return;

        CurveModel curve = uiContext.iniFileState.getIniFileModel().getCurves().get(curveId);
        if (curve == null) return;

        Component chartArea = findChartArea(c);

        ConfigurationImage image = uiContext.getLinkManager().getBinaryProtocol().getControllerConfiguration();
        double[] xData = getFieldValues(curve.getxBins(), image, 0.1);
        double[] yData = getFieldValues(curve.getyBins(), image, 0.02);

        if (xData == null || yData == null) return;

        drawTracer(g, c, chartArea, curve, xData, yData);
    }

    private void drawTracer(Graphics g, JComponent layer, Component chart, CurveModel curve, double[] xData, double[] yData) {
        AxisModel xAxis = curve.getxAxis();
        AxisModel yAxis = curve.getyAxis();

        double xRatio = (currentXValue - xAxis.getMin()) / (xAxis.getMax() - xAxis.getMin());
        xRatio = Math.max(0, Math.min(1, xRatio));

        double currentYValue = interpolate(currentXValue, xData, yData);
        double yRatio = (currentYValue - yAxis.getMin()) / (yAxis.getMax() - yAxis.getMin());
        yRatio = Math.max(0, Math.min(1, yRatio));

        int xPos, yPos;

        if (chart != null) {

            Point loc = SwingUtilities.convertPoint(chart, 0, 0, layer);
            int pad = 35; // Margen interno de la gráfica
            xPos = loc.x + pad + (int) (xRatio * (chart.getWidth() - pad * 2));
            yPos = loc.y + chart.getHeight() - pad - (int) (yRatio * (chart.getHeight() - pad * 2));
        } else {
            xPos = 50 + (int)(xRatio * 100);
            yPos = 50;
        }

        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g2.setColor(TRACER_COLOR);
        g2.fillOval(xPos - DOT_SIZE/2, yPos - DOT_SIZE/2, DOT_SIZE, DOT_SIZE);
        g2.setColor(Color.WHITE);
        g2.setStroke(new BasicStroke(2));
        g2.drawOval(xPos - DOT_SIZE/2, yPos - DOT_SIZE/2, DOT_SIZE, DOT_SIZE);
        g2.dispose();
    }

    private Component findChartArea(Container container) {
        for (Component child : container.getComponents()) {
            String name = child.getClass().getSimpleName();
            if (name.contains("Chart") || name.contains("GaugesPanel") || name.contains("Canvas")) {
                return child;
            }
            if (child instanceof Container) {
                Component found = findChartArea((Container) child);
                if (found != null) return found;
            }
        }
        return null;
    }

    private double[] getFieldValues(String fieldName, ConfigurationImage image, double defaultScale) {
        IniField field = uiContext.iniFileState.getIniFileModel().getIniField(fieldName);
        if (field == null) return null;

        byte[] buffer = image.getContent();
        int offset = field.getOffset();
        double[] values = new double[8];
        for (int i = 0; i < 8; i++) {
            if (offset + i >= buffer.length) break;
            int raw = buffer[offset + i] & 0xFF;
            values[i] = raw * defaultScale;
        }
        return values;
    }

    private double interpolate(double x, double[] xBins, double[] yBins) {
        if (x <= xBins[0]) return yBins[0];
        if (x >= xBins[xBins.length - 1]) return yBins[yBins.length - 1];
        for (int i = 0; i < xBins.length - 1; i++) {
            if (x >= xBins[i] && x <= xBins[i + 1]) {
                double diff = xBins[i + 1] - xBins[i];
                double factor = (diff == 0) ? 0 : (x - xBins[i]) / diff;
                return yBins[i] + factor * (yBins[i + 1] - yBins[i]);
            }
        }
        return yBins[0];
    }
}
