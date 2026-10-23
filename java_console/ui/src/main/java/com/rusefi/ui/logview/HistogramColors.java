package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import java.awt.Color;
import java.util.Arrays;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * How histogram cells are coloured, remembered for each Z signal, since lambda, ignition timing and knock need
 * different ranges: the palette, whether it runs backwards, and which values get its end colours. A manual
 * range and centre apply to the value modes (Mean / Min / Max); Time and Count always start from nothing.
 */
final class HistogramColors {
    enum Palette {
        HEATMAP("Heatmap", "Blue, green, yellow, red: the classic tuning heatmap"),
        VIRIDIS("Viridis", "Dark blue to yellow: easy to read for colour-blind eyes and in bright light"),
        DIVERGING("Diverging", "Two colours either side of a centre value, e.g. rich and lean of stoichiometric"),
        SINGLE("Single", "Pale to dark blue");

        final String text;
        final String tooltip;

        Palette(String text, String tooltip) {
            this.text = text;
            this.tooltip = tooltip;
        }
    }

    enum Range {
        AUTO("Auto", "From the lowest to the highest cell shown"),
        ROBUST("No outliers", "From the 5th to the 95th percentile of the cells shown, so one odd cell does not "
                + "wash out the rest; cells beyond get the end colours"),
        MANUAL("Manual", "From and to values you enter");

        final String text;
        final String tooltip;

        Range(String text, String tooltip) {
            this.text = text;
            this.tooltip = tooltip;
        }
    }

    /** The percentiles of {@link Range#ROBUST}. */
    static final double ROBUST_LOW = 0.05;
    static final double ROBUST_HIGH = 0.95;
    /** Fewer cells than this are all kept by {@link Range#ROBUST}: too few to call any an outlier. */
    static final int ROBUST_MIN_CELLS = 8;

    private final Node node;

    HistogramColors(Node node) {
        this.node = node;
    }

    Palette palette() {
        try {
            return Palette.valueOf(node.getProperty("palette", Palette.HEATMAP.name()));
        } catch (IllegalArgumentException e) {
            return Palette.HEATMAP;
        }
    }

    void setPalette(Palette palette) {
        node.setProperty("palette", palette.name());
    }

    boolean reversed() {
        return node.getBoolProperty("reversed", false);
    }

    void setReversed(boolean reversed) {
        node.setBoolProperty("reversed", reversed);
    }

    Range range() {
        try {
            return Range.valueOf(node.getProperty("range", Range.ROBUST.name()));
        } catch (IllegalArgumentException e) {
            return Range.ROBUST;
        }
    }

    void setRange(Range range) {
        node.setProperty("range", range.name());
    }

    /** Manual range ends, in the logged unit of the Z signal; NaN until entered. */
    double manualLow() {
        return node.getDoubleProperty("low", Double.NaN);
    }

    double manualHigh() {
        return node.getDoubleProperty("high", Double.NaN);
    }

    void setManual(double low, double high) {
        node.setProperty("low", Math.min(low, high));
        node.setProperty("high", Math.max(low, high));
    }

    /** The diverging palette's centre in the logged unit; NaN for automatic (stoichiometric, or the middle). */
    double centre() {
        return node.getDoubleProperty("centre", Double.NaN);
    }

    void setCentre(double centre) {
        node.setProperty("centre", centre);
    }

    void reset() {
        node.getConfig().clear();
    }

    /** A palette colour at {@code t} from 0 (low end) to 1 (high end); 0.5 is the centre of a diverging one. */
    static Color color(Palette palette, double t) {
        t = Math.max(0, Math.min(1, t));
        switch (palette) {
            case HEATMAP:
                // the ends of Turbo are near black: keep to the readable middle of it
                return turbo(0.07 + 0.86 * t);
            case VIRIDIS:
                return stops(VIRIDIS, t);
            case DIVERGING: {
                Color neutral = themed("Console.LogViewer.histogram.neutral", new Color(0xf4f5f7));
                return t < 0.5
                        ? HistogramTable.mix(neutral, themed("Console.LogViewer.histogram.rich", new Color(0x9a3412)),
                        (0.5 - t) * 2 * 0.92)
                        : HistogramTable.mix(neutral, themed("Console.LogViewer.histogram.lean", new Color(0x1e3a8a)),
                        (t - 0.5) * 2 * 0.92);
            }
            default:
                return HistogramTable.mix(themed("Console.LogViewer.histogram.neutral", new Color(0xf4f5f7)),
                        themed("Console.LogViewer.histogram.lean", new Color(0x1e3a8a)), t * 0.92);
        }
    }

    /** Google's Turbo colormap, by its published polynomial approximation. */
    static Color turbo(double t) {
        double r = 0.13572138 + t * (4.61539260 + t * (-42.66032258 + t * (132.13108234 + t * (-152.94239396 + t * 59.28637943))));
        double g = 0.09140261 + t * (2.19418839 + t * (4.84296658 + t * (-14.18503333 + t * (4.27729857 + t * 2.82956604))));
        double b = 0.10667330 + t * (12.64194608 + t * (-60.58204836 + t * (110.36276771 + t * (-89.90310912 + t * 27.34824973))));
        return new Color(channel(r), channel(g), channel(b));
    }

    private static int channel(double value) {
        return (int) Math.round(Math.max(0, Math.min(1, value)) * 255);
    }

    private static final int[] VIRIDIS = {0x440154, 0x482878, 0x3e4989, 0x31688e, 0x26828e, 0x1f9e89, 0x35b779,
            0x6ece58, 0xb5de2b, 0xfde725};

    private static Color stops(int[] stops, double t) {
        double position = t * (stops.length - 1);
        int index = Math.min(stops.length - 2, (int) Math.floor(position));
        return HistogramTable.mix(new Color(stops[index]), new Color(stops[index + 1]), position - index);
    }

    /** Dark or white text, whichever reads better on a background. */
    static boolean wantsLightText(Color background) {
        double luminance = 0.2126 * linear(background.getRed()) + 0.7152 * linear(background.getGreen())
                + 0.0722 * linear(background.getBlue());
        // the contrast with white equals the contrast with near-black text at about this luminance
        return luminance < 0.18;
    }

    private static double linear(int channel) {
        double c = channel / 255.0;
        return c <= 0.04045 ? c / 12.92 : Math.pow((c + 0.055) / 1.055, 2.4);
    }

    /** The values at a palette's ends and centre, and whether shown values lie beyond the ends. */
    static final class Scale {
        final double low;
        final double high;
        /** The diverging palette's centre, or NaN. */
        final double centre;
        final boolean clippedLow;
        final boolean clippedHigh;

        Scale(double low, double high, double centre, boolean clippedLow, boolean clippedHigh) {
            this.low = low;
            this.high = high;
            this.centre = centre;
            this.clippedLow = clippedLow;
            this.clippedHigh = clippedHigh;
        }
    }

    /**
     * The colour scale for the shown values (the first {@code count}): Auto from the lowest to the highest, No
     * outliers from the 5th to the 95th percentile (all of them below {@link #ROBUST_MIN_CELLS}), or Manual. For
     * occupancy ({@code values} false: seconds or counts) the scale starts at 0 and the manual range and centre do
     * not apply. The diverging palette centres on the entered value, else stoichiometric, else the middle, with the
     * same span on both sides.
     *
     * @param factor  logged unit to shown unit, for the manual range and centre
     * @param stoich  stoichiometric in the shown unit, or NaN when Z is not a mixture
     */
    Scale scale(double[] shown, int count, boolean values, double factor, double stoich) {
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;
        for (int i = 0; i < count; i++) {
            min = Math.min(min, shown[i]);
            max = Math.max(max, shown[i]);
        }
        boolean robust = range() == Range.ROBUST && count >= ROBUST_MIN_CELLS;
        double low;
        double high;
        double centre = Double.NaN;
        if (!values) {
            low = 0;
            high = robust ? quantile(shown, count, ROBUST_HIGH) : max;
        } else {
            if (range() == Range.MANUAL && manualHigh() > manualLow()) {
                low = manualLow() * factor;
                high = manualHigh() * factor;
            } else if (robust) {
                low = quantile(shown, count, ROBUST_LOW);
                high = quantile(shown, count, ROBUST_HIGH);
            } else {
                low = min;
                high = max;
            }
            if (palette() == Palette.DIVERGING) {
                centre = Double.isFinite(centre()) ? centre() * factor
                        : Double.isFinite(stoich) ? stoich : (low + high) / 2;
                double half = Math.max(Math.abs(centre - low), Math.abs(high - centre));
                low = centre - half;
                high = centre + half;
            }
        }
        return new Scale(low, high, centre, count > 0 && min < low - 1e-12, count > 0 && max > high + 1e-12);
    }

    /** The {@code fraction} quantile (0..1) of the values, by linear interpolation; NaN for none. */
    static double quantile(double[] values, int count, double fraction) {
        if (count == 0) {
            return Double.NaN;
        }
        double[] sorted = Arrays.copyOf(values, count);
        Arrays.sort(sorted);
        double position = fraction * (count - 1);
        int index = (int) Math.floor(position);
        if (index >= count - 1) {
            return sorted[count - 1];
        }
        return sorted[index] + (sorted[index + 1] - sorted[index]) * (position - index);
    }
}
