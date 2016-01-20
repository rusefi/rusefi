package com.rusefi.ui.config;

import java.util.ArrayList;
import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 1/17/2016.
 */
public class DialogModel {
    private final String key;
    private final String uiName;
    private final List<String> fields;

    public DialogModel(String key, String uiName, List<String> fields) {
        this.key = key;
        this.uiName = uiName;
        this.fields = new ArrayList<>(fields);
    }

    public List<String> getFields() {
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
}
