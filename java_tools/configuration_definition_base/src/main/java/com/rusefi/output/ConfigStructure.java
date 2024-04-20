package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.util.List;
import java.util.Map;

public interface ConfigStructure {
    String UNUSED_ANYTHING_PREFIX = "unused";

    Map<String, ConfigField> getCurrentInstance();

    ConfigStructure getParent();

    String getName();

    void addAlignmentFill(ReaderState state, int alignment);

    ConfigField getTsFieldByName(String name);

    void addBitPadding(ReaderState readerState);

    int getTotalSize();

    List<ConfigField> getTsFields();

    List<ConfigField> getcFields();

    boolean isWithPrefix();

    String getComment();
}
