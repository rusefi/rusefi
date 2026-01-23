package com.opensr5.ini;

/**
 * Represents a table configuration from INI file [TableEditor] section.
 * Tables define 2D/3D visualizations of configuration data with x/y/z bins.
 */
public class TableModel {
    private final String tableId;
    private final String map3dId;
    private final String title;
    private final String topicHelp;
    private final String xLabel;
    private final String yLabel;
    private final String xBinsConstant;
    private final String xBinsChannel;
    private final boolean xBinsReadOnly;
    private final String yBinsConstant;
    private final String yBinsChannel;
    private final boolean yBinsReadOnly;
    private final String zBinsConstant;
    private final String upLabel;
    private final String downLabel;
    private final Double gridHeight;
    private final String gridOrient;

    public TableModel(String tableId, String map3dId, String title,
                      String topicHelp, String xLabel, String yLabel,
                      String xBinsConstant, String xBinsChannel, boolean xBinsReadOnly,
                      String yBinsConstant, String yBinsChannel, boolean yBinsReadOnly,
                      String zBinsConstant, String upLabel, String downLabel,
                      Double gridHeight, String gridOrient) {
        this.tableId = tableId;
        this.map3dId = map3dId;
        this.title = title;
        this.topicHelp = topicHelp;
        this.xLabel = xLabel;
        this.yLabel = yLabel;
        this.xBinsConstant = xBinsConstant;
        this.xBinsChannel = xBinsChannel;
        this.xBinsReadOnly = xBinsReadOnly;
        this.yBinsConstant = yBinsConstant;
        this.yBinsChannel = yBinsChannel;
        this.yBinsReadOnly = yBinsReadOnly;
        this.zBinsConstant = zBinsConstant;
        this.upLabel = upLabel;
        this.downLabel = downLabel;
        this.gridHeight = gridHeight;
        this.gridOrient = gridOrient;
    }

    public String getTableId() {
        return tableId;
    }

    public String getMap3dId() {
        return map3dId;
    }

    public String getTitle() {
        return title;
    }

    public String getTopicHelp() {
        return topicHelp;
    }

    public String getXLabel() {
        return xLabel;
    }

    public String getYLabel() {
        return yLabel;
    }

    public String getXBinsConstant() {
        return xBinsConstant;
    }

    public String getXBinsChannel() {
        return xBinsChannel;
    }

    public boolean isXBinsReadOnly() {
        return xBinsReadOnly;
    }

    public String getYBinsConstant() {
        return yBinsConstant;
    }

    public String getYBinsChannel() {
        return yBinsChannel;
    }

    public boolean isYBinsReadOnly() {
        return yBinsReadOnly;
    }

    public String getZBinsConstant() {
        return zBinsConstant;
    }

    public String getUpLabel() {
        return upLabel;
    }

    public String getDownLabel() {
        return downLabel;
    }

    public Double getGridHeight() {
        return gridHeight;
    }

    public String getGridOrient() {
        return gridOrient;
    }
}
