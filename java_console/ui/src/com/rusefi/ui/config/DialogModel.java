package com.rusefi.ui.config;

import java.util.ArrayList;
import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 1/17/2016.
 */
public class DialogModel {
    private final String name;
    private final List<String> fields;

    public DialogModel(String name, List<String> fields) {
        this.name = name;
        this.fields = new ArrayList<>(fields);
    }
}
