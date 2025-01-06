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
    public int run(ReaderState state, ConfigStructure structure, int structureStartingTsPosition) {
        if (state.isStackEmpty()) {
            return writeFields(structure.getTsFields(), "", structureStartingTsPosition);
        }
        return structureStartingTsPosition;
    }

    protected int writeFields(List<ConfigField> tsFields, String prefix, int tsPosition) {
        FieldIterator iterator = new FieldIterator(tsFields);
        for (int i = 0; i < tsFields.size(); i++) {
            iterator.start(i);
            tsPosition = writeOneField(iterator, prefix, tsPosition);

            iterator.end();
        }
        return tsPosition;
    }

    abstract int writeOneField(FieldIterator iterator, String prefix, int tsPosition);

}
