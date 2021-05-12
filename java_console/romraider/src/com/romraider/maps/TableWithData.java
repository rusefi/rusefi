package com.romraider.maps;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.xml.RomAttributeParser;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyListener;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/20/2015.
 */
public abstract class TableWithData extends Table {
    public void setData(DataCell[] data) {
        this.data = data;
    }

    public DataCell getDataCell(int location) {
        return data[location];
    }

    public void horizontalInterpolate() {
        int[] coords = {getDataSize(), 0};
        DataCell[] tableData = getData();

        int y;
        for (y = 0; y < getDataSize(); y++) {
            if (tableData[y].isSelected()) {
                if (y < coords[0])
                    coords[0] = y;
                if (y > coords[1])
                    coords[1] = y;
            }
        }
        if (coords[1] - coords[0] > 1) {
            double diff = (tableData[coords[0]].getRealValue() - tableData[coords[1]].getRealValue()) / (coords[1] - coords[0]);
            if (Math.abs(diff) > 0) {
                for (y = coords[0] + 1; y < coords[1]; y++)
                    data[y].setRealValue(String.valueOf(tableData[y - 1].getRealValue() - diff));
            }
        }
    }

    public void selectCellAt(int y) {
        if(y >= 0 && y < data.length) {
            clearSelection();
            data[y].setSelected(true);
            highlightY = y;
            ECUEditorManager.getECUEditor().getTableToolBar().updateTableToolBar(this);
        }
    }

    public Dimension getFrameSize() {
        int height = verticalOverhead + cellHeight;
        int width = horizontalOverhead + data.length * cellWidth;
        if (height < minHeight) {
            height = minHeight;
        }
        int minWidth = isLiveDataSupported() ? minWidthOverlay : minWidthNoOverlay;
        if (width < minWidth) {
            width = minWidth;
        }
        return new Dimension(width, height);
    }

    public void increment(double increment) {
        if (!locked && !(userLevel > getSettings().getUserLevel())) {
            for (DataCell cell : data) {
                if (cell.isSelected()) {
                    cell.increment(increment);
                }
            }
        } else if (userLevel > getSettings().getUserLevel()) {
            JOptionPane.showMessageDialog(this, "This table can only be modified by users with a userlevel of \n" +
                            userLevel + " or greater. Click View->User Level to change your userlevel.",
                    "Table cannot be modified",
                    JOptionPane.INFORMATION_MESSAGE);
        }
    }

    public void multiply(double factor) {
        if (!locked && !(userLevel > getSettings().getUserLevel())) {
            for (DataCell cell : data) {
                if (cell.isSelected()) {
                    cell.multiply(factor);
                }
            }
        } else if (userLevel > getSettings().getUserLevel()) {
            JOptionPane.showMessageDialog(this, "This table can only be modified by users with a userlevel of \n" +
                            userLevel + " or greater. Click View->User Level to change your userlevel.",
                    "Table cannot be modified",
                    JOptionPane.INFORMATION_MESSAGE);
        }
    }


    @Override
    public void addKeyListener(KeyListener listener) {
        super.addKeyListener(listener);
        for (DataCell cell : data) {
            for (int z = 0; z < storageType; z++) {
                cell.addKeyListener(listener);
            }
        }
    }

    public void drawTable() {
        for(DataCell cell : data) {
            if(null != cell) {
                cell.drawCell();
            }
        }
    }


    public byte[] saveFile(byte[] binData) {
        if (userLevel <= getSettings().getUserLevel() && (userLevel < 5 || getSettings().isSaveDebugTables()) ) {
            for (int i = 0; i < data.length; i++) {
                // determine output byte values
                byte[] output;
                if (storageType != Settings.STORAGE_TYPE_FLOAT) {
                    // convert byte values
                    output = RomAttributeParser.parseIntegerValue((int) data[i].getBinValue(), endian, storageType);
                    for (int z = 0; z < storageType; z++) { // insert into file
                        binData[i * storageType + z + getStorageAddress() - ramOffset] = output[z];
                    }

                } else { // float
                    // convert byte values
                    output = RomAttributeParser.floatToByte((float) data[i].getBinValue(), endian);
                    for (int z = 0; z < 4; z++) { // insert in to file
                        binData[i * 4 + z + getStorageAddress() - ramOffset] = output[z];
                    }
                }
            }
        }
        return binData;
    }

    public void populateTable(byte[] input, int romRamOffset) {
        // temporarily remove lock
        boolean tempLock = locked;
        locked = false;

        if (!beforeRam) {
            this.ramOffset = romRamOffset;
        }

        for (int i = 0; i < data.length; i++) {
            if (data[i] == null) {
                double dataValue = 0.0;

                // populate data cells
                if (storageType == Settings.STORAGE_TYPE_FLOAT) { //float storage type
                    byte[] byteValue = new byte[4];
                    byteValue[0] = input[getStorageAddress() + i * 4 - ramOffset];
                    byteValue[1] = input[getStorageAddress() + i * 4 - ramOffset + 1];
                    byteValue[2] = input[getStorageAddress() + i * 4 - ramOffset + 2];
                    byteValue[3] = input[getStorageAddress() + i * 4 - ramOffset + 3];
                    dataValue = RomAttributeParser.byteToFloat(byteValue, endian);

                } else { // integer storage type
                    dataValue = RomAttributeParser.parseByteValue(input,
                            endian,
                            getStorageAddress() + i * storageType - ramOffset,
                            storageType,
                            signed);
                }

                data[i] = new DataCell(this, dataValue, 0, i);
                data[i].setPreferredSize(new Dimension(cellWidth, cellHeight));
                centerPanel.add(data[i]);

                // show locked cell
                if (tempLock) {
                    data[i].setForeground(Color.GRAY);
                }
            }
        }

        // reset locked status
        locked = tempLock;
        calcCellRanges();
    }
}
