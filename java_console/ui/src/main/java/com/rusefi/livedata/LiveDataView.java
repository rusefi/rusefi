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
    AC_CONTROL(live_data_e.LDS_ac_control, AcControl.VALUES),
    FUEL_PUMP(live_data_e.LDS_fuel_pump, FuelPump.VALUES),
    MAIN_RELAY(live_data_e.LDS_main_relay, MainRelay.VALUES),
    TPS_ACCEL(live_data_e.LDS_tps_accel_state, TpsAccelState.VALUES),
    BOOST_CONTROL(live_data_e.LDS_boost_control, BoostControl.VALUES),
    LAUNCH_CONTROL(live_data_e.LDS_launch_control_state, LaunchControl.VALUES),
    HIGH_PRESSURE(live_data_e.LDS_high_pressure_fuel_pump, HighPressureFuelPump.VALUES),
    ;

    private final live_data_e liveDataE;
    private final Field[] values;
    private final String fileName;

    LiveDataView(live_data_e liveDataE, Field[] values) {
        this.liveDataE = liveDataE;
        this.values = values;
        this.fileName = StateDictionary.INSTANCE.getFileName(liveDataE);
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
