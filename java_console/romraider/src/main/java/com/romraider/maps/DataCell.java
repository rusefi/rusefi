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

import static com.romraider.util.ColorScaler.getScaledColor;
import static com.romraider.util.ParamChecker.isNullOrEmpty;
import static javax.swing.BorderFactory.createLineBorder;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.Serializable;
import java.text.DecimalFormat;

import javax.swing.JLabel;
import javax.swing.border.Border;

import org.apache.log4j.Logger;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.util.JEPUtil;
import com.romraider.util.SettingsManager;

public class DataCell extends JLabel implements MouseListener, Serializable {
    private static final long serialVersionUID = -2904293227148940937L;
    private static final Logger LOGGER = Logger.getLogger(DataCell.class);
    private final DecimalFormat PERCENT_FORMAT = new DecimalFormat("#,##0.0%");
    private final Font defaultFont = new Font("Arial", Font.BOLD, 12);
    private static final int unSelectMask1 = MouseEvent.BUTTON1_DOWN_MASK + MouseEvent.CTRL_DOWN_MASK + MouseEvent.ALT_DOWN_MASK;
    private static final int unSelectMask2 = MouseEvent.BUTTON3_DOWN_MASK + MouseEvent.CTRL_DOWN_MASK + MouseEvent.ALT_DOWN_MASK;

    private final Table table;

    private Boolean selected = false;
    private Boolean highlighted = false;
    private Boolean traced = false;

    private int x = 0;
    private int y = 0;

    private double binValue = 0.0;
    private double originalValue = 0.0;
    private double compareToValue = 0.0;
    private String liveValue = Settings.BLANK;

    private final Color defaultBorderColor = new Color(0, 0, 0);
    private final Color increaseBorderColor = getSettings().getIncreaseBorder();
    private final Color decreaseBorderColor = getSettings().getDecreaseBorder();

    private String staticText = null;

    public DataCell(Table table) {
        this.table = table;
        this.setHorizontalAlignment(CENTER);
        this.setVerticalAlignment(CENTER);
        this.setFont(defaultFont);
        this.setOpaque(true);
        this.setVisible(true);
        this.addMouseListener(this);
    }

    public DataCell(Table table, String staticText) {
        this(table);
        this.staticText = staticText;
        table.setStaticDataTable(true);
    }

    public DataCell(Table table, double originalValue, int x, int y) {
        this(table);
        this.originalValue = originalValue;
        this.binValue = originalValue;
        this.x = x;
        this.y = y;
        this.setPreferredSize(getSettings().getCellSize());
    }

    public double getBinValue() {
        return binValue;
    }

    public double getRealValue() {
        return JEPUtil.evaluate(table.getCurrentScale().getExpression(), binValue);
    }

    public void setRealValue(String input) {
        // create parser
        try {
            double result = 0.0;
            if (!"x".equalsIgnoreCase(input)) {
                result = JEPUtil.evaluate(table.getCurrentScale().getByteExpression(), Double.parseDouble(input));
                if (table.getStorageType() != Settings.STORAGE_TYPE_FLOAT) {
                    result = (int) Math.round(result);
                }

                if(binValue != result) {
                    this.setBinValue(result);
                }
            }
        } catch (NumberFormatException e) {
            // Do nothing.  input is null or not a valid number.
        }
    }

    public double getCompareValue() {
        return binValue - compareToValue;
    }

    public double getRealCompareValue() {
        return JEPUtil.evaluate(table.getCurrentScale().getExpression(), binValue) - JEPUtil.evaluate(table.getCurrentScale().getExpression(), compareToValue);
    }

    public double getRealCompareChangeValue() {
        double realBinValue = JEPUtil.evaluate(table.getCurrentScale().getExpression(), binValue);
        double realCompareValue = JEPUtil.evaluate(table.getCurrentScale().getExpression(), compareToValue);

        if(realCompareValue != 0.0) {
            // Compare change formula ((V2 - V1) / |V1|).
            return ((realBinValue - realCompareValue) / Math.abs(realCompareValue));
        } else {
            // Use this to avoid divide by 0 or infinite increase.
            return realBinValue - realCompareValue;
        }
    }

    public Color getCompareColor() {
        if(table instanceof Table1D) {
            Table1D checkTable = (Table1D)table;
            if(checkTable.isAxis() && !getSettings().isColorAxis()) {
                return getSettings().getAxisColor();
            }
        }

        double compareScale;
        if (0.0 == getCompareValue()) {
            return Settings.UNCHANGED_VALUE_COLOR;
        }else if(table.getMinCompare() == table.getMaxCompare()) {
            return getSettings().getMaxColor();
        } else {
            compareScale = (getCompareValue() - table.getMinCompare()) / (table.getMaxCompare() - table.getMinCompare());
        }
        return getScaledColor(compareScale);
    }

    public Color getBinColor() {
        if(table instanceof Table1D) {
            Table1D checkTable = (Table1D)table;
            if(checkTable.isAxis() && !getSettings().isColorAxis()) {
                return getSettings().getAxisColor();
            }
        }

        if (table.getMaxAllowedBin() < getBinValue()) {
            return getSettings().getWarningColor();
        } else if (table.getMinAllowedBin() > getBinValue()) {
            return getSettings().getWarningColor();
        } else {
            // limits not set, scale based on table values
            double colorScale;
            if (table.getMaxBin() - table.getMinBin() == 0.0) {
                // if all values are the same, color will be middle value
                colorScale = .5;
            } else {
                colorScale = (getBinValue() - table.getMinBin()) / (table.getMaxBin() - table.getMinBin());
            }

            return getScaledColor(colorScale);
        }
    }

    public void drawCell() {
        if(table == null) {
            // Table will be null in the static case.
            return;
        }

        this.invalidate();
        setFont(getSettings().getTableFont());
        setText(getCellText());
        setToolTipText(getCellToolTip());
        setBackground(getCellBackgroundColor());
        setForeground(getCellTextColor());
        setBorder(getCellBorder());
        this.validate();
        table.validate();
        table.repaint();
    }

    private Color getCellBackgroundColor() {
        Settings settings = getSettings();
        Color backgroundColor;

        if(highlighted) {
            backgroundColor = settings.getHighlightColor();
        } else if(selected) {
            backgroundColor = settings.getSelectColor();
        } else if(null == table.getCompareTable()) {
            backgroundColor = getBinColor();
        }else {
            backgroundColor = getCompareColor();
        }

        return backgroundColor;
    }

    private Color getCellTextColor() {
        Color textColor;

        if(traced) {
            if(!getLiveValue().isEmpty()) {
                if(table instanceof Table1D) {
                    textColor = Settings.scaleTextColor;
                } else {
                    textColor = Settings.liveDataTraceTextColor;
                }
            } else {
                textColor = Settings.scaleTextColor;
            }
        } else if (highlighted) {
            textColor = Settings.highlightTextColor;
        } else if (selected) {
            textColor = Settings.selectTextColor;
        } else {
            textColor = Settings.scaleTextColor;
        }

        return textColor;
    }

    private Border getCellBorder() {
        Border border;
        if(traced) {
            border = createLineBorder(getSettings().getliveValueColor(), 2);
        } else {
            double checkValue;

            if(null == table.getCompareTable()) {
                checkValue= originalValue;
            } else {
                checkValue = compareToValue;
            }

            if (checkValue < binValue) {
                border = createLineBorder(increaseBorderColor, 2);
            } else if (checkValue > binValue) {
                border = createLineBorder(decreaseBorderColor, 2);
            } else {
                border = createLineBorder(defaultBorderColor, 1);
            }
        }

        return border;
    }

    private String getCellText() {
        if(table.isStaticDataTable()) {
            return getStaticText();
        }

        DecimalFormat formatter = new DecimalFormat(table.getCurrentScale().getFormat());
        String displayString = "";

        if (null == table.getCompareTable()) {
            displayString = formatter.format(getRealValue());
        } else if (table.getCompareDisplay() == Settings.COMPARE_DISPLAY_ABSOLUTE) {
            displayString = formatter.format(getRealCompareValue());
        } else if (table.getCompareDisplay() == Settings.COMPARE_DISPLAY_PERCENT) {
            if (getCompareValue() == 0.0) {
                displayString = PERCENT_FORMAT.format(0.0);
            } else {
                displayString = PERCENT_FORMAT.format(getRealCompareChangeValue());
            }
        }

        if(traced) {
            if(!(table instanceof Table1D)) {
                displayString = getLiveValueString(displayString);
            }
        }
        return displayString;
    }

    private String getCellToolTip() {
        if(table.isStaticDataTable()) {
            return getStaticText();
        }

        return Double.toString(getRealValue());
    }

    private String getLiveValue() {
        return this.liveValue;
    }

    private String getLiveValueString(String currentValue) {
        return currentValue + (isNullOrEmpty(getLiveValue()) ? Settings.BLANK : (':' + getLiveValue()));
    }

    public void setBinValue(double newBinValue) {
        if(binValue == newBinValue) {
            return;
        }

        double checkedValue = newBinValue;

        // make sure it's in range
        if(checkedValue < table.getMinAllowedBin()) {
            checkedValue = table.getMinAllowedBin();
        }

        if(checkedValue > table.getMaxAllowedBin()) {
            checkedValue = table.getMaxAllowedBin();
        }

        if(binValue == checkedValue) {
            return;
        }

        // set bin.
        binValue = checkedValue;
        drawCell();
    }

    @Override
    public String toString() {
        return getCellText();
    }

    public Boolean isSelected() {
        return selected;
    }

    public void setSelected(Boolean selected) {
        if(!table.isStaticDataTable() && this.selected != selected) {
            this.selected = selected;
            drawCell();
        }
    }

    public void setHighlighted(Boolean highlighted) {
        if(!table.isStaticDataTable() && this.highlighted != highlighted) {
            this.highlighted = highlighted;
            drawCell();
        }
    }

    public boolean isHighlighted() {
        return highlighted;
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        if(unSelectMask1 == (e.getModifiersEx() & unSelectMask1)) {
            clearCell();
        } else if(unSelectMask2 == (e.getModifiersEx() & unSelectMask2)) {
            clearCell();
        } else {
            table.highlight(x, y);
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {
        if (!e.isControlDown()) {
            table.clearSelection();
        }

        if (e.isControlDown() && e.isAltDown()) {
            clearCell();
        } else {
            table.startHighlight(x, y);
        }
        requestFocus();
        ECUEditorManager.getECUEditor().getTableToolBar().updateTableToolBar(table);
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        table.stopHighlight();
    }

    @Override
    public void mouseClicked(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    private void clearCell() {
        if(isHighlighted()) {
            setHighlighted(false);
        }
        if(isSelected()) {
            setSelected(false);
        }
    }

    public void increment(double increment) {
        double oldValue = getRealValue();

        if (table.getCurrentScale().getCoarseIncrement() < 0.0) {
            increment = 0.0 - increment;
        }

        double incResult = JEPUtil.evaluate(table.getCurrentScale().getByteExpression(), (oldValue + increment));
        if (table.getStorageType() == Settings.STORAGE_TYPE_FLOAT) {
            if(binValue != incResult) {
                this.setBinValue(incResult);
            }
        } else {
            int roundResult = (int) Math.round(incResult);
            if(binValue != roundResult) {
                this.setBinValue(roundResult);
            }
        }

        // make sure table is incremented if change isn't great enough
        int maxValue = (int) Math.pow(8, table.getStorageType());

        if (table.getStorageType() != Settings.STORAGE_TYPE_FLOAT &&
                oldValue == getRealValue() &&
                binValue > 0.0 &&
                binValue < maxValue) {
            LOGGER.debug(maxValue + " " + binValue);
            increment(increment * 2);
        }
    }

    public void undo() {
        this.setBinValue(originalValue);
    }

    public void setRevertPoint() {
        this.setOriginalValue(binValue);
        this.drawCell();
    }

    public void setOriginalValue(double originalValue) {
        this.originalValue = originalValue;
    }

    public void setCompareValue(DataCell compareCell) {
        if(Settings.DATA_TYPE_BIN == table.getCompareValueType())
        {
            if(this.compareToValue == compareCell.binValue) {
                return;
            }

            this.compareToValue = compareCell.binValue;
        } else {
            if(this.compareToValue == compareCell.originalValue) {
                return;
            }

            this.compareToValue = compareCell.originalValue;
        }
    }

    public void multiply(double factor) {
        setBinValue(binValue * factor);
    }

    public void setLiveDataTrace(boolean trace) {
        if(traced != trace) {
            traced = trace;
            drawCell();
        }
    }

    public void setLiveDataTraceValue(String liveValue) {
        if(this.liveValue != liveValue) {
            this.liveValue = liveValue;
            drawCell();
        }
    }

    private Settings getSettings() {
        return SettingsManager.getSettings();
    }

    @Override
    public boolean equals(Object other) {
        if(other == null) {
            return false;
        }

        if(!(other instanceof DataCell)) {
            return false;
        }

        DataCell otherCell = (DataCell) other;

        if(this.table.isStaticDataTable() != otherCell.table.isStaticDataTable()) {
            return false;
        }

        return binValue == otherCell.binValue;
    }

    public String getStaticText() {
        String displayString = null;
        try {
            DecimalFormat formatter = new DecimalFormat(table.getCurrentScale().getFormat());

            double staticDouble = Double.parseDouble(staticText);
            displayString = formatter.format(JEPUtil.evaluate(table.getCurrentScale().getExpression(), staticDouble));
        } catch (Exception ex) {
            displayString = this.staticText;
        }
        return displayString;
    }

    public void setY(int y) {
        this.y = y;
    }
}