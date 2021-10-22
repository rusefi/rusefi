package com.rusefi.ldmp;

import com.rusefi.config.Field;
import com.rusefi.config.generated.*;
import com.rusefi.enums.live_data_e;
import org.jetbrains.annotations.NotNull;

import java.util.HashMap;
import java.util.Map;

public enum StateDictionary {
    INSTANCE;

    private final Map<live_data_e, Field[]> map = new HashMap<>();

    StateDictionary() {
        register(live_data_e.LDS_SPEED_DENSITY, EngineState.VALUES); // 2
        register(live_data_e.LDS_ENGINE, EngineState.VALUES); // 3
        register(live_data_e.LDS_FUEL_TRIM, EngineState.VALUES);
        register(live_data_e.LDS_TPS_TPS_ENRICHMENT, EngineState.VALUES); // 5
        register(live_data_e.LDS_TRIGGER_CENTRAL, TriggerCentral.VALUES);
        register(live_data_e.LDS_ETB_PID, PidState.VALUES); // 7
        register(live_data_e.LDS_IDLE_PID, PidState.VALUES);
        // LDS_ALTERNATOR_PID
        // LDS_CJ125_PID
        register(live_data_e.LDS_TRIGGER_STATE, TriggerState.VALUES); // 11
        register(live_data_e.LDS_AC_CONTROL, AcControl.VALUES); // 12
    }

    private void register(live_data_e ldsIndex, Field[] values) {
        map.put(ldsIndex, values);
    }

    @NotNull
    public Field[] getFields(live_data_e indexFieldName) {
        Field[] result = map.get(indexFieldName);
        if (result == null) {
            throw new IllegalStateException("Nothing for " + indexFieldName);
        }
        return result;
    }
}
