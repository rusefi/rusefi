/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2013 RomRaider.com
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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Collection;
import java.util.Map;
import java.util.Vector;

import javax.imageio.metadata.IIOMetadataNode;

import com.romraider.Settings;
//import com.romraider.logger.external.phidget.interfacekit.io.IntfKitSensor;
import com.romraider.swing.JProgressPane;
import com.sun.org.apache.xml.internal.serialize.OutputFormat;
import com.sun.org.apache.xml.internal.serialize.XMLSerializer;

public final class DOMSettingsBuilder {

    public void buildSettings(Settings settings, File output, JProgressPane progress, String versionNumber) throws IOException {

        IIOMetadataNode settingsNode = new IIOMetadataNode("settings");

        // create settings
        progress.update("Saving window settings...", 15);
        settingsNode.appendChild(buildWindow(settings));
        progress.update("Saving file settings...", 30);
        settingsNode.appendChild(buildFiles(settings));
        progress.update("Saving options...", 45);
        settingsNode.appendChild(buildOptions(settings, versionNumber));
        progress.update("Saving display settings...", 60);
        settingsNode.appendChild(buildTableDisplay(settings));
        progress.update("Saving logger settings...", 75);
        settingsNode.appendChild(buildLogger(settings));
        progress.update("Saving table clipboard format settings...", 80);
        settingsNode.appendChild(buildTableClipboardFormat(settings));
        progress.update("Saving icon scale settings...", 85);
        settingsNode.appendChild(buildIcons(settings));

        OutputFormat of = new OutputFormat("XML", "ISO-8859-1", true);
        of.setIndent(1);
        of.setIndenting(true);

        progress.update("Writing to file...", 90);

        FileOutputStream fos = new FileOutputStream(output);
        try {
            XMLSerializer serializer = new XMLSerializer(fos, of);
            serializer.serialize(settingsNode);
            fos.flush();
        } finally {
            fos.close();
        }
    }

    private IIOMetadataNode buildWindow(Settings settings) {
        IIOMetadataNode windowSettings = new IIOMetadataNode("window");

        // window maximized
        IIOMetadataNode maximized = new IIOMetadataNode("maximized");
        maximized.setAttribute("value", String.valueOf((settings.isWindowMaximized())));
        windowSettings.appendChild(maximized);

        // window size
        IIOMetadataNode size = new IIOMetadataNode("size");
        size.setAttribute("x", String.valueOf(((int) settings.getWindowSize().getHeight())));
        size.setAttribute("y", String.valueOf(((int) settings.getWindowSize().getWidth())));
        windowSettings.appendChild(size);

        // window location
        IIOMetadataNode location = new IIOMetadataNode("location");
        location.setAttribute("x", String.valueOf(((int) settings.getWindowLocation().getX())));
        location.setAttribute("y", String.valueOf(((int) settings.getWindowLocation().getY())));
        windowSettings.appendChild(location);

        // splitpane location
        IIOMetadataNode splitpane = new IIOMetadataNode("splitpane");
        splitpane.setAttribute("location", String.valueOf(settings.getSplitPaneLocation()));
        windowSettings.appendChild(splitpane);

        return windowSettings;
    }

    private IIOMetadataNode buildFiles(Settings settings) {
        IIOMetadataNode files = new IIOMetadataNode("files");

        // image directory
        IIOMetadataNode imageDir = new IIOMetadataNode("image_dir");
        imageDir.setAttribute("path", settings.getLastImageDir().getAbsolutePath());
        files.appendChild(imageDir);

        // repository directory
        IIOMetadataNode repositoryDir = new IIOMetadataNode(Settings.REPOSITORY_ELEMENT_NAME);
        repositoryDir.setAttribute(Settings.REPOSITORY_ATTRIBUTE_NAME, settings.getLastRepositoryDir().getAbsolutePath());
        files.appendChild(repositoryDir);

        // ecu definition files
        Vector<File> defFiles = settings.getEcuDefinitionFiles();

        for (File defFile : defFiles) {
            IIOMetadataNode ecuDef = new IIOMetadataNode("ecudefinitionfile");
            ecuDef.setAttribute("name", defFile.getAbsolutePath());
            files.appendChild(ecuDef);
        }

        return files;
    }

    private IIOMetadataNode buildOptions(Settings settings, String versionNumber) {
        IIOMetadataNode options = new IIOMetadataNode("options");

        // obsolete warning
        IIOMetadataNode obsoleteWarning = new IIOMetadataNode("obsoletewarning");
        obsoleteWarning.setAttribute("value", String.valueOf(settings.isObsoleteWarning()));
        options.appendChild(obsoleteWarning);

        // calcultion conflicting warning
        IIOMetadataNode calcConflictWarning = new IIOMetadataNode("calcconflictwarning");
        calcConflictWarning.setAttribute("value", String.valueOf(settings.isCalcConflictWarning()));
        options.appendChild(calcConflictWarning);

        // debug mode
        IIOMetadataNode debug = new IIOMetadataNode("debug");
        debug.setAttribute("value", String.valueOf(settings.isDebug()));
        options.appendChild(debug);

        // userlevel
        IIOMetadataNode userLevel = new IIOMetadataNode("userlevel");
        userLevel.setAttribute("value", String.valueOf(settings.getUserLevel()));
        options.appendChild(userLevel);

        // table click count
        IIOMetadataNode tableClickCount = new IIOMetadataNode("tableclickcount");
        tableClickCount.setAttribute("value", String.valueOf(settings.getTableClickCount()));
        options.appendChild(tableClickCount);

        // table click behavior
        IIOMetadataNode tableClickBehavior = new IIOMetadataNode("tableclickbehavior");
        tableClickBehavior.setAttribute("value", String.valueOf(settings.getTableClickBehavior()));
        options.appendChild(tableClickBehavior);

        // last version used
        IIOMetadataNode version = new IIOMetadataNode("version");
        version.setAttribute("value", versionNumber);
        options.appendChild(version);

        // save debug level tables
        IIOMetadataNode saveDebugTables = new IIOMetadataNode("savedebugtables");
        saveDebugTables.setAttribute("value", String.valueOf(settings.isSaveDebugTables()));
        options.appendChild(saveDebugTables);

        // display tables higher than userlevel
        IIOMetadataNode displayHighTables = new IIOMetadataNode("displayhightables");
        displayHighTables.setAttribute("value", String.valueOf(settings.isDisplayHighTables()));
        options.appendChild(displayHighTables);

        // warning when exceeding limits
        IIOMetadataNode valueLimitWarning = new IIOMetadataNode("valuelimitwarning");
        valueLimitWarning.setAttribute("value", String.valueOf(settings.isValueLimitWarning()));
        options.appendChild(valueLimitWarning);

        // color axis
        IIOMetadataNode colorAxis = new IIOMetadataNode("coloraxis");
        colorAxis.setAttribute("value", String.valueOf(settings.isColorAxis()));
        options.appendChild(colorAxis);

        // show table toolbar border
        IIOMetadataNode showTableToolbarBorder = new IIOMetadataNode("showtabletoolbarborder");
        showTableToolbarBorder.setAttribute("value", String.valueOf(settings.isShowTableToolbarBorder()));
        options.appendChild(showTableToolbarBorder);

        // open top level rom node expanded
        IIOMetadataNode openRomExpanded = new IIOMetadataNode("openromexpanded");
        openRomExpanded.setAttribute("value", String.valueOf(settings.isOpenExpanded()));
        options.appendChild(openRomExpanded);

        // always open at 0
        IIOMetadataNode alwaysOpenTableAtZero = new IIOMetadataNode("alwaysopentableatzero");
        alwaysOpenTableAtZero.setAttribute("value", String.valueOf(settings.isAlwaysOpenTableAtZero()));
        options.appendChild(alwaysOpenTableAtZero);

        // defaultScale
        IIOMetadataNode defaultScale = new IIOMetadataNode("defaultscale");
        defaultScale.setAttribute("value", String.valueOf(settings.getDefaultScale()));
        options.appendChild(defaultScale);

        // scale headers with data
        IIOMetadataNode scaleHeaderAndData = new IIOMetadataNode("scaleHeadersAndData");
        scaleHeaderAndData.setAttribute("value", String.valueOf(settings.isScaleHeadersAndData()));
        options.appendChild(scaleHeaderAndData);

        return options;
    }

    private IIOMetadataNode buildTableDisplay(Settings settings) {
        IIOMetadataNode tableDisplay = new IIOMetadataNode("tabledisplay");

        // font
        IIOMetadataNode font = new IIOMetadataNode("font");
        font.setAttribute("face", settings.getTableFont().getName());
        font.setAttribute("size", String.valueOf(settings.getTableFont().getSize()));
        font.setAttribute("decoration", String.valueOf(settings.getTableFont().getStyle()));
        tableDisplay.appendChild(font);

        // table cell size
        IIOMetadataNode cellSize = new IIOMetadataNode("cellsize");
        cellSize.setAttribute("height", String.valueOf((int) settings.getCellSize().getHeight()));
        cellSize.setAttribute("width", String.valueOf(((int) settings.getCellSize().getWidth())));
        tableDisplay.appendChild(cellSize);

        // colors
        IIOMetadataNode colors = new IIOMetadataNode("colors");
        // max
        IIOMetadataNode max = new IIOMetadataNode("max");
        max.setAttribute("r", String.valueOf(settings.getMaxColor().getRed()));
        max.setAttribute("g", String.valueOf(settings.getMaxColor().getGreen()));
        max.setAttribute("b", String.valueOf(settings.getMaxColor().getBlue()));
        colors.appendChild(max);
        // min
        IIOMetadataNode min = new IIOMetadataNode("min");
        min.setAttribute("r", String.valueOf(settings.getMinColor().getRed()));
        min.setAttribute("g", String.valueOf(settings.getMinColor().getGreen()));
        min.setAttribute("b", String.valueOf(settings.getMinColor().getBlue()));
        colors.appendChild(min);
        // highlight
        IIOMetadataNode highlight = new IIOMetadataNode("highlight");
        highlight.setAttribute("r", String.valueOf(settings.getHighlightColor().getRed()));
        highlight.setAttribute("g", String.valueOf(settings.getHighlightColor().getGreen()));
        highlight.setAttribute("b", String.valueOf(settings.getHighlightColor().getBlue()));
        colors.appendChild(highlight);
        // select
        IIOMetadataNode select = new IIOMetadataNode("select");
        select.setAttribute("r", String.valueOf(settings.getSelectColor().getRed()));
        select.setAttribute("g", String.valueOf(settings.getSelectColor().getGreen()));
        select.setAttribute("b", String.valueOf(settings.getSelectColor().getBlue()));
        colors.appendChild(select);
        // increased cell border
        IIOMetadataNode increaseBorder = new IIOMetadataNode("increaseborder");
        increaseBorder.setAttribute("r", String.valueOf(settings.getIncreaseBorder().getRed()));
        increaseBorder.setAttribute("g", String.valueOf(settings.getIncreaseBorder().getGreen()));
        increaseBorder.setAttribute("b", String.valueOf(settings.getIncreaseBorder().getBlue()));
        colors.appendChild(increaseBorder);
        // decreased cell border
        IIOMetadataNode decreaseBorder = new IIOMetadataNode("decreaseborder");
        decreaseBorder.setAttribute("r", String.valueOf(settings.getDecreaseBorder().getRed()));
        decreaseBorder.setAttribute("g", String.valueOf(settings.getDecreaseBorder().getGreen()));
        decreaseBorder.setAttribute("b", String.valueOf(settings.getDecreaseBorder().getBlue()));
        colors.appendChild(decreaseBorder);
        // axis cells
        IIOMetadataNode axis = new IIOMetadataNode("axis");
        axis.setAttribute("r", String.valueOf(settings.getAxisColor().getRed()));
        axis.setAttribute("g", String.valueOf(settings.getAxisColor().getGreen()));
        axis.setAttribute("b", String.valueOf(settings.getAxisColor().getBlue()));
        colors.appendChild(axis);
        // warning cells
        IIOMetadataNode warning = new IIOMetadataNode("warning");
        warning.setAttribute("r", String.valueOf(settings.getWarningColor().getRed()));
        warning.setAttribute("g", String.valueOf(settings.getWarningColor().getGreen()));
        warning.setAttribute("b", String.valueOf(settings.getWarningColor().getBlue()));
        colors.appendChild(warning);

        tableDisplay.appendChild(colors);

        return tableDisplay;
    }

    private IIOMetadataNode buildLogger(Settings settings) {
        IIOMetadataNode loggerSettings = new IIOMetadataNode("logger");
        loggerSettings.setAttribute("locale", settings.getLocale());

        // serial connection
        IIOMetadataNode serial = new IIOMetadataNode("serial");
        serial.setAttribute("port", settings.getLoggerPortDefault());
        serial.setAttribute("refresh", String.valueOf(settings.getRefreshMode()));
        serial.setAttribute("ecuid", String.valueOf(settings.getDestinationId()));
        serial.setAttribute("fastpoll", String.valueOf(settings.isFastPoll()));
        loggerSettings.appendChild(serial);

        // Protocol connection
        IIOMetadataNode protocol = new IIOMetadataNode("protocol");
        protocol.setAttribute("name", settings.getLoggerProtocol());
        protocol.setAttribute("transport", settings.getTransportProtocol());
        protocol.setAttribute("library", settings.getJ2534Device());
        loggerSettings.appendChild(protocol);

        // window maximized
        IIOMetadataNode maximized = new IIOMetadataNode("maximized");
        maximized.setAttribute("value", String.valueOf((settings.isLoggerWindowMaximized())));
        loggerSettings.appendChild(maximized);

        // window size
        IIOMetadataNode size = new IIOMetadataNode("size");
        size.setAttribute("x", String.valueOf(((int) settings.getLoggerWindowSize().getHeight())));
        size.setAttribute("y", String.valueOf(((int) settings.getLoggerWindowSize().getWidth())));
        size.setAttribute("divider", String.valueOf(((int) settings.getDividerLocation())));
        loggerSettings.appendChild(size);

        // window location
        IIOMetadataNode location = new IIOMetadataNode("location");
        location.setAttribute("x", String.valueOf(((int) settings.getLoggerWindowLocation().getX())));
        location.setAttribute("y", String.valueOf(((int) settings.getLoggerWindowLocation().getY())));
        loggerSettings.appendChild(location);

        // last tab index
        IIOMetadataNode tabs = new IIOMetadataNode("tabs");
        tabs.setAttribute("selected", String.valueOf(settings.getLoggerSelectedTabIndex()));
        tabs.setAttribute("showlist", String.valueOf(settings.getLoggerParameterListState()));
        loggerSettings.appendChild(tabs);

        // definition path
        IIOMetadataNode definition = new IIOMetadataNode("definition");
        definition.setAttribute("path", settings.getLoggerDefinitionFilePath());
        loggerSettings.appendChild(definition);

        // profile path
        IIOMetadataNode profile = new IIOMetadataNode("profile");
        profile.setAttribute("path", settings.getLoggerProfileFilePath());
        loggerSettings.appendChild(profile);

        // file logging
        IIOMetadataNode filelogging = new IIOMetadataNode("filelogging");
        filelogging.setAttribute("path", settings.getLoggerOutputDirPath());
        filelogging.setAttribute("switchid", settings.getFileLoggingControllerSwitchId());
        filelogging.setAttribute("active", String.valueOf(settings.isFileLoggingControllerSwitchActive()));
        filelogging.setAttribute("absolutetimestamp", String.valueOf(settings.isFileLoggingAbsoluteTimestamp()));
        loggerSettings.appendChild(filelogging);

        // debug level
        IIOMetadataNode debug = new IIOMetadataNode("debug");
        debug.setAttribute("level", settings.getLoggerDebuggingLevel());
        loggerSettings.appendChild(debug);

        // plugin ports
        Map<String, String> pluginPorts = settings.getLoggerPluginPorts();
        if (pluginPorts != null && !pluginPorts.isEmpty()) {
            IIOMetadataNode plugins = new IIOMetadataNode("plugins");
            for (Map.Entry<String, String> entry : pluginPorts.entrySet()) {
                IIOMetadataNode plugin = new IIOMetadataNode("plugin");
                plugin.setAttribute("id", entry.getKey());
                plugin.setAttribute("port", entry.getValue());
                plugins.appendChild(plugin);
            }
//            final Map<String, IntfKitSensor> phidgets = settings.getPhidgetSensors();
//            if (phidgets != null && !phidgets.isEmpty()) {
//                final Collection<IntfKitSensor> sensors = phidgets.values();
//                IIOMetadataNode phidgetsNode = new IIOMetadataNode("phidgets");
//                for (IntfKitSensor entry : sensors) {
//                    IIOMetadataNode phidgetNode = new IIOMetadataNode("phidget");
//                    phidgetNode.setAttribute("name", entry.getInputName());
//                    phidgetNode.setAttribute("number", String.valueOf(entry.getInputNumber()));
//                    phidgetNode.setAttribute("units", entry.getUnits());
//                    phidgetNode.setAttribute("expression", entry.getExpression());
//                    phidgetNode.setAttribute("format", entry.getFormat());
//                    phidgetNode.setAttribute("min", String.valueOf(entry.getMinValue()));
//                    phidgetNode.setAttribute("max", String.valueOf(entry.getMaxValue()));
//                    phidgetNode.setAttribute("step", String.valueOf(entry.getStepValue()));
//                    phidgetsNode.appendChild(phidgetNode);
//                }
//                plugins.appendChild(phidgetsNode);
//            }
//            loggerSettings.appendChild(plugins);
        }

        return loggerSettings;
    }

    private IIOMetadataNode buildTableClipboardFormat(Settings settings) {
        // Head Node
        IIOMetadataNode tableClipboardFormatSetting = new IIOMetadataNode(Settings.TABLE_CLIPBOARD_FORMAT_ELEMENT);
        tableClipboardFormatSetting.setAttribute(Settings.TABLE_CLIPBOARD_FORMAT_ATTRIBUTE, settings.getTableClipboardFormat());

        // Table Child
        IIOMetadataNode tableFormatSetting = new IIOMetadataNode(Settings.TABLE_ELEMENT);
        // Table1D Child
        IIOMetadataNode table1DFormatSetting = new IIOMetadataNode(Settings.TABLE1D_ELEMENT);
        // Table2D Child
        IIOMetadataNode table2DFormatSetting = new IIOMetadataNode(Settings.TABLE2D_ELEMENT);
        // Table3D Child
        IIOMetadataNode table3DFormatSetting = new IIOMetadataNode(Settings.TABLE3D_ELEMENT);

        tableFormatSetting.setAttribute(Settings.TABLE_HEADER_ATTRIBUTE, settings.getTableHeader());
        table1DFormatSetting.setAttribute(Settings.TABLE_HEADER_ATTRIBUTE, settings.getTable1DHeader());
        table2DFormatSetting.setAttribute(Settings.TABLE_HEADER_ATTRIBUTE, settings.getTable2DHeader());
        table3DFormatSetting.setAttribute(Settings.TABLE_HEADER_ATTRIBUTE, settings.getTable3DHeader());

        tableClipboardFormatSetting.appendChild(tableFormatSetting);
        tableClipboardFormatSetting.appendChild(table1DFormatSetting);
        tableClipboardFormatSetting.appendChild(table2DFormatSetting);
        tableClipboardFormatSetting.appendChild(table3DFormatSetting);

        return tableClipboardFormatSetting;
    }

    private IIOMetadataNode buildIcons(Settings settings) {
        // Head Node
        IIOMetadataNode iconsSettings = new IIOMetadataNode(Settings.ICONS_ELEMENT_NAME);

        // Editor Icons Child
        IIOMetadataNode editorIconsScaleSettings = new IIOMetadataNode(Settings.EDITOR_ICONS_ELEMENT_NAME);
        editorIconsScaleSettings.setAttribute(Settings.EDITOR_ICONS_SCALE_ATTRIBUTE_NAME, String.valueOf(settings.getEditorIconScale()));

        // Table Icons Child
        IIOMetadataNode tableIconsScaleSettings = new IIOMetadataNode(Settings.TABLE_ICONS_ELEMENT_NAME);
        tableIconsScaleSettings.setAttribute(Settings.TABLE_ICONS_SCALE_ATTRIBUTE_NAME, String.valueOf(settings.getTableIconScale()));

        iconsSettings.appendChild(editorIconsScaleSettings);
        iconsSettings.appendChild(tableIconsScaleSettings);

        return iconsSettings;
    }
}
