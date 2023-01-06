package com.rusefi.output;

import com.rusefi.ConfigFieldImpl;

import java.util.List;

public class FieldIteratorWithOffset extends FieldIterator {
    public int currentOffset;

    public FieldIteratorWithOffset(List<ConfigFieldImpl> tsFields) {
        super(tsFields);
    }
}
