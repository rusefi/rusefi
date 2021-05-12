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
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.swing.TableToolBar;
import com.romraider.util.JEPUtil;
import com.romraider.util.SettingsManager;
import com.rusefi.ConsoleUI;

import javax.naming.NameNotFoundException;
import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.io.Serializable;
import java.util.StringTokenizer;
import java.util.Vector;

public abstract class Table extends JPanel implements Serializable {
    private static final long serialVersionUID = 6559256489995552645L;

    protected String name;
    protected int type;
    protected String category = "Other";
    protected String description = Settings.BLANK;
    protected Vector<Scale> scales = new Vector<>();
    protected Scale curScale;

    protected int storageAddress;
    protected int storageType;
    protected boolean signed;
    protected int endian;
    protected boolean flip;
    protected DataCell[] data = new DataCell[0];
    protected boolean beforeRam = false;
    protected int ramOffset = 0;
    protected BorderLayout borderLayout = new BorderLayout();
    protected GridLayout centerLayout = new GridLayout(1, 1, 0, 0);
    protected JPanel centerPanel = new JPanel(centerLayout);
    protected JLabel tableLabel;
    protected int verticalOverhead = 103;
    protected int horizontalOverhead = 2;
    protected int cellHeight = (int) getSettings().getCellSize().getHeight();
    protected int cellWidth = (int) getSettings().getCellSize().getWidth();
    protected int minHeight = 100;
    protected int minWidthNoOverlay = 465;
    protected int minWidthOverlay = 700;
    protected int highlightX;
    protected int highlightY;
    protected boolean highlight = false;
    protected int userLevel = 0;
    protected boolean locked = false;

    protected String logParam = Settings.BLANK;
    protected boolean overlayLog = true;

    protected CopyTableWorker copyTableWorker;
    protected CopySelectionWorker copySelectionWorker;

    protected double minAllowedBin = 0.0;
    protected double maxAllowedBin = 0.0;

    protected double maxBin;
    protected double minBin;

    protected double maxCompare = 0.0;
    protected double minCompare = 0.0;

    protected int compareDisplay = Settings.COMPARE_DISPLAY_ABSOLUTE;
    protected int compareValueType = Settings.DATA_TYPE_BIN;

    protected boolean staticDataTable = false;
    protected String liveAxisValue = Settings.BLANK;
    protected int liveDataIndex = 0;

    private Table compareTable = null;

    protected Table() {
        scales.clear();
        scales.add(new Scale());

        this.setLayout(borderLayout);
        this.add(centerPanel, BorderLayout.CENTER);
        centerPanel.setVisible(true);

        // key binding actions
        Action rightAction = new AbstractAction() {
            private static final long serialVersionUID = 1042884198300385041L;

            @Override
            public void actionPerformed(ActionEvent e) {
                cursorRight();
            }
        };
        Action leftAction = new AbstractAction() {
            private static final long serialVersionUID = -4970441255677214171L;

            @Override
            public void actionPerformed(ActionEvent e) {
                cursorLeft();
            }
        };
        Action downAction = new AbstractAction() {
            private static final long serialVersionUID = -7898502951121825984L;

            @Override
            public void actionPerformed(ActionEvent e) {
                cursorDown();
            }
        };
        Action upAction = new AbstractAction() {
            private static final long serialVersionUID = 6937621541727666631L;

            @Override
            public void actionPerformed(ActionEvent e) {
                cursorUp();
            }
        };
        Action incCoarseAction = new AbstractAction() {
            private static final long serialVersionUID = -8308522736529183148L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().incrementCoarse();
            }
        };
        Action decCoarseAction = new AbstractAction() {
            private static final long serialVersionUID = -7407628920997400915L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().decrementCoarse();
            }
        };
        Action incFineAction = new AbstractAction() {
            private static final long serialVersionUID = 7261463425941761433L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().incrementFine();
            }
        };
        Action decFineAction = new AbstractAction() {
            private static final long serialVersionUID = 8929400237520608035L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().decrementFine();
            }
        };
        Action num0Action = new AbstractAction() {
            private static final long serialVersionUID = -6310984176739090034L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('0');
            }
        };
        Action num1Action = new AbstractAction() {
            private static final long serialVersionUID = -6187220355403883499L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('1');
            }
        };
        Action num2Action = new AbstractAction() {
            private static final long serialVersionUID = -8745505977907325720L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('2');
            }
        };
        Action num3Action = new AbstractAction() {
            private static final long serialVersionUID = 4694872385823448942L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('3');
            }
        };
        Action num4Action = new AbstractAction() {
            private static final long serialVersionUID = 4005741329254221678L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('4');
            }
        };
        Action num5Action = new AbstractAction() {
            private static final long serialVersionUID = -5846094949106279884L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('5');
            }
        };
        Action num6Action = new AbstractAction() {
            private static final long serialVersionUID = -5338656374925334150L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('6');
            }
        };
        Action num7Action = new AbstractAction() {
            private static final long serialVersionUID = 1959983381590509303L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('7');
            }
        };
        Action num8Action = new AbstractAction() {
            private static final long serialVersionUID = 7442763278699460648L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('8');
            }
        };
        Action num9Action = new AbstractAction() {
            private static final long serialVersionUID = 7475171864584215094L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('9');
            }
        };
        Action numPointAction = new AbstractAction() {
            private static final long serialVersionUID = -4729135055857591830L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('.');
            }
        };
        Action copyAction = new AbstractAction() {
            private static final long serialVersionUID = -6978981449261938672L;

            @Override
            public void actionPerformed(ActionEvent e) {
                copySelection();
            }
        };
        Action pasteAction = new AbstractAction() {
            private static final long serialVersionUID = 2026817603236490899L;

            @Override
            public void actionPerformed(ActionEvent e) {
                paste();
            }
        };
        Action interpolate = new AbstractAction() {
            private static final long serialVersionUID = -2350912575392447149L;

            @Override
            public void actionPerformed(ActionEvent e) {
                interpolate();
            }
        };
        Action verticalInterpolate = new AbstractAction() {
            private static final long serialVersionUID = -2350912575392447149L;

            @Override
            public void actionPerformed(ActionEvent e) {
                verticalInterpolate();
            }
        };
        Action horizontalInterpolate = new AbstractAction() {
            private static final long serialVersionUID = -6346750245035640773L;

            @Override
            public void actionPerformed(ActionEvent e) {
                horizontalInterpolate();
            }
        };
        Action multiplyAction = new AbstractAction() {
            private static final long serialVersionUID = -2350912575392447149L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().multiply();
            }
        };
        Action numNegAction = new AbstractAction() {
            private static final long serialVersionUID = -6346750245035640773L;

            @Override
            public void actionPerformed(ActionEvent e) {
                getToolbar().focusSetValue('-');
            }
        };

        // set input mapping
        InputMap im = getInputMap(WHEN_IN_FOCUSED_WINDOW);

        KeyStroke right = KeyStroke.getKeyStroke(KeyEvent.VK_RIGHT, 0);
        KeyStroke left = KeyStroke.getKeyStroke(KeyEvent.VK_LEFT, 0);
        KeyStroke up = KeyStroke.getKeyStroke(KeyEvent.VK_UP, 0);
        KeyStroke down = KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, 0);
        KeyStroke decrement = KeyStroke.getKeyStroke('-');
        KeyStroke increment = KeyStroke.getKeyStroke('+');
        KeyStroke decrement2 = KeyStroke.getKeyStroke("control DOWN");
        KeyStroke increment2 = KeyStroke.getKeyStroke("control UP");
        KeyStroke decrement3 = KeyStroke.getKeyStroke(KeyEvent.VK_MINUS, KeyEvent.CTRL_DOWN_MASK);
        KeyStroke increment3 = KeyStroke.getKeyStroke(KeyEvent.VK_PLUS, KeyEvent.CTRL_DOWN_MASK);
        KeyStroke decrement4 = KeyStroke.getKeyStroke("control shift DOWN");
        KeyStroke increment4 = KeyStroke.getKeyStroke("control shift UP");
        KeyStroke num0 = KeyStroke.getKeyStroke('0');
        KeyStroke num1 = KeyStroke.getKeyStroke('1');
        KeyStroke num2 = KeyStroke.getKeyStroke('2');
        KeyStroke num3 = KeyStroke.getKeyStroke('3');
        KeyStroke num4 = KeyStroke.getKeyStroke('4');
        KeyStroke num5 = KeyStroke.getKeyStroke('5');
        KeyStroke num6 = KeyStroke.getKeyStroke('6');
        KeyStroke num7 = KeyStroke.getKeyStroke('7');
        KeyStroke num8 = KeyStroke.getKeyStroke('8');
        KeyStroke num9 = KeyStroke.getKeyStroke('9');
        KeyStroke mulKey = KeyStroke.getKeyStroke('*');
        KeyStroke mulKeys = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, KeyEvent.CTRL_DOWN_MASK);
        KeyStroke numPoint = KeyStroke.getKeyStroke('.');
        KeyStroke copy = KeyStroke.getKeyStroke("control C");
        KeyStroke paste = KeyStroke.getKeyStroke("control V");
        KeyStroke interp = KeyStroke.getKeyStroke("shift I");
        KeyStroke vinterp = KeyStroke.getKeyStroke("shift V");
        KeyStroke hinterp = KeyStroke.getKeyStroke("shift H");
        KeyStroke numNeg = KeyStroke.getKeyStroke('-');

        im.put(right, "right");
        im.put(left, "left");
        im.put(up, "up");
        im.put(down, "down");
        im.put(increment, "incCoarseAction");
        im.put(decrement, "decCoarseAction");
        im.put(increment2, "incCoarseAction");
        im.put(decrement2, "decCoarseAction");
        im.put(increment3, "incFineAction");
        im.put(decrement3, "decFineAction");
        im.put(increment4, "incFineAction");
        im.put(decrement4, "decFineAction");
        im.put(num0, "num0Action");
        im.put(num1, "num1Action");
        im.put(num2, "num2Action");
        im.put(num3, "num3Action");
        im.put(num4, "num4Action");
        im.put(num5, "num5Action");
        im.put(num6, "num6Action");
        im.put(num7, "num7Action");
        im.put(num8, "num8Action");
        im.put(num9, "num9Action");
        im.put(numPoint, "numPointAction");
        im.put(copy, "copyAction");
        im.put(paste, "pasteAction");
        im.put(interp, "interpolate");
        im.put(vinterp, "verticalInterpolate");
        im.put(hinterp, "horizontalInterpolate");
        im.put(mulKey, "mulAction");
        im.put(mulKeys, "mulAction");
        im.put(numNeg, "numNeg");

        getActionMap().put(im.get(right), rightAction);
        getActionMap().put(im.get(left), leftAction);
        getActionMap().put(im.get(up), upAction);
        getActionMap().put(im.get(down), downAction);
        getActionMap().put(im.get(increment), incCoarseAction);
        getActionMap().put(im.get(decrement), decCoarseAction);
        getActionMap().put(im.get(increment2), incCoarseAction);
        getActionMap().put(im.get(decrement2), decCoarseAction);
        getActionMap().put(im.get(increment3), incFineAction);
        getActionMap().put(im.get(decrement3), decFineAction);
        getActionMap().put(im.get(increment4), incFineAction);
        getActionMap().put(im.get(decrement4), decFineAction);
        getActionMap().put(im.get(num0), num0Action);
        getActionMap().put(im.get(num1), num1Action);
        getActionMap().put(im.get(num2), num2Action);
        getActionMap().put(im.get(num3), num3Action);
        getActionMap().put(im.get(num4), num4Action);
        getActionMap().put(im.get(num5), num5Action);
        getActionMap().put(im.get(num6), num6Action);
        getActionMap().put(im.get(num7), num7Action);
        getActionMap().put(im.get(num8), num8Action);
        getActionMap().put(im.get(num9), num9Action);
        getActionMap().put(im.get(numPoint), numPointAction);
        getActionMap().put(im.get(mulKey), multiplyAction);
        getActionMap().put(im.get(mulKeys), multiplyAction);
        getActionMap().put(im.get(copy), copyAction);
        getActionMap().put(im.get(paste), pasteAction);
        getActionMap().put(im.get(interp), interpolate);
        getActionMap().put(im.get(vinterp), verticalInterpolate);
        getActionMap().put(im.get(hinterp), horizontalInterpolate);
        getActionMap().put(im.get(numNeg), numNegAction);

        this.setInputMap(WHEN_FOCUSED, im);
    }

    public void reset() {
        centerPanel.removeAll();
    }

    public DataCell[] getData() {
        return data;
    }

    public abstract void drawTable();

    public abstract void populateTable(byte[] input, int romRamOffset);

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    @Override
    public String getName() {
        if(null == name || name.isEmpty()) {
            StringBuilder sb = new StringBuilder();
            sb.append(Settings.DEFAULT_TABLE_NAME);

            if(0 != this.getStorageAddress()) {
                sb.append(" ("+this.getStorageAddress() + ")");
            }

            if(null != this.getLogParam() && !this.getLogParam().isEmpty()) {
                sb.append(" - " + this.getLogParam());
            }

            return sb.toString();
        }
        return name;
    }

    @Override
    public void setName(String name) {
        if(null != name && !name.isEmpty()) {
            this.name = name;
        }
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Scale getCurrentScale() {
        return this.curScale;
    }

    public Scale getScale(String scaleName) throws NameNotFoundException {
        for (Scale scale : scales) {
            if (scale.getName().equalsIgnoreCase(scaleName)) {
                return scale;
            }
        }
        return new Scale();
    }

    public Vector<Scale> getScales() {
        return scales;
    }

    public void addScale(Scale scale) {
        // look for scale, replace or add new
        for (int i = 0; i < scales.size(); i++) {
            if (scales.get(i).getName().equalsIgnoreCase(scale.getName())) {
                scales.remove(i);
                break;
            }
        }
        scales.add(scale);

        if(null == curScale) {
            this.curScale = scale;
        }

        if(SettingsManager.getSettings().getDefaultScale().equalsIgnoreCase(scale.getName())) {
            this.curScale = scale;
        }

        validateScaling();
    }

    public int getStorageAddress() {
        return storageAddress;
    }

    public void setStorageAddress(int storageAddress) {
        this.storageAddress = storageAddress;
    }

    public int getStorageType() {
        return storageType;
    }

    public void setStorageType(int storageType) {
        this.storageType = storageType;
        calcValueRange();
    }

    public boolean isSignedData() {
        return signed;
    }

    public void setSignedData(boolean signed) {
        this.signed = signed;
    }

    public int getEndian() {
        return endian;
    }

    public void setEndian(int endian) {
        this.endian = endian;
    }

    public void setDataSize(int size) {
        data = new DataCell[size];
    }

    public int getDataSize() {
        return data.length;
    }

    public boolean getFlip() {
        return flip;
    }

    public void setFlip(boolean flip) {
        this.flip = flip;
    }

    public void setLogParam(String logParam) {
        this.logParam = logParam;
    }

    public String getLogParam() {
        return logParam;
    }

    @Override
    public String toString() {
        /*String output = "\n   ---- Table " + name + " ----" +
                scale +
                "\n   Category: " + category +
                "\n   Type: " + type +
                "\n   Description: " + description +
                "\n   Storage Address: " + Integer.toHexString(storageAddress) +
                "\n   Storage Type: " + storageType +
                "\n   Endian: " + endian +
                "\n   Flip: " + flip +
                "\n   ---- End Table " + name + " ----";
        for (int i = 0; i < data.length; i++) {
            if (data[i] != null) {
                output = output + "\nData: " + data[i];
            }
        }

        return output;*/
        return getName();
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

            if(!(other instanceof Table)) {
                return false;
            }

            Table otherTable = (Table)other;

            if( (null == this.getName() && null == otherTable.getName())
                    || (this.getName().isEmpty() && otherTable.getName().isEmpty()) ) {
                ;// Skip name compare if name is null or empty.
            } else {
                if(!this.getName().equalsIgnoreCase(otherTable.getName())) {
                    return false;
                }
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
        } catch(Exception ex) {
            // TODO: Log Exception.
            return false;
        }
    }

    public double getMaxAllowedBin() {
        return maxAllowedBin;
    }

    public double getMinAllowedBin() {
        return minAllowedBin;
    }

    public double getMaxAllowedReal() {
        return JEPUtil.evaluate(getCurrentScale().getExpression(), getMaxAllowedBin());
    }

    public double getMinAllowedReal() {
        return JEPUtil.evaluate(getCurrentScale().getExpression(), getMinAllowedBin());
    }

    private void calcValueRange() {
        if (getStorageType() != Settings.STORAGE_TYPE_FLOAT) {
            if (isSignedData()) {
                switch (getStorageType()) {
                case 1:
                    minAllowedBin = Byte.MIN_VALUE;
                    maxAllowedBin = Byte.MAX_VALUE;
                    break;
                case 2:
                    minAllowedBin = Short.MIN_VALUE;
                    maxAllowedBin = Short.MAX_VALUE;
                    break;
                case 4:
                    minAllowedBin = Integer.MIN_VALUE;
                    maxAllowedBin = Integer.MAX_VALUE;
                    break;
                }
            }
            else {
                maxAllowedBin = (Math.pow(256, getStorageType()) - 1);
                minAllowedBin = 0.0;
            }
        } else {
            maxAllowedBin = Float.MAX_VALUE;

            if(isSignedData()) {
                minAllowedBin = 0.0;
            } else {
                minAllowedBin = -Float.MAX_VALUE;
            }
        }
    }

    public void calcCellRanges() {
        double binMax = data[0].getBinValue();
        double binMin = data[0].getBinValue();

        double compareMax = data[0].getCompareValue();
        double compareMin = data[0].getCompareValue();

        for(DataCell cell : data) {
            // Calc bin
            if(binMax < cell.getBinValue()) {
                binMax = cell.getBinValue();
            }
            if(binMin > cell.getBinValue()) {
                binMin = cell.getBinValue();
            }

            // Calc compare
            double compareValue = cell.getCompareValue();
            if(compareMax < compareValue) {
                compareMax = compareValue;
            }
            if(compareMin > compareValue) {
                compareMin = compareValue;
            }
        }
        setMaxBin(binMax);
        setMinBin(binMin);
        setMaxCompare(compareMax);
        setMinCompare(compareMin);
    }

    public double getMaxBin() {
        return this.maxBin;
    }

    public double getMinBin() {
        return this.minBin;
    }

    public double getMaxReal() {
        return JEPUtil.evaluate(getCurrentScale().getExpression(), getMaxBin());
    }

    public double getMinReal() {
        return JEPUtil.evaluate(getCurrentScale().getExpression(), getMinBin());
    }

    public void setMaxBin(double maxBin) {
        this.maxBin = maxBin;
    }

    public void setMinBin(double minBin) {
        this.minBin = minBin;
    }

    public double getMaxCompare() {
        return this.maxCompare;
    }

    public void setMaxCompare(double maxCompare) {
        this.maxCompare = maxCompare;
    }

    public double getMinCompare() {
        return this.minCompare;
    }

    public void setMinCompare(double minCompare) {
        this.minCompare = minCompare;
    }

    public abstract void increment(double increment);

    public abstract void multiply(double factor);


    public void setRealValue(String realValue) {
        if (!locked && userLevel <= getSettings().getUserLevel()) {
            for(DataCell cell : data) {
                if (cell.isSelected()) {
                    cell.setRealValue(realValue);
                }
            }
        } else if (userLevel > getSettings().getUserLevel()) {
            JOptionPane.showMessageDialog(this, "This table can only be modified by users with a userlevel of \n" +
                    userLevel + " or greater. Click View->User Level to change your userlevel.",
                    "Table cannot be modified",
                    JOptionPane.INFORMATION_MESSAGE);
        }
    }

    public void clearSelection() {
        clearSelectedData();
    }

    public void clearSelectedData() {
        for (DataCell cell : data) {
            if(cell.isSelected()) {
                cell.setSelected(false);
            }
        }
    }

    public void startHighlight(int x, int y) {
        this.highlightY = y;
        this.highlightX = x;
        highlight = true;
        highlight(x, y);
    }

    public void highlight(int x, int y) {
        if (highlight) {
            for (int i = 0; i < data.length; i++) {
                if ((i >= highlightY && i <= y) || (i <= highlightY && i >= y)) {
                    data[i].setHighlighted(true);
                } else {
                    data[i].setHighlighted(false);
                }
            }
        }
    }

    public void stopHighlight() {
        highlight = false;
        // loop through, selected and un-highlight
        for (DataCell cell : data) {
            if (cell.isHighlighted()) {
                cell.setHighlighted(false);
                if(!cell.isSelected()) {
                    cell.setSelected(true);
                }
            }
        }
    }

    public abstract void cursorUp();

    public abstract void cursorDown();

    public abstract void cursorLeft();

    public abstract void cursorRight();

    public void setRevertPoint() {
        for (DataCell cell : data) {
            cell.setRevertPoint();
        }
    }

    public void undoAll() {
        clearLiveDataTrace();
        for (DataCell cell : data) {
            cell.undo();
        }
    }

    public void undoSelected() {
        clearLiveDataTrace();
        for (DataCell cell : data) {
            // reset current value to original value
            if (cell.isSelected()) {
                cell.undo();
            }
        }
    }

    public abstract byte[] saveFile(byte[] binData);

    public void setBeforeRam(boolean beforeRam) {
        this.beforeRam = beforeRam;
    }

    public void copySelection() {
        Window ancestorWindow = SwingUtilities.getWindowAncestor(this);

        if(null != ancestorWindow) {
            ancestorWindow.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        }

        ConsoleUI.getFrame().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        copySelectionWorker = new CopySelectionWorker(this);
        copySelectionWorker.execute();
    }

    public StringBuffer getTableAsString() {
        StringBuffer output = new StringBuffer(Settings.BLANK);
        for (int i = 0; i < data.length; i++) {
            output.append(data[i].getRealValue());
            if (i < data.length - 1) {
                output.append(Settings.TAB);
            }
        }
        return output;
    }

    public void copyTable() {
        Window ancestorWindow = SwingUtilities.getWindowAncestor(this);
        if(null != ancestorWindow) {
            ancestorWindow.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        }
        ConsoleUI.getFrame().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        copyTableWorker = new CopyTableWorker(this);
        copyTableWorker.execute();
    }

    public String getCellAsString(int index) {
        return data[index].getText();
    }

    public void pasteValues(String[] input) {
        //set real values
        for (int i = 0; i < input.length; i++) {
            try {
                Double.parseDouble(input[i]);
                data[i].setRealValue(input[i]);
            } catch (NumberFormatException ex) { /* not a number, do nothing */ }
        }
    }

    public void paste() {
        // TODO: This sounds like desearialize.

        StringTokenizer st = new StringTokenizer(Settings.BLANK);
        try {
            String input = (String) Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null).getTransferData(DataFlavor.stringFlavor);
            st = new StringTokenizer(input);
        } catch (UnsupportedFlavorException ex) { /* wrong paste type -- do nothing */
        } catch (IOException ex) {
        }

        String pasteType = st.nextToken();

        if ("[Table1D]".equalsIgnoreCase(pasteType)) { // copied entire table
            int i = 0;
            while (st.hasMoreTokens()) {
                String currentToken = st.nextToken();
                try {
                    if (!data[i].getText().equalsIgnoreCase(currentToken)) {
                        data[i].setRealValue(currentToken);
                    }
                } catch (ArrayIndexOutOfBoundsException ex) { /* table larger than target, ignore*/ }
                i++;
            }
        } else if ("[Selection1D]".equalsIgnoreCase(pasteType)) { // copied selection
            if (data[highlightY].isSelected()) {
                int i = 0;
                while (st.hasMoreTokens()) {
                    try {
                        data[highlightY + i].setRealValue(st.nextToken());
                    } catch (ArrayIndexOutOfBoundsException ex) { /* paste larger than target, ignore */ }
                    i++;
                }
            }
        }
    }

    public void verticalInterpolate() {
        horizontalInterpolate();
    }

    public abstract void horizontalInterpolate();

    public void interpolate() {
        horizontalInterpolate();
    }

    public void validateScaling() {
        if (type != Settings.TABLE_SWITCH) {

            // make sure a scale is present
            if (scales.isEmpty()) {
                scales.add(new Scale());
            }

            for(Scale scale : scales) {
                double startValue = 5;
                double toReal = JEPUtil.evaluate(scale.getExpression(), startValue); // convert real world value of "5"
                double endValue = JEPUtil.evaluate(scale.getByteExpression(), toReal);

                // if real to byte doesn't equal 5, report conflict
                if (Math.abs(endValue - startValue) > .001) {

                    JPanel panel = new JPanel();
                    panel.setLayout(new GridLayout(4, 1));
                    panel.add(new JLabel("The real value and byte value conversion expressions for table " + getName() + " are invalid."));
                    panel.add(new JLabel("To real value: " + scale.getExpression()));
                    panel.add(new JLabel("To byte: " + scale.getByteExpression()));

                    JCheckBox check = new JCheckBox("Always display this message", true);
                    check.setHorizontalAlignment(JCheckBox.RIGHT);
                    panel.add(check);

                    check.addActionListener(
                            new ActionListener() {
                                @Override
                                public void actionPerformed(ActionEvent e) {
                                    getSettings().setCalcConflictWarning(((JCheckBox) e.getSource()).isSelected());
                                }
                            }
                            );

                    JOptionPane.showMessageDialog(SwingUtilities.windowForComponent(this), panel,
                            "Warning", JOptionPane.ERROR_MESSAGE);
                }
            }
        }
    }

    public void populateCompareValues(Table otherTable) {
        if(null == otherTable) {
            return;
        }

        DataCell[] compareData = otherTable.getData();
        if(data.length != compareData.length) {
            return;
        }

        clearLiveDataTrace();

        int i = 0;
        for(DataCell cell : data) {
            cell.setCompareValue(compareData[i]);
            i++;
        }

        calcCellRanges();
        drawTable();
    }

    public void setCompareDisplay(int compareDisplay) {
        this.compareDisplay = compareDisplay;
        drawTable();
    }

    public int getCompareDisplay() {
        return this.compareDisplay;
    }

    public void setCompareValueType(int compareValueType) {
        this.compareValueType = compareValueType;
        drawTable();
    }

    public int getCompareValueType() {
        return this.compareValueType;
    }

    public int getUserLevel() {
        return userLevel;
    }

    public void setUserLevel(int userLevel) {
        this.userLevel = userLevel;
        if (userLevel > 5) {
            userLevel = 5;
        } else if (userLevel < 1) {
            userLevel = 1;
        }
    }

    public void setScaleByName(String scaleName) throws NameNotFoundException {
        for(Scale scale : scales) {
            if(scale.getName().equalsIgnoreCase(scaleName)) {
                Scale currentScale = getCurrentScale();
                if(currentScale == null || !currentScale.equals(scale)) {
                    this.setCurrentScale(scale);
                }
                return;
            }
        }

        throw new NameNotFoundException();
    }

    public void setCurrentScale(Scale curScale) {
        this.curScale = curScale;
        updateTableLabel();
        drawTable();
    }

    public Settings getSettings()
    {
        return SettingsManager.getSettings();
    }

    public TableToolBar getToolbar()
    {
        return ECUEditorManager.getECUEditor().getTableToolBar();
    }

    public boolean isLocked() {
        return locked;
    }

    public void setLocked(boolean locked) {
        this.locked = locked;
    }

    public void setOverlayLog(boolean overlayLog) {
        this.overlayLog = overlayLog;
        if (overlayLog) {
            clearLiveDataTrace();
        }
    }

    public boolean getOverlayLog()
    {
        return this.overlayLog;
    }

    public double getLiveAxisValue() {
        try {
            return Double.parseDouble(liveAxisValue);
        } catch (NumberFormatException e) {
            return 0.0;
        }
    }

    public abstract boolean isLiveDataSupported();

    public abstract boolean isButtonSelected();

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
                double currentValue = data[i].getRealValue();
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
    }

    public void updateLiveDataHighlight() {
        if (getOverlayLog()) {
            data[getLiveDataIndex()].setLiveDataTrace(true);
        }
    }

    public int getLiveDataIndex() {
        return liveDataIndex;
    }

    public void setLiveDataIndex(int index) {
        if (index < 0) {
            index = 0;
        }
        if (index >= data.length) {
            index = data.length - 1;
        }
        this.liveDataIndex = index;
    }

    public void clearLiveDataTrace() {
        for (DataCell cell : data) {
            cell.setLiveDataTrace(false);
        }
    }

    public String getLogParamString() {
        return getName()+ ":" + getLogParam();
    }

    public Table getCompareTable() {
        return compareTable;
    }

    public void setCompareTable(Table compareTable) {
        this.compareTable = compareTable;
    }

    public void updateTableLabel() {
        if(null == name || name.isEmpty()) {
            ;// Do not update label.
        } else if(null == getCurrentScale () || "0x" == getCurrentScale().getUnit()) {
            // static or no scale exists.
            tableLabel.setText(getName());
        } else {
            tableLabel.setText(getName() + " (" + getCurrentScale().getUnit() + ")");
        }
    }

    public void colorCells() {
        calcCellRanges();
        drawTable();
    }

    public void refreshCompare() {
        populateCompareValues(getCompareTable());
    }

    public boolean isStaticDataTable() {
        return staticDataTable;
    }

    public void setStaticDataTable(boolean staticDataTable) {
        this.staticDataTable = staticDataTable;
    }
}


