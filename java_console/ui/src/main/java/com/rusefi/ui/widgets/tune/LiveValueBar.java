package com.rusefi.ui.widgets.tune;

import com.rusefi.ui.widgets.CircularGauge;
import com.rusefi.ui.widgets.GaugeStyle;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.RoundRectangle2D;

/**
 * A dialog readout's value label with a {@link LiveValueTrace} behind the text. Without a usable
 * range it is a plain label.
 */
public class LiveValueBar extends JLabel {
    /** Wide enough that the fill reads as a bar, fixed so the bar does not resize with the text. */
    static final int BAR_WIDTH = 120;
    private static final int ARC = 6;

    private final LiveValueTrace trace = new LiveValueTrace();
    private final Color normalForeground;

    public LiveValueBar(String text) {
        super(text);
        normalForeground = getForeground();
    }

    /** Warning and danger limits for the fill and text colour; null for none. */
    public void setZones(CircularGauge.Zones zones) {
        trace.setZones(zones);
        setForeground(trace.textColor(normalForeground));
    }

    /** Shows the bar for {@code [min, max]}; a missing or empty range leaves a plain label. */
    public void setRange(double min, double max) {
        trace.setRange(min, max);
        if (trace.hasRange()) {
            setHorizontalAlignment(CENTER);
            setBorder(BorderFactory.createEmptyBorder(1, 6, 1, 6));
            Dimension size = new Dimension(BAR_WIDTH, getPreferredSize().height);
            setPreferredSize(size);
            setMinimumSize(size);
            setMaximumSize(size);
        }
    }

    public void setValue(double value) {
        setValue(value, System.currentTimeMillis());
    }

    void setValue(double value, long nowMs) {
        trace.add(value, nowMs);
        setForeground(trace.textColor(normalForeground));
        repaint();
    }

    LiveValueTrace trace() {
        return trace;
    }

    @Override
    protected void paintComponent(Graphics g) {
        if (trace.hasRange()) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.clip(new RoundRectangle2D.Double(0, 0, getWidth(), getHeight(), ARC, ARC));
                trace.paint(g2, new Rectangle(0, 0, getWidth(), getHeight()), GaugeStyle.track(), getForeground());
            } finally {
                g2.dispose();
            }
        }
        super.paintComponent(g);
    }
}
