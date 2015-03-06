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

package com.romraider.logger.ecu.ui.swing.tools.tablemodels.renderers;

import java.awt.Color;
import java.awt.Component;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.text.ParseException;

import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;

/**
 * This class is used to set the Default Table Cell Renderer for String and
 * Double object types.  The cell will be centred and text coloured according
 * to its value above and below zero. 
 */

public class LtvCellRenderer extends DefaultTableCellRenderer {
    private static final long serialVersionUID = -171010635742251272L;
    private static final DecimalFormat numberFormat =
            new DecimalFormat("0.00;-0.00");

    @Override
    public Component getTableCellRendererComponent(
            JTable table, Object value, boolean isSelected, boolean hasFocus,
            int row, int column) {

        final Component c = super.getTableCellRendererComponent(
                table, value, isSelected, hasFocus, row, column);

        if (c instanceof JLabel) {
            final JLabel cell = (JLabel) c;
            cell.setHorizontalAlignment(JLabel.CENTER);
            cell.setForeground(Color.BLACK);
    
            if (column > 0) {
                if (value instanceof Double) {
                    cell.setText(numberFormat.format(value));
                    setColour(cell, ((Double) value).doubleValue());
    
                }
                if (value instanceof String) {
                    final String data = (String) value;
                    if (data.contains("%")) {
                        final String numText = data.split(" ")[0];
                        double num;
                        try {
                            num = NumberFormat.getNumberInstance()
                                    .parse(numText).doubleValue();
                            setColour(cell, num);
                        } catch (ParseException e) {
                            throw new IllegalArgumentException(e);
                        }
                    }
                }
            }
        }
        return c;
    }

    private final void setColour(JLabel cell, double value) {
        if (value < 0) {
            cell.setForeground(Color.RED);
        }
        else if (value > 0) {
            cell.setForeground(Color.BLUE);
        }
    }
}
