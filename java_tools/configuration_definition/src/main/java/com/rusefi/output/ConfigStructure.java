package com.rusefi.output;

import com.rusefi.ConfigFieldImpl;

import java.util.List;

public interface ConfigStructure {
    String getName();

    int getTotalSize();

    List<ConfigFieldImpl> getTsFields();

    List<ConfigFieldImpl> getcFields();

    boolean isWithPrefix();

    String getComment();
}
