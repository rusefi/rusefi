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

import javax.swing.JComboBox;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;
import java.awt.Component;

public final class UnitsComboBoxRenderer extends JComboBox implements TableCellRenderer {

    private static final long serialVersionUID = -6288079743431509778L;

    public Component getTableCellRendererComponent(JTable table, Object ecuData, boolean isSelected, boolean hasFocus, int row, int column) {
        LoggerData currentEcuData = (LoggerData) ecuData;
        EcuDataConvertor[] convertors = currentEcuData.getConvertors();
        JComboBox comboBox = new JComboBox();
        for (EcuDataConvertor convertor : convertors) {
            comboBox.addItem(convertor);
        }
        comboBox.setSelectedItem(currentEcuData.getSelectedConvertor());
        return comboBox;
    }
}
