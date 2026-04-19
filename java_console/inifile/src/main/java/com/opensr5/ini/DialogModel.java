package com.opensr5.ini;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/17/2016.
 */
public class DialogModel {
    private final String key;
    private final String uiName;
    private final List<Field> fields;
    private final List<Command> commandsOfCurrentDialog;
    private final List<PanelModel> panels;
    private final List<IndicatorModel> indicators;
    private final List<ReadoutModel> readouts;
    private final int readoutColumns;
    private final List<String> gaugeNames;
    private final String topicHelp;
    private final String layoutHint;
    private final List<DialogEntry> orderedEntries;

    /** Ordered entry list preserving INI declaration order across all element types. */
    public static final class DialogEntry {
        public enum Kind { FIELD, COMMAND, INDICATOR, PANEL, GAUGE }
        public final Kind kind;
        private final Object element;

        public DialogEntry(Kind kind, Object element) {
            this.kind = kind;
            this.element = element;
        }

        public <T> T getAs(Class<T> clazz) {
            return clazz.cast(element);
        }
    }

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog) {
        this(key, uiName, fields, commandsOfCurrentDialog, new ArrayList<>(), new ArrayList<>(), new ArrayList<>(), 1, new ArrayList<>(), null, null, Collections.emptyList());
    }

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog, String topicHelp) {
        this(key, uiName, fields, commandsOfCurrentDialog, new ArrayList<>(), new ArrayList<>(), new ArrayList<>(), 1, new ArrayList<>(), topicHelp, null, Collections.emptyList());
    }

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog, List<PanelModel> panels, String topicHelp) {
        this(key, uiName, fields, commandsOfCurrentDialog, panels, new ArrayList<>(), new ArrayList<>(), 1, new ArrayList<>(), topicHelp, null, Collections.emptyList());
    }

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog, List<PanelModel> panels, String topicHelp, String layoutHint) {
        this(key, uiName, fields, commandsOfCurrentDialog, panels, new ArrayList<>(), new ArrayList<>(), 1, new ArrayList<>(), topicHelp, layoutHint, Collections.emptyList());
    }

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog, List<PanelModel> panels, List<IndicatorModel> indicators, String topicHelp, String layoutHint) {
        this(key, uiName, fields, commandsOfCurrentDialog, panels, indicators, new ArrayList<>(), 1, new ArrayList<>(), topicHelp, layoutHint, Collections.emptyList());
    }

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog, List<PanelModel> panels, List<IndicatorModel> indicators, List<ReadoutModel> readouts, int readoutColumns, List<String> gaugeNames, String topicHelp, String layoutHint) {
        this(key, uiName, fields, commandsOfCurrentDialog, panels, indicators, readouts, readoutColumns, gaugeNames, topicHelp, layoutHint, Collections.emptyList());
    }

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog, List<PanelModel> panels, List<IndicatorModel> indicators, List<ReadoutModel> readouts, int readoutColumns, List<String> gaugeNames, String topicHelp, String layoutHint, List<DialogEntry> orderedEntries) {
        this.key = key;
        this.uiName = uiName;
        this.fields = new ArrayList<>(fields);
        this.commandsOfCurrentDialog = new ArrayList<>(commandsOfCurrentDialog);
        this.panels = new ArrayList<>(panels);
        this.indicators = new ArrayList<>(indicators);
        this.readouts = new ArrayList<>(readouts);
        this.readoutColumns = readoutColumns;
        this.gaugeNames = new ArrayList<>(gaugeNames);
        this.topicHelp = topicHelp;
        this.layoutHint = layoutHint;
        this.orderedEntries = new ArrayList<>(orderedEntries);
    }

    public List<Command> getCommandsOfCurrentDialog() {
        return commandsOfCurrentDialog;
    }

    public List<Field> getFields() {
        return fields;
    }

    public List<PanelModel> getPanels() {
        return panels;
    }

    public List<IndicatorModel> getIndicators() {
        return indicators;
    }

    public List<ReadoutModel> getReadouts() {
        return readouts;
    }

    public int getReadoutColumns() {
        return readoutColumns;
    }

    public List<String> getGaugeNames() {
        return gaugeNames;
    }

    public String getKey() {
        return key;
    }

    public String getUiName() {
        return uiName;
    }

    public String getTopicHelp() {
        return topicHelp;
    }

    public String getLayoutHint() {
        return layoutHint;
    }

    public List<DialogEntry> getOrderedEntries() {
        return orderedEntries;
    }

    @Override
    public String toString() {
        return "DialogModel{" +
                "key='" + key + '\'' +
                ", uiName='" + uiName + '\'' +
                ", fields=" + fields.size() +
                ", panels=" + panels.size() +
                ", topicHelp='" + topicHelp + '\'' +
                ", layoutHint='" + layoutHint + '\'' +
                '}';
    }

    public static class Field {
        private final String key;
        private final String uiName;
        private final String enableExpression;
        private final String visibleExpression;

        public Field(String key, String uiName) {
            this(key, uiName, null, null);
        }

        public Field(String key, String uiName, String enableExpression, String visibleExpression) {
            this.key = key;
            this.uiName = uiName;
            this.enableExpression = enableExpression;
            this.visibleExpression = visibleExpression;
        }

        public String getKey() {
            return key;
        }

        public String getUiName() {
            return uiName;
        }

        public String getEnableExpression() {
            return enableExpression;
        }

        public String getVisibleExpression() {
            return visibleExpression;
        }

        @Override
        public String toString() {
            return "Field{" +
                    "key='" + key + '\'' +
                    ", uiName='" + uiName + '\'' +
                    ", enableExpression='" + enableExpression + '\'' +
                    ", visibleExpression='" + visibleExpression + '\'' +
                    '}';
        }
    }

    public static class Command {
        private final String uiName;
        private final String command;

        public Command(String uiName, String command) {
            this.uiName = uiName;
            this.command = command;
        }

        public String getUiName() {
            return uiName;
        }

        public String getCommand() {
            return command;
        }
    }
}
