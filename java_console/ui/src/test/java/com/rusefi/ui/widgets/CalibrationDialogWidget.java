package com.rusefi.ui.widgets;

import com.opensr5.ini.DialogModel;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;

public class CalibrationDialogWidget {
    private final JPanel contentPane = new JPanel(new BorderLayout());
    private final DefaultTableModel tableModel = new DefaultTableModel(new Object[]{"Key", "Name"}, 0);
    private final JTable table = new JTable(tableModel);

    public CalibrationDialogWidget() {
        contentPane.add(new JScrollPane(table), BorderLayout.CENTER);
    }

    public void update(DialogModel dialogModel) {
        tableModel.setRowCount(0);
        if (dialogModel != null) {
            for (DialogModel.Field field : dialogModel.getFields()) {
                tableModel.addRow(new Object[]{field.getKey(), field.getUiName()});
            }
        }
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
