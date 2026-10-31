package com.rusefi.ini.reader;

import com.devexperts.logging.Logging;
import com.opensr5.ini.TableModel;

import java.util.LinkedList;
import java.util.function.Consumer;

/**
 * Builder class for constructing TableModel instances while parsing INI files.
 * Accumulates table properties as they are encountered in the [TableEditor] section.
 */
class TableBuilder {
    private static final Logging log = Logging.getLogging(TableBuilder.class);
    private String tableId;
    private String map3dId;
    private String title;
    private String topicHelp;
    private String xLabel;
    private String yLabel;
    private String xBinsConstant;
    private String xBinsChannel;
    private boolean xBinsReadOnly;
    private String yBinsConstant;
    private String yBinsChannel;
    private boolean yBinsReadOnly;
    private String zBinsConstant;
    private String upLabel;
    private String downLabel;
    private Double gridHeight;
    private String gridOrient;

    void setTableDefinition(String tableId, String map3dId, String title) {
        this.tableId = tableId;
        this.map3dId = map3dId;
        this.title = title;
    }

    void setTopicHelp(String topicHelp) {
        this.topicHelp = topicHelp;
    }

    void setXyLabels(String xLabel, String yLabel) {
        this.xLabel = xLabel;
        this.yLabel = yLabel;
    }

    void setXBins(String constant, String channel, boolean readOnly) {
        this.xBinsConstant = constant;
        this.xBinsChannel = channel;
        this.xBinsReadOnly = readOnly;
    }

    void setYBins(String constant, String channel, boolean readOnly) {
        this.yBinsConstant = constant;
        this.yBinsChannel = channel;
        this.yBinsReadOnly = readOnly;
    }

    void setZBins(String constant) {
        this.zBinsConstant = constant;
    }

    void setUpDownLabel(String upLabel, String downLabel) {
        this.upLabel = upLabel;
        this.downLabel = downLabel;
    }

    void setGridHeight(Double gridHeight) {
        this.gridHeight = gridHeight;
    }

    void setGridOrient(String gridOrient) {
        this.gridOrient = gridOrient;
    }

    boolean isComplete() {
        return tableId != null && xBinsConstant != null && yBinsConstant != null && zBinsConstant != null;
    }

    boolean isActive() {
        return tableId != null;
    }

    String getTableId() {
        return tableId;
    }

    String getMap3dId() {
        return map3dId;
    }

    String getXBinsConstant() {
        return xBinsConstant;
    }

    String getYBinsConstant() {
        return yBinsConstant;
    }

    String getZBinsConstant() {
        return zBinsConstant;
    }

    TableModel build() {
        if (!isComplete()) {
            throw new IllegalStateException("Cannot build incomplete table: " + tableId);
        }

        return new TableModel(
                tableId,
                map3dId,
                title,
                topicHelp,
                xLabel,
                yLabel,
                xBinsConstant,
                xBinsChannel,
                xBinsReadOnly,
                yBinsConstant,
                yBinsChannel,
                yBinsReadOnly,
                zBinsConstant,
                upLabel,
                downLabel,
                gridHeight,
                gridOrient
        );
    }

    void reset() {
        tableId = null;
        map3dId = null;
        title = null;
        topicHelp = null;
        xLabel = null;
        yLabel = null;
        xBinsConstant = null;
        xBinsChannel = null;
        xBinsReadOnly = false;
        yBinsConstant = null;
        yBinsChannel = null;
        yBinsReadOnly = false;
        zBinsConstant = null;
        upLabel = null;
        downLabel = null;
        gridHeight = null;
        gridOrient = null;
    }

    /**
     * Handles a line from the [TableEditor] section.
     * Returns true if this was a "table" definition line (indicating a new table is starting).
     *
     * @param list The parsed tokens from the line
     * @param fieldRegistrar Callback to register bin constants in fieldsInUiOrder
     * @param finishCurrentTable Callback to finish/save the current table before starting a new one
     * @return true if this was a "table" definition, false otherwise
     */
    boolean handleLine(LinkedList<String> list, Consumer<String> fieldRegistrar, Runnable finishCurrentTable) {
        if (list.size() < 2)
            return false;

        String keyword = list.get(0);

        switch (keyword) {
            case "table":
                // Finish the previous table before starting a new one
                finishCurrentTable.run();
                parseTableDefinition(list);
                return true;
            case "topicHelp":
                if (isActive()) {
                    setTopicHelp(list.get(1));
                }
                break;
            case "xyLabels":
                if (isActive() && list.size() >= 3) {
                    setXyLabels(list.get(1), list.get(2));
                }
                break;
            case "xBins":
                if (isActive() && list.size() >= 2) {
                    String constant = list.get(1);
                    String channel = list.size() >= 3 ? list.get(2) : null;
                    boolean readOnly = list.contains("readOnly");
                    setXBins(constant, channel, readOnly);
                    fieldRegistrar.accept(constant);
                }
                break;
            case "yBins":
                if (isActive() && list.size() >= 2) {
                    String constant = list.get(1);
                    String channel = list.size() >= 3 ? list.get(2) : null;
                    boolean readOnly = list.contains("readOnly");
                    setYBins(constant, channel, readOnly);
                    fieldRegistrar.accept(constant);
                }
                break;
            case "zBins":
                if (isActive() && list.size() >= 2) {
                    String constant = list.get(1);
                    setZBins(constant);
                    fieldRegistrar.accept(constant);
                }
                break;
            case "upDownLabel":
                if (isActive() && list.size() >= 3) {
                    setUpDownLabel(list.get(1), list.get(2));
                }
                break;
            case "gridHeight":
                if (isActive() && list.size() >= 2) {
                    try {
                        setGridHeight(Double.parseDouble(list.get(1)));
                    } catch (NumberFormatException e) {
                        log.warn("Failed to parse gridHeight: " + list.get(1));
                    }
                }
                break;
            case "gridOrient":
                if (isActive() && list.size() >= 2) {
                    // gridOrient can have multiple values (e.g., "250, 0, 340")
                    setGridOrient(String.join(",", list.subList(1, list.size())));
                }
                break;
        }
        return false;
    }

    private void parseTableDefinition(LinkedList<String> list) {
        // table = table_id, map3d_id, "title" [, page]
        if (list.size() < 4) {
            log.warn("Incomplete table definition, expected at least 4 fields");
            return;
        }

        list.removeFirst(); // remove "table"
        String tableId = list.removeFirst().trim();
        String map3dId = list.removeFirst().trim();
        String title = list.removeFirst().trim();

        setTableDefinition(tableId, map3dId, title);
    }
}
