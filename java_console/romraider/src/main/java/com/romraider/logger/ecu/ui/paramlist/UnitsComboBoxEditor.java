/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.logger.ecu.ui.paramlist;

import com.romraider.logger.ecu.definition.EcuDataConvertor;
import com.romraider.logger.ecu.definition.LoggerData;

import javax.swing.AbstractCellEditor;
import javax.swing.JComboBox;
import javax.swing.JTable;
import javax.swing.table.TableCellEditor;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public final class UnitsComboBoxEditor extends AbstractCellEditor implements TableCellEditor, ActionListener {
    private static final long serialVersionUID = -3472910399604360821L;
    private static final String EDIT_COMMAND = "EDIT";
    private LoggerData currentEcuData;

    public Object getCellEditorValue() {
        return currentEcuData.getSelectedConvertor();
    }

    public Component getTableCellEditorComponent(JTable table, Object ecuData, boolean isSelected, int row, int column) {
        currentEcuData = (LoggerData) ecuData;
        EcuDataConvertor[] convertors = currentEcuData.getConvertors();
        JComboBox comboBox = new JComboBox();
        for (EcuDataConvertor convertor : convertors) {
            comboBox.addItem(convertor);
        }
        comboBox.setSelectedItem(currentEcuData.getSelectedConvertor());
        comboBox.setEditable(false);
        comboBox.setEnabled(true);
        comboBox.setActionCommand(EDIT_COMMAND);
        comboBox.addActionListener(this);
        return comboBox;
    }

    public void actionPerformed(ActionEvent actionEvent) {
        if (EDIT_COMMAND.equals(actionEvent.getActionCommand())) {
            Object source = actionEvent.getSource();
            if (source != null && JComboBox.class.isAssignableFrom(source.getClass())) {
                JComboBox comboBox = (JComboBox) source;
                currentEcuData.selectConvertor((EcuDataConvertor) comboBox.getSelectedItem());
                fireEditingStopped();
            }
        }
    }
}
