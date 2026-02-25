package com.rusefi.output;

import com.rusefi.BitState;
import com.rusefi.ConfigField;

import java.util.List;

/**
 * TODO java code: two or three classes with potential code duplication #7259
 * <p>
 * custom iterator with references to previous and next elements
 *
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

    public ConfigField getPrev() {
        return prev;
    }

    public void start(int index) {
        int nextIndex = index + 1;
        next = nextIndex >= fields.size() ? ConfigField.VOID : fields.get(nextIndex);
        cf = fields.get(index);
    }

    public int loop(int tsPosition) {
        return FieldsStrategy.VOID.loopIterator(fields, "", tsPosition, this);
    }

    public void end(int currentPosition) {
        prev = cf;
        bitState.incrementBitIndex(cf, next);
    }

    public int adjustSize(int tsPosition) {
        return tsPosition + cf.getSize(next);
    }
}
