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

package com.romraider.swing;

import javax.swing.tree.DefaultMutableTreeNode;

import com.romraider.maps.Table;

public class TableTreeNode extends DefaultMutableTreeNode {

    private static final long serialVersionUID = 2824050968863990871L;
    private String toolTip;

    public TableTreeNode(TableFrame tableFrame) {
        super(tableFrame);
    }

    public TableFrame getFrame() {
        if(getUserObject() instanceof TableFrame) {
            return (TableFrame)getUserObject();
        }
        return null;
    }

    public void setFrame(TableFrame tableFrame) {
        this.setUserObject(tableFrame);
    }

    public Table getTable() {
        return this.getFrame().getTable();
    }

    public void setToolTipText(String input) {
        toolTip = input;
    }

    public String getToolTipText() {
        return toolTip;
    }
}