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

import java.awt.Dimension;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Vector;

import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.ScrollPaneConstants;
import javax.swing.SwingUtilities;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;

import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.maps.Rom;
import com.romraider.maps.Table;

public class JTableChooser extends JOptionPane implements MouseListener {
    public JTableChooser() {
    }

    private static final long serialVersionUID = 5611729002131147882L;
    JPanel displayPanel = new JPanel();
    DefaultMutableTreeNode rootNode = new DefaultMutableTreeNode("Open Images");
    JTree displayTree = new JTree(rootNode);
    JScrollPane displayScrollPane;

    public Table showChooser(Table targetTable) {
        Vector<Rom> roms = ECUEditorManager.getECUEditor().getImages();
        int nameLength = 0;

        for (int i = 0; i < roms.size(); i++) {
            Rom rom = roms.get(i);
            DefaultMutableTreeNode romNode = new DefaultMutableTreeNode(rom.getFileName());
            rootNode.add(romNode);

            for (TableTreeNode tableTreeNode : rom.getTableNodes()) {
                Table table = tableTreeNode.getTable();
                // use the length of the table name to set the width of the displayTree
                // so the entire name can be read without being cut off on the right
                if (table.getName().length() > nameLength) {
                    nameLength = table.getName().length();
                }
                TableChooserTreeNode tableNode = new TableChooserTreeNode(table.getName(), table);

                // categories
                boolean categoryExists = false;
                for (int k = 0; k < romNode.getChildCount(); k++) {
                    if (romNode.getChildAt(k).toString().equalsIgnoreCase(table.getCategory())) {
                        ((DefaultMutableTreeNode) romNode.getChildAt(k)).add(tableNode);
                        categoryExists = true;
                        break;
                    }
                }

                if (!categoryExists) {
                    DefaultMutableTreeNode categoryNode = new DefaultMutableTreeNode(table.getCategory());
                    romNode.add(categoryNode);
                    categoryNode.add(tableNode);
                }
            }
        }

        displayTree.setPreferredSize(new Dimension(nameLength*7, 400));
        displayTree.setMinimumSize(new Dimension(nameLength*7, 400));

        displayTree.expandPath(new TreePath(rootNode.getPath()));
        displayTree.setRootVisible(false);

        displayTree.addMouseListener(this);
        displayScrollPane = new JScrollPane(displayTree);
        displayScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        displayPanel.add(displayScrollPane);

        Object[] values = {"Compare", "Cancel"};

        if ((showOptionDialog(SwingUtilities.windowForComponent(targetTable), displayPanel, "Select a Map", JOptionPane.DEFAULT_OPTION,
                JOptionPane.PLAIN_MESSAGE, null, values, values[0]) == 0
                && (displayTree.getLastSelectedPathComponent() instanceof TableChooserTreeNode))) {
            return ((TableChooserTreeNode) displayTree.getLastSelectedPathComponent()).getTable();
        } else {
            return null;
        }
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        displayTree.setPreferredSize(new Dimension(displayTree.getWidth(), (displayTree.getRowCount()*displayTree.getRowHeight())));
        displayTree.revalidate();
    }
    @Override
    public void mouseClicked(MouseEvent e){}
    @Override
    public void mouseEntered(MouseEvent e){}
    @Override
    public void mouseExited(MouseEvent e){}
    @Override
    public void mousePressed(MouseEvent e){}

}