package com.rusefi.livedata;

import com.rusefi.config.Field;
import com.rusefi.config.generated.*;
import com.rusefi.enums.live_data_e;
import com.rusefi.ldmp.StateDictionary;

/**
 * @see StateDictionary
 */
public enum LiveDataView {
    // todo: code generate this part of the enum with some BEFORE/AFTER tag?
    AC_CONTROL(live_data_e.LDS_ac_control, AcControl.VALUES, "ac_control.cpp"),
    FUEL_PUMP(live_data_e.LDS_fuel_pump, FuelPump.VALUES, "fuel_pump.cpp"),
    MAIN_RELAY(live_data_e.LDS_main_relay, MainRelay.VALUES, "main_relay.cpp"),
    TPS_ACCEL(live_data_e.LDS_tps_accel_state, TpsAccelState.VALUES, "accel_enrichment.cpp"),
    BOOST_CONTROL(live_data_e.LDS_boost_control, BoostControl.VALUES, "boost_control.cpp"),
    LAUNCH_CONTROL(live_data_e.LDS_launch_control_state, LaunchControl.VALUES, "launch_control.cpp"),
    HIGH_PRESSURE(live_data_e.LDS_high_pressure_fuel_pump, HighPressureFuelPump.VALUES, "high_pressure_fuel_pump.cpp"),
    ;

    private final live_data_e liveDataE;
    private final Field[] values;
    private final String fileName;

    LiveDataView(live_data_e liveDataE, Field[] values, String fileName) {
        this.liveDataE = liveDataE;
        this.values = values;
        this.fileName = fileName;
    }

    public live_data_e getLiveDataE() {
        return liveDataE;
    }

    public Field[] getValues() {
        return values;
    }

    public String getFileName() {
        return fileName;
    }
}
