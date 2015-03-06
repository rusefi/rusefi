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

package com.romraider.xml;

import static com.romraider.xml.DOMHelper.unmarshallAttribute;
import static java.awt.Font.BOLD;
import static org.w3c.dom.Node.ELEMENT_NODE;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.romraider.Settings;

public final class DOMSettingsUnmarshaller {

    public Settings unmarshallSettings(Node rootNode) {
        Settings settings = new Settings();
        Node n;
        NodeList nodes = rootNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("window")) {
                settings = unmarshallWindow(n, settings);

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("files")) {
                settings = unmarshallFiles(n, settings);

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("options")) {
                settings = unmarshallOptions(n, settings);

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("tabledisplay")) {
                settings = unmarshallTableDisplay(n, settings);

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("logger")) {
                settings = unmarshallLogger(n, settings);

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase(Settings.TABLE_CLIPBOARD_FORMAT_ELEMENT)) {
                settings = this.unmarshallClipboardFormat(n, settings);
            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase(Settings.ICONS_ELEMENT_NAME)) {
                settings = this.unmarshallIcons(n, settings);
            }
        }
        return settings;
    }


    private Settings unmarshallWindow(Node windowNode, Settings settings) {
        Node n;
        NodeList nodes = windowNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("maximized")) {
                settings.setWindowMaximized(unmarshallAttribute(n, "value", false));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("size")) {
                settings.setWindowSize(new Dimension(unmarshallAttribute(n, "y", 600),
                        unmarshallAttribute(n, "x", 800)));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("location")) {
                // set default location in top left screen if no settings file found
                settings.setWindowLocation(new Point(unmarshallAttribute(n, "x", 0),
                        unmarshallAttribute(n, "y", 0)));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("splitpane")) {
                settings.setSplitPaneLocation(unmarshallAttribute(n, "location", 150));

            }
        }
        return settings;
    }

    private Settings unmarshallFiles(Node urlNode, Settings settings) {
        Node n;
        NodeList nodes = urlNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("ecudefinitionfile")) {
                settings.addEcuDefinitionFile(new File(unmarshallAttribute(n, "name", "ecu_defs.xml")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("image_dir")) {
                settings.setLastImageDir(new File(unmarshallAttribute(n, "path", "ecu_defs.xml")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase(Settings.REPOSITORY_ELEMENT_NAME)) {
                settings.setLastRepositoryDir(new File(unmarshallAttribute(n, Settings.REPOSITORY_ATTRIBUTE_NAME, "repositories")));

            }
        }
        return settings;
    }

    private Settings unmarshallOptions(Node optionNode, Settings settings) {
        Node n;
        NodeList nodes = optionNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("obsoletewarning")) {
                settings.setObsoleteWarning(Boolean.parseBoolean(unmarshallAttribute(n, "value", "true")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("debug")) {
                settings.setDebug(Boolean.parseBoolean(unmarshallAttribute(n, "value", "true")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("calcconflictwarning")) {
                settings.setCalcConflictWarning(Boolean.parseBoolean(unmarshallAttribute(n, "value", "true")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("userlevel")) {
                settings.setUserLevel(unmarshallAttribute(n, "value", 1));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("tableclickcount")) {
                settings.setTableClickCount(unmarshallAttribute(n, "value", 2));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("tableclickbehavior")) {
                settings.setTableClickBehavior(unmarshallAttribute(n, "value", 0));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("version")) {
                settings.setRecentVersion(unmarshallAttribute(n, "value", ""));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("savedebugtables")) {
                settings.setSaveDebugTables(Boolean.parseBoolean(unmarshallAttribute(n, "value", "false")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("displayhightables")) {
                settings.setDisplayHighTables(Boolean.parseBoolean(unmarshallAttribute(n, "value", "false")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("valuelimitwarning")) {
                settings.setValueLimitWarning(Boolean.parseBoolean(unmarshallAttribute(n, "value", "true")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("coloraxis")) {
                settings.setColorAxis(Boolean.parseBoolean(unmarshallAttribute(n, "value", "false")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("showtabletoolbarborder")) {
                settings.setShowTableToolbarBorder(Boolean.parseBoolean(unmarshallAttribute(n, "value", "false")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("openromexpanded")) {
                settings.setOpenExpanded(Boolean.parseBoolean(unmarshallAttribute(n, "value", "true")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("alwaysopentableatzero")) {
                settings.setAlwaysOpenTableAtZero(Boolean.parseBoolean(unmarshallAttribute(n, "value", "false")));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("defaultscale")) {
                settings.setDefaultScale(unmarshallAttribute(n, "value", "Metric"));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("scaleHeadersAndData")) {
                settings.setScaleHeadersAndData(Boolean.parseBoolean(unmarshallAttribute(n, "value", "true")));

            }
        }
        return settings;
    }

    private Settings unmarshallTableDisplay(Node tableDisplayNode, Settings settings) {
        Node n;
        NodeList nodes = tableDisplayNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("font")) {
                settings.setTableFont(new Font(unmarshallAttribute(n, "face", "Arial"),
                        unmarshallAttribute(n, "decoration", BOLD),
                        unmarshallAttribute(n, "size", 12)));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("cellsize")) {
                settings.setCellSize(new Dimension(unmarshallAttribute(n, "width", 42),
                        unmarshallAttribute(n, "height", 18)));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("colors")) {
                settings = unmarshallColors(n, settings);

            }
        }
        return settings;
    }

    private Settings unmarshallColors(Node colorNode, Settings settings) {
        Node n;
        NodeList nodes = colorNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("max")) {
                settings.setMaxColor(unmarshallColor(n));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("min")) {
                settings.setMinColor(unmarshallColor(n));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("highlight")) {
                settings.setHighlightColor(unmarshallColor(n));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("select")) {
                settings.setSelectColor(unmarshallColor(n));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("increaseborder")) {
                settings.setIncreaseBorder(unmarshallColor(n));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("decreaseborder")) {
                settings.setDecreaseBorder(unmarshallColor(n));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("axis")) {
                settings.setAxisColor(unmarshallColor(n));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("warning")) {
                settings.setWarningColor(unmarshallColor(n));

            }
        }
        return settings;
    }


    private Settings unmarshallLogger(Node loggerNode, Settings settings) {
        NodeList nodes = loggerNode.getChildNodes();
        if (loggerNode.getNodeType() == ELEMENT_NODE && loggerNode.getNodeName().equalsIgnoreCase("logger")) {
            settings.setLocale(unmarshallAttribute(loggerNode, "locale", "system"));
        }

        for (int i = 0; i < nodes.getLength(); i++) {
            Node n = nodes.item(i);

            if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("serial")) {
                settings.setLoggerPortDefault(unmarshallAttribute(n, "port", ""));
                settings.setRefreshMode(unmarshallAttribute(n, "refresh", false));
                settings.setDestinationId((byte) unmarshallAttribute(n, "ecuid", (byte) 0x10));
                settings.setFastPoll(unmarshallAttribute(n, "fastpoll", true));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("protocol")) {
                settings.setLoggerProtocol(unmarshallAttribute(n, "name", "SSM"));
                settings.setTransportProtocol(unmarshallAttribute(n, "transport", "ISO9141"));
                settings.setJ2534Device(unmarshallAttribute(n, "library", null));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("maximized")) {
                settings.setLoggerWindowMaximized(unmarshallAttribute(n, "value", false));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("size")) {
                settings.setLoggerWindowSize(new Dimension(unmarshallAttribute(n, "y", 600),
                        unmarshallAttribute(n, "x", 1000)));
                settings.setLoggerDividerLocation(unmarshallAttribute(n, "divider", 500));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("location")) {
                settings.setLoggerWindowLocation(new Point(unmarshallAttribute(n, "x", 150),
                        unmarshallAttribute(n, "y", 150)));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("tabs")) {
                settings.setLoggerSelectedTabIndex(unmarshallAttribute(n, "selected", 0));
                settings.setLoggerParameterListState(unmarshallAttribute(n, "showlist", true));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("definition")) {
                settings.setLoggerDefinitionFilePath(unmarshallAttribute(n, "path", settings.getLoggerDefinitionFilePath()));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("profile")) {
                settings.setLoggerProfileFilePath(unmarshallAttribute(n, "path", ""));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("filelogging")) {
                settings.setLoggerOutputDirPath(unmarshallAttribute(n, "path", ""));
                settings.setFileLoggingControllerSwitchId(unmarshallAttribute(n, "switchid", settings.getFileLoggingControllerSwitchId()));
                settings.setFileLoggingControllerSwitchActive(unmarshallAttribute(n, "active", true));
                settings.setFileLoggingAbsoluteTimestamp(unmarshallAttribute(n, "absolutetimestamp", false));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("debug")) {
                settings.setLoggerDebuggingLevel(unmarshallAttribute(n, "level", "info"));

            } else if (n.getNodeType() == ELEMENT_NODE && n.getNodeName().equalsIgnoreCase("plugins")) {
                Map<String, String> pluginPorts = new HashMap<String, String>();
                NodeList pluginNodes = n.getChildNodes();
                for (int j = 0; j < pluginNodes.getLength(); j++) {
                    Node pluginNode = pluginNodes.item(j);
                    if (pluginNode.getNodeType() == ELEMENT_NODE && pluginNode.getNodeName().equalsIgnoreCase("plugin")) {
                        String id = unmarshallAttribute(pluginNode, "id", null);
                        if (id == null || id.trim().length() == 0) continue;
                        String port = unmarshallAttribute(pluginNode, "port", null);
                        if (port == null || port.trim().length() == 0) continue;
                        pluginPorts.put(id.trim(), port.trim());
                    }
                    else if (pluginNode.getNodeType() == ELEMENT_NODE && pluginNode.getNodeName().equalsIgnoreCase("phidgets")) {
//                        final Map<String, IntfKitSensor> phidgets = new HashMap<String, IntfKitSensor>();
//                        NodeList sensorNodes = pluginNode.getChildNodes();
//                        for (int k = 0; k < sensorNodes.getLength(); k++) {
//                            Node sensorNode = sensorNodes.item(k);
//                            if (sensorNode.getNodeType() == ELEMENT_NODE && sensorNode.getNodeName().equalsIgnoreCase("phidget")) {
//                                final String name = unmarshallAttribute(sensorNode, "name", null);
//                                final int number = unmarshallAttribute(sensorNode, "number", -1);
//                                final String units = unmarshallAttribute(sensorNode, "units", null);
//                                final String expression = unmarshallAttribute(sensorNode, "expression", null);
//                                final String format = unmarshallAttribute(sensorNode, "format", null);
//                                final float min = Float.parseFloat(unmarshallAttribute(sensorNode, "min", "-1.0"));
//                                final float max = Float.parseFloat(unmarshallAttribute(sensorNode, "max", "-1.0"));
//                                final float step = Float.parseFloat(unmarshallAttribute(sensorNode, "step", "-1.0"));
//                                if (name != null && number != -1) {
//                                    final String inputName = name.replaceAll("Phidget IK Sensor ", "");
//                                    final IntfKitSensor sensor = new IntfKitSensor();
//                                    sensor.setInputNumber(number);
//                                    sensor.setInputName(name);
//                                    sensor.setUnits(units);
//                                    sensor.setExpression(expression);
//                                    sensor.setFormat(format);
//                                    sensor.setMinValue(min);
//                                    sensor.setMaxValue(max);
//                                    sensor.setStepValue(step);
//                                    phidgets.put(inputName, sensor);
//                                }
//                            }
//                        }
//                        settings.setPhidgetSensors(phidgets);
                    }
                }
                settings.setLoggerPluginPorts(pluginPorts);
            }
        }
        return settings;
    }

    private Color unmarshallColor(Node colorNode) {
        return new Color(unmarshallAttribute(colorNode, "r", 155),
                unmarshallAttribute(colorNode, "g", 155),
                unmarshallAttribute(colorNode, "b", 155));
    }

    private Settings unmarshallClipboardFormat(Node formatNode, Settings settings) {
        String tableClipboardFormat = unmarshallAttribute(formatNode, Settings.TABLE_CLIPBOARD_FORMAT_ATTRIBUTE, Settings.DEFAULT_CLIPBOARD_FORMAT);
        if(tableClipboardFormat.equalsIgnoreCase(Settings.CUSTOM_CLIPBOARD_FORMAT)) {
            settings.setTableClipboardFormat(Settings.CUSTOM_CLIPBOARD_FORMAT);
        } else if (tableClipboardFormat.equalsIgnoreCase(Settings.AIRBOYS_CLIPBOARD_FORMAT)) {
            settings.setAirboysFormat();
            return settings;
        } else {
            settings.setDefaultFormat();
            return settings;
        }

        NodeList tableFormats = formatNode.getChildNodes();
        for( int i = 0; i < tableFormats.getLength(); i++) {
            Node tableNode = tableFormats.item(i);
            if(tableNode.getNodeType() == ELEMENT_NODE) {
                if(tableNode.getNodeName().equalsIgnoreCase(Settings.TABLE_ELEMENT)) {
                    settings.setTableHeader(unmarshallAttribute(tableNode, Settings.TABLE_HEADER_ATTRIBUTE, Settings.DEFAULT_TABLE_HEADER));
                } else if(tableNode.getNodeName().equalsIgnoreCase(Settings.TABLE1D_ELEMENT)) {
                    settings.setTable1DHeader(unmarshallAttribute(tableNode, Settings.TABLE_HEADER_ATTRIBUTE, Settings.DEFAULT_TABLE1D_HEADER));
                } else if(tableNode.getNodeName().equalsIgnoreCase(Settings.TABLE2D_ELEMENT)) {
                    settings.setTable2DHeader(unmarshallAttribute(tableNode, Settings.TABLE_HEADER_ATTRIBUTE, Settings.DEFAULT_TABLE2D_HEADER));
                } else if(tableNode.getNodeName().equalsIgnoreCase(Settings.TABLE3D_ELEMENT)) {
                    settings.setTable3DHeader(unmarshallAttribute(tableNode, Settings.TABLE_HEADER_ATTRIBUTE, Settings.DEFAULT_TABLE3D_HEADER));
                }
            }
        }
        return settings;
    }

    private Settings unmarshallIcons(Node iconsNode, Settings settings) {
        NodeList iconScales = iconsNode.getChildNodes();
        for(int i = 0; i < iconScales.getLength(); i++) {
            Node scaleNode = iconScales.item(i);
            if(scaleNode.getNodeType() == ELEMENT_NODE) {
                if(scaleNode.getNodeName().equalsIgnoreCase(Settings.EDITOR_ICONS_ELEMENT_NAME)) {
                    try{
                        settings.setEditorIconScale(unmarshallAttribute(scaleNode, Settings.EDITOR_ICONS_SCALE_ATTRIBUTE_NAME, Settings.DEFAULT_EDITOR_ICON_SCALE));
                    } catch(NumberFormatException ex) {
                        settings.setEditorIconScale(Settings.DEFAULT_EDITOR_ICON_SCALE);
                    }
                } else if(scaleNode.getNodeName().equalsIgnoreCase(Settings.TABLE_ICONS_ELEMENT_NAME)) {
                    try{
                        settings.setTableIconScale(unmarshallAttribute(scaleNode, Settings.TABLE_ICONS_SCALE_ATTRIBUTE_NAME, Settings.DEFAULT_TABLE_ICON_SCALE));
                    } catch(NumberFormatException ex) {
                        settings.setTableIconScale(Settings.DEFAULT_TABLE_ICON_SCALE);
                    }
                }
            }
        }
        return settings;
    }
}
