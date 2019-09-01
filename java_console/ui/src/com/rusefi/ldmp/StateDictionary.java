package com.rusefi.ldmp;

import com.rusefi.config.Field;
import com.rusefi.config.generated.EngineState;
import com.rusefi.config.generated.Fields;
import com.rusefi.config.generated.PidState;
import com.rusefi.config.generated.TriggerState;
import com.rusefi.ui.livedocs.LiveDataContext;
import org.jetbrains.annotations.NotNull;

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
        String indexFieldName = getContextIndexFieldName(state);
        LiveDataContext indexValue = getStateContext(indexFieldName);
        Field[] result = map.get(indexValue.getId());
        if (result == null) {
            throw new IllegalStateException("Nothing for " + indexFieldName + "/" + indexValue);
        }
        return result;
    }

    @NotNull
    public static String getContextIndexFieldName(String state) {
        return "LDS_" + state.toUpperCase() + "_STATE_INDEX";
    }

    public static LiveDataContext getStateContext(String contextIndexFieldName) {
        java.lang.reflect.Field field;
        try {
            field = Fields.class.getField(contextIndexFieldName);
        } catch (NoSuchFieldException e) {
            throw new IllegalStateException(e);
        }
        Objects.requireNonNull(field, "Field " + contextIndexFieldName);
        int indexValue;
        try {
            indexValue = (int) field.get(null);
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }
        return new LiveDataContext(indexValue);
    }

}
