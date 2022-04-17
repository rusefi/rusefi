package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;
import java.util.List;

public abstract class FieldsStrategy {
    public void run(ReaderState state, ConfigStructure structure, int sensorTsPosition) throws IOException {
        if (state.stack.isEmpty()) {
            writeFields(structure.tsFields, "", sensorTsPosition);
        }
    }

    protected int writeFields(List<ConfigField> tsFields, String prefix, int tsPosition) throws IOException {
        FieldIterator iterator = new FieldIterator(tsFields);
        for (int i = 0; i < tsFields.size(); i++) {
            iterator.start(i);
            tsPosition = writeOneField(iterator, iterator.cf, prefix, tsPosition, iterator.next,
                    iterator.bitState.get(),
                    iterator.getPrev());

            iterator.end();
        }
        return tsPosition;
    }

    abstract int writeOneField(FieldIterator iterator, ConfigField configField, String prefix, int tsPosition, ConfigField next, int bitIndex, ConfigField prev) throws IOException;

}
