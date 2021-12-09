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
    private ConfigField prev = ConfigField.VOID;
    ConfigField next;
    ConfigField cf;

    public FieldIterator(List<ConfigField> tsFields) {
        this.tsFields = tsFields;
    }

    public ConfigField getPrev() {
        return prev;
    }

    public void start(int index) {
        next = index == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(index + 1);
        cf = tsFields.get(index);
    }

    public void end() {
        if (!cf.isDirective())
            prev = cf;
        bitState.incrementBitIndex(cf, next);
    }
}
