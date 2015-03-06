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

import java.util.List;

import javax.swing.table.DefaultTableModel;

import com.romraider.logger.ecu.comms.query.EcuQuery;

/**
 * Fine Learning Knock Correction Table Model for Learning Table Values display.
 * <i>Note that column 0 is used as a row header</i>
 */
public final class FineLearningKnockCorrectionTableModel extends DefaultTableModel {
    private static final long serialVersionUID = -7369506358159217982L;
    private List<List<EcuQuery>> flkc;
    private String[] columnNames = new String[]{" ","A","B","C","D","E"};
    private String[] rowNames = new String[]{"1","2","3","4","5","6","7","8"};

    @Override
    public final int getColumnCount() {
        return columnNames.length;
    }

    @Override
    public final String getColumnName(int column) {
        return columnNames[column];
    }

    /**
     * Column 0 is used as a row header
     */
    @Override
    public final Object getValueAt(int row, int column) {
        Object result = null;
        if (flkc != null) {
            result = (column == 0)
                   ? result = rowNames[row]
                   : flkc.get(row).get(column).getResponse();
        }
        return result;
    }

    @Override
    public final int getRowCount() {
        return (flkc != null) ? flkc.size() : 0;
    }

    @Override
    public final Class<? extends Object> getColumnClass(int column) {
        return getValueAt(0, column).getClass();
    }

    @Override
    public final boolean isCellEditable(int row, int column) {
        return false;
    }

    public final void setColumnHeadings(String[] loadRanges) {
        if (loadRanges.length > 0) {
            this.columnNames = loadRanges;
        }
    }

    public final void setRomHeadings(String[] rpmRanges) {
        if (rpmRanges.length > 0) {
            this.rowNames = rpmRanges;
        }
    }

    public final void setFlkcData(List<List<EcuQuery>> flkc) {
        this.flkc = flkc;
    }
}
