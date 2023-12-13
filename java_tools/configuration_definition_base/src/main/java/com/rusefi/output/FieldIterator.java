package com.rusefi.output;

import com.rusefi.BitState;
import com.rusefi.ConfigField;

import java.util.List;

/**
 * custom iterator with references to previous and next elements
 * @see PerFieldWithStructuresIterator is there a duplication?
 */
public class FieldIterator {
    private final List<ConfigField> fields;
    final BitState bitState = new BitState();
    private ConfigField prev = ConfigField.VOID;
    ConfigField next;
    ConfigField cf;

    public FieldIterator(List<ConfigField> fields) {
        this.fields = fields;
    }

    /**
     * return previous field which is not a directive
     */
    public ConfigField getPrev() {
        return prev;
    }

    public void start(int index) {
        int nextIndex = index + 1;
        while (nextIndex < fields.size() && fields.get(nextIndex).isDirective())
            nextIndex++;

        next = nextIndex >= fields.size() ? ConfigField.VOID : fields.get(nextIndex);
        cf = fields.get(index);
    }

    public void loop() {
        for (int i = 0; i < fields.size(); i++) {
            start(i);
            end();
        }
    }

    public void end() {
        if (!cf.isDirective())
            prev = cf;
        bitState.incrementBitIndex(cf, next);
    }
}
