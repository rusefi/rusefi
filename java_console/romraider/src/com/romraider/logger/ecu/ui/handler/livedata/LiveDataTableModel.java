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

package com.romraider.logger.ecu.ui.handler.livedata;

import com.romraider.logger.ecu.definition.LoggerData;
import static java.util.Collections.synchronizedList;
import static java.util.Collections.synchronizedMap;
import javax.swing.table.AbstractTableModel;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public final class LiveDataTableModel extends AbstractTableModel {
    private static final long serialVersionUID = 3712433453224086342L;
    private final String[] columnNames = {"Logger Data", "Min Value", "Current Value", "Max Value", "Units"};
    private final List<LoggerData> registeredLoggerData = synchronizedList(new LinkedList<LoggerData>());
    private final Map<LoggerData, LiveDataRow> dataRowMap = synchronizedMap(new LinkedHashMap<LoggerData, LiveDataRow>());

    public synchronized int getRowCount() {
        return dataRowMap.size();
    }

    public int getColumnCount() {
        return columnNames.length;
    }

    public String getColumnName(int col) {
        return columnNames[col];
    }

    public boolean isCellEditable(int row, int col) {
        return false;
    }

    public synchronized Object getValueAt(int row, int col) {
        LiveDataRow dataRow = dataRowMap.get(registeredLoggerData.get(row));
        switch (col) {
            case 0:
                return dataRow.getName();
            case 1:
                return dataRow.getMinValue();
            case 2:
                return dataRow.getCurrentValue();
            case 3:
                return dataRow.getMaxValue();
            case 4:
                return dataRow.getUnits();
            default:
                return "Error!";
        }
    }

    public synchronized void addParam(LoggerData loggerData) {
        if (!registeredLoggerData.contains(loggerData)) {
            dataRowMap.put(loggerData, new LiveDataRow(loggerData));
            registeredLoggerData.add(loggerData);
            fireTableDataChanged();
        }
    }

    public synchronized void removeParam(LoggerData loggerData) {
        registeredLoggerData.remove(loggerData);
        dataRowMap.remove(loggerData);
        fireTableDataChanged();
    }

    public synchronized void updateParam(LoggerData loggerData, double value) {
        LiveDataRow dataRow = dataRowMap.get(loggerData);
        if (dataRow != null) {
            dataRow.updateValue(value);
            int index = registeredLoggerData.indexOf(loggerData);
            fireTableRowsUpdated(index, index);
        }
    }

    public synchronized void reset() {
        for (LiveDataRow liveDataRow : dataRowMap.values()) {
            liveDataRow.reset();
        }
        fireTableDataChanged();
    }

    public synchronized void resetRow(LoggerData loggerData) {
        LiveDataRow liveDataRow = dataRowMap.get(loggerData);
        if (liveDataRow != null) {
            liveDataRow.reset();
            fireTableDataChanged();
        }
    }
}
