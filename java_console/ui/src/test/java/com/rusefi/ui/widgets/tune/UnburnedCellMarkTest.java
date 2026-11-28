package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * [tag:unburned_marks] Table and curve cells changed since the last burn are marked, with the burned
 * value in their tooltip and a count beside the table (#10671).
 */
public class UnburnedCellMarkTest {
    private static final String INI = "[Constants]\npage = 1\n"
        + "z = array, U08, 0, [3x2], \"\", 1, 0, 0, 255, 0\n"
        + "x = array, U08, 6, [3], \"\", 1, 0, 0, 255, 0\n"
        + "y = array, U08, 9, [2], \"\", 1, 0, 0, 255, 0\n"
        + "cx = array, U08, 12, [3], \"\", 1, 0, 0, 255, 0\n"
        + "cy = array, U08, 15, [3], \"\", 1, 0, 0, 255, 0\n"
        + "[TableEditor]\ntable = table, tableMap, \"Table\", 1\nxBins = x\nyBins = y\nzBins = z\n"
        + "[CurveEditor]\ncurve = curve, \"Curve\"\ncolumnLabel = \"X\", \"Y\"\n"
        + "xAxis = 0, 255, 5\nyAxis = 0, 255, 5\nxBins = cx\nyBins = cy\n";

    private static IniFileModel ini() throws Throwable {
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(18);
        when(meta.getSignature()).thenReturn("unburned-cells");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(INI.getBytes(StandardCharsets.US_ASCII))), "unburned-cells.ini", meta);
    }

    /** z = 10..60, x bins 1..3, y bins 7..8, curve points (1,11) (2,12) (3,13). */
    private static ConfigurationImage burned() {
        byte[] bytes = {10, 20, 30, 40, 50, 60, 1, 2, 3, 7, 8, 0, 1, 2, 3, 11, 12, 13};
        return ConfigurationImageTestHelper.newLittleEndian(bytes);
    }

    private static TuningTableView table(IniFileModel ini, ConfigurationImage working, ConfigurationImage burned,
                                         Node orientation) {
        TuningTableView view = new TuningTableView("Table", false, orientation);
        view.displayTable(ini, "table", working);
        view.setBurnedImage(() -> burned);
        return view;
    }

    @Test
    void unchangedTableHasNoMarks() throws Throwable {
        ConfigurationImage burned = burned();
        TuningTableView view = table(ini(), burned.clone(), burned, new Node());
        JTable table = findTable(view.getContent());
        for (int row = 0; row < table.getRowCount(); row++) {
            for (int column = 0; column < table.getColumnCount(); column++) {
                assertNull(view.burnedCellText(row, column), row + "," + column);
                assertFalse(UnburnedCellMark.isMarked(render(table, row, column)));
            }
        }
        assertFalse(view.getUnburnedCountLabelForTests().isVisible());
    }

    @Test
    void editedCellIsMarkedWithItsBurnedValue() throws Throwable {
        ConfigurationImage burned = burned();
        TuningTableView view = table(ini(), burned.clone(), burned, new Node());
        JTable table = findTable(view.getContent());
        // the top row shows the last stored row: (0, 3) is stored z[1][2] = 60
        assertEquals("60", table.getValueAt(0, 3));
        table.changeSelection(0, 3, false, false);
        table.setValueAt("99", 0, 3);
        view.refreshUnburnedMarks();

        assertEquals("60", view.burnedCellText(0, 3));
        Component cell = render(table, 0, 3);
        assertTrue(UnburnedCellMark.isMarked(cell));
        String tooltip = ((JComponent) cell).getToolTipText();
        assertTrue(tooltip.contains("Now: 99") && tooltip.contains("Burned: 60"), tooltip);
        assertNull(view.burnedCellText(0, 2));
        assertFalse(UnburnedCellMark.isMarked(render(table, 0, 2)), "a reused renderer drops the mark");
        assertEquals("1 cell not burned", view.getUnburnedCountLabelForTests().getText());
        assertTrue(view.getUnburnedCountLabelForTests().isVisible());
    }

    @Test
    void changedAxisValuesAreMarkedAndCounted() throws Throwable {
        ConfigurationImage burned = burned();
        ConfigurationImage working = burned.clone();
        TuningTableView view = table(ini(), working, burned, new Node());
        JTable table = findTable(view.getContent());
        table.setValueAt("9", 1, 0); // the bottom row is the first Y bin, 7
        working.getContent()[7] = 5; // second X bin, 2
        view.refreshUnburnedMarks();

        assertEquals("7", view.burnedCellText(1, 0));
        assertNull(view.burnedCellText(0, 0));
        assertEquals("2", view.burnedXAxisText(2));
        assertNull(view.burnedXAxisText(1));
        Component header = table.getTableHeader().getDefaultRenderer()
            .getTableCellRendererComponent(table, table.getColumnName(2), false, false, -1, 2);
        assertTrue(UnburnedCellMark.isMarked(header));
        assertEquals("2 cells not burned", view.getUnburnedCountLabelForTests().getText());
    }

    @Test
    void changeCountUsesTheImageForEachDisplayedAxis() throws Throwable {
        IniFileModel ini = ini();
        ini.findIniField("x").orElseThrow().setPageIndex(0x0100);
        ConfigurationImage burned = burned();
        ConfigurationImage xPage = burned.clone();
        ConfigurationImage working = burned.clone();
        working.getContent()[9] = 9; // Y changed from 7 on the main page; the X page is unchanged.
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("Table", true, new Node());
            view.displayTable(ini, "table", working, xPage, working);
            view.setBurnedImage(() -> burned);

            assertEquals("7", view.burnedCellText(1, 0));
            assertTrue(view.getUnburnedCountLabelForTests().isVisible());
            assertEquals("1 cell not burned", view.getUnburnedCountLabelForTests().getText());

            ((JCheckBoxMenuItem) view.orientationMenuFor(0).getComponent(0)).doClick(0);
            assertEquals("7", view.burnedXAxisText(1));
            assertEquals("1 cell not burned", view.getUnburnedCountLabelForTests().getText());
        });
    }

    @Test
    void marksFollowFlippedAndInvertedAxes() throws Throwable {
        IniFileModel ini = ini();
        ArrayIniField z = (ArrayIniField) ini.findIniField("z").get();
        for (int options = 0; options < 8; options++) {
            Node orientation = new Node();
            Node tableNode = orientation.getChild("table");
            tableNode.setBoolProperty("flip_xy", (options & 1) != 0);
            tableNode.setBoolProperty("invert_x", (options & 2) != 0);
            tableNode.setBoolProperty("invert_y", (options & 4) != 0);
            ConfigurationImage burned = burned();
            ConfigurationImage working = burned.clone();
            working.getContent()[z.getOffset(0, 1)] = 99; // stored z[0][1], burned 20
            TuningTableView view = table(ini, working, burned, orientation);
            JTable table = findTable(view.getContent());
            Double[][] stored = ConfigurationImageGetterSetter.getArrayValues(z, working);

            int marked = 0;
            for (int row = 0; row < table.getRowCount(); row++) {
                for (int column = 1; column < table.getColumnCount(); column++) {
                    int[] cell = view.storedValuesCell(row, column);
                    assertEquals(stored[cell[0]][cell[1]], Double.valueOf((String) table.getValueAt(row, column)),
                        "options " + options + " cell " + row + "," + column);
                    if (view.burnedCellText(row, column) != null) {
                        marked++;
                        assertEquals("99", table.getValueAt(row, column));
                        assertEquals("20", view.burnedCellText(row, column));
                    }
                }
            }
            assertEquals(1, marked, "options " + options);
        }
    }

    @Test
    void editedCurvePointIsMarkedAndCounted() throws Throwable {
        IniFileModel ini = ini();
        ConfigurationImage burned = burned();
        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = new CurveWidget(ini.getCurves().get("curve"), ini, burned.clone());
            widget.setBurnedImage(() -> burned);
            JTable table = findTable(widget.getContentPane());
            table.changeSelection(1, 1, false, false);
            table.setValueAt("42", 1, 1);
            widget.refreshUnburnedMarks();

            assertEquals("12", widget.burnedCellText(1, 1));
            assertNull(widget.burnedCellText(1, 0));
            assertNull(widget.burnedCellText(0, 1));
            assertTrue(UnburnedCellMark.isMarked(render(table, 1, 1)));
            assertEquals("1 cell not burned", widget.getUnburnedCountLabelForTests().getText());
        });
    }

    @Test
    void tablePageClearsItsMarksWhenTheBurnedTuneChanges() throws Throwable {
        IniFileModel ini = ini();
        ConfigurationImage[] burned = {burned()};
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.setBurnedImage(() -> burned[0]);
                widget.update("table", ini, burned[0].clone());
                JTable table = findTable(widget.getContentPane());
                table.changeSelection(0, 3, false, false);
                table.setValueAt("99", 0, 3);
                JLabel count = find(widget.getContentPane(), "unburnedCellCount");
                assertEquals("1 cell not burned", count.getText(), "an edit refreshes the count");

                burned[0] = widget.getWorkingImage().clone(); // as after a burn
                widget.refreshUnburnedMarks();
                assertFalse(count.isVisible());
                assertFalse(UnburnedCellMark.isMarked(render(table, 0, 3)));
            } finally {
                widget.destroy();
            }
        });
    }

    @Test
    void tooltipNamesTheReferenceTune() {
        String burned = UnburnedCellMark.tooltip("99", "60", UnburnedWording.BURNED);
        assertTrue(burned.contains("Now: 99") && burned.contains("Burned: 60")
            && burned.contains("Not burned to the ECU yet"), burned);
        String loaded = UnburnedCellMark.tooltip("99", "60", UnburnedWording.LOADED);
        assertTrue(loaded.contains("Loaded: 60") && loaded.contains("since the tune was loaded"), loaded);
    }

    /** The dark theme's amber vanished on the orange selection: the mark keeps its contrast on every cell. */
    @Test
    void markStandsOutOnEveryCellColour() {
        Color lightAmber = new Color(0xb45309);
        Color darkAmber = new Color(0xfbbf24);
        List<Color> cells = new ArrayList<>();
        cells.add(Color.ORANGE); // the selected cells
        for (int value = 0; value <= 100; value++) {
            cells.add(CalibrationColorPalette.background(value, 0, 100));
        }
        for (Color amber : Arrays.asList(lightAmber, darkAmber)) {
            for (Color cell : cells) {
                Color mark = UnburnedCellMark.markColor(cell, amber);
                assertTrue(UnburnedCellMark.contrast(mark, cell) >= UnburnedCellMark.MIN_CONTRAST,
                    "mark " + mark + " on " + cell);
            }
        }
        assertEquals(lightAmber, UnburnedCellMark.markColor(Color.ORANGE, lightAmber), "amber where it stands out");
        assertNotEquals(darkAmber, UnburnedCellMark.markColor(Color.ORANGE, darkAmber));
    }

    @Test
    void countWording() {
        assertEquals("", UnburnedCellMark.countText(0));
        assertEquals("1 cell not burned", UnburnedCellMark.countText(1));
        assertEquals("4 cells not burned", UnburnedCellMark.countText(4));
    }

    private static Component render(JTable table, int row, int column) {
        return table.prepareRenderer(table.getCellRenderer(row, column), row, column);
    }

    private static JTable findTable(Container parent) {
        for (Component c : all(parent)) {
            if (c instanceof JTable) {
                return (JTable) c;
            }
        }
        throw new AssertionError("no table");
    }

    private static JLabel find(Container parent, String name) {
        for (Component c : all(parent)) {
            if (c instanceof JLabel && name.equals(c.getName())) {
                return (JLabel) c;
            }
        }
        throw new AssertionError("no label " + name);
    }

    private static List<Component> all(Container parent) {
        List<Component> result = new ArrayList<>();
        for (Component c : parent.getComponents()) {
            result.add(c);
            if (c instanceof Container) {
                result.addAll(all((Container) c));
            }
        }
        return result;
    }
}
