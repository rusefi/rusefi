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

import static javax.swing.BorderFactory.createLineBorder;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditor;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.maps.Rom;
import com.romraider.util.SettingsManager;
import com.opensr5.ConfigurationImage;
import com.rusefi.UploadChanges;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;

public class ECUEditorToolBar extends JToolBar {

    private static final long serialVersionUID = 7778170684606193919L;
    //    private final JButton saveImage = new JButton();
    private final JButton uploadImage = new JButton();
    private final JButton downloadImage = new JButton();
    private final JButton burnImage = new JButton();

    public ECUEditorToolBar(String name) {
        super(name);
        this.setFloatable(true);
        this.setRollover(true);
        FlowLayout toolBarLayout = new FlowLayout(FlowLayout.LEFT, 0, 0);
        this.setLayout(toolBarLayout);
        //this.setBorder(BorderFactory.createTitledBorder("Editor Tools"));

        this.updateIcons();

//        this.add(saveImage);
        this.add(downloadImage);
        this.add(uploadImage);
        this.add(burnImage);

        uploadImage.setToolTipText("Send changes to controller");
        downloadImage.setToolTipText("Read configuration from controller");


//        saveImage.setMaximumSize(new Dimension(50, 50));
//        saveImage.setBorder(createLineBorder(new Color(150, 150, 150), 0));
        downloadImage.setMaximumSize(new Dimension(50, 50));
        downloadImage.setBorder(
                BorderFactory.createCompoundBorder(BorderFactory.createEmptyBorder(0, 5, 0, 5),
                createLineBorder(new Color(150, 150, 150), 0)));
        uploadImage.setMaximumSize(new Dimension(50, 50));
        uploadImage.setBorder(createLineBorder(new Color(150, 150, 150), 0));

        this.updateButtons();

//        saveImage.addActionListener(this);
//        closeImage.addActionListener(this);
        uploadImage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Rom lastSelectedRom = ECUEditorManager.getECUEditor().getLastSelectedRom();
                byte[] newVersion = ConfigurationImage.extractContent(lastSelectedRom.saveFile());
                System.out.println("new version size: " + newVersion.length);
                throw new UnsupportedOperationException("disabled");
                //UploadChanges.scheduleUpload(new ConfigurationImage(newVersion));
            }
        });
        downloadImage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

            }
        });
        burnImage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                throw new UnsupportedOperationException("Disabled");
                //CommandQueue.getInstance().write(Fields.CMD_WRITECONFIG);
            }
        });
    }

    public void updateIcons() {
        int iconScale = getSettings().getEditorIconScale();
//        saveImage.setIcon(rescaleImageIcon(new ImageIcon(getClass().getResource("/graphics/icon-save.png")), iconScale));
        uploadImage.setIcon(rescaleImageIcon(new ImageIcon(getClass().getResource( "/upload48.jpg" )), iconScale));
        downloadImage.setIcon(rescaleImageIcon(new ImageIcon(getClass().getResource( "/download48.jpg" )), iconScale));
        burnImage.setIcon(rescaleImageIcon(new ImageIcon(getClass().getResource( "/writeconfig48.jpg" )), iconScale));
//        closeImage.setIcon(rescaleImageIcon(new ImageIcon( getClass().getResource("/graphics/icon-close.png")), iconScale));
        repaint();
    }

    private ImageIcon rescaleImageIcon(ImageIcon imageIcon, int percentOfOriginal) {
        int newHeight = (int) (imageIcon.getImage().getHeight(this) * (percentOfOriginal * .01));
        int newWidth = (int) (imageIcon.getImage().getWidth(this) * (percentOfOriginal * .01));

        if (newHeight > 0 && newWidth > 0) {
            imageIcon.setImage(imageIcon.getImage().getScaledInstance(newWidth, newHeight, Image.SCALE_SMOOTH));
        }
        return imageIcon;
    }

    public void updateButtons() {
//        String file = getEditor().getLastSelectedRomFileName();
//
////        saveImage.setToolTipText("Save " + file + " As New Image...");
////        closeImage.setToolTipText("Close " + file);
//
//        if ("".equals(file)) {
////            saveImage.setEnabled(false);
//            uploadImage.setEnabled(false);
//            closeImage.setEnabled(false);
//        } else {
//            saveImage.setEnabled(true);
//            uploadImage.setEnabled(true);
//            closeImage.setEnabled(true);
//        }
        revalidate();
    }

    private Settings getSettings() {
        return SettingsManager.getSettings();
    }

    private ECUEditor getEditor() {
        return ECUEditorManager.getECUEditor();
    }
}
