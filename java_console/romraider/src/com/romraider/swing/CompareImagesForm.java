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

import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListCellRenderer;
import javax.swing.ListSelectionModel;
import javax.swing.border.EmptyBorder;
import javax.swing.border.EtchedBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.maps.Rom;
import com.romraider.maps.Table;

public class CompareImagesForm extends JFrame implements ActionListener {

    private static final long serialVersionUID = -8937472127815934398L;
    private final Vector<Rom> roms;
    private final JPanel contentPane;
    private final JComboBox comboBoxImageLeft;
    private final JComboBox comboBoxImageRight;
    private final JButton btnCompare;
    private final JList listChanges;
    private final DefaultListModel listModelChanges = new DefaultListModel();
    private final ChangeListCellRenderer changeRenderer = new ChangeListCellRenderer();
    private final JScrollPane scrollPaneResults;
    private final JLabel lblImageResultString;

    public CompareImagesForm(Vector<Rom> roms, Image parentImage) {
        this.setIconImage(parentImage);
        setResizable(false);
        this.roms = roms;

        setTitle("Compare Images");

        setBounds(100, 100, 600, 450);
        this.contentPane = new JPanel();
        this.contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
        setContentPane(this.contentPane);

        JLabel lblSelectImages = new JLabel("Selected Images");
        lblSelectImages.setBounds(10, 11, 79, 14);

        contentPane.setLayout(null);

        JPanel panelImageSelector = new JPanel();
        panelImageSelector.setBounds(10, 36, 574, 94);
        panelImageSelector.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
        panelImageSelector.setLayout(null);

        this.comboBoxImageLeft = new JComboBox();
        this.comboBoxImageLeft.setBounds(10, 7, 554, 20);
        this.comboBoxImageLeft.setToolTipText("Select an image to compare.");
        this.comboBoxImageLeft.setRenderer( new ComboBoxRenderer() );
        panelImageSelector.add(this.comboBoxImageLeft);

        this.comboBoxImageRight = new JComboBox();
        this.comboBoxImageRight.setBounds(10, 32, 554, 20);
        this.comboBoxImageRight.setToolTipText("Select an image to compare.");
        this.comboBoxImageRight.setRenderer( new ComboBoxRenderer() );
        panelImageSelector.add(this.comboBoxImageRight);

        this.btnCompare = new JButton("Compare");
        this.btnCompare.addActionListener(this);
        this.btnCompare.setBounds(10, 64, 89, 23);
        panelImageSelector.add(this.btnCompare);
        this.contentPane.add(panelImageSelector);
        this.contentPane.add(lblSelectImages);

        JLabel lblResults = new JLabel("Results:");
        lblResults.setBounds(10, 141, 46, 14);
        contentPane.add(lblResults);

        lblImageResultString = new JLabel("Compare images...");
        lblImageResultString.setBounds(66, 141, 518, 14);
        contentPane.add(lblImageResultString);
        scrollPaneResults = new JScrollPane();
        scrollPaneResults.setBounds(10, 166, 574, 245);
        contentPane.add(scrollPaneResults);

        this.listChanges = new JList(this.listModelChanges);
        scrollPaneResults.setViewportView(this.listChanges);
        listChanges.setCellRenderer(changeRenderer);
        listChanges.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        listChanges.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent arg0) {
                if (!arg0.getValueIsAdjusting()) {
                    ListItem selectedTableName = (ListItem) listChanges.getSelectedValue();
                    String tableName = selectedTableName.getValue();
                    Rom leftRom = (Rom) comboBoxImageLeft.getSelectedItem();
                    Rom rightRom = (Rom) comboBoxImageRight.getSelectedItem();

                    // Display Tables
                    TableTreeNode leftNode = findAndShowTable(leftRom, tableName);
                    TableTreeNode rightNode = findAndShowTable(rightRom, tableName);

                    // Set Comparison
                    if(leftNode != null && rightNode != null) {
                        leftNode.getFrame().compareByTable(rightNode.getTable());
                        // Update menu bar
                        for(int i = 0; i< leftNode.getFrame().getTableMenuBar().getSimilarOpenTables().getItemCount(); i++) {
                            JMenuItem leftItem = leftNode.getFrame().getTableMenuBar().getSimilarOpenTables().getItem(i);
                            String romFileName = leftItem.getText();
                            if(rightRom.getFileName().equalsIgnoreCase(romFileName)) {
                                leftItem.setSelected(true);
                                break;
                            }
                        }
                    }
                }
            }
        });
        populateComboBoxes();
    }

    private TableTreeNode findAndShowTable(Rom rom, String tableName) {
        for(TableTreeNode node : rom.getTableNodes()) {
            if(node != null && node.getTable().getName().equals(tableName)){
                ECUEditorManager.getECUEditor().displayTable(node.getFrame());
                return node;
            }
        }
        return null;
    }

    public void populateComboBoxes()
    {
        for(int i=0; i<roms.size(); i++) {
            Rom curRom = roms.get(i);
            comboBoxImageLeft.addItem(curRom);
            comboBoxImageRight.addItem(curRom);
        }
        if(comboBoxImageRight.getItemCount() > 1) {
            comboBoxImageRight.setSelectedIndex(1);
        }
    }

    public void compareTables(Rom left, Rom right)
    {
        listModelChanges.clear();

        int equal = 0;
        int different = 0;
        int missing = 0;

        for(TableTreeNode leftNode : left.getTableNodes())
        {
            Boolean found = false;
            Table leftTable = leftNode.getTable();
            for(TableTreeNode rightNode : right.getTableNodes())
            {
                Table rightTable = rightNode.getTable();

                if(leftTable.getName().equalsIgnoreCase(rightTable.getName()))
                {
                    if(leftTable.equals(rightTable)) {
                        equal++;
                        listModelChanges.addElement(new ListItem(1, leftTable.getName()));
                    }
                    else {
                        different++;
                        listModelChanges.addElement(new ListItem(2, leftTable.getName()));
                    }
                    found = true;
                    break;
                }
            }

            if(!found) {
                missing++;
                listModelChanges.addElement(new ListItem(3, leftTable.getName()));
            }
        }

        // Check if rightTables has tables that do not exist in left table.
        for(TableTreeNode rightFrame : right.getTableNodes()) {
            Boolean found = false;
            for(TableTreeNode leftFrame : left.getTableNodes()) {
                if(leftFrame.getTable().getName().equalsIgnoreCase(rightFrame.getTable().getName()))
                {
                    found = true;
                    break;
                }
            }

            if(!found) {
                missing++;
                listModelChanges.addElement(new ListItem(3, rightFrame.getTable().getName()));
            }
        }

        // Fill out the result string.
        if(equal > 0 && different == 0 && missing == 0) {
            lblImageResultString.setText("Images are equal.");
            lblImageResultString.setForeground(Settings.TABLE_EQUAL_COLOR);
        } else if(different > 0) {
            lblImageResultString.setText("Images are NOT equal.  Equal Tables: "+equal+", Changed Tables: "+different+", Missing Tables: "+missing);
            lblImageResultString.setForeground(Settings.TABLE_DIFFERENT_COLOR);
        } else {
            lblImageResultString.setText("Images are NOT equal.  Equal Tables: "+equal+", Changed Tables: "+different+", Missing Tables: "+missing);
            lblImageResultString.setForeground(Settings.TABLE_MISSING_COLOR);
        }

        // Check if the list has items.
        if(listModelChanges.size() < 1)
        {
            listModelChanges.addElement(new ListItem(0, "No tables are equal, different, or missing."));
            lblImageResultString.setText("Unable to compare images.");
            lblImageResultString.setForeground(Color.RED);
            return;
        }

        // Add list items for 0 counts.
        if(equal == 0)
        {
            listModelChanges.addElement(new ListItem(1, "No Equal Tables."));
        }

        if(different == 0)
        {
            listModelChanges.addElement(new ListItem(2, "No Changed Tables."));
        }

        if(missing == 0)
        {
            listModelChanges.addElement(new ListItem(3, "No Missing Tables."));
        }

    }

    @Override
    public void actionPerformed(ActionEvent e) {
        this.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        if (e.getSource() == this.btnCompare) {
            if(this.comboBoxImageLeft.getItemCount() > 0 && this.comboBoxImageRight.getItemCount() > 0)
            {
                Rom leftRom = (Rom)this.comboBoxImageLeft.getSelectedItem();
                Rom rightRom = (Rom)this.comboBoxImageRight.getSelectedItem();
                if(leftRom != null && rightRom != null)
                {
                    compareTables(leftRom, rightRom);
                }
            }
        }
        this.setCursor(Cursor.getDefaultCursor());
    }

    class ComboBoxRenderer extends JLabel implements ListCellRenderer
    {

        private static final long serialVersionUID = 831689602429105854L;

        public ComboBoxRenderer() {
            setOpaque(true);
            setHorizontalAlignment(LEFT);
            setVerticalAlignment(CENTER);
        }

        @Override
        public Component getListCellRendererComponent(JList list, Object value,
                int index, boolean isSelected, boolean cellHasFocus) {

            if(isSelected) {
                setBackground(list.getSelectionBackground());
                setForeground(list.getSelectionForeground());
            } else {
                setBackground(list.getBackground());
                setForeground(list.getForeground());
            }

            if(value != null)
            {
                // Set the text to the rom file name.
                Rom rom = (Rom)value;
                setText(rom.getFileName());
                setFont(list.getFont());
            }
            return this;
        }
    }

    class ChangeListCellRenderer extends JLabel implements ListCellRenderer {

        private static final long serialVersionUID = -3645192077787635239L;

        public ChangeListCellRenderer()
        {
            setOpaque(true);
            setHorizontalAlignment(LEFT);
            setVerticalAlignment(CENTER);
        }

        @Override
        public Component getListCellRendererComponent(JList paramList, Object value,
                int index, boolean isSelected, boolean cellHasFocus) {

            // Set the background color.
            if(isSelected) {
                setBackground(paramList.getSelectionBackground());
            } else {
                setBackground(paramList.getBackground());
            }

            // Set the foreground color based on the item type.
            ListItem item = (ListItem)value;
            switch(item.getType()) {
            case 1:
                // equal - default green
                setForeground(Settings.TABLE_EQUAL_COLOR);
                break;
            case 2:
                // different - default red
                setForeground(Settings.TABLE_DIFFERENT_COLOR);
                break;
            case 3:
                // missing - default yellow
                setForeground(Settings.TABLE_MISSING_COLOR);
                break;
            default:
                setForeground(paramList.getForeground());
                break;
            }
            setText(item.getValue());
            return this;
        }
    }

    class ListItem {

        private int type;
        private String value;

        public ListItem(int type, String value) {
            this.type = type;
            this.value = value;
        }

        public int getType() {
            return type;
        }

        public void setType(int type) {
            this.type = type;
        }

        public String getValue() {
            return value;
        }

        public void setValue(String value) {
            this.value = value;
        }

    }
}
