package com.rusefi.ts_plugin.knock;

import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

/**
 * Scrolling knock spectrogram.
 * <p>
 * The spectrogram is accumulated into a {@link BufferedImage} of exactly one pixel per
 * (time, frequency bin) cell, so the on-screen result is resolution independent: the image is
 * simply stretched into the plot area on every repaint. All decorations (axis, legend, markers)
 * are laid out relative to the component size and measured with {@link FontMetrics}, so nothing
 * is clipped or scattered around the corners when the component is resized.
 */
public class KnockCanvas {
    private static final int SPECTROGRAM_X_AXIS_SIZE_BASE = 1024 * 4;
    /** Number of frequency bins, matches the firmware protocol size. */
    private static final int BIN_COUNT = 64;
    /** Draw one frequency label per this many bins. */
    private static final int LABEL_STEP = 8;

    private static final int MARGIN_LEFT = 52;
    private static final int MARGIN_RIGHT = 10;
    /** Log Viewer layout: room for the colour bar right of the plot. */
    private static final int COLOUR_BAR_MARGIN = 58;
    private static final int COLOUR_BAR_WIDTH = 10;
    private static final int CHIP_GAP = 8;
    /**
     * Lines drawn over the spectrogram itself. Its colour map always starts at black, so these
     * stay light in both console plot themes; the theme's own grid and cursor colours are used
     * only around the plot.
     */
    private static final Color OVER_SPECTROGRAM_GRID = new Color(0xff, 0xff, 0xff, 40);
    private static final Color OVER_SPECTROGRAM_CURSOR = new Color(0xff, 0xff, 0xff, 190);
    private static final int MARGIN_BOTTOM = 6;
    private static final int LEGEND_PADDING = 4;


    private final JComponent component = new JComponent() {
        @Override
        protected void paintComponent(Graphics g) {
            render((Graphics2D) g, getWidth(), getHeight());
        }
    };

    private KnockPlotStyle style = KnockPlotStyle.DEFAULT;
    private final boolean isCombined;
    /** Title in the Log Viewer layout, e.g. "Cylinder 3"; null for "Combined" or the number. */
    private String title;
    private final int number;

    private final int spectrogramXAxisSize;
    private final float[][] spectrogram;
    private final BufferedImage bufferedImage;
    private final Color[] colorspace;

    private int currentIndexXAxis = 0;

    public double[] yAxisHz = new double[BIN_COUNT];
    private int yAxisFrequencyStart = -1;
    private float yAxisFrequencyStep = -1;

    private float currentFrequency = 0;
    private float currentAmplitude = 0;

    private int peakY = 0;
    private float peakFrequency = 0;
    private float peakAmplitude = 0;

    /** Slowly decaying reference used to normalize colors, avoids a full-history rescan per line. */
    private float referenceAmplitude = 1;

    private int mouseX = -1;
    private int mouseY = -1;

    public KnockCanvas(int number, int divider, boolean isCombined) {
        this.isCombined = isCombined;
        this.number = number;
        this.spectrogramXAxisSize = SPECTROGRAM_X_AXIS_SIZE_BASE / Math.max(1, divider);

        spectrogram = new float[spectrogramXAxisSize][BIN_COUNT];
        bufferedImage = new BufferedImage(spectrogramXAxisSize, BIN_COUNT, BufferedImage.TYPE_INT_RGB);

        component.setOpaque(true);
        component.setBackground(style.background);

        //linear-gradient(to right, #000000, #290d1a, #490b32, #670353, #81007b, #a60085, #ca008b, #ef008f, #ff356b, #ff6947, #ff9a22, #ffc700);
        colorspace = new Color[]{
            Color.decode("#000000"),
            Color.decode("#290d1a"),
            Color.decode("#490b32"),
            Color.decode("#670353"),
            Color.decode("#81007b"),
            Color.decode("#a60085"),
            Color.decode("#ca008b"),
            Color.decode("#ef008f"),
            Color.decode("#ff356b"),
            Color.decode("#ff6947"),
            Color.decode("#ff9a22"),
            Color.decode("#ffc700"),
        };

        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayoutAndRepaint(component));
    }

    public JComponent getComponent() {
        return component;
    }

    /** Title shown above the plot in the Log Viewer layout. */
    public void setTitle(String title) {
        this.title = title;
        component.repaint();
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
            setupFrequencyYAxis();
    }

    public void setFrequencyStep(float step) {
        boolean needSetup = this.yAxisFrequencyStep < 0;
        this.yAxisFrequencyStep = step;
        if (needSetup)
            setupFrequencyYAxis();
    }

    public void setupFrequencyYAxis() {
        if (this.yAxisFrequencyStep < 0 || this.yAxisFrequencyStart < 0)
            return;

        for (int i = 0; i < BIN_COUNT; ++i)
            yAxisHz[i] = yAxisFrequencyStart + (double) yAxisFrequencyStep * i;
    }

    // ---- data ----------------------------------------------------------------

    public void processValues(float[] values) {
        float[] column = spectrogram[currentIndexXAxis];

        float columnMax = 0;
        int columnMaxIndex = 0;
        for (int i = 0; i < BIN_COUNT; ++i) {
            float value = i < values.length ? values[i] : 0;
            column[i] = value;
            if (value > columnMax) {
                columnMax = value;
                columnMaxIndex = i;
            }
        }

        currentAmplitude = columnMax;
        currentFrequency = (float) yAxisHz[columnMaxIndex];

        // decay keeps the palette adaptive without rescanning the whole history
        referenceAmplitude = Math.max(columnMax, referenceAmplitude * 0.995f);
        if (referenceAmplitude < 1)
            referenceAmplitude = 1;

        if (currentAmplitude >= peakAmplitude) {
            peakFrequency = currentFrequency;
            peakAmplitude = currentAmplitude;
            peakY = columnMaxIndex;
        }

        for (int y = 0; y < BIN_COUNT; ++y) {
            float normalized = column[y] / referenceAmplitude;
            if (normalized < 0)
                normalized = 0;
            if (normalized > 1)
                normalized = 1;
            int colorIndex = (int) ((colorspace.length - 1) * normalized);
            // image row 0 is the highest frequency bin
            bufferedImage.setRGB(currentIndexXAxis, BIN_COUNT - 1 - y, colorspace[colorIndex].getRGB());
        }

        if (++currentIndexXAxis >= spectrogramXAxisSize)
            currentIndexXAxis = 0;
    }

    public void resetPeak() {
        peakFrequency = 0;
        peakAmplitude = 0;
        peakY = 0;
        referenceAmplitude = 1;
    }

    // ---- rendering -----------------------------------------------------------

    private int plotLeft() {
        return MARGIN_LEFT;
    }

    private int plotWidth(int width) {
        return Math.max(1, width - MARGIN_LEFT - marginRight());
    }

    private int marginRight() {
        return style.logViewerLayout ? COLOUR_BAR_MARGIN : MARGIN_RIGHT;
    }

    private int plotTop(int legendHeight) {
        return legendHeight;
    }

    private int plotHeight(int height, int legendHeight) {
        return Math.max(1, height - legendHeight - MARGIN_BOTTOM);
    }

    private void render(Graphics2D g, int width, int height) {
        if (style.logViewerLayout) {
            renderLogViewerLayout(g, width, height);
            return;
        }
        g.setColor(style.background);
        g.fillRect(0, 0, width, height);

        Font base = g.getFont();
        Font legendFont = base.deriveFont(Font.BOLD, Math.max(10f, base.getSize2D()));
        FontMetrics legendMetrics = g.getFontMetrics(legendFont);
        int legendHeight = legendMetrics.getHeight() * 2 + LEGEND_PADDING * 2;

        int left = plotLeft();
        int top = plotTop(legendHeight);
        int plotW = plotWidth(width);
        int plotH = plotHeight(height, legendHeight);

        drawSpectrogram(g, left, top, plotW, plotH);
        drawTitle(g, base, left, top, plotW, plotH);
        drawFrequencyAxis(g, base, left, top, plotW, plotH);
        drawMarkers(g, left, top, plotW, plotH);
        drawLegend(g, legendFont, legendMetrics, width, legendHeight, top, plotH);

        g.setFont(base);
    }

    private void drawSpectrogram(Graphics2D g, int left, int top, int plotW, int plotH) {
        int n = spectrogramXAxisSize;
        int cur = currentIndexXAxis;
        // oldest column goes to the left edge, newest one to the right edge
        int split = left + Math.round(plotW * (float) (n - cur) / n);
        if (cur < n)
            g.drawImage(bufferedImage, left, top, split, top + plotH, cur, 0, n, BIN_COUNT, null);
        if (cur > 0)
            g.drawImage(bufferedImage, split, top, left + plotW, top + plotH, 0, 0, cur, BIN_COUNT, null);
    }

    private void drawTitle(Graphics2D g, Font base, int left, int top, int plotW, int plotH) {
        String title = isCombined ? "Combined" : Integer.toString(number);
        float size = Math.min(plotH * 0.3f, base.getSize2D() * 2f);
        g.setFont(base.deriveFont(Font.BOLD, Math.max(11f, size)));
        FontMetrics fm = g.getFontMetrics();
        g.setColor(style.title);
        // top right corner: the left side is occupied by the frequency labels
        g.drawString(title, left + plotW - fm.stringWidth(title) - 16, top + fm.getAscent() + 2);
    }

    private void drawFrequencyAxis(Graphics2D g, Font base, int left, int top, int plotW, int plotH) {
        g.setFont(base.deriveFont(Font.PLAIN, Math.max(9f, base.getSize2D() - 2)));
        FontMetrics fm = g.getFontMetrics();

        int step = labelStep(plotH, fm.getHeight() + 2);
        for (int i = 0; i < BIN_COUNT; i += step) {
            int y = binToY(i, top, plotH);

            g.setColor(style.logViewerLayout ? OVER_SPECTROGRAM_GRID : style.grid);
            g.drawLine(left, y, left + plotW, y);

            String label = formatHz(yAxisHz[i]);
            int textY = Math.min(top + plotH - 1, Math.max(top + fm.getAscent(), y + fm.getAscent() / 2));
            g.setColor(style.axisText);
            g.drawString(label, Math.max(2, left - 6 - fm.stringWidth(label)), textY);
        }
    }

    /** Picks a label step so that neighbour labels never overlap on a short component. */
    private static int labelStep(int plotH, int labelHeight) {
        for (int step = LABEL_STEP; step < BIN_COUNT; step *= 2) {
            if ((double) plotH * step / BIN_COUNT >= labelHeight)
                return step;
        }
        return BIN_COUNT;
    }

    private void drawMarkers(Graphics2D g, int left, int top, int plotW, int plotH) {
        int right = left + plotW;

        g.setColor(style.peakMarker);
        drawArrow(g, right, binToY(peakY, top, plotH));

        g.setColor(style.current);
        drawArrow(g, right, hzToY(currentFrequency, top, plotH));

        if (!style.logViewerLayout && isMouseInside(left, top, plotW, plotH)) {
            g.setColor(style.cursor);
            g.drawLine(left, mouseY, right, mouseY);
        }
    }

    private static void drawArrow(Graphics2D g, int x, int y) {
        g.fillPolygon(new int[]{x, x - 7, x}, new int[]{y - 4, y, y + 4}, 3);
    }

    private void drawLegend(Graphics2D g, Font legendFont, FontMetrics fm, int width, int legendHeight, int top, int plotH) {
        g.setFont(legendFont);

        float mouseFrequency = (float) yToHz(mouseY, top, plotH);
        float mouseAmplitude = mouseAmplitude(top, plotH);
        boolean mouseInside = mouseX >= plotLeft() && mouseY >= top && mouseY <= top + plotH;

        String[] titles = {"current", "peak", "mouse"};
        String[] frequencies = {
            formatHz(currentFrequency) + " Hz",
            formatHz(peakFrequency) + " Hz",
            mouseInside ? formatHz(mouseFrequency) + " Hz" : "-",
        };
        String[] amplitudes = {
            formatAmplitude(currentAmplitude),
            formatAmplitude(peakAmplitude),
            mouseInside ? formatAmplitude(mouseAmplitude) : "-",
        };
        Color[] colors = {style.current, style.peak, style.mouse};

        int cellWidth = Math.max(1, width / titles.length);
        int firstLine = LEGEND_PADDING + fm.getAscent();
        int secondLine = firstLine + fm.getHeight();

        for (int i = 0; i < titles.length; i++) {
            int cellLeft = i * cellWidth + LEGEND_PADDING;
            int available = cellWidth - LEGEND_PADDING * 2;

            g.setColor(colors[i]);
            g.drawString(clip(fm, "[" + titles[i] + "]", available), cellLeft, firstLine);
            String values = frequencies[i] + "   " + amplitudes[i];
            g.drawString(clip(fm, values, available), cellLeft, secondLine);
        }

        g.setColor(style.grid);
        g.drawLine(0, legendHeight - 1, width, legendHeight - 1);
    }

    private float mouseAmplitude(int top, int plotH) {
        float[] column = spectrogram[columnAt(mouseX)];
        return column[yToBin(mouseY, top, plotH)];
    }

    // ---- coordinate conversions ----------------------------------------------

    private int binToY(int bin, int top, int plotH) {
        double relative = (bin + 0.5) / BIN_COUNT;
        return top + (int) Math.round(plotH * (1 - relative));
    }

    private int hzToY(double hz, int top, int plotH) {
        return binToY((int) Math.round(hzToBin(hz)), top, plotH);
    }

    private double hzToBin(double hz) {
        if (yAxisFrequencyStep <= 0)
            return 0;
        double bin = (hz - yAxisFrequencyStart) / yAxisFrequencyStep;
        return Math.max(0, Math.min(BIN_COUNT - 1, bin));
    }

    private int yToBin(int y, int top, int plotH) {
        double relative = 1 - (double) (y - top) / plotH;
        int bin = (int) (relative * BIN_COUNT);
        return Math.max(0, Math.min(BIN_COUNT - 1, bin));
    }

    private double yToHz(int y, int top, int plotH) {
        return yAxisHz[yToBin(y, top, plotH)];
    }

    /** Maps a component x coordinate into the ring buffer column which is drawn there. */
    private int columnAt(int x) {
        int plotW = plotWidth(component.getWidth());
        double relative = (double) (x - plotLeft()) / plotW;
        relative = Math.max(0, Math.min(1, relative));
        int offset = (int) (relative * (spectrogramXAxisSize - 1));
        int index = currentIndexXAxis + offset;
        if (index >= spectrogramXAxisSize)
            index -= spectrogramXAxisSize;
        return Math.max(0, Math.min(spectrogramXAxisSize - 1, index));
    }

    private boolean isMouseInside(int left, int top, int plotW, int plotH) {
        return mouseX >= left && mouseX <= left + plotW && mouseY >= top && mouseY <= top + plotH;
    }

    // ---- Log Viewer layout ------------------------------------------------------

    /**
     * Title and legend chips above the plot, a colour bar right of it and "Older / Now" below;
     * a dashed cursor with a value box follows the pointer, as in the console's Log Viewer.
     */
    private void renderLogViewerLayout(Graphics2D g, int width, int height) {
        g.setColor(style.background);
        g.fillRect(0, 0, width, height);
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

        Font base = g.getFont();
        Font small = base.deriveFont(Font.PLAIN, Math.max(9f, base.getSize2D() - 2));
        Font value = style.valueFontFamily == null ? base
            : new Font(style.valueFontFamily, Font.PLAIN, base.getSize());
        FontMetrics baseMetrics = g.getFontMetrics(base);
        FontMetrics smallMetrics = g.getFontMetrics(small);

        int chipHeight = baseMetrics.getHeight() + 6;
        int headerHeight = chipHeight + 2 * LEGEND_PADDING + 4;
        int footerHeight = smallMetrics.getHeight() + 4;

        int left = plotLeft();
        int top = headerHeight;
        int plotW = plotWidth(width);
        int plotH = Math.max(1, height - headerHeight - footerHeight);

        drawSpectrogram(g, left, top, plotW, plotH);
        drawFrequencyAxis(g, base, left, top, plotW, plotH);
        g.setColor(style.grid);
        g.drawRect(left, top, plotW - 1, plotH - 1);
        drawMarkers(g, left, top, plotW, plotH);
        drawColourBar(g, small, smallMetrics, left + plotW + 14, top, plotH);
        drawTimeHints(g, small, smallMetrics, left, top + plotH, plotW);
        drawHeader(g, base, value, baseMetrics, width, chipHeight, top, plotH);
        if (isMouseInside(left, top, plotW, plotH)) {
            drawCursor(g, small, value, left, top, plotW, plotH);
        }
        g.setFont(base);
    }

    private void drawHeader(Graphics2D g, Font base, Font value, FontMetrics fm, int width, int chipHeight,
                            int top, int plotH) {
        int y = LEGEND_PADDING + 2;
        String name = title != null ? title : isCombined ? "Combined" : Integer.toString(number);
        g.setFont(base.deriveFont(Font.BOLD));
        FontMetrics bold = g.getFontMetrics();
        g.setColor(style.text);
        g.drawString(name, plotLeft(), y + (chipHeight + bold.getAscent() - bold.getDescent()) / 2);
        int x = plotLeft() + bold.stringWidth(name) + 14;

        boolean mouseInside = isMouseInside(plotLeft(), top, plotWidth(width), plotH);
        String[] labels = {"Current", "Peak", "Cursor"};
        String[] values = {
            formatHz(currentFrequency) + " Hz · " + Math.round(currentAmplitude),
            formatHz(peakFrequency) + " Hz · " + Math.round(peakAmplitude),
            mouseInside
                ? formatHz(yToHz(mouseY, top, plotH)) + " Hz · " + Math.round(mouseAmplitude(top, plotH))
                : "–",
        };
        Color[] dots = {style.current, style.peak, style.mouse};
        FontMetrics valueMetrics = g.getFontMetrics(value);
        for (int i = 0; i < labels.length; i++) {
            int labelWidth = fm.stringWidth(labels[i]);
            int valueWidth = valueMetrics.stringWidth(values[i]);
            int chipWidth = 10 + 8 + 6 + labelWidth + 6 + valueWidth + 10;
            if (x + chipWidth > width - 8) {
                // narrow plot: drop the chips that do not fit rather than overlap them
                break;
            }
            g.setColor(style.chipBackground);
            g.fillRoundRect(x, y, chipWidth, chipHeight, chipHeight, chipHeight);
            int textY = y + (chipHeight + fm.getAscent() - fm.getDescent()) / 2;
            g.setColor(dots[i]);
            g.fillOval(x + 10, y + chipHeight / 2 - 4, 8, 8);
            g.setFont(base);
            g.setColor(style.axisText);
            g.drawString(labels[i], x + 24, textY);
            g.setFont(value);
            g.setColor(style.text);
            g.drawString(values[i], x + 24 + labelWidth + 6, textY);
            x += chipWidth + CHIP_GAP;
        }
    }

    private void drawColourBar(Graphics2D g, Font small, FontMetrics fm, int x, int top, int plotH) {
        g.setFont(small);
        int barTop = top + fm.getHeight() + 2;
        int barBottom = top + plotH - fm.getHeight() - 2;
        int barHeight = Math.max(1, barBottom - barTop);
        // loudest colour at the top, matching "Loud" above the bar
        for (int i = 0; i < colorspace.length; i++) {
            int y0 = barBottom - barHeight * (i + 1) / colorspace.length;
            int y1 = barBottom - barHeight * i / colorspace.length;
            g.setColor(colorspace[i]);
            g.fillRect(x, y0, COLOUR_BAR_WIDTH, y1 - y0);
        }
        g.setColor(style.grid);
        g.drawRect(x, barTop, COLOUR_BAR_WIDTH - 1, barHeight - 1);
        g.setColor(style.axisText);
        int centre = x + COLOUR_BAR_WIDTH / 2;
        g.drawString("Loud", centre - fm.stringWidth("Loud") / 2, top + fm.getAscent());
        g.drawString("Quiet", centre - fm.stringWidth("Quiet") / 2, top + plotH - fm.getDescent());
    }

    private void drawTimeHints(Graphics2D g, Font small, FontMetrics fm, int left, int bottom, int plotW) {
        g.setFont(small);
        g.setColor(style.axisText);
        int y = bottom + 2 + fm.getAscent();
        g.drawString("Older", left, y);
        g.drawString("Now", left + plotW - fm.stringWidth("Now"), y);
    }

    private void drawCursor(Graphics2D g, Font small, Font value, int left, int top, int plotW, int plotH) {
        Stroke stroke = g.getStroke();
        g.setColor(OVER_SPECTROGRAM_CURSOR);
        g.setStroke(new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER, 10, new float[]{4, 3}, 0));
        g.drawLine(mouseX, top, mouseX, top + plotH);
        g.drawLine(left, mouseY, left + plotW, mouseY);
        g.setStroke(stroke);

        String label = "At cursor";
        String text = formatHz(yToHz(mouseY, top, plotH)) + " Hz · " + Math.round(mouseAmplitude(top, plotH));
        Font bold = value.deriveFont(Font.BOLD);
        FontMetrics labelMetrics = g.getFontMetrics(small);
        FontMetrics textMetrics = g.getFontMetrics(bold);
        int boxW = Math.max(labelMetrics.stringWidth(label), textMetrics.stringWidth(text)) + 20;
        int boxH = labelMetrics.getHeight() + textMetrics.getHeight() + 10;
        // right of the pointer, flipped to the left near the plot's right edge; kept inside the plot
        int x = mouseX + 12;
        if (x + boxW > left + plotW) {
            x = mouseX - 12 - boxW;
        }
        int y = Math.max(top + 4, Math.min(mouseY - boxH - 8, top + plotH - boxH - 4));
        g.setColor(new Color(0, 0, 0, 40));
        g.fillRoundRect(x + 1, y + 2, boxW, boxH, 8, 8);
        g.setColor(style.background);
        g.fillRoundRect(x, y, boxW, boxH, 8, 8);
        g.setColor(style.grid);
        g.drawRoundRect(x, y, boxW, boxH, 8, 8);
        g.setFont(small);
        g.setColor(style.axisText);
        g.drawString(label, x + 10, y + 5 + labelMetrics.getAscent());
        g.setFont(bold);
        g.setColor(style.text);
        g.drawString(text, x + 10, y + 5 + labelMetrics.getHeight() + textMetrics.getAscent());
    }

    private static String clip(FontMetrics fm, String text, int available) {
        if (available <= 0 || fm.stringWidth(text) <= available)
            return text;
        String ellipsis = "...";
        int width = available - fm.stringWidth(ellipsis);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < text.length(); i++) {
            if (fm.stringWidth(sb.toString() + text.charAt(i)) > width)
                break;
            sb.append(text.charAt(i));
        }
        return sb + ellipsis;
    }

    private static String formatHz(double hz) {
        if (hz >= 1000)
            return String.format("%.1fk", hz / 1000);
        return String.format("%.0f", hz);
    }

    private static String formatAmplitude(float amplitude) {
        return String.format("%.0f amp", amplitude);
    }

    // ---- mouse ----------------------------------------------------------------

    public float[] getCurrentMouseMagnitudes() {
        return spectrogram[columnAt(mouseX)];
    }

    public void setMousePosition(int x, int y) {
        mouseX = x;
        mouseY = y;
        component.repaint();
    }
}
