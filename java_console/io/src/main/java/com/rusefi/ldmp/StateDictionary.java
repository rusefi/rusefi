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
    private final Map<live_data_e, String> fileNames = new HashMap<>();

    StateDictionary() {
        //register(live_data_e.LDS_ETB_PID, PidState.VALUES); // 7
        //register(live_data_e.LDS_IDLE_PID, PidState.VALUES);
        //register(live_data_e.LDS_ALTERNATOR_PID,
        // LDS_CJ125_PID
        //register(live_data_e.LDS_IDLE, IdleState.VALUES);
        // todo: code generate this!
        register(live_data_e.LDS_engine_state, EngineState.VALUES, "engine.cpp"); // 3
        register(live_data_e.LDS_trigger_central, TriggerCentral.VALUES, "trigger_central.cpp");
        register(live_data_e.LDS_trigger_state, TriggerState.VALUES, "trigger_decoder.cpp"); // 11
        register(live_data_e.LDS_ac_control, AcControl.VALUES, "ac_control.cpp"); // 12
        register(live_data_e.LDS_fan_control, FuelPump.VALUES, "fan_control.cpp");
        register(live_data_e.LDS_injector_model, InjectorModel.VALUES, "injector_model");
        register(live_data_e.LDS_idle_state, IdleState.VALUES, "idle_thread.cpp");
        register(live_data_e.LDS_fuel_pump, FuelPump.VALUES, "fuel_pump.cpp");
        register(live_data_e.LDS_wall_fuel_state, WallFuelState.VALUES, "wall_fuel.cpp");
        register(live_data_e.LDS_tps_accel_state, TpsAccelState.VALUES, "accel_enrichment.cpp");
        register(live_data_e.LDS_main_relay, MainRelay.VALUES, "main_relay.cpp");
        register(live_data_e.LDS_boost_control, BoostControl.VALUES, "boost_control.cpp");
        register(live_data_e.LDS_launch_control_state, LaunchControl.VALUES, "launch_control.cpp");
        register(live_data_e.LDS_high_pressure_fuel_pump, HighPressureFuelPump.VALUES, "high_pressure_fuel_pump.cpp");
        if (map.size() != live_data_e.values().length) {
            Set<live_data_e> missing = new HashSet<>(Arrays.asList(live_data_e.values()));
            missing.removeAll(map.keySet());
            throw new IllegalStateException("Some live_data_e does not have values: " + missing);
        }
    }

    private void register(live_data_e ldsIndex, Field[] values, String fileName) {
        map.put(ldsIndex, values);
        fileNames.put(ldsIndex, fileName);
    }

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
