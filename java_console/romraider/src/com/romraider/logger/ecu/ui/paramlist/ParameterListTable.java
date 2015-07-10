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

import com.romraider.logger.ecu.definition.EcuData;
import com.romraider.logger.ecu.definition.ExternalData;

import static com.romraider.util.ParamChecker.isNullOrEmpty;
import javax.swing.JTable;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableRowSorter;

import java.awt.event.MouseEvent;
import java.util.List;

public final class ParameterListTable extends JTable {
    private static final long serialVersionUID = -8489190548281346227L;
    private UnitsComboBoxEditor comboBoxEditor = new UnitsComboBoxEditor();
    private UnitsComboBoxRenderer comboBoxRenderer = new UnitsComboBoxRenderer();
    private final ParameterListTableModel tableModel;

    public ParameterListTable(ParameterListTableModel tableModel) {
        super(tableModel);
        this.tableModel = tableModel;
        this.getTableHeader().setReorderingAllowed(false);
        for (int column = 0; column < tableModel.getColumnCount(); column++) {
            if (tableModel.getColumnName(2).equalsIgnoreCase("units")) {
                setColumnSortable(column, false);
            }
            else {
                setColumnSortable(column, true);
            }
        }
    }

    public TableCellRenderer getCellRenderer(int row, int col) {
        return displayComboBox(row, col) ? comboBoxRenderer : super.getCellRenderer(row, col);
    }

    public TableCellEditor getCellEditor(int row, int col) {
        return displayComboBox(row, col) ? comboBoxEditor : super.getCellEditor(row, col);
    }

    public String getToolTipText(MouseEvent mouseEvent) {
        List<ParameterRow> parameterRows = tableModel.getParameterRows();
        if (!isNullOrEmpty(parameterRows)) {
            ParameterRow parameterRow = parameterRows.get(rowAtPoint(mouseEvent.getPoint()));
            if (parameterRow != null) {
                String description = parameterRow.getLoggerData().getDescription();
                if (!isNullOrEmpty(description)) {
                    return description;
                }
            }
        }
        return super.getToolTipText(mouseEvent);
    }

    private boolean displayComboBox(int row, int col) {
        Object value = getValueAt(row, col);
        if (EcuData.class.isAssignableFrom(value.getClass())) {
            EcuData ecuData = (EcuData) value;
            if (ecuData.getConvertors().length > 1)
                return true;
        }
        if (ExternalData.class.isAssignableFrom(value.getClass())) {
            ExternalData externalData = (ExternalData) value;
            if (externalData.getConvertors().length > 1)
                return true;
        }
        return false;
    }

    private void setColumnSortable(int column, boolean state) {
        TableRowSorter<ParameterListTableModel> sorter =
            new TableRowSorter<ParameterListTableModel>(tableModel);
        sorter.setSortable(column, state);
        setRowSorter(sorter);
    }
}
