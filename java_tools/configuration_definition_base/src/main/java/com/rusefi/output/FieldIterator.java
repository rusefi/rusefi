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

    public int loop(int tsPosition) {
        return FieldsStrategy.VOID.loopIterator(fields, "", tsPosition, this);
    }

    public void end(int currentPosition) {
        if (!cf.isDirective())
            prev = cf;
        bitState.incrementBitIndex(cf, next);
    }

    public int adjustSize(int tsPosition) {
        if (cf.isBit()) {
            if (next.isBit()) {
                // No increment while we are in the bit sequence.
                // But wait, if we cross 32 bits, we might need to increment?
                // Actually, let's keep tsPosition at the START of the bit sequence.
                return tsPosition;
            } else {
                // End of bit sequence. Return tsPosition + size of all U32s used.
                return tsPosition + bitState.getByteOffset() + 4;
            }
        }
        return tsPosition + cf.getSize(next);
    }
}
