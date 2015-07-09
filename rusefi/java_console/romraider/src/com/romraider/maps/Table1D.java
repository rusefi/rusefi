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

package com.romraider.maps;

import java.awt.BorderLayout;

import javax.swing.JLabel;

import com.romraider.Settings;

public class Table1D extends TableWithData {
    public static final String TYPE_1D = "1D";
    public static final String TYPE_X_AXIS = "X Axis";
    public static final String TYPE_Y_AXIS = "Y Axis";
    private static final long serialVersionUID = -8747180767803835631L;
    private Table axisParent = null;

    public void setAxisParent(Table axisParent) {
        this.axisParent = axisParent;
    }

    public Table getAxisParent() {
        return axisParent;
    }

    public void addStaticDataCell(DataCell input) {
        for(int i = 0; i < data.length; i++) {
            if(data[i] == null) {
                data[i] = input;
                data[i].setY(i);
                break;
            }
        }
    }

    @Override
    public void populateTable(byte[] input, int romRamOffset)  {
        centerLayout.setRows(1);
        centerLayout.setColumns(this.getDataSize());

        super.populateTable(input, romRamOffset);

        // add to table
        for (int i = 0; i < this.getDataSize(); i++) {
            centerPanel.add(this.getDataCell(i));
        }

        if(null == name || name.isEmpty()) {
            ;// Do not add label.
        } else if(null == getCurrentScale () || "0x" == getCurrentScale().getUnit()) {
            // static or no scale exists.
            tableLabel = new JLabel(getName(), JLabel.CENTER);
            add(tableLabel, BorderLayout.NORTH);
        } else {
            tableLabel = new JLabel(getName() + " (" + getCurrentScale().getUnit() + ")", JLabel.CENTER);
            add(tableLabel, BorderLayout.NORTH);
        }
    }

    @Override
    public String toString() {
        return super.toString() + " (1D)";
    }

    @Override
    public void cursorUp() {
        if (type == Settings.TABLE_Y_AXIS) {
            if (highlightY > 0 && data[highlightY].isSelected()) {
                selectCellAt(highlightY - 1);
            }
        } else if (type == Settings.TABLE_X_AXIS) {
            // Y axis is on top.. nothing happens
        } else if (type == Settings.TABLE_1D) {
            // no where to move up to
        }
    }

    @Override
    public void cursorDown() {
        if (type == Settings.TABLE_Y_AXIS) {
            if (getAxisParent().getType() == Settings.TABLE_3D) {
                if (highlightY < getDataSize() - 1 && data[highlightY].isSelected()) {
                    selectCellAt(highlightY + 1);
                }
            } else if (getAxisParent().getType() == Settings.TABLE_2D) {
                Table2D axisParent = (Table2D) getAxisParent();
                if (data[highlightY].isSelected()) {
                    axisParent.selectCellAt(highlightY);
                }
            }
        } else if (type == Settings.TABLE_X_AXIS && data[highlightY].isSelected()) {
            ((Table3D) getAxisParent()).selectCellAt(highlightY, this);
        } else if (type == Settings.TABLE_1D) {
            // no where to move down to
        }
    }

    @Override
    public void cursorLeft() {
        if (type == Settings.TABLE_Y_AXIS) {
            // X axis is on left.. nothing happens
            if (getAxisParent().getType() == Settings.TABLE_2D) {
                if (data[highlightY].isSelected()) {
                    selectCellAt(highlightY - 1);
                }
            }
        } else if (type == Settings.TABLE_X_AXIS && data[highlightY].isSelected()) {
            if (highlightY > 0) {
                selectCellAt(highlightY - 1);
            }
        } else if (type == Settings.TABLE_1D && data[highlightY].isSelected()) {
            if (highlightY > 0) {
                selectCellAt(highlightY - 1);
            }
        }
    }

    @Override
    public void cursorRight() {
        if (type == Settings.TABLE_Y_AXIS && data[highlightY].isSelected()) {
            if (getAxisParent().getType() == Settings.TABLE_3D) {
                ((Table3D) getAxisParent()).selectCellAt(highlightY, this);
            } else if (getAxisParent().getType() == Settings.TABLE_2D) {
                selectCellAt(highlightY + 1);
            }
        } else if (type == Settings.TABLE_X_AXIS && data[highlightY].isSelected()) {
            if (highlightY < getDataSize() - 1) {
                selectCellAt(highlightY + 1);
            }
        } else if (type == Settings.TABLE_1D && data[highlightY].isSelected()) {
            if (highlightY < getDataSize() - 1) {
                selectCellAt(highlightY + 1);
            }
        }
    }

    @Override
    public void clearSelection() {
        // Call to the axis parent.  The axis parent should then call to clear this data.
        getAxisParent().clearSelection();
    }

    @Override
    public void startHighlight(int x, int y) {
        Table axisParent = getAxisParent();
        axisParent.clearSelectedData();

        if(axisParent instanceof Table3D) {
            Table3D table3D = (Table3D) axisParent;
            if(getType() == Settings.TABLE_X_AXIS) {
                table3D.getYAxis().clearSelectedData();
            } else if (getType() == Settings.TABLE_Y_AXIS) {
                table3D.getXAxis().clearSelectedData();
            }
        } else if (axisParent instanceof Table2D) {
            ((Table2D) axisParent).getAxis().clearSelectedData();
        }


        super.startHighlight(x, y);
    }

    @Override
    public String getCellAsString(int index) {
        return data[index].getText();
    }

    @Override
    public void highlightLiveData(String liveVal) {
        if (getOverlayLog()) {
            double liveValue = 0.0;
            try{
                liveValue = Double.parseDouble(liveVal);
            } catch(NumberFormatException nex) {
                return;
            }

            int startIdx = data.length;
            for (int i = 0; i < data.length; i++) {
                double currentValue = 0.0;
                if(isStaticDataTable() && null != data[i].getStaticText()) {
                    try {
                        currentValue = Double.parseDouble(data[i].getStaticText());
                    } catch(NumberFormatException nex) {
                        return;
                    }
                } else {
                    currentValue = data[i].getRealValue();
                }

                if (liveValue == currentValue) {
                    startIdx = i;
                    break;
                } else if (liveValue < currentValue){
                    startIdx = i-1;
                    break;
                }
            }

            setLiveDataIndex(startIdx);
            DataCell cell = data[getLiveDataIndex()];
            cell.setLiveDataTrace(true);
            cell.setLiveDataTraceValue(liveVal);
            getToolbar().setLiveDataValue(liveVal);
        }
        getAxisParent().updateLiveDataHighlight();
    }

    @Override
    public boolean isLiveDataSupported() {
        return false;
    }

    @Override
    public boolean isButtonSelected() {
        return true;
    }

    public boolean isAxis() {
        return getType() == Settings.TABLE_X_AXIS ||
                getType() == Settings.TABLE_Y_AXIS || isStaticDataTable();
    }

    @Override
    public boolean equals(Object other) {
    //    try {
            if(null == other) {
                return false;
            }

            if(other == this) {
                return true;
            }

            if(!(other instanceof Table1D)) {
                return false;
            }

            Table1D otherTable = (Table1D)other;

            if(this.isAxis() != otherTable.isAxis()) {
                return false;
            }

            if(this.data.length != otherTable.data.length)
            {
                return false;
            }

            if(this.data.equals(otherTable.data))
            {
                return true;
            }

            // Compare Bin Values
            for(int i=0 ; i < this.data.length ; i++) {
                if(! this.data[i].equals(otherTable.data[i])) {
                    return false;
                }
            }

            return true;
//        } catch(Exception ex) {
//            // TODO: Log Exception.
//            return false;
//        }
    }

    boolean ec = false;

    @Override
    public void clearLiveDataTrace() {
        super.clearLiveDataTrace();
        if (!ec) {
            ec = true;
            getAxisParent().clearLiveDataTrace();
            ec = false;
        }
    }

    @Override
    public void updateTableLabel() {
        this.getAxisParent().updateTableLabel();
    }

    @Override
    public StringBuffer getTableAsString() {
        if(isStaticDataTable()) {
            StringBuffer output = new StringBuffer(Settings.BLANK);
            for (int i = 0; i < data.length; i++) {
                output.append(data[i].getStaticText());
                if (i < data.length - 1) {
                    output.append(Settings.TAB);
                }
            }
            return output;
        } else {
            return super.getTableAsString();
        }
    }
}