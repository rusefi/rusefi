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

package com.romraider.logger.ecu.ui.handler.table;

import static com.romraider.util.ParamChecker.isNullOrEmpty;
import static java.util.Collections.synchronizedMap;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;

import com.romraider.logger.ecu.comms.query.Response;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.handler.DataUpdateHandler;
import com.romraider.maps.Table;
import com.romraider.maps.Table2D;
import com.romraider.maps.Table3D;

public final class TableUpdateHandler implements DataUpdateHandler {
    private static final TableUpdateHandler INSTANCE = new TableUpdateHandler();
    private final Map<String, List<Table>> tableMap = synchronizedMap(new HashMap<String, List<Table>>());

    private TableUpdateHandler() {
        tableMap.clear();
    }

    @Override
    public void registerData(LoggerData loggerData) {
    }

    @Override
    public void handleDataUpdate(Response response) {
        for (LoggerData loggerData : response.getData()) {
            List<Table> tables = tableMap.get(loggerData.getId());
            if (tables != null && !tables.isEmpty()) {
                String formattedValue = loggerData.getSelectedConvertor().format(response.getDataValue(loggerData));
                for(ListIterator<Table> item = tables.listIterator(); item.hasNext();) {
                    item.next().highlightLiveData(formattedValue);
                }
            }
        }
    }

    @Override
    public void deregisterData(LoggerData loggerData) {
    }

    @Override
    public void cleanUp() {
    }

    @Override
    public void reset() {
    }

    public void registerTable(Table table) {
        String logParam = table.getLogParam();
        if (!isNullOrEmpty(logParam)) {
            if (!tableMap.containsKey(logParam)) {
                tableMap.put(logParam, new ArrayList<Table>());
            }
            tableMap.get(logParam).add(table);
        }
        registerAxes(table);
    }

    public void deregisterTable(Table table) {
        String logParam = table.getLogParam();
        if (tableMap.containsKey(logParam)) {
            List<Table> tables = tableMap.get(logParam);
            tables.remove(table);
            if (tables.isEmpty()) {
                tableMap.remove(logParam);
            }
        }
        deregisterAxes(table);
    }

    public static TableUpdateHandler getInstance() {
        return INSTANCE;
    }

    private void registerAxes(Table table) {
        if (table instanceof Table2D) {
            registerTable(((Table2D) table).getAxis());
        }
        if (table instanceof Table3D) {
            registerTable(((Table3D) table).getXAxis());
            registerTable(((Table3D) table).getYAxis());
        }
    }

    private void deregisterAxes(Table table) {
        if (table instanceof Table2D) {
            deregisterTable(((Table2D) table).getAxis());
        }
        if (table instanceof Table3D) {
            deregisterTable(((Table3D) table).getXAxis());
            deregisterTable(((Table3D) table).getYAxis());
        }
    }

}
