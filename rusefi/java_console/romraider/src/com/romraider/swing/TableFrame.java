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

import static javax.swing.BorderFactory.createBevelBorder;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JInternalFrame;
import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.event.InternalFrameEvent;
import javax.swing.event.InternalFrameListener;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditor;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.logger.ecu.ui.handler.table.TableUpdateHandler;
import com.romraider.maps.Rom;
import com.romraider.maps.Table;

public class TableFrame extends JInternalFrame implements InternalFrameListener, ActionListener {

    private static final long serialVersionUID = -2651279694660392351L;
    private final Table table;
    private TableMenuBar tableMenuBar = null;

    public TableFrame(String title, Table table) {
        super(title, true, true);
        this.table = table;
        add(table);
        setFrameIcon(null);
        setBorder(createBevelBorder(0));
        if (System.getProperty("os.name").startsWith("Mac OS"))
            putClientProperty("JInternalFrame.isPalette", true);
        setVisible(false);
        tableMenuBar = new TableMenuBar(this);
        setJMenuBar(tableMenuBar);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        addInternalFrameListener(this);
    }

    @Override
    public void internalFrameActivated(InternalFrameEvent e) {
        ECUEditor parent = getEditor();
        parent.getTableToolBar().updateTableToolBar();
        parent.getToolBar().updateButtons();
        parent.getEditorMenuBar().updateMenu();
    }

    @Override
    public void internalFrameOpened(InternalFrameEvent e) {
        RegisterTable();
    }

    @Override
    public void internalFrameClosing(InternalFrameEvent e) {
        TableUpdateHandler.getInstance().deregisterTable(this.getTable());
    }

    @Override
    public void internalFrameClosed(InternalFrameEvent e) {
        getEditor().getTableToolBar().updateTableToolBar();
    }

    @Override
    public void internalFrameIconified(InternalFrameEvent e) {
        ;
    }

    @Override
    public void internalFrameDeiconified(InternalFrameEvent e) {
        ;
    }

    @Override
    public void internalFrameDeactivated(InternalFrameEvent e) {
        getEditor().getTableToolBar().updateTableToolBar();
    }

    public void RegisterTable() {
        TableUpdateHandler.getInstance().registerTable(this.getTable());
    }

    public Table getTable() {
        return table;
    }

    public ECUEditor getEditor() {
        return ECUEditorManager.getECUEditor();
    }

    public TableMenuBar getTableMenuBar() {
        return this.tableMenuBar;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        TableMenuBar menu = getTableMenuBar();

        if (e.getSource() == menu.getUndoAll()) {
            getTable().undoAll();

        } else if (e.getSource() == menu.getRevert()) {
            getTable().setRevertPoint();

        } else if (e.getSource() == menu.getUndoSel()) {
            getTable().undoSelected();

        } else if (e.getSource() == menu.getClose()) {
            ECUEditorManager.getECUEditor().removeDisplayTable(this);

        } else if (e.getSource() == menu.getTableProperties()) {
            JOptionPane.showMessageDialog(getTable(), new TablePropertyPanel(getTable()),
                    getTable().getName() + " Table Properties", JOptionPane.INFORMATION_MESSAGE);

        } else if (e.getSource() == menu.getCopySel()) {
            getTable().copySelection();

        } else if (e.getSource() == menu.getCopyTable()) {
            getTable().copyTable();

        } else if (e.getSource() == menu.getPaste()) {
            getTable().paste();

        } else if (e.getSource() == menu.getCompareOff()) {
            getTable().setCompareTable(null);
            getTable().setCompareValueType(Settings.DATA_TYPE_BIN);
            getTableMenuBar().getCompareToBin().setSelected(true);

        } else if (e.getSource() == menu.getCompareAbsolute()) {
            getTable().setCompareDisplay(Settings.COMPARE_DISPLAY_ABSOLUTE);

        } else if (e.getSource() == menu.getComparePercent()) {
            getTable().setCompareDisplay(Settings.COMPARE_DISPLAY_PERCENT);

        } else if (e.getSource() == menu.getCompareOriginal()) {
            getTable().setCompareValueType(Settings.DATA_TYPE_ORIGINAL);
            getTableMenuBar().getCompareToOriginal().setSelected(true);
            compareByTable(getTable());

        } else if (e.getSource() == menu.getCompareMap()) {
            JTableChooser chooser = new JTableChooser();
            Table selectedTable = chooser.showChooser(getTable());
            if(null != selectedTable) {
                compareByTable(selectedTable);
            }

        } else if (e.getSource() instanceof TableMenuItem) {
            Table selectedTable = findSimilarTable((TableMenuItem)e.getSource());
            if(null != e.getSource()) {
                compareByTable(selectedTable);
            }

        } else if (e.getSource() == menu.getCompareToOriginal()) {
            getTable().setCompareValueType(Settings.DATA_TYPE_ORIGINAL);
            getTable().refreshCompare();

        } else if (e.getSource() == menu.getCompareToBin()) {
            getTable().setCompareValueType(Settings.DATA_TYPE_BIN);
            getTable().refreshCompare();

        } else if (e.getSource() == menu.getInterp()) {
            getTable().interpolate();

        } else if (e.getSource() == menu.getVertInterp()) {
            getTable().verticalInterpolate();

        } else if (e.getSource() == menu.getHorizInterp()) {
            getTable().horizontalInterpolate();
        }
    }

    public void compareByTable(Table selectedTable) {
        if(null == selectedTable) {
            return;
        }
        getTable().setCompareTable(selectedTable);
        ECUEditorManager.getECUEditor().getTableToolBar().updateTableToolBar(getTable());
        getTable().populateCompareValues(selectedTable);
    }

    public void refreshSimilarOpenTables() {
        JMenu similarTables =  getTableMenuBar().getSimilarOpenTables();
        similarTables.removeAll();

        for(Rom rom : ECUEditorManager.getECUEditor().getImages()) {
            for(TableTreeNode tableNode : rom.getTableNodes()) {
                if(tableNode.getTable().getName().equalsIgnoreCase(getTable().getName())) {
                    JRadioButtonMenuItem similarTable = new TableMenuItem(rom.getFileName());
                    similarTable.setToolTipText(tableNode.getFrame().getTable().getName());
                    similarTable.addActionListener(this);
                    similarTables.add(similarTable);
                    break;
                }
            }
        }

        getTableMenuBar().initCompareGroup();
        getTableMenuBar().repaint();
    }

    private Table findSimilarTable(TableMenuItem menuItem) {
        for(Rom rom : ECUEditorManager.getECUEditor().getImages()) {
            if(menuItem.getText().equalsIgnoreCase(rom.getFileName())) {
                for(TableTreeNode treeNode : rom.getTableNodes()) {
                    if(menuItem.getToolTipText().equalsIgnoreCase(treeNode.getFrame().getTable().getName())) {
                        return treeNode.getFrame().getTable();
                    }
                }
            }
        }
        return null;
    }
}