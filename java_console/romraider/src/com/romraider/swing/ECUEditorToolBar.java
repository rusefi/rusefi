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

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JToolBar;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditor;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.util.SettingsManager;

public class ECUEditorToolBar extends JToolBar implements ActionListener {

    private static final long serialVersionUID = 7778170684606193919L;
    private final JButton openImage = new JButton();
    private final JButton saveImage = new JButton();
    private final JButton refreshImage = new JButton();
    private final JButton closeImage = new JButton();

    public ECUEditorToolBar(String name) {
        super(name);
        this.setFloatable(true);
        this.setRollover(true);
        FlowLayout toolBarLayout = new FlowLayout(FlowLayout.LEFT, 0, 0);
        this.setLayout(toolBarLayout);
        //this.setBorder(BorderFactory.createTitledBorder("Editor Tools"));

        this.updateIcons();

        this.add(openImage);
        this.add(saveImage);
        this.add(closeImage);
        this.add(refreshImage);

        openImage.setMaximumSize(new Dimension(58, 50));
        openImage.setBorder(createLineBorder(new Color(150, 150, 150), 0));
        saveImage.setMaximumSize(new Dimension(50, 50));
        saveImage.setBorder(createLineBorder(new Color(150, 150, 150), 0));
        closeImage.setMaximumSize(new Dimension(50, 50));
        closeImage.setBorder(createLineBorder(new Color(150, 150, 150), 0));
        refreshImage.setMaximumSize(new Dimension(50, 50));
        refreshImage.setBorder(createLineBorder(new Color(150, 150, 150), 0));

        this.updateButtons();

        openImage.addActionListener(this);
        saveImage.addActionListener(this);
        closeImage.addActionListener(this);
        refreshImage.addActionListener(this);
    }

    public void updateIcons() {
        int iconScale = getSettings().getEditorIconScale();
        openImage.setIcon(rescaleImageIcon(new ImageIcon(getClass().getResource("/graphics/icon-open.png")), iconScale));
        saveImage.setIcon(rescaleImageIcon(new ImageIcon(getClass().getResource("/graphics/icon-save.png")), iconScale));
        refreshImage.setIcon(rescaleImageIcon(new ImageIcon(getClass().getResource("/graphics/icon-refresh.png")), iconScale));
        closeImage.setIcon(rescaleImageIcon(new ImageIcon( getClass().getResource("/graphics/icon-close.png")), iconScale));
        repaint();
    }

    private ImageIcon rescaleImageIcon(ImageIcon imageIcon, int percentOfOriginal) {
        int newHeight = (int) (imageIcon.getImage().getHeight(this) * (percentOfOriginal * .01));
        int newWidth = (int) (imageIcon.getImage().getWidth(this) * (percentOfOriginal * .01));

        if(newHeight > 0 && newWidth > 0)
        {
            imageIcon.setImage(imageIcon.getImage().getScaledInstance(newWidth, newHeight, Image.SCALE_SMOOTH));
        }
        return imageIcon;
    }

    public void updateButtons() {
        String file = getEditor().getLastSelectedRomFileName();

        openImage.setToolTipText("Open Image");
        saveImage.setToolTipText("Save " + file + " As New Image...");
        refreshImage.setToolTipText("Refresh " + file + " from saved copy");
        closeImage.setToolTipText("Close " + file);

        if ("".equals(file)) {
            saveImage.setEnabled(false);
            refreshImage.setEnabled(false);
            closeImage.setEnabled(false);
        } else {
            saveImage.setEnabled(true);
            refreshImage.setEnabled(true);
            closeImage.setEnabled(true);
        }
        revalidate();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == openImage) {
            try {
                ((ECUEditorMenuBar) getEditor().getJMenuBar()).openImageDialog();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(getEditor(), new DebugPanel(ex,
                        getSettings().getSupportURL()), "Exception", JOptionPane.ERROR_MESSAGE);
            }
        } else if (e.getSource() == saveImage) {
            try {
                ((ECUEditorMenuBar) getEditor().getJMenuBar()).saveImage();
                getEditor().refreshUI();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(getEditor(), new DebugPanel(ex,
                        getSettings().getSupportURL()), "Exception", JOptionPane.ERROR_MESSAGE);
            }
        } else if (e.getSource() == closeImage) {
            getEditor().closeImage();
        } else if (e.getSource() == refreshImage) {
            try {
                ((ECUEditorMenuBar) getEditor().getJMenuBar()).refreshImage();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(getEditor(), new DebugPanel(ex,
                        getSettings().getSupportURL()), "Exception", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private Settings getSettings() {
        return SettingsManager.getSettings();
    }

    private ECUEditor getEditor() {
        return ECUEditorManager.getECUEditor();
    }
}
