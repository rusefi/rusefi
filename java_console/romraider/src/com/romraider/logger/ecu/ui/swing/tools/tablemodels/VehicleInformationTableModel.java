/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2013 RomRaider.com
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

package com.romraider.logger.ecu.ui.swing.tools.tablemodels;

import java.util.Map;

import javax.swing.table.DefaultTableModel;

public final class VehicleInformationTableModel extends DefaultTableModel {
    private static final long serialVersionUID = 9214968582253321667L;
    private Map<String, Object> vehicleInfo;

    @Override
    public final int getColumnCount() {
        return vehicleInfo.keySet().size();
    }

    @Override
    public final String getColumnName(int column) {
        return (String) vehicleInfo.keySet().toArray()[column];
    }

    @Override
    public final Object getValueAt(int row, int column) {
        if (vehicleInfo != null) {
            return vehicleInfo.get(getColumnName(column));
        }
        else {
            return null;
        }
    }

    @Override
    public final int getRowCount() {
        return (vehicleInfo != null) ? 1 : 0;
    }

    @Override
    public final Class<? extends Object> getColumnClass(int column) {
        return getValueAt(0, column).getClass();
    }

    @Override
    public final boolean isCellEditable(int row, int column) {
        return false;
    }

    public final void setVehicleInfo(Map<String, Object> vehicleInfo) {
        this.vehicleInfo = vehicleInfo;
    }
}
