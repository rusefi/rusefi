package com.rusefi.output;

import com.rusefi.ConfigField;

import java.util.List;

public interface ConfigStructure {
    String getName();

    int getTotalSize();

    List<ConfigField> getTsFields();

    List<ConfigField> getcFields();

    boolean isWithPrefix();

    String getComment();
}
