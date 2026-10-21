package com.rusefi.ui.logview;

import javax.swing.UIManager;
import java.awt.Color;

/**
 * Colours of the log chart's plot area, light or dark independently of the console theme.
 * Values come from the Console.LogChart.light.* / Console.LogChart.dark.* theme keys
 * [tag:console_theme]; the constants below are the same values, used when no theme is installed.
 * PlotPaletteTest keeps both in sync and checks trace contrast and distinctness.
 */
final class PlotPalette {
    static final int TRACE_COUNT = 8;

    static final PlotPalette LIGHT = new PlotPalette(2f,
            new Color(0xffffff), new Color(0xe3e7ed), new Color(0x586273), new Color(0x6b7485),
            new Color(0x33, 0x41, 0x55, 0x99), new Color(0x25, 0x63, 0xeb, 0x26), new Color(0x2563eb),
            new Color(0x2563eb), new Color(0x9f3804), new Color(0x049f9f), new Color(0xfa428c),
            new Color(0x7d9004), new Color(0xa0224c), new Color(0x1898d8), new Color(0x0c6a5d));
    static final PlotPalette DARK = new PlotPalette(1.5f,
            new Color(0x151b26), new Color(0x374352), new Color(0xc6d1de), new Color(0x788698),
            new Color(0xdc, 0xe6, 0xf5, 0x9b), new Color(0x5b, 0xcd, 0xfa, 0x30), new Color(0x5bcdfa),
            new Color(0xffbf47), new Color(0x5bcdfa), new Color(0x77e2ab), new Color(0xf6839d),
            new Color(0xb09bfb), new Color(0xff8f58), new Color(0x76deda), new Color(0x1f9374));

    final float strokeWidth;
    final Color background;
    final Color grid;
    final Color label;
    final Color hint;
    final Color cursor;
    final Color selection;
    /** Marks the active plot when several are stacked. */
    final Color accent;
    private final Color[] traces;

    private PlotPalette(float strokeWidth, Color background, Color grid, Color label, Color hint,
                        Color cursor, Color selection, Color accent, Color... traces) {
        this.strokeWidth = strokeWidth;
        this.background = background;
        this.grid = grid;
        this.label = label;
        this.hint = hint;
        this.cursor = cursor;
        this.selection = selection;
        this.accent = accent;
        this.traces = traces;
    }

    /** Colour of the {@code index}-th plotted trace; the palette repeats after {@link #TRACE_COUNT}. */
    Color trace(int index) {
        return traces[index % traces.length];
    }

    static PlotPalette of(boolean dark) {
        PlotPalette fallback = dark ? DARK : LIGHT;
        String prefix = "Console.LogChart." + (dark ? "dark" : "light") + ".";
        Color[] traces = new Color[TRACE_COUNT];
        for (int i = 0; i < TRACE_COUNT; i++) {
            traces[i] = themed(prefix + "trace" + (i + 1), fallback.traces[i]);
        }
        return new PlotPalette(fallback.strokeWidth,
                themed(prefix + "background", fallback.background),
                themed(prefix + "grid", fallback.grid),
                themed(prefix + "label", fallback.label),
                themed(prefix + "hint", fallback.hint),
                themed(prefix + "cursor", fallback.cursor),
                themed(prefix + "selection", fallback.selection),
                themed(prefix + "accent", fallback.accent),
                traces);
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }
}
