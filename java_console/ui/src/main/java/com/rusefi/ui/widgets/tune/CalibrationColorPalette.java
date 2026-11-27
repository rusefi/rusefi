package com.rusefi.ui.widgets.tune;

import javax.swing.BorderFactory;
import javax.swing.JTable;
import javax.swing.UIManager;
import java.awt.Color;

/**
 * Shared value colours and readable text for calibration editors and previews: the table grid, its
 * 3D views and the curves. Low values are cool blue and high values warm orange in every theme; the
 * theme sets the lightness (Console.Calibration.* keys [tag:console_theme]): light cells in the light
 * theme, dark cells in the dark theme, so a whole table never glares in dark mode. Each cell picks
 * black or white text for its own colour.
 */
final class CalibrationColorPalette {

    private CalibrationColorPalette() {
    }

    static Color background(double value, double minimum, double maximum) {
        Color middle = middle();
        if (maximum <= minimum) {
            return middle;
        }
        double ratio = Math.max(0, Math.min(1, (value - minimum) / (maximum - minimum)));
        Color start = ratio <= 0.5 ? low() : middle;
        Color end = ratio <= 0.5 ? middle : high();
        double blend = ratio <= 0.5 ? ratio * 2 : (ratio - 0.5) * 2;
        int red = (int) Math.round(start.getRed() + blend * (end.getRed() - start.getRed()));
        int green = (int) Math.round(start.getGreen() + blend * (end.getGreen() - start.getGreen()));
        int blue = (int) Math.round(start.getBlue() + blend * (end.getBlue() - start.getBlue()));
        return new Color(red, green, blue); // [tag:fixed_color] blend of the value colours
    }

    static Color low() {
        return themed("Console.Calibration.low", 0xB7D0EA);
    }

    static Color middle() {
        return themed("Console.Calibration.middle", 0xEDEFF1);
    }

    static Color high() {
        return themed("Console.Calibration.high", 0xF3B36F);
    }

    /** Axis value cells: the header row and the first column. */
    static Color axisBackground() {
        return themed("Console.Calibration.axisBackground", 0xEEF1F5);
    }

    static Color axisForeground() {
        return themed("Console.Calibration.axisForeground", 0x3D444D);
    }

    /** Outline of the selected block, also mixed into its cells and the active axis values. */
    static Color selection() {
        return themed("Console.Calibration.selection", 0x2563EB);
    }

    /** A cell's colour with the selection tint over it, so the value colour still shows through. */
    static Color selected(Color cell) {
        return mix(cell, selection(), SELECTION_TINT);
    }

    /** Width of the outline around a selected block of cells. */
    static final int SELECTION_EDGE = 2;

    /**
     * The outline of a selected cell: the selection colour on the sides that face an unselected cell
     * or the table's edge, so a block of selected cells gets one outline around it.
     *
     * @param firstValueColumn the first column holding values; columns before it are axis labels
     */
    static javax.swing.border.Border selectionEdges(JTable table, int row, int column, int firstValueColumn) {
        int top = row == 0 || !table.isCellSelected(row - 1, column) ? SELECTION_EDGE : 0;
        int left = column <= firstValueColumn || !table.isCellSelected(row, column - 1) ? SELECTION_EDGE : 0;
        int bottom = row == table.getRowCount() - 1 || !table.isCellSelected(row + 1, column) ? SELECTION_EDGE : 0;
        int right = column == table.getColumnCount() - 1 || !table.isCellSelected(row, column + 1) ? SELECTION_EDGE : 0;
        return BorderFactory.createMatteBorder(top, left, bottom, right, selection());
    }

    /** Share of the selection colour in a selected cell. */
    static final double SELECTION_TINT = 0.25;

    static Color mix(Color base, Color over, double share) {
        // [tag:fixed_color] not a written colour: a blend of two theme colours
        return new Color(
            (int) Math.round(base.getRed() + share * (over.getRed() - base.getRed())),
            (int) Math.round(base.getGreen() + share * (over.getGreen() - base.getGreen())),
            (int) Math.round(base.getBlue() + share * (over.getBlue() - base.getBlue())));
    }

    private static Color themed(String key, int lightFallback) {
        Color c = UIManager.getColor(key);
        return c != null ? c : new Color(lightFallback);
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
