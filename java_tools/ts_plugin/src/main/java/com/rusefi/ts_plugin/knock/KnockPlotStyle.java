package com.rusefi.ts_plugin.knock;

import java.awt.*;

/**
 * Colours and layout of the knock spectrogram and magnitude plots. {@link #DEFAULT} is the plots'
 * original dark look, kept by the TunerStudio plugin; the rusEFI console passes its Log Viewer
 * palette and {@link #logViewerLayout} instead. Plain Java 8: the plugin runs on TunerStudio's
 * bundled runtime.
 */
public final class KnockPlotStyle {
    public static final KnockPlotStyle DEFAULT = new Builder().build();

    /** Plot and margin background. */
    public final Color background;
    /** Horizontal grid lines and the line under the legend. */
    public final Color grid;
    /** Axis labels. */
    public final Color axisText;
    /** Watermark title in the spectrogram ("Combined" or the channel number). */
    public final Color title;
    /** Pointer line over the spectrogram. */
    public final Color cursor;
    /** Current dominant frequency: marker and legend. */
    public final Color current;
    /** Highest amplitude seen since the last reset: legend. */
    public final Color peak;
    /** Highest amplitude seen since the last reset: marker at the plot edge. */
    public final Color peakMarker;
    /** Values under the pointer: legend. */
    public final Color mouse;
    /** Area under the magnitude curve. */
    public final Color magnitudeFill;
    /** Magnitude curve. */
    public final Color magnitudeLine;
    /** Peak line and label in the magnitude plot. */
    public final Color magnitudePeak;
    /** Main text, e.g. the plot title and legend values ({@link #logViewerLayout} only). */
    public final Color text;
    /** Legend chip and value box background ({@link #logViewerLayout} only). */
    public final Color chipBackground;
    /**
     * The console's Log Viewer layout: a title and legend chips above the plot, a dashed
     * cursor with a value box, a colour bar and "Older / Now" under the time axis. Off: the
     * original layout (bracketed legend text, watermark title, solid horizontal cursor line).
     */
    public final boolean logViewerLayout;
    /** Font family for values, e.g. the console's monospaced font; null for the plot's own font. */
    public final String valueFontFamily;

    private KnockPlotStyle(Builder b) {
        background = b.background;
        grid = b.grid;
        axisText = b.axisText;
        title = b.title;
        cursor = b.cursor;
        current = b.current;
        peak = b.peak;
        peakMarker = b.peakMarker;
        mouse = b.mouse;
        magnitudeFill = b.magnitudeFill;
        magnitudeLine = b.magnitudeLine;
        magnitudePeak = b.magnitudePeak;
        text = b.text;
        chipBackground = b.chipBackground;
        logViewerLayout = b.logViewerLayout;
        valueFontFamily = b.valueFontFamily;
    }

    public Builder toBuilder() {
        Builder b = new Builder();
        b.background = background;
        b.grid = grid;
        b.axisText = axisText;
        b.title = title;
        b.cursor = cursor;
        b.current = current;
        b.peak = peak;
        b.peakMarker = peakMarker;
        b.mouse = mouse;
        b.magnitudeFill = magnitudeFill;
        b.magnitudeLine = magnitudeLine;
        b.magnitudePeak = magnitudePeak;
        b.text = text;
        b.chipBackground = chipBackground;
        b.logViewerLayout = logViewerLayout;
        b.valueFontFamily = valueFontFamily;
        return b;
    }

    /** Starts from {@link #DEFAULT}; set only what differs. */
    public static final class Builder {
        private Color background = new Color(0x1b, 0x1b, 0x1f);
        private Color grid = new Color(0xff, 0xff, 0xff, 40);
        private Color axisText = new Color(0xd0, 0xd0, 0xd0);
        private Color title = new Color(0xff, 0xff, 0xff, 60);
        private Color cursor = new Color(0xff, 0xff, 0xff, 90);
        private Color current = Color.YELLOW;
        private Color peak = new Color(0xff, 0x6e, 0x6e);
        private Color peakMarker = Color.RED;
        private Color mouse = Color.ORANGE;
        private Color magnitudeFill = new Color(0x55, 0x8f, 0xd0, 140);
        private Color magnitudeLine = new Color(0x9f, 0xd0, 0xff);
        private Color magnitudePeak = Color.YELLOW;
        private Color text = new Color(0xe6, 0xe6, 0xe6);
        private Color chipBackground = new Color(0xff, 0xff, 0xff, 24);
        private boolean logViewerLayout = false;
        private String valueFontFamily = null;

        public Builder background(Color c) { background = c; return this; }
        public Builder grid(Color c) { grid = c; return this; }
        public Builder axisText(Color c) { axisText = c; return this; }
        public Builder title(Color c) { title = c; return this; }
        public Builder cursor(Color c) { cursor = c; return this; }
        public Builder current(Color c) { current = c; return this; }
        public Builder peak(Color c) { peak = c; return this; }
        public Builder peakMarker(Color c) { peakMarker = c; return this; }
        public Builder mouse(Color c) { mouse = c; return this; }
        public Builder magnitudeFill(Color c) { magnitudeFill = c; return this; }
        public Builder magnitudeLine(Color c) { magnitudeLine = c; return this; }
        public Builder magnitudePeak(Color c) { magnitudePeak = c; return this; }
        public Builder text(Color c) { text = c; return this; }
        public Builder chipBackground(Color c) { chipBackground = c; return this; }
        public Builder logViewerLayout(boolean on) { logViewerLayout = on; return this; }
        public Builder valueFontFamily(String family) { valueFontFamily = family; return this; }

        public KnockPlotStyle build() {
            return new KnockPlotStyle(this);
        }
    }
}
