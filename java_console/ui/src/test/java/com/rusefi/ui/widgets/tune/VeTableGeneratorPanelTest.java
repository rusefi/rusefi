package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

/** The generator's preview grids look like the tuning tables, in every theme. */
public class VeTableGeneratorPanelTest {
    private static final double[] RPM = {1000, 2000};
    private static final double[] MAP = {50, 100};

    @Test
    public void valueCellsUseTheSharedColourScale() {
        JTable table = table(VeTableGeneratorPanel.buildVeTable(new double[][]{{40, 60}, {80, 100}}, RPM, MAP, false));
        for (int row = 0; row < 2; row++) {
            for (int column = 1; column < 3; column++) {
                double value = Double.parseDouble(String.valueOf(table.getValueAt(row, column)));
                Component cell = table.prepareRenderer(table.getCellRenderer(row, column), row, column);
                Color expected = CalibrationColorPalette.background(value, 40, 100);
                assertEquals(expected, cell.getBackground(), "cell " + row + "," + column);
                assertEquals(CalibrationColorPalette.foreground(expected), cell.getForeground());
            }
        }
        Component axis = table.prepareRenderer(table.getCellRenderer(0, 0), 0, 0);
        assertEquals(CalibrationColorPalette.axisBackground(), axis.getBackground());
        assertTrue(axis.getFont().isBold(), "axis values read as headers");
        assertEquals(new Dimension(2, 2), table.getIntercellSpacing());
        assertFalse(table.getShowHorizontalLines());
    }

    @Test
    public void deltaCellsUseTheThemeChangeColours() {
        JTable table = table(VeTableGeneratorPanel.buildVeTable(new double[][]{{2, -2}, {0, 0.01}}, RPM, MAP, true));
        assertEquals(CalibrationColorPalette.change(1, 0.05), background(table, rowOf(table, "2"), 1));
        assertNotEquals(CalibrationColorPalette.middle(), CalibrationColorPalette.change(1, 0.05), "a raised cell stands out");
        assertNotEquals(CalibrationColorPalette.middle(), CalibrationColorPalette.change(-1, 0.05), "a lowered cell stands out");
        assertEquals(CalibrationColorPalette.middle(), CalibrationColorPalette.change(0.01, 0.05), "an unchanged cell is not white");
    }

    @Test
    public void selectedCellsKeepTheirColourUnderTheTint() {
        JTable table = table(VeTableGeneratorPanel.buildVeTable(new double[][]{{40, 60}, {80, 100}}, RPM, MAP, false));
        Color unselected = background(table, 0, 1);
        table.changeSelection(0, 1, false, false);
        assertEquals(CalibrationColorPalette.selected(unselected), background(table, 0, 1));
    }

    private static int rowOf(JTable table, String value) {
        for (int row = 0; row < table.getRowCount(); row++) {
            Object cell = table.getValueAt(row, 1);
            if (Double.parseDouble(String.valueOf(cell)) == Double.parseDouble(value)) {
                return row;
            }
        }
        fail("no row with " + value);
        return -1;
    }

    private static Color background(JTable table, int row, int column) {
        return table.prepareRenderer(table.getCellRenderer(row, column), row, column).getBackground();
    }

    private static JTable table(JComponent scroll) {
        return (JTable) ((JScrollPane) scroll).getViewport().getView();
    }
}
