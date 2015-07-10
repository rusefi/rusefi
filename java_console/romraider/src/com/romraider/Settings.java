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

package com.romraider;

import static com.romraider.Version.RELEASE_NOTES;
//import static com.romraider.Version.ROM_REVISION_URL;
import static com.romraider.Version.SUPPORT_URL;
import static com.romraider.util.ParamChecker.checkNotNullOrEmpty;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.io.File;
import java.io.Serializable;
import java.util.Locale;
import java.util.Map;
import java.util.Vector;

import com.romraider.io.connection.ConnectionProperties;
import com.romraider.logger.ecu.definition.EcuDefinition;
//import com.romraider.logger.external.phidget.interfacekit.io.IntfKitSensor;

public class Settings implements Serializable {

    private static final long serialVersionUID = 1026542922680475190L;

    /* String Format Settings */
    public static final String NEW_LINE = System.getProperty("line.separator");
    public static final String TAB = "\t";
    public static final String BLANK = "";

    /* Clipboard Settings */
    public static final String TABLE_CLIPBOARD_FORMAT_ELEMENT = "table-clipboard-format";
    public static final String TABLE_CLIPBOARD_FORMAT_ATTRIBUTE = "format-string";
    public static final String TABLE_ELEMENT = "table";
    public static final String TABLE1D_ELEMENT = "table1D";
    public static final String TABLE2D_ELEMENT = "table2D";
    public static final String TABLE3D_ELEMENT = "table3D";
    public static final String TABLE_HEADER_ATTRIBUTE = "table-header";

    public static final String DEFAULT_CLIPBOARD_FORMAT = "Default";
    public static final String DEFAULT_TABLE_HEADER = "[Table1D]" + NEW_LINE;
    public static final String DEFAULT_TABLE1D_HEADER = "";
    public static final String DEFAULT_TABLE2D_HEADER = "[Table2D]" + NEW_LINE;
    public static final String DEFAULT_TABLE3D_HEADER = "[Table3D]" + NEW_LINE;

    public static final String AIRBOYS_CLIPBOARD_FORMAT = "Airboys";
    public static final String AIRBOYS_TABLE_HEADER = "";
    public static final String AIRBOYS_TABLE1D_HEADER = "";
    public static final String AIRBOYS_TABLE2D_HEADER = "[Table2D]" + NEW_LINE;
    public static final String AIRBOYS_TABLE3D_HEADER = "[Table3D]" + TAB;

    public static final String CUSTOM_CLIPBOARD_FORMAT = "Custom";

    /* XML Settings */
    public static final String REPOSITORY_ELEMENT_NAME = "repository-dir";
    public static final String REPOSITORY_ATTRIBUTE_NAME = "path";

    public static final String ICONS_ELEMENT_NAME = "icons";
    public static final String EDITOR_ICONS_ELEMENT_NAME = "editor-toolbar";
    public static final String EDITOR_ICONS_SCALE_ATTRIBUTE_NAME = "scale";
    public static final String TABLE_ICONS_ELEMENT_NAME = "table-toolbar";
    public static final String TABLE_ICONS_SCALE_ATTRIBUTE_NAME = "scale";

    /* UI Settings */
    public static final int DEFAULT_EDITOR_ICON_SCALE = 50;
    public static final int DEFAULT_TABLE_ICON_SCALE = 70;

    /* Table Settings */
    public static final String defaultTableToolBarName = "Table Tools";

    public static final int ENDIAN_LITTLE = 1;
    public static final int ENDIAN_BIG = 2;

    public static final int TABLE_1D = 1;
    public static final int TABLE_2D = 2;
    public static final int TABLE_3D = 3;
    public static final int TABLE_X_AXIS = 4;
    public static final int TABLE_Y_AXIS = 5;
    public static final int TABLE_SWITCH = 6;

    public static final int DATA_TYPE_ORIGINAL = 0;
    public static final int DATA_TYPE_BIN = 1;
    public static final int DATA_TYPE_REAL = 3;

    public static final int COMPARE_DISPLAY_PERCENT = 1;
    public static final int COMPARE_DISPLAY_ABSOLUTE = 2;

    public static final int STORAGE_TYPE_FLOAT = 99;
    public static final boolean STORAGE_DATA_SIGNED = false;

    public static final Color UNCHANGED_VALUE_COLOR = new Color(160, 160, 160);

    public static final String INVALID_ATTRIBUTE_TEXT = "invalid";

    public static final String DEFAULT_TABLE_NAME = "Unknown";

    /* Rom Settings */
    public static final int CHECK_TOTAL = 0x5AA5A55A;
    public static final int SIXTEENBIT_START_ADDRESS = 0x20000;
    public static final int SIXTEENBIT_END_ADDRESS = 0x28000;
    public static final int SIXTEENBIT_SMALL_SIZE = 0x28000;
    public static final int SIXTEENBIT_LARGE_SIZE = 0x30000;
    public static final int SIXTEENBIT_SEGMENT_SIZE = SIXTEENBIT_SMALL_SIZE - SIXTEENBIT_START_ADDRESS; // 0x8000
    public static final int SIXTEENBIT_SEGMENT_VALUE = 0x00;

    /* Scale Settings */
    public static final int LINEAR = 1;
    public static final int INVERSE = 2;

    /* Compare Image Settings */
    public static Color TABLE_EQUAL_COLOR = new Color(52,114,53);
    public static Color TABLE_DIFFERENT_COLOR = new Color(193, 27, 23);
    public static Color TABLE_MISSING_COLOR = new Color(251,185,23);

    /* Compare DTC Foreground Colors */
    public static Color TABLESWITCH_DEFAULT_COLOR = Color.black;
    public static Color TABLESWITCH_DIFFERENT_COLOR = new Color(193, 27, 23);

    /* MDI Desktop Settings*/
    public static int FRAME_OFFSET = 20;

    /* Scale Settings */
    public static String DEFAULT_SCALE = "Default";
    public static String METRIC_SCALE = "Metric";
    public static String STANDARD_SCALE = "Standard";

    /* DataCell Colors */
    public static final Color scaleTextColor = new Color(0, 0, 0);
    public static final Color highlightTextColor = new Color(255, 255, 255);
    public static final Color selectTextColor = new Color(0, 0, 0);
    public static final Color liveDataTraceTextColor = new Color(50, 50, 50);

    private static final String ISO15765 = "ISO15765";
    private static final String ISO9141 = "ISO9141";
    private static final String SYSTEM_NUMFORMAT = "system";
    private static final String USER_LANGUAGE = "user.language";
    private static final String USER_COUNTRY = "user.country";
    private static final String EN_US = "en_US";
    private static final String SSM = "SSM";
    private static final String OBD = "OBD";

    private final Dimension windowSize = new Dimension(800, 600);
    private final Point windowLocation = new Point();
    private int splitPaneLocation = 150;
    private boolean windowMaximized;

    private String recentVersion = "x";

    private Vector<File> ecuDefinitionFiles = new Vector<File>();
    private File lastImageDir = new File("images");
    private File lastRepositoryDir = new File("repositories");
    private boolean obsoleteWarning = true;
    private boolean calcConflictWarning = true;
    private boolean debug;
    private int userLevel = 1;
    private boolean saveDebugTables = true;
    private boolean displayHighTables = true;
    private boolean valueLimitWarning = true;

    private Font tableFont = new Font("Arial", Font.BOLD, 11);
    private Dimension cellSize = new Dimension(42, 18);
    private Color maxColor = new Color(255, 102, 102);
    private Color minColor = new Color(153, 153, 255);

    private Color selectColor = new Color(204, 204, 204);
    private Color highlightColor = new Color(27, 161, 226);
    private Color liveValueColor = new Color (0, 255, 0);

    private Color decreaseBorder = new Color(0, 0, 255);
    private Color increaseBorder = new Color(255, 0, 0);

    private Color axisColor = new Color(255, 255, 255);
    private Color warningColor = new Color(255, 0, 0);
    private int tableClickCount = 1; // number of clicks to open table
    private int tableClickBehavior = 0; // TableTreeNode click behavior. 0=open/close frame, 1=open/focus frame
    private boolean colorAxis = false;

    private String loggerPort;
    private String loggerPortDefault;
    private static String loggerProtocol = SSM;
    private static String loggerDefinitionFilePath;
    private static String loggerProfileFilePath;
    private static String loggerOutputDirPath = System.getProperty("user.home");
    private String fileLoggingControllerSwitchId = "S20"; // defogger switch by default
    private boolean fileLoggingControllerSwitchActive = true;
    private boolean fileLoggingAbsoluteTimestamp;
    private String logfileNameText;
    private boolean logExternalsOnly;
    private static String userLocale = SYSTEM_NUMFORMAT;

    private Dimension loggerWindowSize = new Dimension(1000, 600);
    private Point loggerWindowLocation = new Point();
    private boolean loggerWindowMaximized;
    private int loggerSelectedTabIndex;
    private boolean loggerParameterListState = true;
    private ConnectionProperties loggerConnectionProperties;
    private Map<String, EcuDefinition> loggerEcuDefinitionMap;
    private Map<String, String> loggerPluginPorts;
    private boolean loggerRefreshMode;
    private static byte loggerDestinationId = 0x10;
    private boolean fastPoll = true;
    private double loggerDividerLocation = 400;
    private String loggerDebuggingLevel = "info";
    private static String j2534Device;
    private static String transportProtocol = ISO9141;

    private String tableClipboardFormat = DEFAULT_CLIPBOARD_FORMAT; // Currently 2 options.  Default and Airboy. Custom is not hooked up.
    private String tableHeader = DEFAULT_TABLE_HEADER;
    private String table1DHeader = DEFAULT_TABLE1D_HEADER;
    private String table2DHeader = DEFAULT_TABLE2D_HEADER;
    private String table3DHeader = DEFAULT_TABLE3D_HEADER;

    private int editorIconScale = DEFAULT_EDITOR_ICON_SCALE;
    private int tableIconScale = DEFAULT_TABLE_ICON_SCALE;

    private boolean openExpanded = true;
    private boolean showTableToolbarBorder = false;
    private boolean alwaysOpenTableAtZero = false;
    private boolean scaleHeadersAndData = true;

    private String defaultScale = "Metric";

//    private Map<String, IntfKitSensor> phidgetSensors;

    public Settings() {
        //center window by default
        Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
        windowLocation.move(((int) (screenSize.getWidth() - windowSize.getWidth()) / 2),
                ((int) (screenSize.getHeight() - windowSize.getHeight()) / 2));
    }

    public Dimension getWindowSize() {
        return windowSize;
    }

    public Point getWindowLocation() {
        return windowLocation;
    }

    public void setWindowSize(Dimension size) {
        windowSize.setSize(size);
    }

    public void setWindowLocation(Point location) {
        windowLocation.setLocation(location);
    }

    public Vector<File> getEcuDefinitionFiles() {
        return ecuDefinitionFiles;
    }

    public void addEcuDefinitionFile(File ecuDefinitionFile) {
        ecuDefinitionFiles.add(ecuDefinitionFile);
    }

    public void setEcuDefinitionFiles(Vector<File> ecuDefinitionFiles) {
        this.ecuDefinitionFiles = ecuDefinitionFiles;
    }

    public File getLastImageDir() {
        return lastImageDir;
    }

    public void setLastImageDir(File lastImageDir) {
        this.lastImageDir = lastImageDir;
    }

    public File getLastRepositoryDir() {
        return lastRepositoryDir;
    }

    public void setLastRepositoryDir(File lastRepositoryDir) {
        this.lastRepositoryDir = lastRepositoryDir;
    }

    public int getSplitPaneLocation() {
        return splitPaneLocation;
    }

    public void setSplitPaneLocation(int splitPaneLocation) {
        this.splitPaneLocation = splitPaneLocation;
    }

    public boolean isWindowMaximized() {
        return windowMaximized;
    }

    public void setWindowMaximized(boolean windowMaximized) {
        this.windowMaximized = windowMaximized;
    }

//    public String getRomRevisionURL() {
//        return ROM_REVISION_URL;
//    }

    public String getSupportURL() {
        return SUPPORT_URL;
    }

    public Font getTableFont() {
        return tableFont;
    }

    public void setTableFont(Font tableFont) {
        this.tableFont = tableFont;
    }

    public boolean isObsoleteWarning() {
        return obsoleteWarning;
    }

    public void setObsoleteWarning(boolean obsoleteWarning) {
        this.obsoleteWarning = obsoleteWarning;
    }

    public boolean isDebug() {
        return debug;
    }

    public void setDebug(boolean debug) {
        this.debug = debug;
    }

    public Dimension getCellSize() {
        return cellSize;
    }

    public void setCellSize(Dimension cellSize) {
        this.cellSize = cellSize;
    }

    public Color getMaxColor() {
        return maxColor;
    }

    public void setMaxColor(Color maxColor) {
        this.maxColor = maxColor;
    }

    public Color getMinColor() {
        return minColor;
    }

    public void setMinColor(Color minColor) {
        this.minColor = minColor;
    }

    public Color getHighlightColor() {
        return highlightColor;
    }

    public void setHighlightColor(Color highlightColor) {
        this.highlightColor = highlightColor;
    }

    public Color getliveValueColor() {
        return this.liveValueColor;
    }

    public void setLiveValueColor(Color liveValueColor) {
        this.liveValueColor = liveValueColor;
    }

    public Color getSelectColor() {
        return selectColor;
    }

    public void setSelectColor(Color selectColor) {
        this.selectColor = selectColor;
    }

    public boolean isCalcConflictWarning() {
        return calcConflictWarning;
    }

    public void setCalcConflictWarning(boolean calcConflictWarning) {
        this.calcConflictWarning = calcConflictWarning;
    }

    public Color getIncreaseBorder() {
        return increaseBorder;
    }

    public void setIncreaseBorder(Color increaseBorder) {
        this.increaseBorder = increaseBorder;
    }

    public Color getDecreaseBorder() {
        return decreaseBorder;
    }

    public void setDecreaseBorder(Color decreaseBorder) {
        this.decreaseBorder = decreaseBorder;
    }

    public Color getAxisColor() {
        return axisColor;
    }

    public void setAxisColor(Color axisColor) {
        this.axisColor = axisColor;
    }

    public int getUserLevel() {
        return userLevel;
    }

    public void setUserLevel(int userLevel) {
        if (userLevel > 5) {
            this.userLevel = 5;
        } else if (userLevel < 1) {
            this.userLevel = 1;
        } else {
            this.userLevel = userLevel;
        }
    }

    public int getTableClickCount() {
        return tableClickCount;
    }

    public void setTableClickCount(int tableClickCount) {
        this.tableClickCount = tableClickCount;
    }

    public int getTableClickBehavior() {
        return tableClickBehavior;
    }

    public void setTableClickBehavior(int clickBehavior) {
        // 0 = open/close
        // 1 = open/focus
        this.tableClickBehavior = clickBehavior;
    }

    public String getRecentVersion() {
        return recentVersion;
    }

    public void setRecentVersion(String recentVersion) {
        this.recentVersion = recentVersion;
    }

    public String getReleaseNotes() {
        return RELEASE_NOTES;
    }

    public boolean isSaveDebugTables() {
        return saveDebugTables;
    }

    public void setSaveDebugTables(boolean saveDebugTables) {
        this.saveDebugTables = saveDebugTables;
    }

    public boolean isDisplayHighTables() {
        return displayHighTables;
    }

    public void setDisplayHighTables(boolean displayHighTables) {
        this.displayHighTables = displayHighTables;
    }

    public boolean isValueLimitWarning() {
        return valueLimitWarning;
    }

    public void setValueLimitWarning(boolean valueLimitWarning) {
        this.valueLimitWarning = valueLimitWarning;
    }

    public Color getWarningColor() {
        return warningColor;
    }

    public void setWarningColor(Color warningColor) {
        this.warningColor = warningColor;
    }

    public String getLoggerPort() {
        return loggerPort;
    }

    public void setLoggerPort(String loggerPort) {
        this.loggerPort = loggerPort;
    }

    public String getLoggerPortDefault() {
        return loggerPortDefault;
    }

    public void setLoggerPortDefault(String loggerPortDefault) {
        this.loggerPortDefault = loggerPortDefault;
    }

    public void setLoggerProtocol(String protocol) {
        Settings.loggerProtocol = protocol;
    }

    public String getLoggerProtocol() {
        return loggerProtocol;
    }

    public String getLoggerDefinitionFilePath() {
        return loggerDefinitionFilePath;
    }

    public void setLoggerDefinitionFilePath(String loggerDefinitionFilePath) {
        Settings.loggerDefinitionFilePath = loggerDefinitionFilePath;
    }

    public String getLoggerOutputDirPath() {
        return loggerOutputDirPath;
    }

    public Point getLoggerWindowLocation() {
        return loggerWindowLocation;
    }

    public void setLoggerWindowLocation(Point loggerWindowLocation) {
        this.loggerWindowLocation = loggerWindowLocation;
    }

    public boolean isLoggerWindowMaximized() {
        return loggerWindowMaximized;
    }

    public void setLoggerWindowMaximized(boolean loggerWindowMaximized) {
        this.loggerWindowMaximized = loggerWindowMaximized;
    }

    public Dimension getLoggerWindowSize() {
        return loggerWindowSize;
    }

    public void setLoggerWindowSize(Dimension loggerWindowSize) {
        this.loggerWindowSize = loggerWindowSize;
    }

    public double getDividerLocation() {
        return loggerDividerLocation;
    }

    public void setLoggerDividerLocation(double dividerLocation) {
        this.loggerDividerLocation = dividerLocation;
    }

    public String getLoggerProfileFilePath() {
        return loggerProfileFilePath;
    }

    public void setLoggerProfileFilePath(String loggerProfileFilePath) {
        Settings.loggerProfileFilePath = loggerProfileFilePath;
    }

    public void setLoggerOutputDirPath(String loggerOutputDirPath) {
        Settings.loggerOutputDirPath = loggerOutputDirPath;
    }

    public String getFileLoggingControllerSwitchId() {
        return fileLoggingControllerSwitchId;
    }

    public void setFileLoggingControllerSwitchId(String fileLoggingControllerSwitchId) {
        checkNotNullOrEmpty(fileLoggingControllerSwitchId, "fileLoggingControllerSwitchId");
        this.fileLoggingControllerSwitchId = fileLoggingControllerSwitchId;
    }

    public boolean isFileLoggingControllerSwitchActive() {
        return fileLoggingControllerSwitchActive;
    }

    public void setFileLoggingControllerSwitchActive(boolean fileLoggingControllerSwitchActive) {
        this.fileLoggingControllerSwitchActive = fileLoggingControllerSwitchActive;
    }

    public boolean isFileLoggingAbsoluteTimestamp() {
        return fileLoggingAbsoluteTimestamp;
    }

    public void setFileLoggingAbsoluteTimestamp(boolean fileLoggingAbsoluteTimestamp) {
        this.fileLoggingAbsoluteTimestamp = fileLoggingAbsoluteTimestamp;
    }

    public ConnectionProperties getLoggerConnectionProperties() {
        return loggerConnectionProperties;
    }

    public void setLoggerConnectionProperties(ConnectionProperties loggerConnectionProperties) {
        this.loggerConnectionProperties = loggerConnectionProperties;
    }

    public Map<String, EcuDefinition> getLoggerEcuDefinitionMap() {
        return loggerEcuDefinitionMap;
    }

    public void setLoggerEcuDefinitionMap(Map<String, EcuDefinition> loggerEcuDefinitionMap) {
        this.loggerEcuDefinitionMap = loggerEcuDefinitionMap;
    }

    public void setLoggerSelectedTabIndex(int loggerSelectedTabIndex) {
        this.loggerSelectedTabIndex = loggerSelectedTabIndex;
    }

    public int getLoggerSelectedTabIndex() {
        return loggerSelectedTabIndex;
    }

    public Map<String, String> getLoggerPluginPorts() {
        return loggerPluginPorts;
    }

    public void setLoggerPluginPorts(Map<String, String> loggerPluginPorts) {
        this.loggerPluginPorts = loggerPluginPorts;
    }

    public void setLoggerParameterListState(boolean ShowListState) {
        this.loggerParameterListState = ShowListState;
    }

    public boolean getLoggerParameterListState() {
        return loggerParameterListState;
    }

    public void setRefreshMode(boolean selected) {
        this.loggerRefreshMode = selected;
    }

    public boolean getRefreshMode() {
        return loggerRefreshMode;
    }

    public void setDestinationId(byte id) {
        loggerDestinationId = id;
    }

    public byte getDestinationId() {
        return loggerDestinationId;
    }

    public void setFastPoll(boolean state) {
        this.fastPoll = state;
    }

    public boolean isFastPoll() {
        return fastPoll;
    }

    public void setLogfileNameText(String text) {
        this.logfileNameText = text;
    }

    public String getLogfileNameText() {
        return logfileNameText;
    }

    public void setLoggerDebuggingLevel(String level) {
        this.loggerDebuggingLevel  = level;
    }

    public String getLoggerDebuggingLevel() {
        return loggerDebuggingLevel;
    }

    public void setJ2534Device(String j2534Device) {
        Settings.j2534Device = j2534Device;
    }

    public String getJ2534Device() {
        return j2534Device;
    }

    public void setTransportProtocol(String transport) {
        Settings.transportProtocol = transport;
    }

    public String getTransportProtocol() {
        return transportProtocol;
    }

    public void setTableClipboardFormat(String formatString) {
        this.tableClipboardFormat = formatString;
    }

    public String getTableClipboardFormat() {
        return this.tableClipboardFormat;
    }

    public void setTableHeader(String header) {
        this.tableHeader = header;
    }

    public String getTableHeader() {
        return this.tableHeader;
    }

    public void setTable1DHeader(String header) {
        this.table1DHeader = header;
    }

    public String getTable1DHeader() {
        return this.table1DHeader;
    }

    public void setTable2DHeader(String header) {
        this.table2DHeader = header;
    }

    public String getTable2DHeader() {
        return this.table2DHeader;
    }

    public void setTable3DHeader(String header) {
        this.table3DHeader = header;
    }

    public String getTable3DHeader() {
        return this.table3DHeader;
    }

    public void setDefaultFormat() {
        this.tableClipboardFormat = DEFAULT_CLIPBOARD_FORMAT;
        this.tableHeader = DEFAULT_TABLE_HEADER;
        this.table1DHeader = DEFAULT_TABLE1D_HEADER;
        this.table2DHeader = DEFAULT_TABLE2D_HEADER;
        this.table3DHeader = DEFAULT_TABLE3D_HEADER;
    }

    public void setAirboysFormat() {
        this.tableClipboardFormat = AIRBOYS_CLIPBOARD_FORMAT;
        this.tableHeader = AIRBOYS_TABLE_HEADER;
        this.table1DHeader = AIRBOYS_TABLE1D_HEADER;
        this.table2DHeader = AIRBOYS_TABLE2D_HEADER;
        this.table3DHeader = AIRBOYS_TABLE3D_HEADER;
    }

    public int getEditorIconScale() {
        return this.editorIconScale;
    }

    public void setEditorIconScale(int scale) {
        this.editorIconScale = scale;
    }

    public int getTableIconScale() {
        return this.tableIconScale;
    }

    public void setTableIconScale(int scale) {
        this.tableIconScale = scale;
    }

    public void setLogExternalsOnly(boolean state) {
        this.logExternalsOnly = state;
    }

    public boolean isLogExternalsOnly() {
        return logExternalsOnly;
    }

    public boolean isCanBus() {
        if (transportProtocol.equals(ISO15765)) {
            return true;
        }
        return false;
    }

    public boolean isObdProtocol() {
        if (loggerProtocol.equals(OBD)) {
            return true;
        }
        return false;
    }

    public final boolean isUsNumberFormat() {
        if (userLocale.equalsIgnoreCase(EN_US)) {
            return true;
        }
        return false;
    }

    public final String getLocale() {
        return userLocale;
    }

    public final void setLocale(String locale) {
        userLocale = locale;
        if (!locale.equalsIgnoreCase(SYSTEM_NUMFORMAT)) {
            final String[] language = locale.split("_");
            System.setProperty(USER_LANGUAGE, language[0]);
            System.setProperty(USER_COUNTRY, language[1]);
            final Locale lc = new Locale(language[0], language[1]);
            Locale.setDefault(lc);
        }
    }

    public boolean isOpenExpanded() {
        return openExpanded;
    }

    public void setOpenExpanded(boolean expanded) {
        this.openExpanded = expanded;
    }

    public boolean isShowTableToolbarBorder() {
        return showTableToolbarBorder;
    }

    public void setShowTableToolbarBorder(boolean showBorder) {
        this.showTableToolbarBorder = showBorder;
    }

    public boolean isAlwaysOpenTableAtZero() {
        return alwaysOpenTableAtZero;
    }

    public void setAlwaysOpenTableAtZero(boolean openAtZero) {
        this.alwaysOpenTableAtZero = openAtZero;
    }

    public boolean isScaleHeadersAndData() {
        return this.scaleHeadersAndData;
    }

    public void setScaleHeadersAndData(boolean headersAndData) {
        this.scaleHeadersAndData = headersAndData;
    }

    public boolean isColorAxis() {
        return this.colorAxis;
    }

    public void setColorAxis(boolean colorAxis) {
        this.colorAxis = colorAxis;
    }

    public String getDefaultScale() {
        return this.defaultScale;
    }

    public void setDefaultScale(String defaultScale) {
        this.defaultScale = defaultScale;
    }

//    public Map<String, IntfKitSensor> getPhidgetSensors() {
//        return this.phidgetSensors;
//    }
//
//    public void setPhidgetSensors(
//            Map<String, IntfKitSensor> phidgetSensors) {
//
//        this.phidgetSensors = phidgetSensors;
//    }
}
