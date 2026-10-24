package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.beans.PropertyChangeListener;
import java.beans.XMLDecoder;
import java.beans.XMLEncoder;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.Optional;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Predicate;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeTrue;
import static org.junit.jupiter.api.Assumptions.assumeFalse;
import static org.mockito.Mockito.*;

public class TuningTableViewTest {
    @Test
    public void popupTriggerOpensOnlyFromCornerHeader() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless());
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = editableView(false, new ConfigurationImage(4));
            JFrame frame = new JFrame("Table orientation menu test");
            try {
                frame.add(view.getContent());
                frame.setSize(600, 400);
                frame.setVisible(true);
                JTable table = findTable(view.getContent());
                JComponent header = table.getTableHeader();
                for (int column : new int[]{0, 1}) {
                    Rectangle bounds = table.getTableHeader().getHeaderRect(column);
                    header.dispatchEvent(new MouseEvent(header, MouseEvent.MOUSE_RELEASED,
                        System.currentTimeMillis(), 0, bounds.x + 5, bounds.y + 5, 1, true,
                        MouseEvent.BUTTON3));
                    boolean open = java.util.Arrays.stream(MenuSelectionManager.defaultManager().getSelectedPath())
                        .anyMatch(element -> element instanceof JPopupMenu);
                    assertEquals(column == 0, open);
                    MenuSelectionManager.defaultManager().clearSelectedPath();
                }
            } finally {
                MenuSelectionManager.defaultManager().clearSelectedPath();
                frame.dispose();
            }
        });
    }

    @Test
    public void orientationMenuMapsDisplayedCellsAndAxesBackToStoredTune() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node settings = new Node();
            IniFileModel ini = mock(IniFileModel.class);
            TableModel definition = mock(TableModel.class);
            ArrayIniField timing = new ArrayIniField("timing", 0, FieldType.UINT8, 3, 2, "", 1, "0", "255", "0");
            ArrayIniField rpm = new ArrayIniField("rpm", 0, FieldType.UINT16, 3, 1, "RPM", 1, "0", "8000", "0");
            ArrayIniField load = new ArrayIniField("load", 6, FieldType.UINT16, 1, 2, "kPa", 1, "0", "300", "0");
            ConfigurationImage timingImage = new ConfigurationImage(6);
            ConfigurationImage axisImage = new ConfigurationImage(10);
            ConfigurationImageGetterSetter.setArrayValues(timing, timingImage,
                new Double[][]{{11.0, 22.0, 33.0}, {44.0, 55.0, 66.0}});
            ConfigurationImageGetterSetter.setArrayValues(rpm, axisImage,
                new Double[][]{{1000.0, 2500.0, 6000.0}});
            ConfigurationImageGetterSetter.setArrayValues(load, axisImage,
                new Double[][]{{40.0}, {120.0}});
            byte[] originalTiming = timingImage.getContent().clone();
            byte[] originalAxes = axisImage.getContent().clone();
            when(ini.getTable("ignition")).thenReturn(definition);
            when(definition.getZBinsConstant()).thenReturn("timing");
            when(definition.getXBinsConstant()).thenReturn("rpm");
            when(definition.getYBinsConstant()).thenReturn("load");
            when(ini.findIniField("timing")).thenReturn(Optional.of(timing));
            when(ini.findIniField("rpm")).thenReturn(Optional.of(rpm));
            when(ini.findIniField("load")).thenReturn(Optional.of(load));

            TuningTableView view = new TuningTableView("Ignition timing", false, settings);
            AtomicInteger orientationChanges = new AtomicInteger();
            view.setOnOrientationChange(orientationChanges::incrementAndGet);
            view.displayTable(ini, "ignition", timingImage, axisImage);
            JTable table = findTable(view.getContent());
            assertNull(view.orientationMenuFor(1), "Only the top-left header opens orientation settings");
            JPopupMenu menu = view.orientationMenuFor(0);
            assertEquals(3, menu.getComponentCount());
            for (Component component : menu.getComponents()) {
                assertInstanceOf(JCheckBoxMenuItem.class, component);
                assertFalse(((JCheckBoxMenuItem) component).isSelected());
            }
            assertEquals("Flip X/Y axes", ((JCheckBoxMenuItem) menu.getComponent(0)).getText());
            assertEquals("Invert X axis", ((JCheckBoxMenuItem) menu.getComponent(1)).getText());
            assertEquals("Invert Y axis", ((JCheckBoxMenuItem) menu.getComponent(2)).getText());

            ((JCheckBoxMenuItem) menu.getComponent(0)).doClick();
            assertEquals("RPM \\ Load", table.getColumnName(0));
            assertEquals("40", table.getColumnName(1));
            assertEquals("120", table.getColumnName(2));
            assertArrayEquals(new Object[]{"6000", "33", "66"}, tableRow(table, 0));
            assertArrayEquals(new Object[]{"2500", "22", "55"}, tableRow(table, 1));
            assertArrayEquals(new Object[]{"1000", "11", "44"}, tableRow(table, 2));

            ((JCheckBoxMenuItem) view.orientationMenuFor(0).getComponent(1)).doClick();
            assertEquals("120", table.getColumnName(1));
            assertArrayEquals(new Object[]{"6000", "66", "33"}, tableRow(table, 0));
            ((JCheckBoxMenuItem) view.orientationMenuFor(0).getComponent(2)).doClick();
            assertArrayEquals(new Object[]{"1000", "44", "11"}, tableRow(table, 0));
            assertArrayEquals(new Object[]{"2500", "55", "22"}, tableRow(table, 1));
            assertArrayEquals(new Object[]{"6000", "66", "33"}, tableRow(table, 2));
            assertArrayEquals(originalTiming, timingImage.getContent(), "Display options do not edit the tune");
            assertArrayEquals(originalAxes, axisImage.getContent());
            assertEquals(3, orientationChanges.get());

            table.setValueAt("77", 0, 1);
            table.setValueAt("88", 2, 2);
            Double[][] stored = ConfigurationImageGetterSetter.getArrayValues(timing, timingImage);
            assertArrayEquals(new Double[]{11.0, 22.0, 88.0}, stored[0]);
            assertArrayEquals(new Double[]{77.0, 55.0, 66.0}, stored[1]);
            table.setValueAt("1100", 0, 0);
            assertArrayEquals(new Double[]{1100.0, 2500.0, 6000.0},
                ConfigurationImageGetterSetter.getArrayValues(rpm, axisImage)[0]);
            table.getTableHeader().setSize(300, 30);
            Rectangle header = table.getTableHeader().getHeaderRect(1);
            table.getTableHeader().dispatchEvent(new MouseEvent(table.getTableHeader(), MouseEvent.MOUSE_CLICKED,
                System.currentTimeMillis(), 0, header.x + 5, 10, 2, false, MouseEvent.BUTTON1));
            JTextField inlineAxis = findTextField(table.getTableHeader(), "120");
            assertNotNull(inlineAxis);
            inlineAxis.setText("130");
            pressKey(inlineAxis, "ENTER");
            assertArrayEquals(new Double[]{40.0}, ConfigurationImageGetterSetter.getArrayValues(load, axisImage)[0]);
            assertArrayEquals(new Double[]{130.0}, ConfigurationImageGetterSetter.getArrayValues(load, axisImage)[1]);

            Node restored = roundTrip(settings);
            TuningTableView reopened = new TuningTableView("Ignition timing", false, restored);
            reopened.displayTable(ini, "ignition", timingImage, axisImage);
            JTable reopenedTable = findTable(reopened.getContent());
            assertArrayEquals(new Object[]{"1100", "77", "11"}, tableRow(reopenedTable, 0));
            assertEquals("130", reopenedTable.getColumnName(1));
            for (Component component : reopened.orientationMenuFor(0).getComponents()) {
                assertTrue(((JCheckBoxMenuItem) component).isSelected());
            }
            when(ini.getTable("other")).thenReturn(definition);
            reopened.displayTable(ini, "other", timingImage, axisImage);
            assertEquals("Load \\ RPM", reopenedTable.getColumnName(0),
                "Orientation preferences are scoped to a table ID");
            reopened.displayTable(ini, "ignition", timingImage, axisImage);
            ((JCheckBoxMenuItem) reopened.orientationMenuFor(0).getComponent(0)).doClick();
            assertArrayEquals(new Object[]{"40", "88", "22", "11"}, tableRow(reopenedTable, 0));
            assertEquals("6000", reopenedTable.getColumnName(1));
            ((JCheckBoxMenuItem) reopened.orientationMenuFor(0).getComponent(2)).doClick();
            assertArrayEquals(new Object[]{"130", "66", "55", "77"}, tableRow(reopenedTable, 0));
            ((JCheckBoxMenuItem) reopened.orientationMenuFor(0).getComponent(1)).doClick();
            assertArrayEquals(new Object[]{"130", "77", "55", "66"}, tableRow(reopenedTable, 0));
            assertEquals("1100", reopenedTable.getColumnName(1));
            ((JCheckBoxMenuItem) reopened.orientationMenuFor(0).getComponent(2)).doClick();
            assertArrayEquals(new Object[]{"40", "11", "22", "88"}, tableRow(reopenedTable, 0));
            assertArrayEquals(new Object[]{"130", "77", "55", "66"}, tableRow(reopenedTable, 1));
        });
    }

    private Node roundTrip(Node node) {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (XMLEncoder encoder = new XMLEncoder(bytes)) {
            encoder.writeObject(node);
        }
        try (XMLDecoder decoder = new XMLDecoder(new ByteArrayInputStream(bytes.toByteArray()))) {
            return (Node) decoder.readObject();
        }
    }

    @Test
    public void rectangularTableKeepsRpmHorizontalAndLoadDescendingVertically() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("Ignition timing");
            IniFileModel ini = mock(IniFileModel.class);
            TableModel definition = mock(TableModel.class);
            ArrayIniField timing = new ArrayIniField("timing", 0, FieldType.UINT8, 3, 2, "", 1, "0", "255", "0");
            ArrayIniField rpm = new ArrayIniField("rpm", 0, FieldType.UINT16, 3, 1, "RPM", 1, "0", "8000", "0");
            ArrayIniField load = new ArrayIniField("load", 6, FieldType.UINT16, 1, 2, "kPa", 1, "0", "300", "0");
            ConfigurationImage timingImage = new ConfigurationImage(6);
            ConfigurationImage axisImage = new ConfigurationImage(10);
            ConfigurationImageGetterSetter.setArrayValues(timing, timingImage,
                new Double[][]{{11.0, 22.0, 33.0}, {44.0, 55.0, 66.0}});
            ConfigurationImageGetterSetter.setArrayValues(rpm, axisImage,
                new Double[][]{{1000.0, 2500.0, 6000.0}});
            ConfigurationImageGetterSetter.setArrayValues(load, axisImage,
                new Double[][]{{40.0}, {120.0}});
            when(ini.getTable("ignition")).thenReturn(definition);
            when(definition.getZBinsConstant()).thenReturn("timing");
            when(definition.getXBinsConstant()).thenReturn("rpm");
            when(definition.getYBinsConstant()).thenReturn("load");
            when(ini.findIniField("timing")).thenReturn(Optional.of(timing));
            when(ini.findIniField("rpm")).thenReturn(Optional.of(rpm));
            when(ini.findIniField("load")).thenReturn(Optional.of(load));

            view.displayTable(ini, "ignition", timingImage, axisImage);
            JTable table = findTable(view.getContent());
            assertEquals(2, table.getRowCount());
            assertEquals(4, table.getColumnCount());
            assertEquals("Load \\ RPM", table.getColumnName(0));
            assertArrayEquals(new String[]{"1000", "2500", "6000"},
                new String[]{table.getColumnName(1), table.getColumnName(2), table.getColumnName(3)});
            assertArrayEquals(new Object[]{"120", "44", "55", "66"}, tableRow(table, 0));
            assertArrayEquals(new Object[]{"40", "11", "22", "33"}, tableRow(table, 1));
            assertEquals(CalibrationColorPalette.background(44, 11, 66), cellBackground(table, 0, 1));
            assertEquals(CalibrationColorPalette.background(33, 11, 66), cellBackground(table, 1, 3));
            assertEquals(table.getBackground(), cellBackground(table, 0, 0),
                "Axis labels are not shaded as timing values");

            table.setValueAt("77", 0, 1);
            table.setValueAt("88", 1, 3);
            Double[][] stored = ConfigurationImageGetterSetter.getArrayValues(timing, timingImage);
            assertArrayEquals(new Double[]{11.0, 22.0, 88.0}, stored[0]);
            assertArrayEquals(new Double[]{77.0, 55.0, 66.0}, stored[1]);
            assertEquals("77", table.getValueAt(0, 1));
            assertEquals("88", table.getValueAt(1, 3));
            table.setValueAt("130", 0, 0);
            Double[][] storedLoad = ConfigurationImageGetterSetter.getArrayValues(load, axisImage);
            assertArrayEquals(new Double[]{40.0}, storedLoad[0]);
            assertArrayEquals(new Double[]{130.0}, storedLoad[1]);
        });
    }

    private Object[] tableRow(JTable table, int row) {
        Object[] values = new Object[table.getColumnCount()];
        for (int column = 0; column < values.length; column++) {
            values[column] = table.getValueAt(row, column);
        }
        return values;
    }

    @Test
    public void testKeyboardEntryModes() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            JTable table = findTable(editableView(false, image).getContent());
            table.changeSelection(0, 1, false, false);
            pressKey(table, "ENTER");
            assertFalse(table.isEditing(), "Enter without an edit retains the selected cell");
            assertTrue(table.isCellSelected(0, 1));
            pressKey(table, "typed 6");
            assertTrue(table.isEditing());
            JTextField editor = (JTextField) table.getEditorComponent();
            assertEquals("6", editor.getText(), "The first digit replaces the previous value");
            pressKey(table, "typed 4");
            assertEquals("64", editor.getText());
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent(), "Replacement remains pending");
            pressKey(table, "ESCAPE");
            assertFalse(table.isEditing());
            table.setColumnSelectionInterval(1, 2);
            pressKey(table, "F2");
            assertFalse(table.isEditing(), "A selection must not open a single-cell editor");
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
        });
    }

    @Test
    public void testReplacementCommitsWithArrowsAndRetainsCellOnEnter() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (String key : new String[]{"ENTER", "shift ENTER", "LEFT", "RIGHT", "UP", "DOWN", "TAB", "shift TAB"}) {
                ConfigurationImage image = new ConfigurationImage(4);
                TuningTableView view = editableView(false, image);
                AtomicInteger edits = new AtomicInteger();
                view.setOnEdit(edits::incrementAndGet);
                JTable table = findTable(view.getContent());
                table.changeSelection(0, 2, false, false);
                pressKey(table, "typed 6");
                pressKey(table, "typed 4");
                pressKey(table, key);
                assertFalse(table.isEditing(), key);
                assertArrayEquals(new byte[]{1, 2, 3, 64}, image.getContent(), key);
                assertEquals(1, edits.get(), "One write per accepted value: " + key);
                int row = key.equals("DOWN") || key.equals("TAB") ? 1 : 0;
                int column = key.equals("LEFT") || key.equals("shift TAB") || key.equals("TAB") ? 1 : 2;
                assertTrue(table.isCellSelected(row, column), key);
            }
        });
    }

    @Test
    public void testTextEditingKeepsHorizontalCaretNavigation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            JTable table = findTable(editableView(false, image).getContent());
            table.changeSelection(0, 1, false, false);
            pressKey(table, "F2");
            JTextField editor = (JTextField) table.getEditorComponent();
            assertEquals(editor.getText(), editor.getSelectedText());
            editor.setText("64");
            editor.setSize(100, 24);
            editor.setCaretPosition(2);
            pressKey(editor, "LEFT");
            assertTrue(table.isEditing(), "Horizontal text navigation must not accept the value");
            pressKey(editor, "RIGHT");
            assertTrue(table.isCellSelected(0, 1));
            assertTrue(table.isEditing());
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            pressKey(editor, "DOWN");
            assertFalse(table.isEditing());
            assertTrue(table.isCellSelected(1, 1));
            assertArrayEquals(new byte[]{1, 2, 64, 4}, image.getContent());
        });
    }

    @Test
    public void testFocusLossAcceptsValidValuesAndCancelsInvalidValues() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean textEditing : new boolean[]{false, true}) {
                for (String value : new String[]{"65", "", "bad", "NaN", "Infinity"}) {
                    ConfigurationImage image = new ConfigurationImage(4);
                    TuningTableView view = editableView(false, image);
                    AtomicInteger edits = new AtomicInteger();
                    view.setOnEdit(edits::incrementAndGet);
                    JTable table = findTable(view.getContent());
                    table.changeSelection(0, 1, false, false);
                    pressKey(table, textEditing ? "F2" : "typed 6");
                    JTextField editor = (JTextField) table.getEditorComponent();
                    editor.setText(value);
                    Component focused = textEditing ? editor : table;
                    loseFocus(focused, new JButton());
                    assertFalse(table.isEditing(), value);
                    boolean valid = value.equals("65");
                    assertArrayEquals(new byte[]{1, 2, (byte) (valid ? 65 : 3), 4}, image.getContent(), value);
                    assertEquals(valid ? 1 : 0, edits.get(), value);
                    loseFocus(editor, table);
                    assertEquals(valid ? 1 : 0, edits.get(), "A late focus event must not write twice");
                }
            }
        });
    }

    @Test
    public void testMovingFocusIntoEditorDoesNotAcceptReplacement() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            JTable table = findTable(editableView(false, image).getContent());
            table.changeSelection(0, 1, false, false);
            pressKey(table, "typed 6");
            Component editor = table.getEditorComponent();
            pressKey(table, "F2");
            loseFocus(table, editor);
            assertTrue(table.isEditing());
            assertEquals("6", ((JTextField) editor).getText());
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
        });
    }

    @Test
    public void testSelectedCellsAcceptDigitsImmediatelyAndEscapeDoesNotUndo() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            TuningTableView view = editableView(false, image);
            AtomicInteger edits = new AtomicInteger();
            view.setOnEdit(edits::incrementAndGet);
            JTable table = findTable(view.getContent());
            table.setRowSelectionInterval(0, 1);
            table.setColumnSelectionInterval(0, 2);
            Object axis = table.getValueAt(0, 0);
            pressKey(table, "typed 6");
            assertFalse(table.isEditing());
            assertArrayEquals(new byte[]{6, 6, 6, 6}, image.getContent());
            pressKey(table, "typed 4");
            assertArrayEquals(new byte[]{64, 64, 64, 64}, image.getContent());
            assertEquals(2, edits.get(), "Notify once per key, not once per cell");
            assertEquals(axis, table.getValueAt(0, 0), "Bulk numeric entry excludes the axis column");
            assertArrayEquals(new int[]{0, 1}, table.getSelectedRows());
            assertArrayEquals(new int[]{0, 1, 2}, table.getSelectedColumns());
            pressKey(table, "BACK_SPACE");
            assertArrayEquals(new byte[]{6, 6, 6, 6}, image.getContent());
            pressKey(table, "ESCAPE");
            assertArrayEquals(new byte[]{6, 6, 6, 6}, image.getContent());
            pressKey(table, "typed 2");
            assertArrayEquals(new byte[]{2, 2, 2, 2}, image.getContent(), "Escape resets the entry buffer");
            table.setRowSelectionInterval(0, 0);
            pressKey(table, "typed 3");
            assertArrayEquals(new byte[]{2, 2, 3, 3}, image.getContent(), "A new selection starts a new number");
        });
    }

    @Test
    public void testPlusMinusAdjustByOneRegardlessOfPrecision() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(16);
            ArrayIniField field = new ArrayIniField("zBins", 0, FieldType.FLOAT, 2, 2, "", 1, "-100", "100", "2");
            TuningTableView view = editableView(false, image, field);
            JTable table = findTable(view.getContent());
            table.setValueAt(3.25, 0, 1);
            table.changeSelection(0, 1, false, false);
            AtomicInteger edits = new AtomicInteger();
            view.setOnEdit(edits::incrementAndGet);

            pressKey(table, "typed +");
            assertEquals(4.25, ConfigurationImageGetterSetter.getArrayValues(field, image)[1][0]);
            pressKey(table, "shift typed +");
            assertEquals(5.25, ConfigurationImageGetterSetter.getArrayValues(field, image)[1][0]);
            pressKey(table, "typed -");
            Double[][] values = ConfigurationImageGetterSetter.getArrayValues(field, image);
            assertArrayEquals(new Double[]{4.25, 4.0}, values[1]);
            assertArrayEquals(new Double[]{1.0, 2.0}, values[0]);
            assertEquals(3, edits.get());
            assertTrue(table.isCellSelected(0, 1));
            assertFalse(table.isEditing());
        });
    }

    @Test
    public void testPlusMinusAdjustSelectionSkipAxesAndResetBulkEntry() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            TuningTableView view = editableView(false, image);
            JTable table = findTable(view.getContent());
            AtomicInteger edits = new AtomicInteger();
            view.setOnEdit(edits::incrementAndGet);
            table.clearSelection();
            pressKey(table, "typed +");
            table.changeSelection(0, 0, false, false);
            pressKey(table, "typed -");
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            assertEquals(0, edits.get(), "Empty and axis-only selections do not change the tune");

            table.setRowSelectionInterval(0, 1);
            table.setColumnSelectionInterval(0, 2);
            Object axis = table.getValueAt(0, 0);
            pressKey(table, "typed +");
            assertArrayEquals(new byte[]{2, 3, 4, 5}, image.getContent());
            pressKey(table, "typed -");
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            assertEquals(2, edits.get(), "One notification per key, not per cell");
            assertEquals(axis, table.getValueAt(0, 0));
            assertArrayEquals(new int[]{0, 1}, table.getSelectedRows());
            assertArrayEquals(new int[]{0, 1, 2}, table.getSelectedColumns());

            for (String key : new String[]{"typed +", "typed -"}) {
                pressKey(table, "typed 6");
                pressKey(table, key);
                pressKey(table, "typed 2");
                assertArrayEquals(new byte[]{2, 2, 2, 2}, image.getContent(),
                    "Adjustment ends the previous bulk number entry");
            }
        });
    }

    @Test
    public void testPlusMinusRemainTextDuringPendingReplacement() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (String sign : new String[]{"+", "-"}) {
                ConfigurationImage image = new ConfigurationImage(16);
                ArrayIniField field = new ArrayIniField("zBins", 0, FieldType.FLOAT, 2, 2, "", 1, "-100", "100", "1");
                TuningTableView view = editableView(false, image, field);
                JTable table = findTable(view.getContent());
                AtomicInteger edits = new AtomicInteger();
                view.setOnEdit(edits::incrementAndGet);
                table.changeSelection(0, 1, false, false);
                pressKey(table, "DELETE");
                pressKey(table, "typed " + sign);
                pressKey(table, "typed 6");
                assertEquals(sign + "6", ((JTextField) table.getEditorComponent()).getText());
                assertEquals(3.0, ConfigurationImageGetterSetter.getArrayValues(field, image)[1][0]);
                assertEquals(0, edits.get());
                pressKey(table, "ENTER");
                assertEquals(sign.equals("+") ? 6.0 : -6.0,
                    ConfigurationImageGetterSetter.getArrayValues(field, image)[1][0]);
                assertEquals(1, edits.get());
            }
        });
    }

    @Test
    public void testDecimalReplacementAndSelectedCellEntry() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(16);
            ArrayIniField field = new ArrayIniField("zBins", 0, FieldType.FLOAT, 2, 2, "", 1, "0", "100", "1");
            JTable table = findTable(editableView(false, image, field).getContent());
            table.changeSelection(0, 1, false, false);
            pressKey(table, "typed 6");
            pressKey(table, "typed .");
            pressKey(table, "typed 4");
            pressKey(table, "ENTER");
            assertEquals(6.4, ConfigurationImageGetterSetter.getArrayValues(field, image)[1][0], 0.00001);
            table.setColumnSelectionInterval(1, 2);
            pressKey(table, "typed 7");
            pressKey(table, "typed .");
            pressKey(table, "typed .");
            pressKey(table, "typed 5");
            Double[][] values = ConfigurationImageGetterSetter.getArrayValues(field, image);
            assertEquals(7.5, values[1][0], 0.00001);
            assertEquals(7.5, values[1][1], 0.00001);
            assertFalse(table.isEditing());
            pressKey(table, "ESCAPE");
            pressKey(table, "typed .");
            assertEquals(7.6, ConfigurationImageGetterSetter.getArrayValues(field, image)[1][0], 0.00001,
                "A period outside numeric entry increments by the displayed precision");
        });
    }

    @Test
    public void testDeleteAndBackspaceStartEmptyPendingEditsAndSpaceDoesNotEdit() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            JTable table = findTable(editableView(false, image).getContent());
            table.changeSelection(0, 1, false, false);
            pressKey(table, "SPACE");
            assertFalse(table.isEditing());
            for (String key : new String[]{"DELETE", "BACK_SPACE"}) {
                pressKey(table, key);
                assertTrue(table.isEditing());
                assertEquals("", ((JTextField) table.getEditorComponent()).getText());
                pressKey(table, "ESCAPE");
                assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            }
            pressKey(table, "typed 6");
            pressKey(table, "typed 4");
            pressKey(table, "BACK_SPACE");
            assertEquals("6", ((JTextField) table.getEditorComponent()).getText());
            pressKey(table, "ENTER");
            assertArrayEquals(new byte[]{1, 2, 6, 4}, image.getContent());
        });
    }

    @Test
    public void testNavigationStaysInValueGridAndPreservesColumnAtRowEndpoints() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTable table = findTable(editableView(false, new ConfigurationImage(4)).getContent());
            table.changeSelection(0, 1, false, false);
            pressKey(table, "LEFT");
            pressKey(table, "UP");
            assertTrue(table.isCellSelected(0, 1));
            pressKey(table, "END");
            assertTrue(table.isCellSelected(0, 2));
            pressKey(table, "ctrl END");
            assertTrue(table.isCellSelected(1, 2));
            pressKey(table, "RIGHT");
            pressKey(table, "DOWN");
            assertTrue(table.isCellSelected(1, 2));
            pressKey(table, "ctrl HOME");
            assertTrue(table.isCellSelected(0, 2));
            pressKey(table, "HOME");
            assertTrue(table.isCellSelected(0, 1));
        });
    }

    @Test
    public void testLoadingTableCancelsPendingValueAndResetsSelectionEntry() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage original = new ConfigurationImage(4);
            TuningTableView view = editableView(false, original);
            JTable table = findTable(view.getContent());
            table.changeSelection(0, 1, false, false);
            pressKey(table, "typed 9");
            IniFileModel ini = mock(IniFileModel.class);
            TableModel definition = mock(TableModel.class);
            ArrayIniField field = new ArrayIniField("zBins", 0, FieldType.UINT8, 2, 2, "", 1, "0", "255", "0");
            when(ini.getTable("table")).thenReturn(definition);
            when(definition.getZBinsConstant()).thenReturn("zBins");
            when(ini.findIniField("zBins")).thenReturn(Optional.of(field));
            ConfigurationImage next = new ConfigurationImage(new byte[]{10, 20, 30, 40});
            view.displayTable(ini, "table", next);
            assertFalse(table.isEditing());
            assertEquals(-1, table.getSelectedRow());
            assertArrayEquals(new byte[]{1, 2, 3, 4}, original.getContent());
            assertArrayEquals(new byte[]{10, 20, 30, 40}, next.getContent());
            table.setRowSelectionInterval(0, 0);
            table.setColumnSelectionInterval(1, 2);
            pressKey(table, "typed 6");
            view.displayTable(ini, "table", next);
            table.setRowSelectionInterval(0, 0);
            table.setColumnSelectionInterval(1, 2);
            pressKey(table, "typed 2");
            assertArrayEquals(new byte[]{10, 20, 2, 2}, next.getContent());
        });
    }

    @Test
    public void testReadOnlyTableRejectsNumericAndSelectionEntry() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            JTable table = findTable(editableView(true, image).getContent());
            for (boolean multiple : new boolean[]{false, true}) {
                table.setRowSelectionInterval(0, multiple ? 1 : 0);
                table.setColumnSelectionInterval(1, multiple ? 2 : 1);
                for (String key : new String[]{"typed 6", "typed .", "typed +", "shift typed +", "typed -",
                    "DELETE", "BACK_SPACE", "F2", "ENTER"}) {
                    pressKey(table, key);
                    assertFalse(table.isEditing(), key);
                    assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent(), key);
                }
            }
        });
    }

    private void loseFocus(Component component, Component opposite) {
        FocusEvent event = new FocusEvent(component,
            FocusEvent.FOCUS_LOST, false, opposite);
        for (FocusListener listener : component.getFocusListeners()) {
            listener.focusLost(event);
        }
    }

    @Test
    public void testKeyboardFocusWithRealEvents() throws Exception {
        assumeTrue(Boolean.getBoolean("rusefi.test.keyboardFocus"),
            "Opt in with a dedicated display for native keyboard events");
        ConfigurationImage image = new ConfigurationImage(4);
        AtomicReference<JFrame> window = new AtomicReference<>();
        AtomicReference<JTable> grid = new AtomicReference<>();
        AtomicReference<JButton> outside = new AtomicReference<>();
        Robot robot = new Robot();
        robot.setAutoWaitForIdle(true);
        try {
            SwingUtilities.invokeAndWait(() -> {
                TuningTableView view = editableView(false, image);
                JTable table = findTable(view.getContent());
                JFrame frame = new JFrame("Keyboard editing test");
                JButton button = new JButton("Outside table");
                window.set(frame);
                grid.set(table);
                outside.set(button);
                frame.add(view.getContent(), BorderLayout.CENTER);
                frame.add(button, BorderLayout.SOUTH);
                frame.setSize(640, 480);
                frame.setVisible(true);
                table.changeSelection(0, 2, false, false);
                table.requestFocusInWindow();
            });
            JTable table = grid.get();
            awaitFocus(component -> component == table);
            typeKey(robot, KeyEvent.VK_ADD);
            SwingUtilities.invokeAndWait(() -> assertArrayEquals(new byte[]{1, 2, 3, 5}, image.getContent()));
            typeKey(robot, KeyEvent.VK_SUBTRACT);
            SwingUtilities.invokeAndWait(() -> assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent()));
            robot.keyPress(KeyEvent.VK_SHIFT);
            typeKey(robot, KeyEvent.VK_EQUALS);
            robot.keyRelease(KeyEvent.VK_SHIFT);
            robot.waitForIdle();
            SwingUtilities.invokeAndWait(() -> assertArrayEquals(new byte[]{1, 2, 3, 5}, image.getContent()));
            typeKey(robot, KeyEvent.VK_MINUS);
            SwingUtilities.invokeAndWait(() -> assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent()));
            typeKey(robot, KeyEvent.VK_F2);
            awaitFocus(component -> component != null && component == table.getEditorComponent());
            typeKey(robot, KeyEvent.VK_MINUS);
            typeKey(robot, KeyEvent.VK_6);
            SwingUtilities.invokeAndWait(() -> {
                assertEquals("-6", ((JTextField) table.getEditorComponent()).getText());
                assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            });
            typeKey(robot, KeyEvent.VK_ESCAPE);
            awaitFocus(component -> component == table);
            typeKey(robot, KeyEvent.VK_6);
            typeKey(robot, KeyEvent.VK_4);
            SwingUtilities.invokeAndWait(() -> {
                assertTrue(table.isFocusOwner(), "Replacement input keeps focus on the grid");
                assertTrue(table.isEditing());
                assertEquals("64", ((JTextField) table.getEditorComponent()).getText());
                assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            });
            typeKey(robot, KeyEvent.VK_LEFT);
            SwingUtilities.invokeAndWait(() -> {
                assertFalse(table.isEditing());
                assertTrue(table.isCellSelected(0, 1));
                assertArrayEquals(new byte[]{1, 2, 3, 64}, image.getContent());
            });
            typeKey(robot, KeyEvent.VK_F2);
            awaitFocus(component -> component != null && component == table.getEditorComponent());
            typeKey(robot, KeyEvent.VK_6);
            typeKey(robot, KeyEvent.VK_5);
            typeKey(robot, KeyEvent.VK_LEFT);
            SwingUtilities.invokeAndWait(() -> {
                assertTrue(table.isEditing());
                assertEquals(1, ((JTextField) table.getEditorComponent()).getCaretPosition());
                assertTrue(table.isCellSelected(0, 1));
            });
            typeKey(robot, KeyEvent.VK_RIGHT);
            SwingUtilities.invokeAndWait(() -> assertEquals(2,
                ((JTextField) table.getEditorComponent()).getCaretPosition()));
            typeKey(robot, KeyEvent.VK_ENTER);
            awaitFocus(component -> component == table);
            SwingUtilities.invokeAndWait(() -> {
                assertFalse(table.isEditing());
                assertTrue(table.isCellSelected(0, 1));
                assertArrayEquals(new byte[]{1, 2, 65, 64}, image.getContent());
            });
            typeKey(robot, KeyEvent.VK_7);
            typeKey(robot, KeyEvent.VK_ESCAPE);
            SwingUtilities.invokeAndWait(() -> assertArrayEquals(new byte[]{1, 2, 65, 64}, image.getContent()));
            typeKey(robot, KeyEvent.VK_DELETE);
            SwingUtilities.invokeAndWait(() -> outside.get().requestFocusInWindow());
            awaitFocus(component -> component == outside.get());
            SwingUtilities.invokeAndWait(() -> {
                assertFalse(table.isEditing(), "Invalid input is cancelled on real focus loss");
                assertArrayEquals(new byte[]{1, 2, 65, 64}, image.getContent());
                table.requestFocusInWindow();
            });
            awaitFocus(component -> component == table);
            typeKey(robot, KeyEvent.VK_8);
            typeKey(robot, KeyEvent.VK_TAB);
            SwingUtilities.invokeAndWait(() -> {
                assertFalse(table.isEditing());
                assertTrue(table.isCellSelected(0, 2));
                assertArrayEquals(new byte[]{1, 2, 8, 64}, image.getContent());
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (window.get() != null) {
                    window.get().dispose();
                }
            });
        }
    }

    private void typeKey(Robot robot, int code) {
        robot.keyPress(code);
        robot.keyRelease(code);
        robot.waitForIdle();
    }

    private void awaitFocus(Predicate<Component> expected) throws Exception {
        KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        CountDownLatch focused = new CountDownLatch(1);
        PropertyChangeListener listener = event -> {
            if (expected.test(manager.getFocusOwner())) {
                focused.countDown();
            }
        };
        try {
            SwingUtilities.invokeAndWait(() -> {
                manager.addPropertyChangeListener("focusOwner", listener);
                if (expected.test(manager.getFocusOwner())) {
                    focused.countDown();
                }
            });
            assertTrue(focused.await(5, TimeUnit.SECONDS), "Keyboard focus did not reach the target");
        } finally {
            SwingUtilities.invokeAndWait(() -> manager.removePropertyChangeListener("focusOwner", listener));
        }
    }

    @Test
    public void testPlainWheelAdjustsSelection() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(4);
            TuningTableView view = editableView(false, image);
            AtomicInteger edits = new AtomicInteger();
            view.setOnEdit(edits::incrementAndGet);
            JTable table = findTable(view.getContent());
            table.setRowSelectionInterval(0, 0);
            table.setColumnSelectionInterval(1, 2);
            MouseWheelEvent event = new MouseWheelEvent(table, MouseEvent.MOUSE_WHEEL,
                System.currentTimeMillis(), 0, 10, 10, 0, false,
                MouseWheelEvent.WHEEL_UNIT_SCROLL, 3, 3);
            table.dispatchEvent(event);
            assertTrue(event.isConsumed());
            assertArrayEquals(new byte[]{1, 2, 2, 3}, image.getContent(), "One decrement, regardless of rotation magnitude");
            assertEquals(1, edits.get(), "Notify once for the whole selected region");
            assertTrue(table.isCellSelected(0, 1));
            assertTrue(table.isCellSelected(0, 2));
            wheel(table, -3, 0);
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent());
            assertEquals(2, edits.get());
            wheel(table, 0, 0);
            assertEquals(2, edits.get(), "Zero rotation does nothing");
            table.changeSelection(0, 0, false, false);
            wheel(table, -1, 0);
            assertArrayEquals(new byte[]{1, 2, 3, 4}, image.getContent(), "Do not adjust axes");
            assertEquals(2, edits.get());
            table.clearSelection();
            wheel(table, -1, 0);
            assertEquals(2, edits.get());
        });
    }

    @Test
    public void testWheelUsesPrecisionAndShiftThreeSteps() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConfigurationImage image = new ConfigurationImage(16);
            ArrayIniField field = new ArrayIniField("zBins", 0, FieldType.FLOAT, 2, 2, "", 1, "0", "100", "1");
            TuningTableView view = editableView(false, image, field);
            AtomicInteger edits = new AtomicInteger();
            view.setOnEdit(edits::incrementAndGet);
            JTable table = findTable(view.getContent());
            table.setRowSelectionInterval(0, 0);
            table.setColumnSelectionInterval(0, 2);
            wheel(table, -3, 0);
            Double[][] values = ConfigurationImageGetterSetter.getArrayValues(field, image);
            assertEquals(3.1, values[1][0], 0.00001);
            assertEquals(4.1, values[1][1], 0.00001);
            wheel(table, 7, java.awt.event.InputEvent.SHIFT_DOWN_MASK);
            values = ConfigurationImageGetterSetter.getArrayValues(field, image);
            assertEquals(2.8, values[1][0], 0.00001);
            assertEquals(3.8, values[1][1], 0.00001);
            wheel(table, -7, java.awt.event.InputEvent.SHIFT_DOWN_MASK);
            values = ConfigurationImageGetterSetter.getArrayValues(field, image);
            assertEquals(3.1, values[1][0], 0.00001);
            assertEquals(4.1, values[1][1], 0.00001);
            assertArrayEquals(new Double[]{1.0, 2.0}, values[0], "Unselected row stays untouched");
            assertEquals(3, edits.get());
            assertTrue(table.isCellSelected(0, 0));
            assertTrue(table.isCellSelected(0, 2));
            TuningTableView.TuningTableModel model = (TuningTableView.TuningTableModel) table.getModel();
            assertArrayEquals(values[1], model.data[1], "Model reflects encoded values");
        });
    }

    @Test
    public void testAxisEditingWritesSeparateAxisImage() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicInteger headerDialogs = new AtomicInteger();
            TuningTableView view = new TuningTableView("Axes") {
                @Override
                protected int showConfirmDialog(JPanel panel) {
                    headerDialogs.incrementAndGet();
                    return JOptionPane.CANCEL_OPTION;
                }
            };
            IniFileModel ini = mock(IniFileModel.class);
            TableModel definition = mock(TableModel.class);
            ArrayIniField z = new ArrayIniField("z", 0, FieldType.UINT8, 2, 2, "", 1, "0", "255", "0");
            ArrayIniField x = new ArrayIniField("x", 2, FieldType.UINT16, 2, 1, "", 0.5, "0", "1000", "1");
            ArrayIniField y = new ArrayIniField("y", 8, FieldType.UINT16, 1, 2, "", 0.5, "0", "1000", "1");
            ConfigurationImage zImage = new ConfigurationImage(4);
            ConfigurationImage axisImage = new ConfigurationImage(16);
            ConfigurationImageGetterSetter.setArrayValues(z, zImage, new Double[][]{{1.0, 2.0}, {3.0, 4.0}});
            ConfigurationImageGetterSetter.setArrayValues(x, axisImage, new Double[][]{{100.0, 200.0}});
            ConfigurationImageGetterSetter.setArrayValues(y, axisImage, new Double[][]{{10.0}, {20.0}});
            byte[] originalAxes = axisImage.getContent().clone();
            byte[] originalZ = zImage.getContent().clone();
            when(ini.getTable("table")).thenReturn(definition);
            when(definition.getZBinsConstant()).thenReturn("z");
            when(definition.getXBinsConstant()).thenReturn("x");
            when(definition.getYBinsConstant()).thenReturn("y");
            when(ini.findIniField("z")).thenReturn(Optional.of(z));
            when(ini.findIniField("x")).thenReturn(Optional.of(x));
            when(ini.findIniField("y")).thenReturn(Optional.of(y));
            AtomicInteger edits = new AtomicInteger();
            view.setOnEdit(edits::incrementAndGet);
            view.displayTable(ini, "table", zImage, axisImage);
            JTable table = findTable(view.getContent());
            table.getTableHeader().setSize(300, 30);
            Rectangle header = table.getTableHeader().getHeaderRect(1);
            table.getTableHeader().dispatchEvent(new MouseEvent(table.getTableHeader(), MouseEvent.MOUSE_CLICKED,
                System.currentTimeMillis(), 0, header.x + 5, 10, 2, false, MouseEvent.BUTTON1));
            assertEquals(0, headerDialogs.get(), "Axis editing must not open a dialog");
            JTextField inlineAxis = findTextField(table.getTableHeader(), table.getColumnName(1));
            assertNotNull(inlineAxis);
            assertTrue(inlineAxis.isVisible());
            assertEquals(header, inlineAxis.getBounds(), "The editor sits directly over the axis value");
            inlineAxis.setText("175.25");
            pressKey(inlineAxis, "ENTER");
            assertFalse(inlineAxis.isVisible());
            ConfigurationImage expected = new ConfigurationImage(originalAxes.clone());
            ConfigurationImageGetterSetter.setArrayValues(x, expected, new Double[][]{{175.25, 200.0}});
            assertArrayEquals(expected.getContent(), axisImage.getContent());
            double encodedX = ConfigurationImageGetterSetter.getArrayValues(x, expected)[0][0];
            assertEquals(encodedX, Double.parseDouble(table.getColumnName(1)));
            assertEquals(table.getColumnName(1), table.getColumnModel().getColumn(1).getHeaderValue());
            assertEquals(1, edits.get());
            assertTrue(table.isCellEditable(0, 0));
            table.changeSelection(0, 0, false, false);
            pressKey(table, "F2");
            assertTrue(table.isEditing());
            ((JTextField) table.getEditorComponent()).setText("35.25");
            pressKey((JTextField) table.getEditorComponent(), "ENTER");
            ConfigurationImageGetterSetter.setArrayValues(y, expected, new Double[][]{{10.0}, {35.25}});
            assertArrayEquals(expected.getContent(), axisImage.getContent(), "Top displayed row edits the last Y bin");
            assertEquals(ConfigurationImageGetterSetter.getArrayValues(y, expected)[1][0],
                Double.parseDouble(table.getValueAt(0, 0).toString()));
            assertArrayEquals(originalZ, zImage.getContent());
            assertEquals(2, edits.get());
            byte[] afterEdits = axisImage.getContent().clone();
            for (String invalid : new String[]{"bad", "NaN", "Infinity"}) {
                table.setValueAt(invalid, 0, 0);
                table.getTableHeader().dispatchEvent(new MouseEvent(table.getTableHeader(), MouseEvent.MOUSE_CLICKED,
                    System.currentTimeMillis(), 0, header.x + 5, 10, 2, false, MouseEvent.BUTTON1));
                inlineAxis.setText(invalid);
                pressKey(inlineAxis, "ENTER");
                assertTrue(inlineAxis.isVisible(), "Invalid input stays in the inline editor");
                assertArrayEquals(afterEdits, axisImage.getContent());
                pressKey(inlineAxis, "ESCAPE");
                assertFalse(inlineAxis.isVisible());
            }
            table.setValueAt(table.getValueAt(0, 0), 0, 0);
            assertArrayEquals(afterEdits, axisImage.getContent());
            assertEquals(2, edits.get(), "Invalid and unchanged edits do not notify");
            table.getTableHeader().dispatchEvent(new MouseEvent(table.getTableHeader(), MouseEvent.MOUSE_CLICKED,
                System.currentTimeMillis(), 0, header.x + 5, 10, 2, false, MouseEvent.BUTTON1));
            pressKey(inlineAxis, "TAB");
            assertEquals(table.getTableHeader().getHeaderRect(2), inlineAxis.getBounds());
            inlineAxis.setText("245.25");
            pressKey(inlineAxis, "shift TAB");
            assertEquals(header, inlineAxis.getBounds());
            ConfigurationImageGetterSetter.setArrayValues(x, expected,
                new Double[][]{{encodedX, 245.25}});
            assertArrayEquals(expected.getContent(), axisImage.getContent());
            assertEquals(3, edits.get());
            inlineAxis.setText("999");
            pressKey(inlineAxis, "ESCAPE");
            assertArrayEquals(expected.getContent(), axisImage.getContent(), "Escape cancels the header edit");
            assertTrue(table.isCellSelected(0, 0), "Header editing preserves the grid selection");
            afterEdits = axisImage.getContent().clone();
            table.getTableHeader().dispatchEvent(new MouseEvent(table.getTableHeader(), MouseEvent.MOUSE_CLICKED,
                System.currentTimeMillis(), 0, header.x + 5, 10, 2, false, MouseEvent.BUTTON1));
            table.getColumnModel().getColumn(1).setWidth(110);
            table.getTableHeader().doLayout();
            assertEquals(table.getTableHeader().getHeaderRect(1), inlineAxis.getBounds(),
                "The editor follows column resizing");
            for (FocusListener listener : inlineAxis.getFocusListeners()) {
                listener.focusLost(new FocusEvent(inlineAxis, FocusEvent.FOCUS_LOST));
            }
            assertFalse(inlineAxis.isVisible(), "Leaving the header accepts the edit");
            assertArrayEquals(afterEdits, axisImage.getContent(), "Unchanged focus-loss commit preserves the image");
            table.getTableHeader().dispatchEvent(new MouseEvent(table.getTableHeader(), MouseEvent.MOUSE_CLICKED,
                System.currentTimeMillis(), 0, header.x + 5, 10, 2, false, MouseEvent.BUTTON1));
            inlineAxis.setText("999");
            view.displayTable(ini, "table", zImage, axisImage);
            assertFalse(inlineAxis.isVisible(), "Loading another table cancels its old header editor");
            assertArrayEquals(afterEdits, axisImage.getContent());
            TuningTableView readOnly = new TuningTableView("Comparison", true);
            readOnly.displayTable(ini, "table", zImage, axisImage);
            JTable comparison = findTable(readOnly.getContent());
            assertFalse(comparison.isCellEditable(0, 0));
            comparison.setValueAt("70", 0, 0);
            comparison.getTableHeader().setSize(300, 30);
            comparison.getTableHeader().dispatchEvent(new MouseEvent(comparison.getTableHeader(), MouseEvent.MOUSE_CLICKED,
                System.currentTimeMillis(), 0, header.x + 5, 10, 2, false, MouseEvent.BUTTON1));
            assertArrayEquals(afterEdits, axisImage.getContent());
            assertFalse(findTextField(comparison.getTableHeader(), "").isVisible());
            assertEquals(0, headerDialogs.get());
        });
    }

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

            pressKey(table, "F2");
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

            pressKey(table, "F2");
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
            pressKey(table, "F2");
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
                pressKey(table, "F2");
                assertFalse(table.isEditing());
                if (viewMode) {
                    table.changeSelection(0, 1, false, false);
                    pressKey(table, "F2");
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
                wheel(table, 1, java.awt.event.InputEvent.CTRL_DOWN_MASK | java.awt.event.InputEvent.SHIFT_DOWN_MASK);
                assertTrue(table.isCellSelected(1, 2), "Ctrl takes precedence over Shift");
                wheel(table, -1);

                if (!viewMode) {
                    pressKey(table, "F2");
                    JTextField editor = (JTextField) table.getEditorComponent();
                    editor.setText("99");
                    wheel(table, 1);
                    wheel(table, 1, 0);
                    assertTrue(table.isEditing());
                    assertTrue(table.isCellSelected(0, 2));
                    assertEquals("99", editor.getText());
                    pressKey(editor, "ESCAPE");
                }
                if (viewMode) {
                    wheel(table, 1, 0);
                    wheel(table, -1, java.awt.event.InputEvent.SHIFT_DOWN_MASK);
                    assertTrue(table.isCellSelected(0, 2));
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
        wheel(table, rotation, java.awt.event.InputEvent.CTRL_DOWN_MASK);
    }

    private void wheel(JTable table, int rotation, int modifiers) {
        MouseWheelEvent event = new MouseWheelEvent(table, MouseEvent.MOUSE_WHEEL,
            System.currentTimeMillis(), modifiers, 10, 10, 0, false,
            MouseWheelEvent.WHEEL_UNIT_SCROLL, 3, rotation);
        table.dispatchEvent(event);
        assertTrue(event.isConsumed());
    }

    private TuningTableView editableView(boolean viewMode, ConfigurationImage image) {
        return editableView(viewMode, image,
            new ArrayIniField("zBins", 0, FieldType.UINT8, 2, 2, "", 1, "0", "255", "0"));
    }

    private TuningTableView editableView(boolean viewMode, ConfigurationImage image, ArrayIniField field) {
        TuningTableView view = new TuningTableView("Test", viewMode);
        IniFileModel iniFile = mock(IniFileModel.class);
        TableModel tableModel = mock(TableModel.class);
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
