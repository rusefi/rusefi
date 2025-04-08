package com.rusefi.ldmp;

import com.rusefi.config.Field;
import com.rusefi.enums.StateDictionaryFactory;
import com.rusefi.enums.live_data_e;
import org.jetbrains.annotations.NotNull;

import java.util.*;

/**
 * this weird class helps us match generated {@link live_data_e} with manually maintained {@link LiveDataView}
 */
public enum StateDictionary {
    INSTANCE;

    private final Map<live_data_e, Field[]> map = new HashMap<>();
    private final Map<live_data_e, String> fileNames = new HashMap<>();

    StateDictionary() {
        StateDictionaryFactory.initialize(this);
    }

    static int getSize(Field[] values) {
        Field last = values[values.length - 1];
        return last.getOffset() + last.getType().getStorageSize();
    }

    public int getOffset(live_data_e live_data_e) {
        int result = 0;
        for (live_data_e index : live_data_e.values()) {
            if (index.ordinal() < live_data_e.ordinal())
                result += getSize(getFields(index));
        }
        return result;
    }

    public void register(live_data_e ldsIndex, String fileName) {
        //map.put(ldsIndex, values);
        fileNames.put(ldsIndex, fileName);
    }

    @NotNull
    public String getFileName(live_data_e ldsIndex) {
        String result = fileNames.get(ldsIndex);
        if (result == null) {
            throw new IllegalStateException("Nothing for " + ldsIndex);
        }
        return result;
    }

    @NotNull
    public Field[] getFields(live_data_e ldsIndex) {
        Field[] result = map.get(ldsIndex);
        if (result == null) {
            throw new IllegalStateException("Nothing for " + ldsIndex);
        }
        return result;
    }
}
