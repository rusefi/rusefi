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

import java.util.Vector;

import javax.swing.table.DefaultTableModel;

import com.romraider.maps.Scale;

public final class ScalesTableModel extends DefaultTableModel {
    private static final long serialVersionUID = -5967359776053559125L;
    private static final String[] colNames = {
            "Name", "Expression", "Byte Expression",
            "Unit", "Format", "Coarse", "Fine", "Min", "Max"};
    private Vector<Scale> scales;
    
    @Override
    public final int getColumnCount() {
        return colNames.length;
    }
    
    @Override
    public final String getColumnName(int column) {
        return colNames[column].toString();
    }
    
    @Override
    public final Object getValueAt(int row, int column) {
        if (null != scales) {
            final Scale scale = scales.get(row);
            switch (column) {
                case 0:
                        return scale.getName();
                case 1: 
                        return scale.getExpression();
                case 2: 
                        return scale.getByteExpression();
                case 3:
                        return scale.getUnit();
                case 4: 
                        return scale.getFormat();
                case 5: 
                        return scale.getCoarseIncrement();
                case 6:
                        return scale.getFineIncrement();
                case 7: 
                        return scale.getMin();
                case 8: 
                        return scale.getMax();
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
        return (null != scales) ? scales.size() : 0;
    }
    
    @Override
    public final Class<? extends Object> getColumnClass(int column) {
        return getValueAt(0, column).getClass();
    }

    @Override
    public final boolean isCellEditable(int row, int column) {
        return false;
    }
    
    public final void setScalesList(Vector<Scale> scales) {
        this.scales = scales;
    }
}
