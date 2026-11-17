package com.rusefi.ui.widgets.tune;

import java.awt.Color;

/**
 * Shared value colors and readable text for calibration editors and previews. Value colours mean the
 * same in every theme, and each cell picks black or white text for its own colour. [tag:fixed_color]
 */
final class CalibrationColorPalette {
    static final Color LOW = new Color(0x24557A); // [tag:fixed_color] value colour
    static final Color MIDDLE = new Color(0x59636D); // [tag:fixed_color] value colour
    static final Color HIGH = new Color(0xC47A20); // [tag:fixed_color] value colour

    private CalibrationColorPalette() {
    }

    static Color background(double value, double minimum, double maximum) {
        if (maximum <= minimum) {
            return MIDDLE;
        }
        double ratio = Math.max(0, Math.min(1, (value - minimum) / (maximum - minimum)));
        Color start = ratio <= 0.5 ? LOW : MIDDLE;
        Color end = ratio <= 0.5 ? MIDDLE : HIGH;
        double blend = ratio <= 0.5 ? ratio * 2 : (ratio - 0.5) * 2;
        int red = (int) Math.round(start.getRed() + blend * (end.getRed() - start.getRed()));
        int green = (int) Math.round(start.getGreen() + blend * (end.getGreen() - start.getGreen()));
        int blue = (int) Math.round(start.getBlue() + blend * (end.getBlue() - start.getBlue()));
        return new Color(red, green, blue); // [tag:fixed_color] blend of the value colours
    }

    static Color foreground(Color background) {
        // Choose the higher contrast text color using linear sRGB luminance.
        double luminance = 0.2126 * linearChannel(background.getRed())
            + 0.7152 * linearChannel(background.getGreen())
            + 0.0722 * linearChannel(background.getBlue());
        return luminance < 0.179 ? Color.WHITE : Color.BLACK; // [tag:fixed_color] text for the cell colour
    }

    private static double linearChannel(int channel) {
        double normalized = channel / 255.0;
        return normalized <= 0.04045 ? normalized / 12.92
            : Math.pow((normalized + 0.055) / 1.055, 2.4);
    }
}
