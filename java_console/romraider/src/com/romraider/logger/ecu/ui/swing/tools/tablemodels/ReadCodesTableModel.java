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

import java.util.ArrayList;

import javax.swing.table.DefaultTableModel;

import com.romraider.logger.ecu.comms.query.EcuQuery;

public final class ReadCodesTableModel extends DefaultTableModel {
    private static final long serialVersionUID = -4229633011594395331L;
    private ArrayList<EcuQuery> dtcSet;
    
    public final int getColumnCount() {
        return 3;
    }
    
    public final String getColumnName(int column) {
        switch (column) {
            case 0:     return "Diagnostic Code Name";
            case 1:     return "Temporary";
            case 2:     return "Memorized";
            default:    return "";
        }
    }
    
    public final Object getValueAt(int row, int column) {
        if (dtcSet != null) {
            final double result = dtcSet.get(row).getResponse();
            switch (column) {
                case 0:
                        return " " + dtcSet.get(row).getLoggerData().getName();
                case 1: 
                        return (result == 1 || result == 3)
                        ? new Boolean(true)
                        : new Boolean(false);
                case 2: 
                        return (result == 2 || result == 3)
                        ? new Boolean(true)
                        : new Boolean(false);
                default:
                        return null;
            }
        }
        else {
            return null;
        }
    }
    
    public final int getRowCount() {
        return (dtcSet != null) ? dtcSet.size() : 0;
    }
    
    public final Class<? extends Object> getColumnClass(int column) {
        return getValueAt(0, column).getClass();
    }

    public final boolean isCellEditable(int row, int column) {
        return false;
    }
    
    public final void setDtcList(ArrayList<EcuQuery> dtcSet) {
        this.dtcSet = dtcSet;
    }
}
