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

package com.romraider.swing;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableColumn;

import com.romraider.Settings;
import com.romraider.maps.Scale;
import com.romraider.maps.Table;
import com.romraider.maps.TableSwitch;

public class TablePropertyPanel extends javax.swing.JPanel {

    private static final long serialVersionUID = -5817685772039277602L;

    public TablePropertyPanel(Table table) {
    initComponents();
    setVisible(true);

    category.setText(table.getCategory());

    int dim;
    if (Settings.TABLE_SWITCH == table.getType()) {
        dim = 1;
        storageSize.setText("switch");
        scrollPane.setViewportView(populateScalesTable(
                ((TableSwitch) table).getSwitchStates()));
    }
    else {
        if (Settings.STORAGE_TYPE_FLOAT == table.getStorageType()) {
            storageSize.setText("float");
        }
        else {
            String dataType;
            if (table.isSignedData()) {
                dataType = "int";
            }
            else {
                dataType = "uint";
            }
            storageSize.setText(dataType + (table.getStorageType() * 8));
        }
        dim = table.getType();
        scrollPane.setViewportView(populateScalesTable(table.getScales()));
    }

    tableName.setText(String.format("%s (%dD)", table.getName(), dim));
    storageAddress.setText("0x" + Integer.toHexString(table.getStorageAddress()));

    if (table.getEndian() == Settings.ENDIAN_BIG) {
        endian.setText("big");
    } else {
        endian.setText("little");
    }

    description.setText(table.getDescription());

    if (table.getUserLevel() == 1) {
        userLevel.setText("Beginner");
    } else if (table.getUserLevel() == 2) {
        userLevel.setText("Intermediate");
    } else if (table.getUserLevel() == 3) {
        userLevel.setText("Advanced");
    } else if (table.getUserLevel() == 4) {
        userLevel.setText("All");
    } else if (table.getUserLevel() == 5) {
        userLevel.setText("Debug");
    }

    logIDscrollPane.setViewportView(populateLogParamTable(table.getLogParamString()));
}

    // <editor-fold defaultstate="collapsed" desc=" Generated Code ">//GEN-BEGIN:initComponents
    private void initComponents() {
        lblTable = new javax.swing.JLabel();
        tableName = new javax.swing.JLabel();
        lblCategory = new javax.swing.JLabel();
        category = new javax.swing.JLabel();
        jPanel1 = new javax.swing.JPanel();
        jPanel2 = new javax.swing.JPanel();
        lblStorageAddress = new javax.swing.JLabel();
        lblStorageSize = new javax.swing.JLabel();
        lblEndian = new javax.swing.JLabel();
        endian = new javax.swing.JLabel();
        storageSize = new javax.swing.JLabel();
        storageAddress = new javax.swing.JLabel();
        jPanel3 = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        description = new javax.swing.JTextArea();
        jLabel5 = new javax.swing.JLabel();
        userLevel = new javax.swing.JLabel();
        scrollPane = new JScrollPane();
        logIDscrollPane = new JScrollPane();
        
        setAutoscrolls(true);
        setFont(new java.awt.Font("Tahoma", 0, 12));
        setInheritsPopupMenu(true);
        lblTable.setText("Table:");
        lblTable.setFocusable(false);

        tableName.setText("Tablename (3D)");
        tableName.setFocusable(false);

        lblCategory.setText("Category:");
        lblCategory.setFocusable(false);

        category.setText("Category");
        category.setFocusable(false);

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createTitledBorder("Conversions")));

        GroupLayout jPanel1Layout = new GroupLayout(jPanel1);
        jPanel1Layout.setHorizontalGroup(
                jPanel1Layout.createParallelGroup(Alignment.LEADING)
                .addComponent(scrollPane, GroupLayout.DEFAULT_SIZE, 170, Short.MAX_VALUE)
                );
        jPanel1Layout.setVerticalGroup(
                jPanel1Layout.createParallelGroup(Alignment.LEADING)
                .addComponent(scrollPane, GroupLayout.DEFAULT_SIZE, 75, Short.MAX_VALUE)
                );

        jPanel1.setLayout(jPanel1Layout);

        jPanel2.setBorder(javax.swing.BorderFactory.createTitledBorder("Storage"));
        lblStorageAddress.setText("Storage Address:");
        lblStorageAddress.setFocusable(false);

        lblStorageSize.setText("Data Type:");
        lblStorageSize.setFocusable(false);

        lblEndian.setText("Endian:");
        lblEndian.setFocusable(false);

        endian.setText("little");
        endian.setFocusable(false);

        storageSize.setText("unkn");
        storageSize.setFocusable(false);

        storageAddress.setText("0x00");
        storageAddress.setFocusable(false);

        org.jdesktop.layout.GroupLayout jPanel2Layout = new org.jdesktop.layout.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
                jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                .add(jPanel2Layout.createSequentialGroup()
                        .addContainerGap()
                        .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                .add(lblStorageAddress)
                                .add(lblStorageSize)
                                .add(lblEndian))
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                        .add(endian)
                                        .add(storageSize)
                                        .add(storageAddress))
                                        .addContainerGap(28, Short.MAX_VALUE))
                );
        jPanel2Layout.setVerticalGroup(
                jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                .add(jPanel2Layout.createSequentialGroup()
                        .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                .add(lblStorageSize)
                                .add(storageSize))
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                        .add(lblStorageAddress)
                                        .add(storageAddress))
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                        .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                                .add(lblEndian)
                                                .add(endian))
                                                .addContainerGap(37, Short.MAX_VALUE))
                );

        jPanel3.setBorder(javax.swing.BorderFactory.createTitledBorder("Description"));
        jScrollPane1.setBorder(null);
        description.setBackground(new java.awt.Color(236, 233, 216));
        description.setColumns(20);
        description.setEditable(false);
        description.setFont(new java.awt.Font("Tahoma", 0, 11));
        description.setLineWrap(true);
        description.setRows(5);
        description.setText("Description");
        description.setWrapStyleWord(true);
        description.setBorder(null);
        description.setOpaque(false);
        description.setRequestFocusEnabled(false);
        jScrollPane1.setViewportView(description);

        org.jdesktop.layout.GroupLayout jPanel3Layout = new org.jdesktop.layout.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
                jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                .add(org.jdesktop.layout.GroupLayout.TRAILING, jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 360, Short.MAX_VALUE)
                );
        jPanel3Layout.setVerticalGroup(
                jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                .add(jPanel3Layout.createSequentialGroup()
                        .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 102, Short.MAX_VALUE)
                        .addContainerGap())
                );

        jLabel5.setText("User Level:");

        userLevel.setText("Beginner");

        lblLogId = new JLabel("Log Param:");

        GroupLayout layout = new GroupLayout(this);
        layout.setHorizontalGroup(
                layout.createParallelGroup(Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(Alignment.LEADING)
                                .addGroup(layout.createSequentialGroup()
                                        .addGroup(layout.createParallelGroup(Alignment.LEADING)
                                                .addComponent(lblCategory)
                                                .addComponent(lblTable)
                                                .addComponent(lblLogId))
                                                .addPreferredGap(ComponentPlacement.RELATED)
                                                .addGroup(layout.createParallelGroup(Alignment.TRAILING)
                                                        .addComponent(tableName, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 370, Short.MAX_VALUE)
                                                        .addComponent(category, GroupLayout.DEFAULT_SIZE, 370, Short.MAX_VALUE)
                                                        .addComponent(logIDscrollPane, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 300, 300)))
                                                        .addGroup(layout.createSequentialGroup()
                                                                .addComponent(jLabel5)
                                                                .addPreferredGap(ComponentPlacement.RELATED)
                                                                .addComponent(userLevel, GroupLayout.DEFAULT_SIZE, 366, Short.MAX_VALUE))
                                                                .addComponent(jPanel1, GroupLayout.DEFAULT_SIZE, 430, Short.MAX_VALUE)
                                                                .addComponent(jPanel2, GroupLayout.DEFAULT_SIZE, 430, Short.MAX_VALUE)
                                                                .addComponent(jPanel3, GroupLayout.DEFAULT_SIZE, 430, Short.MAX_VALUE))
                                                                .addContainerGap())
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(Alignment.BASELINE)
                                .addComponent(tableName)
                                .addComponent(lblTable))
                                .addPreferredGap(ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(Alignment.BASELINE)
                                        .addComponent(lblCategory)
                                        .addComponent(category))
                                        .addGap(6)
                                        .addGroup(layout.createParallelGroup(Alignment.BASELINE)
                                                .addComponent(jLabel5)
                                                .addComponent(userLevel))
                                                .addPreferredGap(ComponentPlacement.RELATED)
                                                .addGroup(layout.createParallelGroup(Alignment.BASELINE)
                                                        .addComponent(lblLogId)
                                                        .addComponent(logIDscrollPane, GroupLayout.PREFERRED_SIZE, 75, GroupLayout.PREFERRED_SIZE))
                                                        .addGap(8)
                                                        .addComponent(jPanel1, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                                        .addPreferredGap(ComponentPlacement.RELATED)
                                                        .addComponent(jPanel2, GroupLayout.PREFERRED_SIZE, 89, GroupLayout.PREFERRED_SIZE)
                                                        .addPreferredGap(ComponentPlacement.RELATED)
                                                        .addComponent(jPanel3, GroupLayout.PREFERRED_SIZE, 93, GroupLayout.PREFERRED_SIZE)
                                                        .addGap(23))
        );
        this.setLayout(layout);
    }// </editor-fold>//GEN-END:initComponents
    private javax.swing.JLabel category;
    private javax.swing.JTextArea description;
    private javax.swing.JLabel endian;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel lblCategory;
    private javax.swing.JLabel lblEndian;
    private javax.swing.JLabel lblStorageAddress;
    private javax.swing.JLabel lblStorageSize;
    private javax.swing.JLabel lblTable;
    private javax.swing.JLabel storageAddress;
    private javax.swing.JLabel storageSize;
    private javax.swing.JLabel tableName;
    private javax.swing.JLabel userLevel;
    private JScrollPane scrollPane;
    private JLabel lblLogId;
    private JScrollPane logIDscrollPane;

    private JTable populateScalesTable(Vector<Scale> scales) {
        final ScalesTableModel scalesModel = new ScalesTableModel();
        scalesModel.setScalesList(scales);
        return createScalesTable(scalesModel);
    }

    private JTable populateScalesTable(Map<String, byte[]> switchStates) {
        final SwitchStateTableModel scalesModel = new SwitchStateTableModel();
        scalesModel.setScalesList(switchStates);
        return createScalesTable(scalesModel);
    }

    private JTable createScalesTable(DefaultTableModel tableModel) {
        final JTable table = new JTable(tableModel);
        table.setAutoCreateRowSorter(false);
        table.setColumnSelectionAllowed(false);
        table.setRowSelectionAllowed(true);
        table.setFillsViewportHeight(true);
        return table;
    }

    private JTable populateLogParamTable(String logParams) {
        final Map<String, String> paramMap = new HashMap<String, String>();
        final String[] paramEntries = logParams.split(", ");
        for (String entry : paramEntries) {
            final String[] entries = entry.split(":");
            if(!paramMap.containsKey(entries[0])){
                paramMap.put(entries[0], entries.length > 1 ? entries[1] : "n/a");
            }
        }
        final ParameterIdsTableModel tableModel = new ParameterIdsTableModel();
        tableModel.setParameterList(paramMap);
        final JTable table = new JTable(tableModel);
        TableColumn column = null;
        for (int i = 0; i < table.getColumnCount(); i++) {
            column = table.getColumnModel().getColumn(i);
            if (i == 0) {
                column.setPreferredWidth(240);
            } else {
                column.setPreferredWidth(80);
            }
        }
        table.setAutoCreateRowSorter(false);
        table.setColumnSelectionAllowed(false);
        table.setRowSelectionAllowed(true);
        table.setFillsViewportHeight(true);
        return table;
    }
}
