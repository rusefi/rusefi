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

import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.DataRegistrationBroker;
import static java.util.Collections.synchronizedList;
import static java.util.Collections.synchronizedMap;
import javax.swing.table.AbstractTableModel;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public final class ParameterListTableModel extends AbstractTableModel {
    private static final long serialVersionUID = -2556400867696538881L;
    private final String[] columnNames;
    private final List<LoggerData> registeredLoggerData = synchronizedList(new LinkedList<LoggerData>());
    private final Map<LoggerData, ParameterRow> paramRowMap = synchronizedMap(new LinkedHashMap<LoggerData, ParameterRow>());
    private final DataRegistrationBroker broker;

    public ParameterListTableModel(DataRegistrationBroker broker, String dataType) {
        this.broker = broker;
        columnNames = new String[]{"Selected?", dataType, "Units"};
    }

    public synchronized int getRowCount() {
        return paramRowMap.size();
    }

    public int getColumnCount() {
        return columnNames.length;
    }

    public String getColumnName(int col) {
        return columnNames[col];
    }

    public boolean isCellEditable(int row, int col) {
        return col == 0 || col == 2;
    }

    public synchronized Object getValueAt(int row, int col) {
        ParameterRow paramRow = paramRowMap.get(registeredLoggerData.get(row));
        switch (col) {
            case 0:
                return paramRow.isSelected();
            case 1:
                return paramRow.getLoggerData().getName();
            case 2:
                LoggerData loggerData = paramRow.getLoggerData();
                return loggerData.getConvertors().length > 1 ? loggerData : loggerData.getSelectedConvertor().getUnits();
            default:
                return "Error!";
        }
    }

    public synchronized void setValueAt(Object value, int row, int col) {
        ParameterRow paramRow = paramRowMap.get(registeredLoggerData.get(row));
        if (col == 0 && paramRow != null) {
            Boolean selected = (Boolean) value;
            setSelected(paramRow, selected);
            fireTableRowsUpdated(row, row);
        }
    }

    public Class<?> getColumnClass(int col) {
        return getValueAt(0, col).getClass();
    }

    public synchronized void addParam(LoggerData loggerData, boolean selected) {
        if (!registeredLoggerData.contains(loggerData)) {
            ParameterRow paramRow = new ParameterRow(loggerData);
            paramRowMap.put(loggerData, paramRow);
            registeredLoggerData.add(loggerData);
            setSelected(paramRow, selected);
            fireTableDataChanged();
        }
    }

    public synchronized void selectParam(LoggerData loggerData, boolean selected) {
        if (registeredLoggerData.contains(loggerData)) {
            setSelected(paramRowMap.get(loggerData), selected);
            fireTableDataChanged();
        }
    }

    public synchronized void clear() {
        broker.clear();
        paramRowMap.clear();
        registeredLoggerData.clear();
        try {
            fireTableDataChanged();
        }
        catch (Exception e) {
            // Swallow complaints from TableRowSorter when the table is empty
        }
    }

    public List<ParameterRow> getParameterRows() {
        return new ArrayList<ParameterRow>(paramRowMap.values());
    }

    private void setSelected(ParameterRow paramRow, boolean selected) {
        paramRow.setSelected(selected);
        if (selected) broker.registerLoggerDataForLogging(paramRow.getLoggerData());
        else broker.deregisterLoggerDataFromLogging(paramRow.getLoggerData());
    }
}
