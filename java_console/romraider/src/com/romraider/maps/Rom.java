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

import static com.romraider.maps.RomChecksum.calculateRomChecksum;
import static com.romraider.util.HexUtil.asBytes;
import static com.romraider.util.HexUtil.asHex;
import static javax.swing.JOptionPane.DEFAULT_OPTION;
import static javax.swing.JOptionPane.QUESTION_MESSAGE;
import static javax.swing.JOptionPane.showOptionDialog;

import java.beans.PropertyVetoException;
import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Vector;

import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.tree.DefaultMutableTreeNode;

import org.apache.log4j.Logger;

import com.romraider.Settings;
import com.romraider.logger.ecu.ui.handler.table.TableUpdateHandler;
import com.romraider.swing.CategoryTreeNode;
import com.romraider.swing.JProgressPane;
import com.romraider.swing.TableFrame;
import com.romraider.swing.TableTreeNode;
import com.romraider.util.SettingsManager;
import com.romraider.xml.InvalidTableNameException;
import com.romraider.xml.TableNotFoundException;

public class Rom extends DefaultMutableTreeNode implements Serializable  {
    private static final long serialVersionUID = 7865405179738828128L;
    private static final Logger LOGGER = Logger.getLogger(Rom.class);
    private RomID romID = new RomID();
    private String fileName = "";
    private final Vector<TableTreeNode> tableNodes = new Vector<>();
    private byte[] binData;
    private boolean isAbstract = false;

    public Rom() {
        tableNodes.clear();
    }

    public void refreshDisplayedTables() {
        // Remove all nodes from the ROM tree node.
        super.removeAllChildren();

        Settings settings = SettingsManager.getSettings();

        // Add nodes to ROM tree.
        for (TableTreeNode tableTreeNode : tableNodes) {
            TableFrame tableFrame = tableTreeNode.getFrame();
            Table table = tableFrame.getTable();

            if (settings.isDisplayHighTables() || settings.getUserLevel() >= table.getUserLevel()) {
                boolean categoryExists = false;

                for (int j = 0; j < getChildCount(); j++) {
                    if (getChildAt(j).toString().equals(table.getCategory())) {
                        // add to appropriate category
                        getChildAt(j).add(tableTreeNode);
                        categoryExists = true;
                        break;
                    }
                }

                if (!categoryExists) { // if category does not already exist, create it
                    CategoryTreeNode categoryNode = new CategoryTreeNode(table.getCategory());
                    categoryNode.add(tableTreeNode);
                    this.add(categoryNode);
                }
            }
        }
    }

    public void addTable(Table table) {
        boolean found = false;
        String frameTitle = this.getFileName()+" - "+table.getName();

        for (int i = 0; i < tableNodes.size(); i++) {
            if (tableNodes.get(i).getTable().equals(table)) {
                tableNodes.remove(i);
                tableNodes.add(i, new TableTreeNode(new TableFrame(frameTitle, table)));
                found = true;
                break;
            }
        }
        if (!found) {
            tableNodes.add(new TableTreeNode(new TableFrame(frameTitle, table)));
        }
    }

    public void removeTable(Table table) {
        for(int i = 0; i < tableNodes.size(); i++) {
            if(tableNodes.get(i).getTable().equals(table)) {
                tableNodes.remove(i);
                return;
            }
        }
    }

    public Table getTableByName(String tableName) throws TableNotFoundException, InvalidTableNameException {
        if(null == tableName || tableName.isEmpty()) {
            throw new InvalidTableNameException();
        }

        for (TableTreeNode tableNode : tableNodes) {
            if (tableNode.getTable().getName().equalsIgnoreCase(tableName)) {
                return tableNode.getTable();
            }
        }
        throw new TableNotFoundException();
    }

    public List<Table> findTables(String regex) {
        List<Table> result = new ArrayList<>();
        for (TableTreeNode tableNode : tableNodes) {
            String name = tableNode.getTable().getName();
            if (name.matches(regex)) result.add(tableNode.getTable());
        }
        return result;
    }

    public void populateTables(byte[] binData, JProgressPane progress) {
        this.binData = binData;
        for (int i = 0; i < tableNodes.size(); i++) {

            // update progress
            int currProgress = (int) (i / (double) tableNodes.size() * 40);
            progress.update("Populating tables...", 50 + currProgress);

            Table table = tableNodes.get(i).getTable();
//            try {
                // if storageaddress has not been set (or is set to 0) omit table
                if (table.getStorageAddress() != 0) {
   //                 try {
                        table.populateTable(binData, this.getRomID().getRamOffset());
                        TableUpdateHandler.getInstance().registerTable(table);

                        if (null != table.getName() && table.getName().equalsIgnoreCase("Checksum Fix")){
                            setEditStamp(binData, table.getStorageAddress());
                        }
//                    } catch (IndexOutOfBoundsException iex) {
//                        LOGGER.error(table.getName() +
//                                " type " + table.getType() + " start " +
//                                table.getStorageAddress() + " " + binData.length + " filesize", iex);
//
//                        // table storage address extends beyond end of file
//                        JOptionPane.showMessageDialog(SwingUtilities.windowForComponent(table), "Storage address for table \"" + table.getName() +
//                                "\" is out of bounds.\nPlease check ECU definition file.", "ECU Definition Error", JOptionPane.ERROR_MESSAGE);
//                        tableNodes.removeElementAt(i);
//                        i--;
//                    }

                } else {
                    tableNodes.removeElementAt(i);
                    // decrement i because length of vector has changed
                    i--;
                }

//            } catch (NullPointerException ex) {
//                LOGGER.error("Error Populating Table", ex);
//                JOptionPane.showMessageDialog(SwingUtilities.windowForComponent(table), "There was an error loading table " + table.getName(), "ECU Definition Error", JOptionPane.ERROR_MESSAGE);
//                tableNodes.removeElementAt(i);
//                i--;
//            }
        }
    }

    private void setEditStamp(byte[] binData, int address) {
        byte[] stampData = new byte[4];
        System.arraycopy(binData, address+204, stampData, 0, stampData.length);
        String stamp = asHex(stampData);
        if (stamp.equalsIgnoreCase("FFFFFFFF")) {
            romID.setEditStamp("");
        }
        else {
            StringBuilder niceStamp = new StringBuilder(stamp);
            niceStamp.replace(6, 9, String.valueOf(0xFF & stampData[3]));
            niceStamp.insert(6, " v");
            niceStamp.insert(4, "-");
            niceStamp.insert(2, "-");
            niceStamp.insert(0, "20");
            romID.setEditStamp(niceStamp.toString());
        }
    }

    public void setRomID(RomID romID) {
        this.romID = romID;
    }

    public RomID getRomID() {
        return romID;
    }

    public String getRomIDString() {
        return romID.getXmlid();
    }

    @Override
    public String toString() {
        String output = "";
        output = output + "\n---- Rom ----" + romID.toString();
        for (TableTreeNode tableNode : tableNodes) {
            output = output + tableNode.getTable();
        }
        output = output + "\n---- End Rom ----";

        return output;
    }

    public String getFileName() {
        return fileName;
    }

    public Vector<Table> getTables() {
        Vector<Table> tables = new Vector<>();
        for(TableTreeNode tableNode : tableNodes) {
            tables.add(tableNode.getTable());
        }
        return tables;
    }

    public Vector<TableTreeNode> getTableNodes() {
        return this.tableNodes;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public byte[] saveFile() {
        for (TableTreeNode tableNode : tableNodes) {
            tableNode.getTable().saveFile(binData);
        }

        return binData;
    }

    public void clearData() {
        super.removeAllChildren();

        // Hide and dispose all frames.
        for(TableTreeNode tableTreeNode : tableNodes) {
            TableFrame frame = tableTreeNode.getFrame();
            frame.setVisible(false);
            try {
                frame.setClosed(true);
            } catch (PropertyVetoException e) {
                ; // Do nothing.
            }
            frame.dispose();
        }

        tableNodes.clear();
        binData = null;
    }

    public int getRealFileSize() {
        return binData.length;
    }

    public void setFullFileName(String fileName) {
        this.setFileName(fileName);
        for (TableTreeNode tableNode : tableNodes) {
            String frameTitle = this.getFileName() + " - " + tableNode.getTable().getName();
            tableNode.getFrame().setTitle(frameTitle);
        }
    }

    public boolean isAbstract() {
        return isAbstract;
    }

    public void setAbstract(boolean isAbstract) {
        this.isAbstract = isAbstract;
    }

    public void refreshTableCompareMenus() {
        for(TableTreeNode tableNode : getTableNodes()) {
            tableNode.getFrame().refreshSimilarOpenTables();
        }
    }

    @Override
    public DefaultMutableTreeNode getChildAt(int i) {
        return (DefaultMutableTreeNode) super.getChildAt(i);
    }

    @Override
    public DefaultMutableTreeNode getLastChild() {
        return (DefaultMutableTreeNode) super.getLastChild();
    }
}