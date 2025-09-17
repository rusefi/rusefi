package com.rusefi.ui.table;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.EventObject;
import javax.swing.AbstractCellEditor;
import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.table.TableCellEditor;

public class ButtonEditor extends AbstractCellEditor implements TableCellEditor, ActionListener {

    private final JTable table;
    private final Clicked clicked;
    private final JButton button;
    private String label;
    private boolean isPushed;

    public ButtonEditor(JButton button, String label, JTable table, Clicked clicked) {
        this.button = button;
        this.label = label;
        this.table = table;
        this.clicked = clicked;
        this.button.setOpaque(true);
        this.button.addActionListener(this);
    }

    @Override
    public Component getTableCellEditorComponent(JTable table, Object value,
                                                 boolean isSelected, int row, int column) {
        if (isSelected) {
            button.setForeground(table.getSelectionForeground());
            button.setBackground(table.getSelectionBackground());
        } else {
            button.setForeground(table.getForeground());
            button.setBackground(table.getBackground());
        }
        label = (value == null) ? "" : value.toString();
        button.setText(label);
        isPushed = true;
        return button;
    }

    @Override
    public Object getCellEditorValue() {
        return label;
    }

    @Override
    public boolean stopCellEditing() {
        isPushed = false;
        return super.stopCellEditing();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (isPushed) {
            // Get the selected row and column
            int selectedRow = table.getSelectedRow();
            int selectedCol = table.getSelectedColumn();
            clicked.clicked(selectedRow - 1);
        }
        fireEditingStopped();
    }

    @Override
    public boolean isCellEditable(EventObject anEvent) {
        // The cell should only be editable when a button is clicked
        return true;
    }

    public interface Clicked {
        void clicked(int row);
    }
}
