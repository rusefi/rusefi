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

import com.romraider.Settings;
import com.romraider.util.SettingsManager;
import com.rusefi.ConsoleUI;

import javax.naming.NameNotFoundException;
import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.KeyListener;
import java.io.IOException;
import java.util.StringTokenizer;

import static com.romraider.util.ParamChecker.isNullOrEmpty;

/**
 * A better implementation would be a composition of two Table1D - one for axis another one for data
 */

public class Table2D extends TableWithData {
    public static final String TYPE_2D = "2D";
    private static final long serialVersionUID = -7684570967109324784L;
    private Table1D axis = new Table1D();
    private JLabel axisLabel;

    private CopySelection2DWorker copySelection2DWorker;

    public Table2D() {
        verticalOverhead += 18;
    }

    public Table1D getAxis() {
        return axis;
    }

    public void setAxis(Table1D axis) {
        this.axis = axis;
        axis.setAxisParent(this);
    }

    @Override
    public String toString() {
        return super.toString() + " (2D)";// + axis;
    }

    @Override
    public void populateCompareValues(Table otherTable) {
        if(null == otherTable || !(otherTable instanceof Table2D)) {
            return;
        }

        Table2D compareTable2D = (Table2D) otherTable;
        if(data.length != compareTable2D.data.length ||
                axis.data.length != compareTable2D.axis.data.length) {
            return;
        }

        super.populateCompareValues(otherTable);
        axis.populateCompareValues(compareTable2D.getAxis());
    }

    @Override
    public void refreshCompare() {
        populateCompareValues(getCompareTable());
        axis.refreshCompare();
    }

    @Override
    public StringBuffer getTableAsString() {
        StringBuffer output = new StringBuffer(Settings.BLANK);
        output.append(axis.getTableAsString());
        output.append(Settings.NEW_LINE);
        output.append(super.getTableAsString());
        return output;
    }

    @Override
    public Dimension getFrameSize() {
        int height = verticalOverhead + cellHeight * 2;
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

    @Override
    public void populateTable(byte[] input, int romRamOffset) throws IndexOutOfBoundsException {
        centerLayout.setRows(2);
        centerLayout.setColumns(this.getDataSize());

        axis.populateTable(input, romRamOffset);
        super.populateTable(input, romRamOffset);

        // add to table
        for (int i = 0; i < this.getDataSize(); i++) {
            centerPanel.add(axis.getDataCell(i));
        }
        if (flip) {
            for (int i = this.getDataSize() - 1; i >= 0; i--) {
                centerPanel.add(this.getDataCell(i));
            }
        } else {
            for (int i = 0; i < this.getDataSize(); i++) {
                centerPanel.add(this.getDataCell(i));
            }
        }

        if(null == axis.getName() || axis.getName().isEmpty() || "" == axis.getName()) {
            ;// Do not add label.
        } else if(null == axis.getCurrentScale() || "0x" == axis.getCurrentScale().getUnit()) {
            // static or no scale exists.
            axisLabel = new JLabel(axis.getName(), JLabel.CENTER);
            add(axisLabel, BorderLayout.NORTH);
        } else {
            axisLabel = new JLabel(axis.getName() + " (" + axis.getCurrentScale().getUnit() + ")", JLabel.CENTER);
            add(axisLabel, BorderLayout.NORTH);
        }

        tableLabel = new JLabel(getCurrentScale().getUnit(), JLabel.CENTER);
        add(tableLabel, BorderLayout.SOUTH);
        repaint();
    }

    @Override
    public void updateTableLabel() {
        if(null == axis.getName() || axis.getName().length() < 1 || "" == axis.getName()) {
            ;// Do not update label.
        } else if(null == axis.getCurrentScale() || "0x" == axis.getCurrentScale().getUnit()) {
            // static or no scale exists.
            axisLabel.setText(axis.getName());
        } else {
            axisLabel.setText(axis.getName() + " (" + axis.getCurrentScale().getUnit() + ")");
        }

        tableLabel.setText(getCurrentScale().getUnit());
    }

    @Override
    public void clearSelection() {
        axis.clearSelectedData();
        clearSelectedData();
    }

    @Override
    public void setRevertPoint() {
        super.setRevertPoint();
        axis.setRevertPoint();
    }

    @Override
    public void undoAll() {
        super.undoAll();
        axis.undoAll();
    }

    @Override
    public byte[] saveFile(byte[] binData) {
        binData = super.saveFile(binData);
        binData = axis.saveFile(binData);
        return binData;
    }

    @Override
    public void addKeyListener(KeyListener listener) {
        super.addKeyListener(listener);
        axis.addKeyListener(listener);
    }

    @Override
    public void cursorUp() {
        if (data[highlightY].isSelected()) {
            axis.selectCellAt(highlightY);
        }
    }

    @Override
    public void drawTable() {
        super.drawTable();
        axis.drawTable();
    }

    @Override
    public void cursorDown() {
        axis.cursorDown();
    }

    @Override
    public void cursorLeft() {
        if (highlightY > 0 && data[highlightY].isSelected()) {
            selectCellAt(highlightY - 1);
        } else {
            axis.cursorLeft();
        }
    }

    @Override
    public void cursorRight() {
        if (highlightY < data.length - 1 && data[highlightY].isSelected()) {
            selectCellAt(highlightY + 1);
        } else {
            axis.cursorRight();
        }
    }

    @Override
    public void startHighlight(int x, int y) {
        axis.clearSelectedData();
        super.startHighlight(x, y);
    }

    @Override
    public void copySelection() {
        Window ancestorWindow = SwingUtilities.getWindowAncestor(this);
        if(null != ancestorWindow) {
            ancestorWindow.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        }
        ConsoleUI.getFrame().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        super.copySelection();
        copySelection2DWorker = new CopySelection2DWorker(this);
        copySelection2DWorker.execute();
    }

    @Override
    public void copyTable() {
        Window ancestorWindow = SwingUtilities.getWindowAncestor(this);
        if(null != ancestorWindow) {
            ancestorWindow.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        }
        ConsoleUI.getFrame().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        CopyTable2DWorker copyTable2DWorker = new CopyTable2DWorker(this);
        copyTable2DWorker.execute();
    }

    @Override
    public void paste() {
        StringTokenizer st = new StringTokenizer("");
        String input = "";
        try {
            input = (String) Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null).getTransferData(DataFlavor.stringFlavor);
            st = new StringTokenizer(input);
        } catch (UnsupportedFlavorException | IOException ex) { /* wrong paste type -- do nothing */
        }

        String pasteType = st.nextToken();

        if (pasteType.equalsIgnoreCase("[Table2D]")) { // Paste table
            String axisValues = "[Table1D]" + Settings.NEW_LINE + st.nextToken(Settings.NEW_LINE);
            String dataValues = "[Table1D]" + Settings.NEW_LINE + st.nextToken(Settings.NEW_LINE);

            // put axis in clipboard and paste
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(axisValues), null);
            axis.paste();
            // put datavalues in clipboard and paste
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(dataValues), null);
            super.paste();
            // reset clipboard
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(input), null);

        } else if (pasteType.equalsIgnoreCase("[Selection1D]")) { // paste selection
            if (data[highlightY].isSelected()) {
                super.paste();
            } else {
                axis.paste();
            }
        }
    }

    @Override
    public void interpolate() {
        super.interpolate();
        this.getAxis().interpolate();
    }

    @Override
    public void verticalInterpolate() {
        super.verticalInterpolate();
        this.getAxis().verticalInterpolate();
    }

    @Override
    public void horizontalInterpolate() {
        super.horizontalInterpolate();
        this.getAxis().horizontalInterpolate();
    }

    @Override
    public boolean isLiveDataSupported() {
        return !isNullOrEmpty(axis.getLogParam());
    }

    @Override
    public boolean isButtonSelected() {
        return true;
    }

    @Override
    public void clearLiveDataTrace() {
        super.clearLiveDataTrace();
        axis.clearLiveDataTrace();
    }

    @Override
    public void updateLiveDataHighlight() {
        if (getOverlayLog()) {
            data[axis.getLiveDataIndex()].setLiveDataTrace(true);
        }
    }

    @Override
    public String getLogParamString() {
        StringBuilder sb = new StringBuilder();
        sb.append(axis.getLogParamString()+ ", ");
        sb.append(getName()+ ":" + getLogParam());
        return sb.toString();
    }

    @Override
    public void setOverlayLog(boolean overlayLog) {
        super.setOverlayLog(overlayLog);
        axis.setOverlayLog(overlayLog);
        if (overlayLog) {
            axis.clearLiveDataTrace();
        }
    }

    @Override
    public void setCompareDisplay(int compareDisplay) {
        super.setCompareDisplay(compareDisplay);
        axis.setCompareDisplay(compareDisplay);
    }

    @Override
    public void setCompareValueType(int compareValueType) {
        super.setCompareValueType(compareValueType);
        axis.setCompareValueType(compareValueType);
    }

    @Override
    public void setCurrentScale(Scale curScale) {
        if(SettingsManager.getSettings().isScaleHeadersAndData() && !axis.isStaticDataTable()) {
            try {
                this.axis.setScaleByName(curScale.getName());
            } catch (NameNotFoundException e) {
                try {
                    this.axis.setScaleByName(SettingsManager.getSettings().getDefaultScale());
                } catch (NameNotFoundException e1) {
                }
            }
        }
        this.curScale = curScale;
        updateTableLabel();
        drawTable();
    }

    @Override
    public boolean equals(Object other) {
        try {
            if(null == other) {
                return false;
            }

            if(other == this) {
                return true;
            }

            if(!(other instanceof Table2D)) {
                return false;
            }

            Table2D otherTable = (Table2D)other;

            if( (null == this.getName() && null == otherTable.getName())
                    || (this.getName().isEmpty() && otherTable.getName().isEmpty()) ) {
                ;// Skip name compare if name is null or empty.
            } else if (!this.getName().equalsIgnoreCase(otherTable.getName())) {
                return false;
            }

            if(!this.axis.equals(otherTable.axis)) {
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
            for(int i = 0 ; i < this.data.length ; i++) {
                if(! this.data[i].equals(otherTable.data[i])) {
                    return false;
                }
            }

            return true;
        } catch(Exception ex) {
            // TODO: Log Exception.
            return false;
        }
    }

    @Override
    public void repaint() {
        super.repaint();
        if(null != axis) {
            axis.repaint();
        }
    }
}

