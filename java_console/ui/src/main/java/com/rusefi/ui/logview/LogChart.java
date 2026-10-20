package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import javax.swing.JComponent;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.geom.Path2D;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;

/** Time plot for the fields selected in the companion list. */
final class LogChart extends JComponent {
    private static final Color BACKGROUND = new Color(21, 27, 38);
    private static final Color GRID = new Color(55, 67, 82);
    private static final Color LABEL = new Color(198, 209, 222);
    private static final Color[] COLORS = {
            new Color(255, 191, 71), new Color(91, 205, 250), new Color(119, 226, 171),
            new Color(246, 131, 157), new Color(176, 155, 251), new Color(255, 143, 88),
            new Color(118, 222, 218), new Color(233, 205, 101)
    };

    private final CsvLogReader log;
    private final boolean[] displayed;
    private int cursorSample = -1;

    LogChart(CsvLogReader log, boolean[] displayed) {
        this.log = log;
        this.displayed = displayed;
        setOpaque(true);
        setBackground(BACKGROUND);
        addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseMoved(MouseEvent event) {
                cursorSample = nearestSample(event.getX());
                repaint();
            }
        });
    }

    private int nearestSample(int x) {
        double start = log.minimum(0);
        double span = log.maximum(0) - start;
        if (span <= 0 || getWidth() <= 70) {
            return -1;
        }
        double time = start + (x - 54.0) * span / (getWidth() - 76.0);
        int nearest = 0;
        double distance = Double.POSITIVE_INFINITY;
        for (int i = 0; i < log.sampleCount(); i++) {
            double candidate = Math.abs(log.value(i, 0) - time);
            if (candidate < distance) {
                distance = candidate;
                nearest = i;
            }
        }
        return nearest;
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        super.paintComponent(graphics);
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.setColor(BACKGROUND);
            g.fillRect(0, 0, getWidth(), getHeight());
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            int left = 54;
            int right = Math.max(left + 1, getWidth() - 22);
            int top = 68;
            int bottom = Math.max(top + 1, getHeight() - 46);
            FontMetrics metrics = g.getFontMetrics();

            g.setColor(LABEL);
            g.drawString("Selected signals", left, 23);
            List<Integer> signals = selectedSignals();
            int legendX = left;
            int legendY = 46;
            for (int index = 0; index < signals.size(); index++) {
                int field = signals.get(index);
                String label = legendLabel(field, cursorSample);
                int width = metrics.stringWidth(label) + 32;
                if (legendX + width > right && legendX > left) {
                    legendX = left;
                    legendY += 19;
                }
                g.setColor(COLORS[index % COLORS.length]);
                g.fillRoundRect(legendX, legendY - 10, 13, 5, 4, 4);
                g.setColor(LABEL);
                g.drawString(label, legendX + 19, legendY);
                legendX += width;
            }
            top = Math.max(top, legendY + 19);
            bottom = Math.max(top + 1, getHeight() - 46);

            g.setColor(GRID);
            for (int i = 0; i <= 4; i++) {
                int y = top + (bottom - top) * i / 4;
                g.drawLine(left, y, right, y);
            }
            for (int i = 0; i <= 6; i++) {
                int x = left + (right - left) * i / 6;
                g.drawLine(x, top, x, bottom);
                double time = log.minimum(0) + (log.maximum(0) - log.minimum(0)) * i / 6.0;
                String text = String.format(Locale.ROOT, "%.1f", time);
                g.setColor(LABEL);
                g.drawString(text, x - metrics.stringWidth(text) / 2, bottom + 19);
                g.setColor(GRID);
            }
            g.setColor(LABEL);
            String timeLabel = GAUGE_NAME_TIME + " (s)";
            g.drawString(timeLabel, (left + right - metrics.stringWidth(timeLabel)) / 2, bottom + 37);
            if (signals.isEmpty()) {
                g.setColor(LABEL);
                g.drawString("Select a field to plot", left + 20, top + 30);
                return;
            }

            double start = log.minimum(0);
            double span = Math.max(1e-9, log.maximum(0) - start);
            g.setStroke(new BasicStroke(1.5f));
            for (int index = 0; index < signals.size(); index++) {
                int field = signals.get(index);
                double min = log.minimum(field);
                double range = log.maximum(field) - min;
                if (!Double.isFinite(min)) {
                    continue;
                }
                Path2D path = new Path2D.Double();
                boolean drawing = false;
                for (int sample = 0; sample < log.sampleCount(); sample++) {
                    double value = log.value(sample, field);
                    if (!Double.isFinite(value)) {
                        drawing = false;
                        continue;
                    }
                    double x = left + (log.value(sample, 0) - start) * (right - left) / span;
                    double y = top + (1 - (range == 0 ? 0.5 : (value - min) / range)) * (bottom - top);
                    if (!drawing) {
                        path.moveTo(x, y);
                        drawing = true;
                    } else {
                        path.lineTo(x, y);
                    }
                }
                g.setColor(COLORS[index % COLORS.length]);
                g.draw(path);
            }
            if (cursorSample >= 0) {
                int x = left + (int) ((log.value(cursorSample, 0) - start) * (right - left) / span);
                g.setColor(new Color(220, 230, 245, 155));
                g.setStroke(new BasicStroke(1));
                g.drawLine(x, top, x, bottom);
            }
        } finally {
            g.dispose();
        }
    }

    private List<Integer> selectedSignals() {
        List<Integer> result = new ArrayList<>();
        for (int field = 1; field < displayed.length; field++) {
            if (displayed[field]) {
                result.add(field);
            }
        }
        return result;
    }

    private String legendLabel(int field, int sample) {
        String unit = log.unit(field);
        String suffix = unit.isEmpty() ? "" : " " + unit;
        if (sample >= 0 && Double.isFinite(log.value(sample, field))) {
            return String.format(Locale.ROOT, "%s %.2f%s", log.name(field), log.value(sample, field), suffix);
        }
        return String.format(Locale.ROOT, "%s %.1f–%.1f%s", log.name(field), log.minimum(field), log.maximum(field), suffix);
    }
}
