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

import com.romraider.maps.Rom;

public class RomPropertyPanel extends javax.swing.JPanel {

    private static final long serialVersionUID = 5583360728106071942L;
    Rom rom = new Rom();

    public RomPropertyPanel(Rom rom) {
        initComponents();

        // populate fields
        fileName.setText(rom.getFileName());
        xmlID.setText(rom.getRomID().getXmlid());
        ecuVersion.setText(rom.getRomID().getCaseId());
        fileSize.setText((rom.getRealFileSize() / 1024) + "kb");
        internalID.setText(rom.getRomID().getInternalIdString());
        storageAddress.setText("0x" + Integer.toHexString(rom.getRomID().getInternalIdAddress()));

        make.setText(rom.getRomID().getMake());
        market.setText(rom.getRomID().getMarket());
        year.setText(rom.getRomID().getYear() + "");
        model.setText(rom.getRomID().getModel());
        submodel.setText(rom.getRomID().getSubModel());
        transmission.setText(rom.getRomID().getTransmission());
        editStamp.setText(rom.getRomID().getEditStamp());

        tableList.setListData(rom.getTables());
    }

    // <editor-fold defaultstate="collapsed" desc=" Generated Code ">//GEN-BEGIN:initComponents
    private void initComponents() {
        lblFilename = new javax.swing.JLabel();
        fileName = new javax.swing.JLabel();
        lblECURevision = new javax.swing.JLabel();
        xmlID = new javax.swing.JLabel();
        lblFilesize = new javax.swing.JLabel();
        fileSize = new javax.swing.JLabel();
        lblEcuVersion = new javax.swing.JLabel();
        ecuVersion = new javax.swing.JLabel();
        lblInternalId = new javax.swing.JLabel();
        internalID = new javax.swing.JLabel();
        lblStorageAddress = new javax.swing.JLabel();
        storageAddress = new javax.swing.JLabel();
        lblMake = new javax.swing.JLabel();
        lblMarket = new javax.swing.JLabel();
        lblTransmission = new javax.swing.JLabel();
        lblModel = new javax.swing.JLabel();
        lblSubmodel = new javax.swing.JLabel();
        lblYear = new javax.swing.JLabel();
        make = new javax.swing.JLabel();
        market = new javax.swing.JLabel();
        year = new javax.swing.JLabel();
        model = new javax.swing.JLabel();
        submodel = new javax.swing.JLabel();
        transmission = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        tableList = new javax.swing.JList();
        lblTables = new javax.swing.JLabel();
        lblEditStamp = new javax.swing.JLabel();
        editStamp = new javax.swing.JLabel();

        lblEditStamp.setText("Edit Stamp:");
        
        editStamp.setText("stamp");
        
        lblFilename.setText("Filename:");

        fileName.setText("Filename");

        lblECURevision.setText("ECU Revision:");

        xmlID.setText("XMLID");

        lblFilesize.setText("Filesize:");

        fileSize.setText("999kb");

        lblEcuVersion.setText("ECU Version:");

        ecuVersion.setText("ECUVER");

        lblInternalId.setText("Internal ID:");

        internalID.setText("INTERNAL");

        lblStorageAddress.setText("ID Storage Address:");

        storageAddress.setText("0x00");

        lblMake.setText("Make:");

        lblMarket.setText("Market:");

        lblTransmission.setText("Transmission:");

        lblModel.setText("Model:");

        lblSubmodel.setText("Submodel:");

        lblYear.setText("Year:");

        make.setText("Make");

        market.setText("Market");

        year.setText("Year");

        model.setText("Model");

        submodel.setText("Submodel");

        transmission.setText("Transmission");

        tableList.setModel(new javax.swing.AbstractListModel() {
            /**
             *
             */
            private static final long serialVersionUID = -8498656966410761726L;
            String[] strings = {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5"};

            public int getSize() {
                return strings.length;
            }

            public Object getElementAt(int i) {
                return strings[i];
            }
        });
        jScrollPane1.setViewportView(tableList);

        lblTables.setText("Tables:");

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
                layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                        .add(layout.createSequentialGroup()
                        .addContainerGap()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                .add(layout.createSequentialGroup()
                                        .add(lblFilename)
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                        .add(fileName, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 302, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                                .add(layout.createSequentialGroup()
                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                        .add(layout.createSequentialGroup()
                                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                                        .add(lblECURevision)
                                                        .add(lblEcuVersion)
                                                        .add(lblFilesize))
                                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                                .add(fileSize)
                                                .add(ecuVersion)
                                                .add(xmlID)))
                                        .add(layout.createSequentialGroup()
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                                .add(lblYear)
                                                .add(lblModel)
                                                .add(lblSubmodel)
                                                .add(lblTransmission)
                                                .add(lblMarket)
                                                .add(lblMake))
                                        .add(7, 7, 7)
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                        .add(make)
                                        .add(market)
                                        .add(year)
                                        .add(layout.createSequentialGroup()
                                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                                .add(transmission)
                                                .add(submodel)))
                                        .add(model))))
                                .add(32, 32, 32)
                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                .add(layout.createSequentialGroup()
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                                .add(lblInternalId)
                                                .add(lblStorageAddress)
                                                .add(lblEditStamp))
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 53, Short.MAX_VALUE)
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                                .add(internalID)
                                                .add(storageAddress)
                                                .add(editStamp))
                                        .add(36, 36, 36))
                                .add(lblTables)
                                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 226, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))))
                        .addContainerGap())
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                        .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                        .add(21, 21, 21)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                        .add(lblFilename)
                                        .add(fileName))
                                .add(layout.createSequentialGroup()
                                .add(40, 40, 40)
                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                        .add(lblECURevision)
                                        .add(xmlID)
                                        .add(lblInternalId)
                                        .add(internalID))
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                        .add(ecuVersion)
                                        .add(lblEcuVersion)
                                        .add(storageAddress)
                                        .add(lblStorageAddress))
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                .add(lblFilesize)
                                .add(fileSize)
                                .add(lblEditStamp)
                                .add(editStamp))))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(lblTables)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                                .add(layout.createSequentialGroup()
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                                .add(lblMake)
                                                .add(make))
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                                .add(lblMarket)
                                                .add(market))
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                                .add(lblYear)
                                                .add(year))
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                                .add(lblModel)
                                                .add(model))
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                                .add(lblSubmodel)
                                                .add(submodel))
                                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                                        .add(lblTransmission)
                                        .add(transmission)))
                                .add(jScrollPane1, 0, 0, Short.MAX_VALUE))
                        .addContainerGap())
        );
    }// </editor-fold>//GEN-END:initComponents

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel ecuVersion;
    private javax.swing.JLabel fileName;
    private javax.swing.JLabel fileSize;
    private javax.swing.JLabel internalID;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel lblECURevision;
    private javax.swing.JLabel lblEcuVersion;
    private javax.swing.JLabel lblFilename;
    private javax.swing.JLabel lblFilesize;
    private javax.swing.JLabel lblInternalId;
    private javax.swing.JLabel lblMake;
    private javax.swing.JLabel lblMarket;
    private javax.swing.JLabel lblModel;
    private javax.swing.JLabel lblStorageAddress;
    private javax.swing.JLabel lblSubmodel;
    private javax.swing.JLabel lblTables;
    private javax.swing.JLabel lblTransmission;
    private javax.swing.JLabel lblYear;
    private javax.swing.JLabel make;
    private javax.swing.JLabel market;
    private javax.swing.JLabel model;
    private javax.swing.JLabel storageAddress;
    private javax.swing.JLabel submodel;
    private javax.swing.JList tableList;
    private javax.swing.JLabel transmission;
    private javax.swing.JLabel xmlID;
    private javax.swing.JLabel year;
    private javax.swing.JLabel lblEditStamp;
    private javax.swing.JLabel editStamp;
    // End of variables declaration//GEN-END:variables

}