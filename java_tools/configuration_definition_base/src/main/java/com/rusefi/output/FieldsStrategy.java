package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.util.List;

/**
 * TODO java code: two or three classes with potential code duplication #7259
 *
 * @see PerFieldWithStructuresIterator#strategy
 */
public abstract class FieldsStrategy {
    static final FieldsStrategy VOID = new FieldsStrategy() {
        @Override
        int writeOneField(FieldIterator iterator, String prefix, int tsPosition) {
            return iterator.adjustSize(tsPosition);
        }
    };

    public int run(ReaderState state, ConfigStructure structure, int structureStartingTsPosition) {
        if (state.isStackEmpty()) {
            return writeFields(structure.getTsFields(), "", structureStartingTsPosition);
        }
        return structureStartingTsPosition;
    }

    protected int writeFields(List<ConfigField> fields, String prefix, int tsPosition) {
        FieldIterator iterator = new FieldIterator(fields);
        return loopIterator(fields, prefix, tsPosition, iterator);
    }

    protected int loopIterator(List<ConfigField> fields, String prefix, int tsPosition, FieldIterator iterator) {
        for (int i = 0; i < fields.size(); i++) {
            iterator.start(i);
            int currentPosition = tsPosition;
            tsPosition = writeOneField(iterator, prefix, tsPosition);

            iterator.end(currentPosition);
        }
        return tsPosition;
    }

    abstract int writeOneField(FieldIterator iterator, String prefix, int tsPosition);

}
