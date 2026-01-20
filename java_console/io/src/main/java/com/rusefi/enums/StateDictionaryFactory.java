package com.rusefi.enums;
//was generated automatically by rusEFI tool config_definition_base-all.jar based on StateDictionaryGenerator integration/LiveData.yaml on Tue Jan 20 04:54:56 UTC 2026n
import com.rusefi.config.generated.*;
import com.rusefi.ldmp.StateDictionary;

public class StateDictionaryFactory {
    public static void initialize(StateDictionary stateDictionary) {
        stateDictionary.register(live_data_e.LDS_output_channels, "status_loop");
        stateDictionary.register(live_data_e.LDS_fuel_computer, "fuel_computer");
        stateDictionary.register(live_data_e.LDS_ignition_state, "ignition_state");
        stateDictionary.register(live_data_e.LDS_knock_controller, "knock_controller");
        stateDictionary.register(live_data_e.LDS_prime_injection, "prime_injection");
        stateDictionary.register(live_data_e.LDS_tcu_controller, "tcu_controller");
        stateDictionary.register(live_data_e.LDS_throttle_model, "throttle_model");
        stateDictionary.register(live_data_e.LDS_high_pressure_fuel_pump, "high_pressure_fuel_pump");
        stateDictionary.register(live_data_e.LDS_injector_model, "injector_model");
        stateDictionary.register(live_data_e.LDS_launch_control_state, "launch_control");
        stateDictionary.register(live_data_e.LDS_shift_torque_reduction_state, "shift_torque_reduction_controller");
        stateDictionary.register(live_data_e.LDS_antilag_system_state, "antilag_system");
        stateDictionary.register(live_data_e.LDS_boost_control, "boost_control");
        stateDictionary.register(live_data_e.LDS_ac_control, "ac_control");
        stateDictionary.register(live_data_e.LDS_nitrous_control_state, "nitrous_control_state");
        stateDictionary.register(live_data_e.LDS_fan_control0, "fan_control");
        stateDictionary.register(live_data_e.LDS_fan_control1, "fan_control");
        stateDictionary.register(live_data_e.LDS_fuel_pump_control, "fuel_pump");
        stateDictionary.register(live_data_e.LDS_main_relay, "main_relay");
        stateDictionary.register(live_data_e.LDS_engine_state, "engine");
        stateDictionary.register(live_data_e.LDS_tps_accel_state, "accel_enrichment");
        stateDictionary.register(live_data_e.LDS_trigger_central, "trigger_central");
        stateDictionary.register(live_data_e.LDS_trigger_state0, "trigger_decoder");
        stateDictionary.register(live_data_e.LDS_trigger_state1, "trigger_decoder");
        stateDictionary.register(live_data_e.LDS_trigger_state2, "trigger_decoder");
        stateDictionary.register(live_data_e.LDS_trigger_state3, "trigger_decoder");
        stateDictionary.register(live_data_e.LDS_trigger_state4, "trigger_decoder");
        stateDictionary.register(live_data_e.LDS_trigger_state_primary, "trigger_decoder");
        stateDictionary.register(live_data_e.LDS_wall_fuel_state, "wall_fuel");
        stateDictionary.register(live_data_e.LDS_closed_loop_idle, "closed_loop_idle");
        stateDictionary.register(live_data_e.LDS_live_data_example, "live_data_example");
        stateDictionary.register(live_data_e.LDS_idle_state, "idle_thread");
        stateDictionary.register(live_data_e.LDS_electronic_throttle0, "electronic_throttle");
        stateDictionary.register(live_data_e.LDS_electronic_throttle1, "electronic_throttle");
        stateDictionary.register(live_data_e.LDS_wideband_state0, "AemXSeriesLambda");
        stateDictionary.register(live_data_e.LDS_wideband_state1, "AemXSeriesLambda");
        stateDictionary.register(live_data_e.LDS_dc_motors, "dc_motors");
        stateDictionary.register(live_data_e.LDS_sent_state, "sent");
        stateDictionary.register(live_data_e.LDS_vvt, "vvt");
        stateDictionary.register(live_data_e.LDS_lambda_monitor, "lambda_monitor");
        stateDictionary.register(live_data_e.LDS_mc33810_state, "mc33810");
        stateDictionary.register(live_data_e.LDS_long_term_fuel_trim_state, "long_term_fuel_trim");
        stateDictionary.register(live_data_e.LDS_short_term_fuel_trim_state, "short_term_fuel_trim");
        stateDictionary.register(live_data_e.LDS_vvl_controller_state, "vvl_controller_state");
        stateDictionary.register(live_data_e.LDS_live_data_rotational_idle, "live_data_rotational_idle");
    }
}