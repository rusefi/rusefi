package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;

import java.util.List;

public class FieldIteratorWithOffset extends FieldIterator {
    public int currentOffset;

    public FieldIteratorWithOffset(List<ConfigField> tsFields) {
        super(tsFields);
    }
}
