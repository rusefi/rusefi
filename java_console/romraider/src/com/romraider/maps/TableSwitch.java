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

package com.romraider.maps;

import static com.romraider.maps.RomChecksum.validateRomChecksum;
import static com.romraider.util.ByteUtil.indexOfBytes;
import static com.romraider.util.HexUtil.asBytes;
import static javax.swing.JOptionPane.ERROR_MESSAGE;
import static javax.swing.JOptionPane.INFORMATION_MESSAGE;
import static javax.swing.JOptionPane.WARNING_MESSAGE;
import static javax.swing.JOptionPane.showMessageDialog;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Insets;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;

import com.romraider.Settings;

public class TableSwitch extends Table {

    public static final String TYPE_SWITCH = "Switch";
    private static final long serialVersionUID = -4887718305447362308L;
    private final ButtonGroup buttonGroup = new ButtonGroup();
    private final Map<String, byte[]> switchStates = new HashMap<String, byte[]>();
    private int dataSize = 0;

    public TableSwitch() {
        storageType = 1;
        type = Settings.TABLE_SWITCH;
        locked = true;
        removeAll();
        setLayout(new BorderLayout());
    }

    @Override
    public void horizontalInterpolate() {

    }

    @Override
    public void increment(double increment) {

    }

    @Override
    public void multiply(double factor) {

    }

    @Override
    public void setDataSize(int size) {
        if (dataSize == 0)
            dataSize = size;
    }

    @Override
    public int getDataSize() {
        return dataSize;
    }

    @Override
    public void populateTable(byte[] input, int romRamOffset) {
        JPanel radioPanel = new JPanel(new GridLayout(0, 1));
        radioPanel.add(new JLabel("  " + getName()));
        for (String stateName : switchStates.keySet()) {
            JRadioButton button = new JRadioButton(stateName);
            buttonGroup.add(button);
            radioPanel.add(button);
        }
        add(radioPanel, BorderLayout.CENTER);

        // Validate XML switch definition data against the ROM data to select
        // the appropriate switch setting or throw an error if there is a
        // mismatch and disable this table's editing ability.
        if (!beforeRam) {
            this.ramOffset = romRamOffset;
        }
        Map<String, Integer> sourceStatus = new HashMap<String, Integer>();
        for (String stateName : switchStates.keySet()) {
            byte[] sourceData = new byte[dataSize];
            System.arraycopy(
                    input,
                    storageAddress - ramOffset,
                    sourceData,
                    0,
                    dataSize);
            int compareResult = indexOfBytes(sourceData, getValues(stateName));
            if (compareResult == -1) {
                getButtonByText(buttonGroup, stateName).setSelected(false);
            }
            else {
                getButtonByText(buttonGroup, stateName).setSelected(true);
            }
            sourceStatus.put(stateName, compareResult);
        }

        for (String source : sourceStatus.keySet()) {
            if (sourceStatus.get(source) != -1) {
                locked = false;
                break;
            }
        }

        if (locked) {
            String mismatch = String.format("Table: %s%nTable editing has been disabled.%nDefinition file or ROM image may be corrupt.", super.getName());
            showMessageDialog(this,
                    mismatch,
                    "ERROR - Data Mismatch",
                    ERROR_MESSAGE);
            setButtonsUnselected(buttonGroup);
        }
    }

    @Override
    public void setName(String name) {
        super.setName(name);
    }

    @Override
    public int getType() {
        return Settings.TABLE_SWITCH;
    }

    @Override
    public void setDescription(String description) {
        super.setDescription(description);
        JTextArea descriptionArea = new JTextArea(description);
        descriptionArea.setOpaque(false);
        descriptionArea.setEditable(false);
        descriptionArea.setWrapStyleWord(true);
        descriptionArea.setLineWrap(true);
        descriptionArea.setMargin(new Insets(0,5,5,5));

        add(descriptionArea, BorderLayout.SOUTH);
    }

    @Override
    public byte[] saveFile(byte[] input) {

            if (!locked) {
                JRadioButton selectedButton = getSelectedButton(buttonGroup);
                System.arraycopy(
                        switchStates.get(selectedButton.getText()),
                        0,
                        input,
                        getStorageAddress() - ramOffset,
                        dataSize);
            }

        return input;
    }

    public void setValues(String name, String input) {
        switchStates.put(name, asBytes(input));
    }

    public byte[] getValues(String key) {
        return switchStates.get(key);
    }

    public Dimension getFrameSize() {
        int height = verticalOverhead + 75;
        int width = horizontalOverhead;
        if (height < minHeight) {
            height = minHeight;
        }
        int minWidth = isLiveDataSupported() ? minWidthOverlay : minWidthNoOverlay;
        if (width < minWidth) {
            width = minWidth;
        }
        return new Dimension(width, height);
    }

    public ButtonGroup getButtonGroup() {
        return this.buttonGroup;
    }

    public Map<String, byte[]> getSwitchStates() {
        return this.switchStates;
    }

    @Override
    public void cursorUp() {
    }

    @Override
    public void cursorDown() {
    }

    @Override
    public void cursorLeft() {
    }

    @Override
    public void cursorRight() {
    }

    @Override
    public boolean isLiveDataSupported() {
        return false;
    }

    @Override
    public boolean isButtonSelected() {
        if (buttonGroup.getSelection() == null) {
            return false;
        }
        else {
            return true;
        }
    }

    @Override
    public boolean equals(Object other) {
        // TODO: Validate DTC equals.
        try {
            if(null == other) {
                return false;
            }

            if(other == this) {
                return true;
            }

            if(!(other instanceof TableSwitch)) {
                return false;
            }

            TableSwitch otherTable = (TableSwitch)other;

            if( (null == this.getName() && null == otherTable.getName())
                    || (this.getName().isEmpty() && otherTable.getName().isEmpty()) ) {
                ;// Skip name compare if name is null or empty.
            } else if(!this.getName().equalsIgnoreCase(otherTable.getName())) {
                return false;
            }

            if(this.getDataSize() != otherTable.getDataSize()) {
                return false;
            }

            if(this.getSwitchStates() == otherTable.getSwitchStates()) {
                return true;
            }

            // Compare Map Keys
            Set<String> keys = new HashSet<String>(this.getSwitchStates().keySet());
            Set<String> otherKeys = new HashSet<String>(otherTable.getSwitchStates().keySet());

            if(keys.size() != otherKeys.size()) {
                return false;
            }

            if(!keys.containsAll(otherKeys)) {
                return false;
            }

            // Compare Map Values.
            Set<byte[]> values = new HashSet<byte[]>(this.getSwitchStates().values());
            Set<byte[]> otherValues = new HashSet<byte[]>(otherTable.getSwitchStates().values());
            if(values.equals(otherValues)) {
                return true;
            }

            // Compare DTC.  Is there a better way to compare the DTC?
            for(String key : keys) {
                JRadioButton button = getButtonByText(this.getButtonGroup(), key);
                JRadioButton otherButton = getButtonByText(otherTable.getButtonGroup(), key);

                if(button.isSelected() != otherButton.isSelected()) {
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
    public void populateCompareValues(Table compareTable) {
        return; // Do nothing.
    }

    @Override
    public void calcCellRanges() {
        return; // Do nothing.
    }

    @Override
    public void drawTable()
    {
        return; // Do nothing.
    }

    @Override
    public void updateTableLabel() {
        return; // Do nothing.
    }

    @Override
    public void setCurrentScale(Scale curScale) {
        return; // Do nothing.
    }


    // returns the selected radio button in the specified group
    private static JRadioButton getSelectedButton(ButtonGroup group) {
        for (Enumeration<AbstractButton> e = group.getElements(); e.hasMoreElements(); ) {
            JRadioButton b = (JRadioButton)e.nextElement();
            if (b.getModel() == group.getSelection()) {
                return b;
            }
        }
        return null;
    }

    // Unselects & disables all radio buttons in the specified group
    private static void setButtonsUnselected(ButtonGroup group) {
        for (Enumeration<AbstractButton> e = group.getElements(); e.hasMoreElements(); ) {
            JRadioButton b = (JRadioButton)e.nextElement();
            b.setSelected(false);
            b.setEnabled(false);
        }
    }

    // returns the radio button based on its display text
    private static JRadioButton getButtonByText(ButtonGroup group, String text) {
        for (Enumeration<AbstractButton> e = group.getElements(); e.hasMoreElements(); ) {
            JRadioButton b = (JRadioButton)e.nextElement();
            if (b.getText().equalsIgnoreCase(text)) {
                return b;
            }
        }
        return null;
    }
}
