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

//DOM XML parser for ROMs

package com.romraider.xml;

import static com.romraider.xml.DOMHelper.unmarshallAttribute;
import static com.romraider.xml.DOMHelper.unmarshallText;
import static org.w3c.dom.Node.ELEMENT_NODE;

import java.util.ArrayList;
import java.util.List;

import javax.management.modelmbean.XMLParseException;
import javax.naming.NameNotFoundException;
import javax.swing.JOptionPane;

import com.rusefi.ConsoleUI;
import com.rusefi.FileLog;
import org.apache.log4j.Logger;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.romraider.Settings;
import com.romraider.maps.DataCell;
import com.romraider.maps.Rom;
import com.romraider.maps.RomID;
import com.romraider.maps.Scale;
import com.romraider.maps.Table;
import com.romraider.maps.Table1D;
import com.romraider.maps.Table2D;
import com.romraider.maps.Table3D;
import com.romraider.maps.TableSwitch;
import com.romraider.swing.DebugPanel;
import com.romraider.swing.JProgressPane;
import com.romraider.util.LogManager;
import com.romraider.util.ObjectCloner;
import com.romraider.util.SettingsManager;

public final class DOMRomUnmarshaller {
    private static final Logger LOGGER = Logger
            .getLogger(DOMRomUnmarshaller.class);
    private JProgressPane progress = null;
    private final List<Scale> scales = new ArrayList<Scale>();

    public DOMRomUnmarshaller() {
    }

    public Rom unmarshallXMLDefinition(Node rootNode, byte[] romContent,
            JProgressPane progress) throws RomNotFoundException,
            XMLParseException, NameNotFoundException {

        this.progress = progress;
        Node n;
        NodeList nodes = rootNode.getChildNodes();
        String romIdString = "(none)";

        // unmarshall scales first
        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE
                    && n.getNodeName().equalsIgnoreCase("scalingbase")) {
                scales.add(unmarshallScale(n, new Scale()));
            }
        }

        // now unmarshall roms
        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE
                    && n.getNodeName().equalsIgnoreCase("rom")) {
                Node n2;
                NodeList nodes2 = n.getChildNodes();

                for (int z = 0; z < nodes2.getLength(); z++) {
                    n2 = nodes2.item(z);
                    if (n2.getNodeType() == ELEMENT_NODE
                            && n2.getNodeName().equalsIgnoreCase("romid")) {

                        RomID romID = unmarshallRomID(n2, new RomID());

                        romIdString = romID.getInternalIdString();
                        FileLog.MAIN.logLine("Looking for " + romIdString);


                        if (romID.getInternalIdString().length() > 0
                                && foundMatch(romID, romContent)) {
                            Rom output = unmarshallRom(n, new Rom());

                            // set ram offset
                            output.getRomID().setRamOffset(
                                    output.getRomID().getFileSize()
                                    - romContent.length);
                            return output;
                        }
                    }
                }
            }
        }
        throw new RomNotFoundException("Match not found for " + romIdString);
    }

    public static boolean foundMatch(RomID romID, byte[] file) {

        String idString = romID.getInternalIdString();

        // romid is hex string
        if (idString.length() > 2
                && idString.substring(0, 2).equalsIgnoreCase("0x")) {

            try {
                // put romid in to byte array to check for match
                idString = idString.substring(2); // remove "0x"
                int[] romIDBytes = new int[idString.length() / 2];

                for (int i = 0; i < romIDBytes.length; i++) {
                    // check to see if each byte matches

                    if ((file[romID.getInternalIdAddress() + i] & 0xff) != Integer
                            .parseInt(idString.substring(i * 2, i * 2 + 2), 16)) {

                        return false;
                    }
                }
                // if no mismatched bytes found, return true
                return true;
            } catch (Exception ex) {
                // if any exception is encountered, names do not match
                LOGGER.warn("Error finding match", ex);
                return false;
            }

            // else romid is NOT hex string
        } else {
            try {
                String ecuID = new String(file, romID.getInternalIdAddress(),
                        romID.getInternalIdString().length());
                return foundMatchByString(romID, ecuID);
            } catch (Exception ex) {
                // if any exception is encountered, names do not match
                return false;
            }
        }
    }

    public static boolean foundMatchByString(RomID romID, String ecuID) {

        try {
            if (ecuID.equalsIgnoreCase(romID.getInternalIdString())) {
                return true;
            } else {
                return false;
            }
        } catch (Exception ex) {
            // if any exception is encountered, names do not match
            return false;
        }
    }

    public static void main(String args[]) {
        LogManager.initDebugLogging();
        RomID romID = new RomID();
        romID.setInternalIdString("Asdfd");

        byte[] file = "Asdfd".getBytes();
        LOGGER.debug(foundMatch(romID, file));

        file[0] = 1;
        file[1] = 1;
        file[2] = 1;
        file[3] = 1;
        LOGGER.debug(foundMatch(romID, file));

        romID.setInternalIdString("0x010101");
        LOGGER.debug(foundMatch(romID, file));
    }

    public Rom unmarshallRom(Node rootNode, Rom rom) throws XMLParseException,
            RomNotFoundException, NameNotFoundException {
        Node n;
        NodeList nodes = rootNode.getChildNodes();

        progress.update("Creating tables...", 15);

        if (!unmarshallAttribute(rootNode, "base", "none").equalsIgnoreCase(
                "none")) {
            rom = getBaseRom(rootNode.getParentNode(),
                    unmarshallAttribute(rootNode, "base", "none"), rom);
            rom.getRomID().setObsolete(false);
        }

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            // update progress
            int currProgress = (int) ((double) i / (double) nodes.getLength() * 40);
            progress.update("Creating tables...", 10 + currProgress);

            if (n.getNodeType() == ELEMENT_NODE) {
                if (n.getNodeName().equalsIgnoreCase("romid")) {
                    rom.setRomID(unmarshallRomID(n, rom.getRomID()));

                } else if (n.getNodeName().equalsIgnoreCase("table")) {
                    Table table = null;
                    try {
                        table = rom.getTableByName(unmarshallAttribute(n, "name",
                                null));
                    } catch (TableNotFoundException e) {
                        /*
                         * table does not
                         * already exist (do
                         * nothing)
                         */
                    } catch (InvalidTableNameException iex) {
                        // Table name is null or empty.  Do nothing.
                    }

                    try {
                        table = unmarshallTable(n, table, rom);
                        //rom.addTableByName(table);
                        rom.addTable(table);
                    } catch (TableIsOmittedException ex) {
                        // table is not supported in inherited def (skip)
                        if (table != null) {
                            //rom.removeTableByName(table);
                            rom.removeTable(table);
                        }
                    } catch (XMLParseException ex) {
                        LOGGER.error("Error unmarshalling rom", ex);
                    }

                } else { /* unexpected element in Rom (skip) */
                }
            } else { /* unexpected node-type in Rom (skip) */
            }
        }
        return rom;
    }

    public Rom getBaseRom(Node rootNode, String xmlID, Rom rom)
            throws XMLParseException, RomNotFoundException, NameNotFoundException {
        Node n;
        NodeList nodes = rootNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE
                    && n.getNodeName().equalsIgnoreCase("rom")) {
                Node n2;
                NodeList nodes2 = n.getChildNodes();

                for (int z = 0; z < nodes2.getLength(); z++) {
                    n2 = nodes2.item(z);
                    if (n2.getNodeType() == ELEMENT_NODE
                            && n2.getNodeName().equalsIgnoreCase("romid")) {

                        RomID romID = unmarshallRomID(n2, new RomID());
                        if (romID.getXmlid().equalsIgnoreCase(xmlID)) {
                            Rom returnrom = unmarshallRom(n, rom);
                            returnrom.getRomID().setObsolete(false);
                            return returnrom;
                        }
                    }
                }
            }
        }
        throw new RomNotFoundException("xmlID=" + xmlID);
    }

    public RomID unmarshallRomID(Node romIDNode, RomID romID) {
        Node n;
        NodeList nodes = romIDNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE) {

                if (n.getNodeName().equalsIgnoreCase("xmlid")) {
                    romID.setXmlid(unmarshallText(n));

                } else if (n.getNodeName()
                        .equalsIgnoreCase("internalidaddress")) {
                    romID.setInternalIdAddress(RomAttributeParser
                            .parseHexString(unmarshallText(n)));

                } else if (n.getNodeName().equalsIgnoreCase("internalidstring")) {
                    romID.setInternalIdString(unmarshallText(n));
                    if (romID.getInternalIdString() == null) {
                        romID.setInternalIdString("");
                    }

                } else if (n.getNodeName().equalsIgnoreCase("caseid")) {
                    romID.setCaseId(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("ecuid")) {
                    romID.setEcuId(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("make")) {
                    romID.setMake(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("market")) {
                    romID.setMarket(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("model")) {
                    romID.setModel(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("submodel")) {
                    romID.setSubModel(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("transmission")) {
                    romID.setTransmission(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("year")) {
                    romID.setYear(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("flashmethod")) {
                    romID.setFlashMethod(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("memmodel")) {
                    romID.setMemModel(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("filesize")) {
                    romID.setFileSize(RomAttributeParser
                            .parseFileSize(unmarshallText(n)));

                } else if (n.getNodeName().equalsIgnoreCase("obsolete")) {
                    romID.setObsolete(Boolean.parseBoolean(unmarshallText(n)));

                } else { /* unexpected element in RomID (skip) */
                }
            } else { /* unexpected node-type in RomID (skip) */
            }
        }
        return romID;
    }

    private Table unmarshallTable(Node tableNode, Table table, Rom rom)
            throws XMLParseException, TableIsOmittedException, NameNotFoundException {

        if (unmarshallAttribute(tableNode, "omit", "false").equalsIgnoreCase(
                "true")) { // remove table if omitted
            throw new TableIsOmittedException();
        }

        if (!unmarshallAttribute(tableNode, "base", "none").equalsIgnoreCase(
                "none")) { // copy base table for inheritance
            try {
                table = (Table) ObjectCloner
                        .deepCopy(rom.getTableByName(unmarshallAttribute(tableNode,
                                "base", "none")));

            } catch (TableNotFoundException ex) { /* table not found, do nothing */

            } catch (InvalidTableNameException ex) { // Table name is invalid, do nothing.

            } catch (NullPointerException ex) {
                JOptionPane.showMessageDialog(ConsoleUI.getFrame(),
                        new DebugPanel(ex, SettingsManager.getSettings().getSupportURL()), "Exception",
                        JOptionPane.ERROR_MESSAGE);

            }
        }

        try {
            if (table.getType() < 1) {
            }
        } catch (NullPointerException ex) { // if type is null or less than 0,
            // create new instance (otherwise it
            // is inherited)
            String typeStr = unmarshallAttribute(tableNode, "type", "unknown");
            if (typeStr.equalsIgnoreCase(Table3D.TYPE_3D)) {
                table = new Table3D();

            } else if (typeStr.equalsIgnoreCase(Table2D.TYPE_2D)) {
                table = new Table2D();

            } else if (typeStr.equalsIgnoreCase(Table1D.TYPE_1D)
                    || typeStr.equalsIgnoreCase(Table1D.TYPE_X_AXIS)
                    || typeStr.equalsIgnoreCase(Table1D.TYPE_Y_AXIS)) {
                table = new Table1D();

            } else if (typeStr.equalsIgnoreCase("Static Y Axis")
                    || typeStr.equalsIgnoreCase("Static X Axis")) {
                table = new Table1D();

            } else if (typeStr.equalsIgnoreCase(TableSwitch.TYPE_SWITCH)) {
                table = new TableSwitch();
            } else {
                throw new XMLParseException("Error loading table, "
                        + tableNode.getAttributes().getNamedItem("name"));
            }
        }

        // unmarshall table attributes
        table.setName(unmarshallAttribute(tableNode, "name", table.getName()));
        table.setType(RomAttributeParser.parseTableType(unmarshallAttribute(
                tableNode, "type", String.valueOf(table.getType()))));
        if (unmarshallAttribute(tableNode, "beforeram", "false")
                .equalsIgnoreCase("true")) {
            table.setBeforeRam(true);
        }

        table.setCategory(unmarshallAttribute(tableNode, "category",
                table.getCategory()));
        if (table.getStorageType() < 1) {
            table.setSignedData(RomAttributeParser
                    .parseStorageDataSign(unmarshallAttribute(tableNode,
                            "storagetype",
                            String.valueOf(table.getStorageType()))));
        }
        table.setStorageType(RomAttributeParser
                .parseStorageType(unmarshallAttribute(tableNode, "storagetype",
                        String.valueOf(table.getStorageType()))));
        table.setEndian(RomAttributeParser.parseEndian(unmarshallAttribute(
                tableNode, "endian", String.valueOf(table.getEndian()))));
        table.setStorageAddress(RomAttributeParser
                .parseHexString(unmarshallAttribute(tableNode,
                        "storageaddress",
                        String.valueOf(table.getStorageAddress()))));
        table.setDescription(unmarshallAttribute(tableNode, "description",
                table.getDescription()));
        table.setDataSize(unmarshallAttribute(tableNode, "sizey",
                unmarshallAttribute(tableNode, "sizex", table.getDataSize())));
        table.setFlip(unmarshallAttribute(tableNode, "flipy",
                unmarshallAttribute(tableNode, "flipx", table.getFlip())));
        table.setUserLevel(unmarshallAttribute(tableNode, "userlevel",
                table.getUserLevel()));
        table.setLocked(unmarshallAttribute(tableNode, "locked",
                table.isLocked()));
        table.setLogParam(unmarshallAttribute(tableNode, "logparam",
                table.getLogParam()));

        if (table.getType() == Settings.TABLE_3D) {
            ((Table3D) table).setSwapXY(unmarshallAttribute(tableNode,
                    "swapxy", ((Table3D) table).getSwapXY()));
            ((Table3D) table).setFlipX(unmarshallAttribute(tableNode, "flipx",
                    ((Table3D) table).getFlipX()));
            ((Table3D) table).setFlipY(unmarshallAttribute(tableNode, "flipy",
                    ((Table3D) table).getFlipY()));
            ((Table3D) table).setSizeX(unmarshallAttribute(tableNode, "sizex",
                    ((Table3D) table).getSizeX()));
            ((Table3D) table).setSizeY(unmarshallAttribute(tableNode, "sizey",
                    ((Table3D) table).getSizeY()));
        }

        Node n;
        NodeList nodes = tableNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE) {
                if (n.getNodeName().equalsIgnoreCase("table")) {

                    if (table.getType() == Settings.TABLE_2D) { // if table is 2D,
                        // parse axis

                        if (RomAttributeParser
                                .parseTableType(unmarshallAttribute(n, "type",
                                        "unknown")) == Settings.TABLE_Y_AXIS
                                        || RomAttributeParser
                                        .parseTableType(unmarshallAttribute(n,
                                                "type", "unknown")) == Settings.TABLE_X_AXIS) {

                            Table1D tempTable = (Table1D) unmarshallTable(n,
                                    ((Table2D) table).getAxis(), rom);
                            if (tempTable.getDataSize() != table.getDataSize()) {
                                tempTable.setDataSize(table.getDataSize());
                            }
                            tempTable.setData(((Table2D) table).getAxis()
                                    .getData());
                            ((Table2D) table).setAxis(tempTable);

                        }
                    } else if (table.getType() == Settings.TABLE_3D) { // if table
                        // is 3D,
                        // populate
                        // xAxis
                        if (RomAttributeParser
                                .parseTableType(unmarshallAttribute(n, "type",
                                        "unknown")) == Settings.TABLE_X_AXIS) {

                            Table1D tempTable = (Table1D) unmarshallTable(n,
                                    ((Table3D) table).getXAxis(), rom);
                            if (tempTable.getDataSize() != ((Table3D) table)
                                    .getSizeX()) {
                                tempTable.setDataSize(((Table3D) table)
                                        .getSizeX());
                            }
                            tempTable.setData(((Table3D) table).getXAxis()
                                    .getData());
                            ((Table3D) table).setXAxis(tempTable);

                        } else if (RomAttributeParser
                                .parseTableType(unmarshallAttribute(n, "type",
                                        "unknown")) == Settings.TABLE_Y_AXIS) {

                            Table1D tempTable = (Table1D) unmarshallTable(n,
                                    ((Table3D) table).getYAxis(), rom);
                            if (tempTable.getDataSize() != ((Table3D) table)
                                    .getSizeY()) {
                                tempTable.setDataSize(((Table3D) table)
                                        .getSizeY());
                            }
                            tempTable.setData(((Table3D) table).getYAxis()
                                    .getData());
                            ((Table3D) table).setYAxis(tempTable);

                        }
                    }

                } else if (n.getNodeName().equalsIgnoreCase("scaling")) {
                    // check whether scale already exists. if so, modify, else
                    // use new instance
                    Scale baseScale = table.getScale(unmarshallAttribute(n,"name", "Default"));
                    table.addScale(unmarshallScale(n, baseScale));

                } else if (n.getNodeName().equalsIgnoreCase("data")) {
                    // parse and add data to table
                    DataCell dataCell = new DataCell(table, unmarshallText(n));
                    if(table instanceof Table1D) {
                        ((Table1D)table).addStaticDataCell(dataCell);
                    } else {
                        // Why would this happen.  Static should only be for axis.
                        LOGGER.error("Error adding static data cell.");
                    }

                } else if (n.getNodeName().equalsIgnoreCase("description")) {
                    table.setDescription(unmarshallText(n));

                } else if (n.getNodeName().equalsIgnoreCase("state")) {
                    ((TableSwitch) table).setValues(
                            unmarshallAttribute(n, "name", ""),
                            unmarshallAttribute(n, "data", "0.0"));

                } else { /* unexpected element in Table (skip) */
                }
            } else { /* unexpected node-type in Table (skip) */
            }
        }

        return table;
    }

    private Scale unmarshallScale(Node scaleNode, Scale scale) {

        // look for base scale first
        String base = unmarshallAttribute(scaleNode, "base", "none");
        if (!base.equalsIgnoreCase("none")) {
            for (Scale scaleItem : scales) {

                // check whether name matches base and set scale if so
                if (scaleItem.getName().equalsIgnoreCase(base)) {
                    try {
                        scale = (Scale) ObjectCloner.deepCopy(scaleItem);

                    } catch (Exception ex) {
                        JOptionPane.showMessageDialog(
                                ConsoleUI.getFrame(),
                                new DebugPanel(ex, SettingsManager.getSettings()
                                        .getSupportURL()), "Exception",
                                        JOptionPane.ERROR_MESSAGE);
                    }
                }
            }
        }

        // set remaining attributes
        scale.setName(unmarshallAttribute(scaleNode, "name", "Default"));
        scale.setUnit(unmarshallAttribute(scaleNode, "units", scale.getUnit()));
        scale.setExpression(unmarshallAttribute(scaleNode, "expression",
                scale.getExpression()));
        scale.setByteExpression(unmarshallAttribute(scaleNode, "to_byte",
                scale.getByteExpression()));
        scale.setFormat(unmarshallAttribute(scaleNode, "format", "#"));
        scale.setMax(unmarshallAttribute(scaleNode, "max", 0.0));
        scale.setMin(unmarshallAttribute(scaleNode, "min", 0.0));

        // get coarse increment with new attribute name (coarseincrement), else
        // look for old (increment)
        scale.setCoarseIncrement(unmarshallAttribute(
                scaleNode,
                "coarseincrement",
                unmarshallAttribute(scaleNode, "increment",
                        scale.getCoarseIncrement())));

        scale.setFineIncrement(unmarshallAttribute(scaleNode, "fineincrement",
                scale.getFineIncrement()));

        return scale;
    }
}