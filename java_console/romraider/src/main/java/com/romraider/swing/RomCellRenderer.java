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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeCellRenderer;

import com.romraider.Settings;
import com.romraider.maps.Rom;
import com.romraider.maps.Table;

public class RomCellRenderer implements TreeCellRenderer {

    JLabel fileName;
    JLabel carInfo;
    DefaultTreeCellRenderer defaultRenderer = new DefaultTreeCellRenderer();

    public RomCellRenderer() {
        fileName = new JLabel(" ");
        fileName.setFont(new Font("Tahoma", Font.BOLD, 11));
        fileName.setHorizontalAlignment(JLabel.CENTER);

        carInfo = new JLabel(" ");
        carInfo.setFont(new Font("Tahoma", Font.PLAIN, 10));
        carInfo.setHorizontalAlignment(JLabel.CENTER);

    }

    @Override
    public Component getTreeCellRendererComponent(JTree tree, Object value,
            boolean selected, boolean expanded, boolean leaf, int row,
            boolean hasFocus) {

        Component returnValue = null;

        if (value != null && value instanceof Rom) {
            Rom rom = ((Rom) value);

            if (expanded) {
                fileName.setText("- " + rom.getFileName());
            } else {
                fileName.setText("+ " + rom.getFileName());
            }


            carInfo.setText(rom.getRomIDString()
//                            + ", " +
//                    rom.getRomID().getCaseId() + "; " +
//                    rom.getRomID().getYear() + " " +
//                    rom.getRomID().getMake() + " " +
//                    rom.getRomID().getModel() + " " +
//                    rom.getRomID().getSubModel() + ", " +
//                    rom.getRomID().getTransmission()
                    );

            JPanel renderer = new JPanel(new GridLayout(2, 1));
            renderer.add(fileName);
            renderer.add(carInfo);

            if (selected) {
                renderer.setBackground(new Color(220, 220, 255));
                renderer.setBorder(createLineBorder(new Color(0, 0, 225)));

            } else {
                renderer.setBorder(createLineBorder(new Color(220, 0, 0)));
                renderer.setBackground(new Color(255, 210, 210));
            }

            renderer.setPreferredSize(new Dimension(tree.getParent().getWidth(), 30));
            renderer.setMaximumSize(new Dimension(tree.getParent().getWidth(), 30));
            renderer.setEnabled(tree.isEnabled());
            returnValue = renderer;
        } else if (value != null && value instanceof TableTreeNode) {

            Table table = ((TableFrame)((DefaultMutableTreeNode) value).getUserObject()).getTable();
            JPanel renderer = new JPanel(new GridLayout(1, 1));
            renderer.setBorder(createLineBorder(Color.WHITE));
            JLabel tableName = new JLabel("");
            renderer.setBackground(Color.WHITE);

            // display icon
            if (table.getType() == Settings.TABLE_1D) {
                tableName = new JLabel(table.getName() + " ", new ImageIcon(getClass().getResource( "/graphics/1d.gif" )), JLabel.LEFT);
            } else if (table.getType() == Settings.TABLE_2D) {
                tableName = new JLabel(table.getName() + " ", new ImageIcon(getClass().getResource( "/graphics/2d.gif" )), JLabel.LEFT);
            } else if (table.getType() == Settings.TABLE_3D) {
                tableName = new JLabel(table.getName() + " ", new ImageIcon(getClass().getResource( "/graphics/3d.gif" )), JLabel.LEFT);
            } else if (table.getType() == Settings.TABLE_SWITCH) {
                tableName = new JLabel(table.getName() + " ", new ImageIcon(getClass().getResource( "/graphics/switch.gif" )), JLabel.LEFT);
            }

            // set color
            renderer.add(tableName);
            tableName.setFont(new Font("Tahoma", Font.PLAIN, 11));

            if (selected) {
                renderer.setBackground(new Color(220, 220, 255));
                renderer.setBorder(createLineBorder(new Color(0, 0, 225)));
            }

            if (table.getUserLevel() == 5) {
                tableName.setForeground(new Color(255, 150, 150));
                tableName.setFont(new Font("Tahoma", Font.ITALIC, 11));

            } else if (table.getUserLevel() > table.getSettings().getUserLevel()) {
                //tableName.setForeground(new Color(185, 185, 185));
                tableName.setFont(new Font("Tahoma", Font.ITALIC, 11));

            }

            returnValue = renderer;
        }

        if (returnValue == null) {
            returnValue = defaultRenderer.getTreeCellRendererComponent(tree,
                    value, selected, expanded, leaf, row, hasFocus);
        }

        return returnValue;

    }
}
