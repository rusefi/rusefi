package com.rusefi.output;

import com.rusefi.BitState;
import com.rusefi.ConfigField;

import java.util.List;

/**
 * custom iterator with references to previous and next elements
 */
public class FieldIterator {
    private final List<ConfigField> tsFields;
    BitState bitState = new BitState();
    ConfigField prev = ConfigField.VOID;
    ConfigField next;
    ConfigField cf;

    public FieldIterator(List<ConfigField> tsFields) {
        this.tsFields = tsFields;
    }

    public void start(int index) {
        next = index == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(index + 1);
        cf = tsFields.get(index);
    }

    public void end() {
        bitState.incrementBitIndex(cf, next);
    }
}
