package com.rusefi.ldmp;

import com.rusefi.config.Field;
import com.rusefi.config.generated.*;
import com.rusefi.ui.livedocs.LiveDataContext;
import org.jetbrains.annotations.NotNull;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public enum StateDictionary {
    INSTANCE;

    public static final int NONE = -1;
    private Map<Integer, Field[]> map = new HashMap<>();

    StateDictionary() {
        register(Fields.LDS_SPEED_DENSITY_STATE_INDEX, EngineState.VALUES); // 2
        register(Fields.LDS_ENGINE_STATE_INDEX, EngineState.VALUES); // 3
        register(Fields.LDS_FUEL_TRIM_STATE_INDEX, EngineState.VALUES);
        register(Fields.LDS_TPS_TPS_ENRICHMENT_STATE_INDEX, EngineState.VALUES); // 5
        register(Fields.LDS_TRIGGER_CENTRAL_STATE_INDEX, TriggerCentral.VALUES);
        register(Fields.LDS_ETB_PID_STATE_INDEX, PidState.VALUES); // 7
        register(Fields.LDS_IDLE_PID_STATE_INDEX, PidState.VALUES);
        // LDS_ALTERNATOR_PID_STATE_INDEX
        // LDS_CJ125_PID_STATE_INDEX
        register(Fields.LDS_TRIGGER_STATE_STATE_INDEX, TriggerState.VALUES); // 11
    }

    private void register(int ldsIndex, Field[] values) {
        map.put(ldsIndex, values);
    }

    public Field[] getValue(String state) {
        String indexFieldName = getContextIndexFieldName(state);
        LiveDataContext indexValue = getStateContext(indexFieldName);
        return getFields(indexFieldName, indexValue);
    }

    @NotNull
    public Field[] getFields(String indexFieldName, LiveDataContext indexValue) {
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
