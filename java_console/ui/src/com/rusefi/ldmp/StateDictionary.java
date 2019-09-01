package com.rusefi.ldmp;

import com.rusefi.config.Field;
import com.rusefi.config.generated.EngineState;
import com.rusefi.config.generated.Fields;
import com.rusefi.config.generated.PidState;
import com.rusefi.config.generated.TriggerState;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public enum StateDictionary {
    INSTANCE;

    private Map<Integer, Field[]> map = new HashMap<>();

    StateDictionary() {
        register(Fields.LDS_ETB_PID_STATE_INDEX, PidState.VALUES);
        register(Fields.LDS_TRIGGER_STATE_INDEX, TriggerState.VALUES);
        register(Fields.LDS_ENGINE_STATE_INDEX, EngineState.VALUES);
    }

    private void register(int ldsIndex, Field[] values) {
        map.put(ldsIndex, values);
    }

    public Field[] getValue(String state) {
        String indexFieldName = "LDS_" + state.toUpperCase() + "_STATE_INDEX";
        java.lang.reflect.Field field = null;
        try {
            field = Fields.class.getField(indexFieldName);
        } catch (NoSuchFieldException e) {
            throw new IllegalStateException(e);
        }
        Objects.requireNonNull(field, "Field " + indexFieldName);
        int indexValue;
        try {
            indexValue = (int) field.get(null);
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }
        Field[] result = map.get(indexValue);
        if (result == null) {
            throw new IllegalStateException("Nothing for " + indexFieldName + "/" + indexValue);
        }
        return result;
    }

}
