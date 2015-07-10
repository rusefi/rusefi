/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2014 RomRaider.com
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

package com.romraider.swing;

import java.util.Map;

import javax.swing.table.DefaultTableModel;

import com.romraider.util.HexUtil;

public final class SwitchStateTableModel extends DefaultTableModel {
    private static final long serialVersionUID = -6053559125596735977L;
    private static final String[] colNames = {
        "Switch State", "State Data"};
    private Map<String, byte[]> switchStates;
    
    @Override
    public final int getColumnCount() {
        return colNames.length;
    }
    
    @Override
    public final String getColumnName(int column) {
        return colNames[column];
    }
    
    @Override
    public final Object getValueAt(int row, int column) {
        if (null != switchStates) {
            final String key = (String) switchStates.keySet().toArray()[row];
            final byte[] value = switchStates.get(key);
            switch (column) {
                case 0:
                        return key;
                case 1: 
                        return HexUtil.asHex(value);
                default:
                        return null;
            }
        }
        else {
            return null;
        }
    }
    
    @Override
    public final int getRowCount() {
        return (null != switchStates) ?  switchStates.size() : 0;
    }
    
    @Override
    public final Class<? extends Object> getColumnClass(int column) {
        return getValueAt(0, column).getClass();
    }

    @Override
    public final boolean isCellEditable(int row, int column) {
        return false;
    }
    
    public final void setScalesList(Map<String, byte[]> switchStates) {
        this.switchStates = switchStates;
    }
}
