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
    private static final int MARGIN_BOTTOM = 6;
    private static final int LEGEND_PADDING = 4;

    private static final Color BACKGROUND = new Color(0x1b, 0x1b, 0x1f);
    private static final Color GRID = new Color(0xff, 0xff, 0xff, 40);
    private static final Color AXIS_TEXT = new Color(0xd0, 0xd0, 0xd0);
    private static final Color TITLE = new Color(0xff, 0xff, 0xff, 60);

    private final JComponent component = new JComponent() {
        @Override
        protected void paintComponent(Graphics g) {
            render((Graphics2D) g, getWidth(), getHeight());
        }
    };

    private final boolean isCombined;
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
        component.setBackground(BACKGROUND);

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
        return Math.max(1, width - MARGIN_LEFT - MARGIN_RIGHT);
    }

    private int plotTop(int legendHeight) {
        return legendHeight;
    }

    private int plotHeight(int height, int legendHeight) {
        return Math.max(1, height - legendHeight - MARGIN_BOTTOM);
    }

    private void render(Graphics2D g, int width, int height) {
        g.setColor(BACKGROUND);
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
        g.setColor(TITLE);
        // top right corner: the left side is occupied by the frequency labels
        g.drawString(title, left + plotW - fm.stringWidth(title) - 16, top + fm.getAscent() + 2);
    }

    private void drawFrequencyAxis(Graphics2D g, Font base, int left, int top, int plotW, int plotH) {
        g.setFont(base.deriveFont(Font.PLAIN, Math.max(9f, base.getSize2D() - 2)));
        FontMetrics fm = g.getFontMetrics();

        int step = labelStep(plotH, fm.getHeight() + 2);
        for (int i = 0; i < BIN_COUNT; i += step) {
            int y = binToY(i, top, plotH);

            g.setColor(GRID);
            g.drawLine(left, y, left + plotW, y);

            String label = formatHz(yAxisHz[i]);
            int textY = Math.min(top + plotH - 1, Math.max(top + fm.getAscent(), y + fm.getAscent() / 2));
            g.setColor(AXIS_TEXT);
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

        g.setColor(Color.RED);
        drawArrow(g, right, binToY(peakY, top, plotH));

        g.setColor(Color.YELLOW);
        drawArrow(g, right, hzToY(currentFrequency, top, plotH));

        if (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= top + plotH) {
            g.setColor(new Color(0xff, 0xff, 0xff, 90));
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
        Color[] colors = {Color.YELLOW, new Color(0xff, 0x6e, 0x6e), Color.ORANGE};

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

        g.setColor(GRID);
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
        int plotW = Math.max(1, component.getWidth() - MARGIN_LEFT - MARGIN_RIGHT);
        double relative = (double) (x - plotLeft()) / plotW;
        relative = Math.max(0, Math.min(1, relative));
        int offset = (int) (relative * (spectrogramXAxisSize - 1));
        int index = currentIndexXAxis + offset;
        if (index >= spectrogramXAxisSize)
            index -= spectrogramXAxisSize;
        return Math.max(0, Math.min(spectrogramXAxisSize - 1, index));
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
