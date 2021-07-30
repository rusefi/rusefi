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

import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JSeparator;

import com.romraider.Settings;
import com.romraider.maps.Table3D;

public class TableMenuBar extends JMenuBar {

    private static final long serialVersionUID = -695692646459410510L;
    private JMenu fileMenu;
    private JMenuItem graph;

    private JMenu compareMenu;
    private JRadioButtonMenuItem compareOriginal;
    private JRadioButtonMenuItem compareMap;
    private JMenu similarOpenTables;
    private JRadioButtonMenuItem compareOff;
    private JMenu compareDisplay;
    private JRadioButtonMenuItem comparePercent;
    private JRadioButtonMenuItem compareAbsolute;
    private JMenu compareToValue;
    private JRadioButtonMenuItem compareToOriginal;
    private JRadioButtonMenuItem compareToBin;

    private JMenuItem close;
    private JMenu editMenu;
    private JMenuItem undoSel;
    private JMenuItem undoAll;
    private JMenuItem revert;
    private JMenuItem copySel;
    private JMenuItem copyTable;
    private JMenuItem paste;
    private JMenuItem interp;
    private JMenuItem vertInterp;
    private JMenuItem horizInterp;
    private JMenu viewMenu;
    private JMenuItem tableProperties;

    private ButtonGroup compareGroup;
    private ButtonGroup compareDisplayGroup;
    private ButtonGroup compareToGroup;

    public TableMenuBar(TableFrame frame) {
        initFileMenu(frame);
        initEditMenu(frame);
        initViewMenu(frame);
        applyTableTypeRules(frame);
    }

    private void initFileMenu(TableFrame frame) {
        fileMenu = new JMenu("Table");
        graph = new JMenuItem("View Graph");
        compareMenu = new JMenu("Compare");
        setClose(new JMenuItem("Close Table"));

        initCompareMenu(frame);
        getClose().setText("Close " + frame.getTable().getName());

        graph.addActionListener(frame);
        getClose().addActionListener(frame);

        graph.setMnemonic('G');
        getClose().setMnemonic('X');
        graph.setEnabled(false);

        fileMenu.add(graph);
        fileMenu.add(compareMenu);
        fileMenu.add(new JSeparator());
        fileMenu.add(getClose());

        this.add(fileMenu);
    }

    private void initEditMenu(TableFrame frame) {
        editMenu = new JMenu("Edit");
        setUndoSel(new JMenuItem("Undo Selected Changes"));
        setUndoAll(new JMenuItem("Undo All Changes"));
        setRevert(new JMenuItem("Set Revert Point"));
        setCopySel(new JMenuItem("Copy Selection"));
        setCopyTable(new JMenuItem("Copy Table"));
        setPaste(new JMenuItem("Paste"));
        setVertInterp(new JMenuItem("Vertical Interpolate"));
        setHorizInterp(new JMenuItem("Horizontal Interpolate"));
        setInterp(new JMenuItem("Interpolate"));

        editMenu.add(getUndoSel());
        editMenu.add(getUndoAll());
        editMenu.add(getRevert());
        editMenu.add(new JSeparator());
        editMenu.add(getCopySel());
        editMenu.add(getCopyTable());
        editMenu.add(new JSeparator());
        editMenu.add(getPaste());
        editMenu.add(new JSeparator());
        if (frame.getTable() instanceof Table3D) {
            editMenu.add(getVertInterp());
            editMenu.add(getHorizInterp());
        }
        editMenu.add(getInterp());

        editMenu.setMnemonic('E');
        getUndoSel().setMnemonic('U');
        getUndoAll().setMnemonic('A');
        getRevert().setMnemonic('R');
        getCopySel().setMnemonic('C');
        getCopyTable().setMnemonic('T');
        getPaste().setMnemonic('P');
        getInterp().setMnemonic('I');
        getVertInterp().setMnemonic('V');
        getHorizInterp().setMnemonic('H');

        getUndoSel().addActionListener(frame);
        getUndoAll().addActionListener(frame);
        getRevert().addActionListener(frame);
        getCopySel().addActionListener(frame);
        getCopyTable().addActionListener(frame);
        getPaste().addActionListener(frame);
        getInterp().addActionListener(frame);
        getVertInterp().addActionListener(frame);
        getHorizInterp().addActionListener(frame);
        this.add(editMenu);
    }

    private void initViewMenu(TableFrame frame) {

        viewMenu = new JMenu("View");
        viewMenu.setMnemonic('V');

        setTableProperties(new JMenuItem("Table Properties"));
        getTableProperties().setToolTipText("Select to view the table properties.");
        getTableProperties().setMnemonic('P');
        getTableProperties().addActionListener(frame);

        fileMenu.setMnemonic('F');
        fileMenu.setMnemonic('T');

        viewMenu.add(getTableProperties());

        this.add(viewMenu);
    }

    private void initCompareMenu(TableFrame frame) {
        setCompareOriginal(new JRadioButtonMenuItem("Show Changes"));
        getCompareOriginal().setToolTipText("Compares the current values to the original or revert point values.");
        setCompareMap(new JRadioButtonMenuItem("Compare to Another Map"));
        getCompareMap().setToolTipText("Compares this table and a selected table.");
        setSimilarOpenTables(new JMenu("Compare to Table"));
        getSimilarOpenTables().setToolTipText("Compares this table to a similar table.");

        setCompareOff(new JRadioButtonMenuItem("Off"));

        setComparePercent(new JRadioButtonMenuItem("Percent Difference"));
        setCompareAbsolute(new JRadioButtonMenuItem("Absolute Difference"));
        compareDisplayGroup = new ButtonGroup();
        compareDisplayGroup.add(getComparePercent());
        compareDisplayGroup.add(getCompareAbsolute());
        compareDisplay = new JMenu("Display");
        compareDisplay.add(getComparePercent());
        compareDisplay.add(getCompareAbsolute());

        setCompareToOriginal(new JRadioButtonMenuItem("Compare to Original Value"));
        getCompareToOriginal().setToolTipText("Compares this table to the selected table's original or revert point values.");
        setCompareToBin(new JRadioButtonMenuItem("Compare to Bin Value"));
        getCompareToBin().setToolTipText("Compares this table to the selected table's current values.");
        compareToGroup = new ButtonGroup();
        compareToGroup.add(getCompareToOriginal());
        compareToGroup.add(getCompareToBin());
        compareToValue = new JMenu("Compare to");
        compareToValue.add(getCompareToOriginal());
        compareToValue.add(getCompareToBin());

        compareMenu.add(getCompareOriginal());
        compareMenu.add(getCompareMap());
        compareMenu.add(getSimilarOpenTables());
        compareMenu.add(getCompareOff());
        compareMenu.add(new JSeparator());
        compareMenu.add(compareDisplay);
        compareMenu.add(new JSeparator());
        compareMenu.add(compareToValue);

        compareMenu.setMnemonic('C');
        getCompareOriginal().setMnemonic('C');
        getCompareMap().setMnemonic('M');
        getCompareOff().setMnemonic('O');
        compareDisplay.setMnemonic('D');
        getComparePercent().setMnemonic('P');
        getCompareAbsolute().setMnemonic('A');
        getSimilarOpenTables().setMnemonic('S');
        compareToValue.setMnemonic('T');
        getCompareToOriginal().setMnemonic('R');
        getCompareToOriginal().setMnemonic('B');

        getCompareOff().setSelected(true);
        getCompareAbsolute().setSelected(true);
        getCompareToOriginal().setSelected(true);

        initCompareGroup();

        getCompareOriginal().addActionListener(frame);
        getCompareMap().addActionListener(frame);
        getCompareOff().addActionListener(frame);
        getComparePercent().addActionListener(frame);
        getCompareAbsolute().addActionListener(frame);
        getCompareToOriginal().addActionListener(frame);
        getCompareToBin().addActionListener(frame);
    }

    public void initCompareGroup() {
        compareGroup = new ButtonGroup();

        compareGroup.add(getCompareOriginal());
        compareGroup.add(getCompareMap());
        compareGroup.add(getCompareOff());

        for(int i = 0; i< getSimilarOpenTables().getItemCount(); i++) {
            compareGroup.add(getSimilarOpenTables().getItem(i));
        }
    }

    private void applyTableTypeRules(TableFrame frame) {
        // Hide items that don't work with a DTC tables.
        if(frame.getTable().getType() == Settings.TABLE_SWITCH) {
            editMenu.setEnabled(false);
            getCompareOriginal().setEnabled(false);
            getComparePercent().setEnabled(false);
            getCompareAbsolute().setEnabled(false);
            getCompareToOriginal().setEnabled(false);
            getCompareToBin().setEnabled(false);
        }
    }

    public JMenuItem getUndoAll() {
        return undoAll;
    }

    public void setUndoAll(JMenuItem undoAll) {
        this.undoAll = undoAll;
    }

    public JMenuItem getRevert() {
        return revert;
    }

    public void setRevert(JMenuItem revert) {
        this.revert = revert;
    }

    public JMenuItem getUndoSel() {
        return undoSel;
    }

    public void setUndoSel(JMenuItem undoSel) {
        this.undoSel = undoSel;
    }

    public JMenuItem getClose() {
        return close;
    }

    public void setClose(JMenuItem close) {
        this.close = close;
    }

    public JMenuItem getTableProperties() {
        return tableProperties;
    }
    public void setTableProperties(JMenuItem tableProperties) {
        this.tableProperties = tableProperties;
    }

    public JMenuItem getCopySel() {
        return copySel;
    }

    public void setCopySel(JMenuItem copySel) {
        this.copySel = copySel;
    }

    public JMenuItem getCopyTable() {
        return copyTable;
    }

    public void setCopyTable(JMenuItem copyTable) {
        this.copyTable = copyTable;
    }

    public JMenuItem getPaste() {
        return paste;
    }

    public void setPaste(JMenuItem paste) {
        this.paste = paste;
    }

    public JRadioButtonMenuItem getCompareOff() {
        return compareOff;
    }

    public void setCompareOff(JRadioButtonMenuItem compareOff) {
        this.compareOff = compareOff;
    }

    public JRadioButtonMenuItem getCompareAbsolute() {
        return compareAbsolute;
    }

    public void setCompareAbsolute(JRadioButtonMenuItem compareAbsolute) {
        this.compareAbsolute = compareAbsolute;
    }

    public JRadioButtonMenuItem getComparePercent() {
        return comparePercent;
    }

    public void setComparePercent(JRadioButtonMenuItem comparePercent) {
        this.comparePercent = comparePercent;
    }

    public JRadioButtonMenuItem getCompareOriginal() {
        return compareOriginal;
    }

    public void setCompareOriginal(JRadioButtonMenuItem compareOriginal) {
        this.compareOriginal = compareOriginal;
    }

    public JRadioButtonMenuItem getCompareToOriginal() {
        return compareToOriginal;
    }

    public void setCompareToOriginal(JRadioButtonMenuItem compareToOriginal) {
        this.compareToOriginal = compareToOriginal;
    }

    public JRadioButtonMenuItem getCompareMap() {
        return compareMap;
    }

    public void setCompareMap(JRadioButtonMenuItem compareMap) {
        this.compareMap = compareMap;
    }

    public JRadioButtonMenuItem getCompareToBin() {
        return compareToBin;
    }

    public void setCompareToBin(JRadioButtonMenuItem compareToBin) {
        this.compareToBin = compareToBin;
    }

    public JMenu getSimilarOpenTables() {
        return similarOpenTables;
    }

    public void setSimilarOpenTables(JMenu similarOpenTables) {
        this.similarOpenTables = similarOpenTables;
    }

    public JMenuItem getInterp() {
        return interp;
    }

    public void setInterp(JMenuItem interp) {
        this.interp = interp;
    }

    public JMenuItem getHorizInterp() {
        return this.horizInterp;
    }

    public void setHorizInterp(JMenuItem horizInterp) {
        this.horizInterp = horizInterp;
    }

    public JMenuItem getVertInterp() {
        return this.vertInterp;
    }

    public void setVertInterp(JMenuItem vertInterp) {
        this.vertInterp = vertInterp;
    }
}