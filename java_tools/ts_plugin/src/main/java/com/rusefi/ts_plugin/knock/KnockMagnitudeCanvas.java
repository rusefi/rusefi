package com.rusefi.ts_plugin.knock;

import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.ts_plugin.knock.KnockAnalyzerTab.VALUE_COUNT;

/**
 * Magnitude (single FFT slice) plot drawn under the spectrogram.
 * <p>
 * Painting happens directly in {@code paintComponent} from the last received slice, so the plot
 * always fits the current component size: no stale back-buffer, no stretched/torn geometry after
 * a resize. Frequency labels are drawn horizontally every {@link #LABEL_STEP} bins to keep them
 * readable instead of overlapping 64 rotated strings.
 */
public class KnockMagnitudeCanvas {
    private static final int LABEL_STEP = 8;
    private static final float MAX_MAGNITUDE = 255f;

    private static final int MARGIN_LEFT = 52;
    private static final int MARGIN_RIGHT = 10;
    private static final int MARGIN_TOP = 8;

    private KnockPlotStyle style = KnockPlotStyle.DEFAULT;

    private final JComponent component = new JComponent() {
        @Override
        protected void paintComponent(Graphics g) {
            render((Graphics2D) g, getWidth(), getHeight());
        }
    };

    private final double[] yAxisHz = new double[VALUE_COUNT];
    private final float[] magnitudes = new float[VALUE_COUNT];

    private int yAxisFrequencyStart = -1;
    private float yAxisFrequencyStep = -1;

    public KnockMagnitudeCanvas() {
        component.setOpaque(true);
        component.setBackground(style.background);
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayoutAndRepaint(component));
    }

    public JComponent getComponent() {
        return component;
    }

    /** Replaces the colours, e.g. with the console's Log Viewer palette. */
    public void setStyle(KnockPlotStyle style) {
        this.style = style;
        component.setBackground(style.background);
        component.repaint();
    }

    public void setFrequencyStart(int start) {
        boolean needSetup = this.yAxisFrequencyStart < 0;
        this.yAxisFrequencyStart = start;
        if (needSetup)
            setupFrequencyyAxis();
    }

    public void setFrequencyStep(float step) {
        boolean needSetup = this.yAxisFrequencyStep < 0;
        this.yAxisFrequencyStep = step;
        if (needSetup)
            setupFrequencyyAxis();
    }

    public void setupFrequencyyAxis() {
        if (this.yAxisFrequencyStep < 0 || this.yAxisFrequencyStart < 0)
            return;

        for (int i = 0; i < VALUE_COUNT; ++i)
            yAxisHz[i] = yAxisFrequencyStart + (double) yAxisFrequencyStep * i;
    }

    /** A copy of the slice currently shown. */
    public float[] getMagnitudes() {
        return magnitudes.clone();
    }

    public void processValues(float[] values) {
        System.arraycopy(values, 0, magnitudes, 0, Math.min(values.length, VALUE_COUNT));
        AutoupdateUtil.trueLayoutAndRepaint(component);
    }

    // ---- rendering -----------------------------------------------------------

    private void render(Graphics2D g, int width, int height) {
        g.setColor(style.background);
        g.fillRect(0, 0, width, height);

        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

        Font base = g.getFont();
        g.setFont(base.deriveFont(Font.PLAIN, Math.max(9f, base.getSize2D() - 2)));
        FontMetrics fm = g.getFontMetrics();

        int labelsHeight = fm.getHeight() + 6;
        int left = MARGIN_LEFT;
        int plotW = Math.max(1, width - MARGIN_LEFT - MARGIN_RIGHT);
        int top = MARGIN_TOP;
        int plotH = Math.max(1, height - MARGIN_TOP - labelsHeight);
        int bottom = top + plotH;

        drawAmplitudeAxis(g, fm, left, top, plotW, plotH);
        drawFrequencyAxis(g, fm, left, bottom, plotW);
        drawMagnitudes(g, left, bottom, plotW, plotH);
        drawPeak(g, fm, left, top, bottom, plotW, plotH);

        g.setFont(base);
    }

    private void drawAmplitudeAxis(Graphics2D g, FontMetrics fm, int left, int top, int plotW, int plotH) {
        int lines = Math.max(1, Math.min(4, plotH / (fm.getHeight() * 2)));
        for (int i = 0; i <= lines; i++) {
            int y = top + plotH - plotH * i / lines;
            g.setColor(style.grid);
            g.drawLine(left, y, left + plotW, y);

            String label = Integer.toString((int) (MAX_MAGNITUDE * i / lines));
            g.setColor(style.axisText);
            g.drawString(label, Math.max(2, left - 6 - fm.stringWidth(label)), y + fm.getAscent() / 2);
        }
    }

    private void drawFrequencyAxis(Graphics2D g, FontMetrics fm, int left, int bottom, int plotW) {
        int step = labelStep(plotW, fm.stringWidth("00.0k") + 8);
        for (int i = 0; i < VALUE_COUNT; i += step) {
            int x = binToX(i, left, plotW);

            g.setColor(style.grid);
            g.drawLine(x, bottom, x, bottom + 4);

            String label = formatHz(yAxisHz[i]);
            int textX = Math.max(0, Math.min(x - fm.stringWidth(label) / 2, left + plotW - fm.stringWidth(label)));
            g.setColor(style.axisText);
            g.drawString(label, textX, bottom + 4 + fm.getAscent());
        }
    }

    /** Picks a label step so that neighbour labels never overlap on a narrow component. */
    private static int labelStep(int plotW, int labelWidth) {
        for (int step = LABEL_STEP; step < VALUE_COUNT; step *= 2) {
            if ((double) plotW * step / VALUE_COUNT >= labelWidth)
                return step;
        }
        return VALUE_COUNT;
    }

    private void drawMagnitudes(Graphics2D g, int left, int bottom, int plotW, int plotH) {
        int[] xPoints = new int[VALUE_COUNT + 2];
        int[] yPoints = new int[VALUE_COUNT + 2];

        for (int i = 0; i < VALUE_COUNT; ++i) {
            xPoints[i] = binToX(i, left, plotW);
            yPoints[i] = magnitudeToY(magnitudes[i], bottom, plotH);
        }
        // close the polygon along the bottom edge
        xPoints[VALUE_COUNT] = left + plotW;
        yPoints[VALUE_COUNT] = bottom;
        xPoints[VALUE_COUNT + 1] = left;
        yPoints[VALUE_COUNT + 1] = bottom;

        g.setColor(style.magnitudeFill);
        g.fillPolygon(xPoints, yPoints, VALUE_COUNT + 2);
        g.setColor(style.magnitudeLine);
        g.drawPolyline(xPoints, yPoints, VALUE_COUNT);
    }

    private void drawPeak(Graphics2D g, FontMetrics fm, int left, int top, int bottom, int plotW, int plotH) {
        int index = searchPeak();
        if (index < 0)
            return;

        int x = binToX(index, left, plotW);
        if (style.logViewerLayout) {
            drawPeakLabel(g, fm, x, left, top, bottom, plotW, index);
            return;
        }
        g.setColor(style.magnitudePeak);
        g.drawLine(x, top, x, bottom);

        String label = formatHz(yAxisHz[index]) + " Hz / " + String.format("%.0f", magnitudes[index]);
        int textX = Math.max(left, Math.min(x + 4, left + plotW - fm.stringWidth(label)));
        g.drawString(label, textX, top + fm.getAscent());
    }

    /** Log Viewer layout: dashed peak line and a tinted "Peak" label in the value font. */
    private void drawPeakLabel(Graphics2D g, FontMetrics axis, int x, int left, int top, int bottom, int plotW, int index) {
        Stroke stroke = g.getStroke();
        g.setColor(style.magnitudePeak);
        g.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER, 10, new float[]{4, 3}, 0));
        g.drawLine(x, top, x, bottom);
        g.setStroke(stroke);

        Font font = style.valueFontFamily == null ? axis.getFont().deriveFont(Font.BOLD)
            : new Font(style.valueFontFamily, Font.BOLD, axis.getFont().getSize());
        g.setFont(font);
        FontMetrics fm = g.getFontMetrics();
        String label = "Peak " + formatHz(yAxisHz[index]) + " Hz · " + String.format("%.0f", magnitudes[index]);
        int w = fm.stringWidth(label) + 14;
        int h = fm.getHeight() + 4;
        int boxX = Math.max(left, Math.min(x + 6, left + plotW - w));
        Color peak = style.magnitudePeak;
        g.setColor(new Color(peak.getRed(), peak.getGreen(), peak.getBlue(), 40));
        g.fillRoundRect(boxX, top + 2, w, h, 8, 8);
        g.setColor(peak);
        g.drawString(label, boxX + 7, top + 4 + fm.getAscent());
        g.setFont(axis.getFont());
    }

    private int searchPeak() {
        int index = -1;
        float max = 0;
        for (int i = 0; i < VALUE_COUNT; ++i) {
            if (magnitudes[i] > max) {
                max = magnitudes[i];
                index = i;
            }
        }
        return index;
    }

    private static int binToX(int bin, int left, int plotW) {
        return left + (int) Math.round((double) plotW * bin / (VALUE_COUNT - 1));
    }

    private static int magnitudeToY(float magnitude, int bottom, int plotH) {
        float normalized = magnitude / MAX_MAGNITUDE;
        if (normalized < 0)
            normalized = 0;
        if (normalized > 1)
            normalized = 1;
        return bottom - (int) (plotH * normalized);
    }

    private static String formatHz(double hz) {
        if (hz >= 1000)
            return String.format("%.1fk", hz / 1000);
        return String.format("%.0f", hz);
    }
}
