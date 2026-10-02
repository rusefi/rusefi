package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

public class TuningTableViewTest {

    @Test
    public void testSelectionPreservedAfterDelta() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        assertNotNull(table);

        // Mock data
        Double[][] data = {{1.0, 2.0}, {3.0, 4.0}};
        Double[] xBins = {100.0, 200.0};
        Double[] yBins = {10.0, 20.0};

        table.setModel(new TuningTableView.TuningTableModel(data, xBins, yBins, 1));

        // Select a cell
        table.setRowSelectionInterval(0, 0);
        table.setColumnSelectionInterval(1, 1);

        assertTrue(table.isCellSelected(0, 1));

        // Apply delta
        JTextField deltaField = findTextField(view.getContent(), "0.5");
        assertNotNull(deltaField);

        // Simulate click Up (via private method access or just calling it if it was public,
        // but since I'm testing the behavior I'll use the buttons)
        JButton upButton = findButton(view.getContent(), "Up");
        assertNotNull(upButton);
        upButton.doClick();

        // Check selection still exists
        assertTrue(table.isCellSelected(0, 1), "Selection should be preserved after Up");

        // Check value updated
        // rowIndex 0 in table is reversedRowIndex 1 in data (data.length - 1 - 0)
        // colIndex 1 in table is col 0 in data (1 - 1)
        assertEquals(3.5, data[1][0], 0.001);
    }

    @Test
    public void testSelectionClearedOnNewTable() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());

        // Setup initial table
        Double[][] data = {{1.0}};
        table.setModel(new TuningTableView.TuningTableModel(data, new Double[]{1.0}, new Double[]{1.0}, 1));
        table.setRowSelectionInterval(0, 0);
        table.setColumnSelectionInterval(1, 1);
        assertTrue(table.isCellSelected(0, 1));

        // Display new table
        IniFileModel iniFile = mock(IniFileModel.class);
        TableModel tableModel = mock(TableModel.class);
        when(iniFile.getTable("newTable")).thenReturn(tableModel);
        when(tableModel.getZBinsConstant()).thenReturn("zBins");
        ArrayIniField zField = new ArrayIniField("zBins", 0, FieldType.FLOAT, 1, 1, "", 1, "0", "100", "1");
        when(iniFile.findIniField("zBins")).thenReturn(Optional.of(zField));

        view.displayTable(iniFile, "newTable", new ConfigurationImage(new byte[100]));

        assertFalse(table.isCellSelected(0, 1), "Selection should be cleared on new table");
    }

    @Test
    public void testEqualsButton() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        Double[][] data = {{1.0, 2.0}, {3.0, 4.0}};
        table.setModel(new TuningTableView.TuningTableModel(data, new Double[]{100.0, 200.0}, new Double[]{10.0, 20.0}, 1));

        // Select row 0, col 1 (which is data[1][0] due to reversal)
        table.setRowSelectionInterval(0, 0);
        table.setColumnSelectionInterval(1, 1);

        // We can't easily test JOptionPane.showConfirmDialog in a headless unit test
        // but we can test the underlying logic if we extract it or if we mock JOptionPane.
        // For now, I'll just verify the button exists.
        JButton equalsButton = findButton(view.getContent(), "=");
        assertNotNull(equalsButton);
    }

    @Test
    public void testTableActionsAreAlwaysVisibleAndHaveShortcuts() {
        TuningTableView view = new TuningTableView("Test");
        JButton horizontalButton = findButton(view.getContent(), "H");
        JButton verticalButton = findButton(view.getContent(), "V");
        JButton interpolateButton = findButton(view.getContent(), "Interpolate");
        JButton smoothButton = findButton(view.getContent(), "Smooth");
        JTable table = findTable(view.getContent());

        assertNotNull(horizontalButton);
        assertNotNull(verticalButton);
        assertNotNull(interpolateButton);
        assertNotNull(smoothButton);
        assertTrue(interpolateButton.getParent().isVisible());
        assertEquals("Interpolate Horizontal - Key: H", horizontalButton.getToolTipText());
        assertEquals("Interpolate Vertical - Key: V", verticalButton.getToolTipText());
        assertEquals("Interpolate selected cells - Key: /", interpolateButton.getToolTipText());
        assertNotNull(view.getContent().getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT)
            .get(KeyStroke.getKeyStroke(KeyEvent.VK_H, 0)));
        assertNotNull(view.getContent().getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT)
            .get(KeyStroke.getKeyStroke(KeyEvent.VK_V, 0)));
        assertNotNull(view.getContent().getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT)
            .get(KeyStroke.getKeyStroke(KeyEvent.VK_SLASH, 0)));
        assertNotNull(view.getContent().getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT)
            .get(KeyStroke.getKeyStroke(KeyEvent.VK_S, 0)));
    }

    @Test
    public void testHorizontalInterpolationWritesEncodedValueAndNotifiesOnce() {
        TuningTableView view = new TuningTableView("Test");
        IniFileModel iniFile = mock(IniFileModel.class);
        TableModel tableModel = mock(TableModel.class);
        ArrayIniField zField = new ArrayIniField("zBins", 0, FieldType.UINT8, 3, 2, "", 1, "0", "255", "0");
        ConfigurationImage image = new ConfigurationImage(6);
        ConfigurationImageGetterSetter.setArrayValues(zField, image, new Double[][]{
            {40.0, 12.0, 81.0},
            {81.0, 12.0, 40.0}
        });

        when(iniFile.getTable("table")).thenReturn(tableModel);
        when(tableModel.getZBinsConstant()).thenReturn("zBins");
        when(tableModel.getXBinsConstant()).thenReturn("xBins");
        when(tableModel.getYBinsConstant()).thenReturn("yBins");
        when(iniFile.findIniField("zBins")).thenReturn(Optional.of(zField));
        when(iniFile.findIniField("xBins")).thenReturn(Optional.empty());
        when(iniFile.findIniField("yBins")).thenReturn(Optional.empty());

        AtomicInteger editCount = new AtomicInteger();
        view.setOnEdit(editCount::incrementAndGet);
        view.displayTable(iniFile, "table", image);
        JTable table = findTable(view.getContent());
        table.setRowSelectionInterval(0, 1);
        table.setColumnSelectionInterval(1, 3);

        Object actionKey = view.getContent().getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT)
            .get(KeyStroke.getKeyStroke(KeyEvent.VK_H, 0));
        view.getContent().getActionMap().get(actionKey).actionPerformed(
            new ActionEvent(table, ActionEvent.ACTION_PERFORMED, "H"));

        Double[][] encoded = ConfigurationImageGetterSetter.getArrayValues(zField, image);
        assertArrayEquals(new Double[]{40.0, 61.0, 81.0}, encoded[0]);
        assertArrayEquals(new Double[]{81.0, 61.0, 40.0}, encoded[1]);
        TuningTableView.TuningTableModel model = (TuningTableView.TuningTableModel) table.getModel();
        assertArrayEquals(encoded[0], model.data[0]);
        assertArrayEquals(encoded[1], model.data[1]);
        assertEquals(1, editCount.get());
        assertTrue(table.isCellSelected(1, 2));
    }

    @Test
    public void testVerticalInterpolationUsesDisplayedRowOrder() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        Double[][] data = {{80.0, 40.0}, {12.0, 99.0}, {40.0, 80.0}};
        table.setModel(new TuningTableView.TuningTableModel(
            data, new Double[]{100.0, 200.0}, new Double[]{10.0, 20.0, 30.0}, 1));
        table.setRowSelectionInterval(0, 2);
        table.setColumnSelectionInterval(1, 2);

        findButton(view.getContent(), "V").doClick();

        assertArrayEquals(new Double[]{80.0, 40.0}, data[0]);
        assertArrayEquals(new Double[]{60.0, 60.0}, data[1]);
        assertArrayEquals(new Double[]{40.0, 80.0}, data[2]);
    }

    @Test
    public void testBilinearInterpolationUsesFourCorners() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        Double[][] data = {
            {70.0, 22.0, 100.0},
            {15.0, 44.0, 82.0},
            {40.0, 93.0, 60.0}
        };
        table.setModel(new TuningTableView.TuningTableModel(
            data, new Double[]{1.0, 2.0, 3.0}, new Double[]{1.0, 2.0, 3.0}, 1));
        table.setRowSelectionInterval(0, 2);
        table.setColumnSelectionInterval(1, 3);

        findButton(view.getContent(), "Interpolate").doClick();

        assertArrayEquals(new Double[]{70.0, 85.0, 100.0}, data[0]);
        assertArrayEquals(new Double[]{55.0, 67.5, 80.0}, data[1]);
        assertArrayEquals(new Double[]{40.0, 50.0, 60.0}, data[2]);
    }

    @Test
    public void testSmoothUsesSnapshotAndTwentyFivePercentBlend() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        Double[][] data = {
            {0.0, 0.0, 0.0},
            {0.0, 100.0, 0.0},
            {0.0, 0.0, 0.0}
        };
        table.setModel(new TuningTableView.TuningTableModel(
            data, new Double[]{1.0, 2.0, 3.0}, new Double[]{1.0, 2.0, 3.0}, 1));
        table.setRowSelectionInterval(0, 2);
        table.setColumnSelectionInterval(1, 3);

        findButton(view.getContent(), "Smooth").doClick();

        assertEquals(75.0, data[1][1], 0.001);
        assertEquals(100.0 / 12.0, data[0][0], 0.001);
        assertEquals(5.0, data[0][1], 0.001);
    }

    @Test
    public void testEqualsButtonLogic() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        Double[][] data = {{1.0, 2.0}, {3.0, 4.0}};
        table.setModel(new TuningTableView.TuningTableModel(data, new Double[]{100.0, 200.0}, new Double[]{10.0, 20.0}, 1));

        // Select row 0, col 1 (reversedRowIndex 1, dataCol 0) -> currently 3.0
        table.setRowSelectionInterval(0, 0);
        table.setColumnSelectionInterval(1, 1);

        view.setValue(70.0, table.getSelectedRows(), table.getSelectedColumns());

        assertEquals(70.0, data[1][0], 0.001);
        assertTrue(table.isCellSelected(0, 1), "Selection should be preserved after SetValue");
    }

    @Test
    public void testDecimalPoints() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        Double[][] data = {{1.0, 2.55}, {3.0, 1.2}};
        // Precision 2
        TuningTableView.TuningTableModel model = new TuningTableView.TuningTableModel(data, new Double[]{100.0, 200.0}, new Double[]{10.0, 20.0}, 2);
        table.setModel(model);

        // Whole numbers omit an all-zero decimal part.
        assertEquals("3", table.getValueAt(0, 1));
        assertEquals("1", table.getValueAt(1, 1));
        // Non-zero decimal parts keep the configured precision.
        assertEquals("1.20", table.getValueAt(0, 2));
        // data[0][1] is 2.55, with precision 2 it should be "2.55"
        assertEquals("2.55", table.getValueAt(1, 2));
    }

    @Test
    public void testNumbersAreCentered() {
        TuningTableView view = new TuningTableView("Test");
        JTable table = findTable(view.getContent());
        table.setModel(new TuningTableView.TuningTableModel(
            new Double[][]{{1.0}}, new Double[]{100.0}, new Double[]{10.0}, 1));

        Component renderer = table.prepareRenderer(table.getCellRenderer(0, 1), 0, 1);
        assertTrue(renderer instanceof JLabel);
        assertEquals(SwingConstants.CENTER, ((JLabel) renderer).getHorizontalAlignment());
    }

    @Test
    public void testViewModeHidesButtons() {
        TuningTableView view = new TuningTableView("Test", true);
        assertNull(findButton(view.getContent(), "Up"), "Up button should be hidden in viewMode");
        assertNull(findButton(view.getContent(), "Down"), "Down button should be hidden in viewMode");
        assertNull(findButton(view.getContent(), "="), "Equals button should be hidden in viewMode");
        assertNull(findButton(view.getContent(), "H"), "Horizontal interpolate should be hidden in viewMode");
        assertNull(findButton(view.getContent(), "V"), "Vertical interpolate should be hidden in viewMode");
        assertNull(findButton(view.getContent(), "Interpolate"), "Interpolate should be hidden in viewMode");
        assertNull(findButton(view.getContent(), "Smooth"), "Smooth should be hidden in viewMode");
        assertNull(findTextField(view.getContent(), "0.5"), "Delta field should be hidden in viewMode");
    }

    @Test
    public void testKeyboardEditWritesImageAndNavigatesValueCells() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicInteger edits = new AtomicInteger();
            ConfigurationImage image = new ConfigurationImage(4);
            TuningTableView view = editableView(false, image);
            view.setOnEdit(edits::incrementAndGet);
            JTable table = findTable(view.getContent());

            pressKey(table, "ENTER");
            assertTrue(table.isEditing());
            assertEquals(0, table.getEditingRow());
            assertEquals(1, table.getEditingColumn());
            JTextField editor = (JTextField) table.getEditorComponent();
            assertEquals(editor.getText(), editor.getSelectedText());
            editor.setText("42.4");
            pressKey(editor, "ENTER");
            assertFalse(table.isEditing());
            assertEquals("42", table.getValueAt(0, 1));
            assertEquals(42, image.getContent()[2]);
            assertEquals(1, edits.get());
            assertTrue(table.isCellSelected(0, 1));

            pressKey(table, "RIGHT");
            pressKey(table, "F2");
            editor = (JTextField) table.getEditorComponent();
            editor.setText("51");
            pressKey(editor, "TAB");
            assertFalse(table.isEditing());
            assertEquals(51, image.getContent()[3]);
            assertEquals(2, edits.get());
            assertTrue(table.isCellSelected(1, 1));

            pressKey(table, "ENTER");
            editor = (JTextField) table.getEditorComponent();
            editor.setText("63");
            pressKey(editor, "shift TAB");
            assertEquals(63, image.getContent()[0]);
            assertTrue(table.isCellSelected(0, 2));
            assertEquals(3, edits.get());

            table.changeSelection(0, 1, false, false);
            pressKey(table, "shift TAB");
            assertTrue(table.isCellSelected(1, 2));
            pressKey(table, "TAB");
            assertTrue(table.isCellSelected(0, 1));
        });
    }

    @Test
    public void testInvalidEditAndEscapeLeaveImageUntouched() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            TuningTableView view = editableView(false, image);
            AtomicInteger edits = new AtomicInteger();
            view.setOnEdit(edits::incrementAndGet);
            JTable table = findTable(view.getContent());
            pressKey(table, "ENTER");
            JTextField editor = (JTextField) table.getEditorComponent();
            pressKey(view.getContent(), "S");
            assertEquals(0, edits.get(), "Bulk shortcuts must not change values while typing an edit");
            for (String invalid : new String[]{"abc", "", "NaN", "Infinity"}) {
                editor.setText(invalid);
                pressKey(editor, "TAB");
                assertTrue(table.isEditing());
                assertTrue(table.isCellSelected(0, 1));
                assertEquals(0, edits.get());
            }
            editor.setText("99");
            pressKey(editor, "ESCAPE");
            assertFalse(table.isEditing());
            assertEquals("3", table.getValueAt(0, 1));
            assertEquals(3, image.getContent()[2]);
            assertEquals(0, edits.get());
        });
    }

    @Test
    public void testAxesAndComparisonTablesCannotBeEdited() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean viewMode : new boolean[]{false, true}) {
                TuningTableView view = editableView(viewMode, new ConfigurationImage(4));
                JTable table = findTable(view.getContent());
                table.changeSelection(0, 0, false, false);
                assertFalse(table.editCellAt(0, 0));
                pressKey(table, "ENTER");
                assertFalse(table.isEditing());
                if (viewMode) {
                    table.changeSelection(0, 1, false, false);
                    pressKey(table, "ENTER");
                    assertFalse(table.isEditing());
                    assertFalse(table.editCellAt(0, 1));
                }
            }
        });
    }

    @Test
    public void testAxisHighlightFollowsActiveCellAndClearsWithSelection() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTable table = findTable(editableView(false, new ConfigurationImage(4)).getContent());
            Color xNormal = headerBackground(table, 1);
            Color yNormal = cellBackground(table, 0, 0);
            table.changeSelection(0, 1, false, false);
            assertNotEquals(xNormal, headerBackground(table, 1));
            assertEquals(xNormal, headerBackground(table, 2));
            assertNotEquals(yNormal, cellBackground(table, 0, 0));
            assertEquals(yNormal, cellBackground(table, 1, 0));

            pressKey(table, "shift RIGHT");
            pressKey(table, "shift DOWN");
            assertTrue(table.isCellSelected(0, 1));
            assertEquals(xNormal, headerBackground(table, 1));
            assertNotEquals(xNormal, headerBackground(table, 2));
            assertEquals(yNormal, cellBackground(table, 0, 0));
            assertNotEquals(yNormal, cellBackground(table, 1, 0));
            table.clearSelection();
            assertEquals(xNormal, headerBackground(table, 2));
            assertEquals(yNormal, cellBackground(table, 1, 0));
        });
    }

    @Test
    public void testWheelMovesActiveCellAndStopsAtEdges() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean viewMode : new boolean[]{false, true}) {
                ConfigurationImage image = new ConfigurationImage(4);
                TuningTableView view = editableView(viewMode, image);
                AtomicInteger edits = new AtomicInteger();
                view.setOnEdit(edits::incrementAndGet);
                JTable table = findTable(view.getContent());
                wheel(table, 1);
                assertEquals(-1, table.getSelectedRow());

                table.changeSelection(0, 2, false, false);
                Color normalAxis = cellBackground(table, 1, 0);
                wheel(table, 1);
                assertTrue(table.isCellSelected(1, 2));
                assertNotEquals(normalAxis, cellBackground(table, 1, 0));
                assertEquals(normalAxis, cellBackground(table, 0, 0));
                wheel(table, 3);
                assertTrue(table.isCellSelected(1, 2));
                wheel(table, -1);
                assertTrue(table.isCellSelected(0, 2));
                wheel(table, -3);
                assertTrue(table.isCellSelected(0, 2));

                if (!viewMode) {
                    pressKey(table, "ENTER");
                    JTextField editor = (JTextField) table.getEditorComponent();
                    editor.setText("99");
                    wheel(table, 1);
                    assertTrue(table.isEditing());
                    assertTrue(table.isCellSelected(0, 2));
                    assertEquals("99", editor.getText());
                    pressKey(editor, "ESCAPE");
                }
                assertEquals(0, edits.get());
                assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            }
        });
    }

    @Test
    public void testWheelKeepsActiveCellVisible() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTable table = findTable(new TuningTableView("Test").getContent());
            Double[][] data = new Double[30][2];
            for (Double[] row : data) {
                java.util.Arrays.fill(row, 1.0);
            }
            table.setModel(new TuningTableView.TuningTableModel(data, null, null, 0));
            JViewport viewport = (JViewport) table.getParent();
            viewport.setSize(300, table.getRowHeight() * 3);
            table.setSize(300, table.getRowHeight() * 30);
            table.changeSelection(0, 2, false, false);
            wheel(table, 10);
            assertTrue(table.isCellSelected(10, 2));
            assertTrue(viewport.getViewRect().contains(table.getCellRect(10, 2, false)));
            wheel(table, -10);
            assertTrue(table.isCellSelected(0, 2));
            assertEquals(0, viewport.getViewPosition().y);
        });
    }

    private void wheel(JTable table, int rotation) {
        MouseWheelEvent event = new MouseWheelEvent(table, MouseEvent.MOUSE_WHEEL,
            System.currentTimeMillis(), 0, 10, 10, 0, false,
            MouseWheelEvent.WHEEL_UNIT_SCROLL, 3, rotation);
        table.dispatchEvent(event);
        assertTrue(event.isConsumed());
    }

    private TuningTableView editableView(boolean viewMode, ConfigurationImage image) {
        TuningTableView view = new TuningTableView("Test", viewMode);
        IniFileModel iniFile = mock(IniFileModel.class);
        TableModel tableModel = mock(TableModel.class);
        ArrayIniField field = new ArrayIniField("zBins", 0, FieldType.UINT8, 2, 2, "", 1, "0", "255", "0");
        ConfigurationImageGetterSetter.setArrayValues(field, image, new Double[][]{{1.0, 2.0}, {3.0, 4.0}});
        when(iniFile.getTable("table")).thenReturn(tableModel);
        when(tableModel.getZBinsConstant()).thenReturn("zBins");
        when(iniFile.findIniField("zBins")).thenReturn(Optional.of(field));
        view.displayTable(iniFile, "table", image);
        return view;
    }

    private void pressKey(JComponent component, String key) {
        KeyStroke stroke = KeyStroke.getKeyStroke(key);
        Object actionKey = component.getInputMap(JComponent.WHEN_FOCUSED).get(stroke);
        if (actionKey == null) {
            actionKey = component.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).get(stroke);
        }
        Action action = component.getActionMap().get(actionKey);
        assertNotNull(action, "Missing action for " + key);
        action.actionPerformed(new ActionEvent(component, ActionEvent.ACTION_PERFORMED, key));
    }

    private Color headerBackground(JTable table, int column) {
        return table.getTableHeader().getDefaultRenderer().getTableCellRendererComponent(
            table, table.getColumnName(column), false, false, -1, column).getBackground();
    }

    private Color cellBackground(JTable table, int row, int column) {
        return table.prepareRenderer(table.getCellRenderer(row, column), row, column).getBackground();
    }

    private JTable findTable(JComponent c) {
        if (c instanceof JTable) return (JTable) c;
        for (int i = 0; i < c.getComponentCount(); i++) {
            if (c.getComponent(i) instanceof JComponent) {
                JTable found = findTable((JComponent) c.getComponent(i));
                if (found != null) return found;
            }
        }
        return null;
    }

    private JTextField findTextField(JComponent c, String text) {
        if (c instanceof JTextField && ((JTextField) c).getText().equals(text)) return (JTextField) c;
        for (int i = 0; i < c.getComponentCount(); i++) {
            if (c.getComponent(i) instanceof JComponent) {
                JTextField found = findTextField((JComponent) c.getComponent(i), text);
                if (found != null) return found;
            }
        }
        return null;
    }

    private JButton findButton(JComponent c, String text) {
        if (c instanceof JButton && ((JButton) c).getText().equals(text)) return (JButton) c;
        for (int i = 0; i < c.getComponentCount(); i++) {
            if (c.getComponent(i) instanceof JComponent) {
                JButton found = findButton((JComponent) c.getComponent(i), text);
                if (found != null) return found;
            }
        }
        return null;
    }

}
