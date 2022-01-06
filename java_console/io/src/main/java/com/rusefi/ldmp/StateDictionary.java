package com.rusefi.ldmp;

import com.rusefi.config.Field;
import com.rusefi.config.generated.*;
import com.rusefi.enums.live_data_e;
import org.jetbrains.annotations.NotNull;

import java.util.*;

/**
 * this weird class helps us match generated {@link live_data_e} with manually maintained {@link LiveDataView}
 */
public enum StateDictionary {
    INSTANCE;

    private final Map<live_data_e, Field[]> map = new HashMap<>();

    StateDictionary() {
        //register(live_data_e.LDS_ETB_PID, PidState.VALUES); // 7
        //register(live_data_e.LDS_IDLE_PID, PidState.VALUES);
        //register(live_data_e.LDS_ALTERNATOR_PID,
        // LDS_CJ125_PID
        //register(live_data_e.LDS_IDLE, IdleState.VALUES);
        // todo: code generate this!
        register(live_data_e.LDS_engine_state, EngineState.VALUES); // 3
        register(live_data_e.LDS_trigger_central, TriggerCentral.VALUES);
        register(live_data_e.LDS_trigger_state, TriggerState.VALUES); // 11
        register(live_data_e.LDS_ac_control, AcControl.VALUES); // 12
        register(live_data_e.LDS_fan_control, FuelPump.VALUES);
        register(live_data_e.LDS_injector_model, InjectorModel.VALUES);
        register(live_data_e.LDS_idle_state, IdleState.VALUES);
        register(live_data_e.LDS_fuel_pump, FuelPump.VALUES);
        register(live_data_e.LDS_wall_fuel_state, WallFuelState.VALUES);
        register(live_data_e.LDS_tps_accel_state, TpsAccelState.VALUES);
        register(live_data_e.LDS_main_relay, MainRelay.VALUES);
        register(live_data_e.LDS_boost_control, BoostControl.VALUES);
        register(live_data_e.LDS_launch_control_state, LaunchControl.VALUES);
        register(live_data_e.LDS_high_pressure_fuel_pump, HighPressureFuelPump.VALUES);
        if (map.size() != live_data_e.values().length) {
            Set<live_data_e> missing = new HashSet<>(Arrays.asList(live_data_e.values()));
            missing.removeAll(map.keySet());
            throw new IllegalStateException("Some live_data_e does not have values: " + missing);
        }
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
