package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.MouseEvent;
import java.util.Optional;

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
        Double[][] data = {{1.0, 2.55}, {3.0, 4.0}};
        // Precision 2
        TuningTableView.TuningTableModel model = new TuningTableView.TuningTableModel(data, new Double[]{100.0, 200.0}, new Double[]{10.0, 20.0}, 2);
        table.setModel(model);

        // data[1][0] is 3.0, with precision 2 it should be "3.00"
        assertEquals("3.00", table.getValueAt(0, 1));
        // data[1][1] is 4.0, with precision 2 it should be "4.00"
        assertEquals("4.00", table.getValueAt(0, 2));
        // data[0][1] is 2.55, with precision 2 it should be "2.55"
        assertEquals("2.55", table.getValueAt(1, 2));
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
