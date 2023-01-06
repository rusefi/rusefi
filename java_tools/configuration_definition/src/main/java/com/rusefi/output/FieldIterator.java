package com.rusefi.output;

import com.rusefi.BitState;
import com.rusefi.ConfigFieldImpl;

import java.util.List;

/**
 * custom iterator with references to previous and next elements
 * @see PerFieldWithStructuresIterator is there a duplication?
 */
public class FieldIterator {
    private final List<ConfigFieldImpl> fields;
    BitState bitState = new BitState();
    private ConfigFieldImpl prev = ConfigFieldImpl.VOID;
    ConfigFieldImpl next;
    ConfigFieldImpl cf;

    public FieldIterator(List<ConfigFieldImpl> fields) {
        this.fields = fields;
    }

    /**
     * return previous field which is not a directive
     */
    public ConfigFieldImpl getPrev() {
        return prev;
    }

    public void start(int index) {
        int nextIndex = index + 1;
        while (nextIndex < fields.size() && fields.get(nextIndex).isDirective())
            nextIndex++;

        next = nextIndex >= fields.size() ? ConfigFieldImpl.VOID : fields.get(nextIndex);
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
