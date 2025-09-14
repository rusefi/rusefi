package com.opensr5.ini;

import java.util.ArrayList;
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

    public DialogModel(String key, String uiName, List<Field> fields, List<Command> commandsOfCurrentDialog) {
        this.key = key;
        this.uiName = uiName;
        this.fields = new ArrayList<>(fields);
        this.commandsOfCurrentDialog = new ArrayList<>(commandsOfCurrentDialog);
    }

    public List<Field> getFields() {
        return fields;
    }

    @Override
    public String toString() {
        return "DialogModel{" +
                "key='" + key + '\'' +
                ", uiName='" + uiName + '\'' +
                ", fields=" + fields.size() +
                '}';
    }

    public static class Field {
        private final String key;
        private final String uiName;

        public Field(String key, String uiName) {
            this.key = key;
            this.uiName = uiName;
        }

        public String getKey() {
            return key;
        }

        public String getUiName() {
            return uiName;
        }

        @Override
        public String toString() {
            return "Field{" +
                    "key='" + key + '\'' +
                    ", uiName='" + uiName + '\'' +
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
