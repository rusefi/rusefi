#include "pch.h"

#include "live_data.h"

#include "tunerstudio.h"
#include "wideband_state_generated.h"
#include "electronic_throttle_impl.h"
#include "knock_controller_generated.h"
#include "tcu_controller_generated.h"
#include "fuel_computer.h"
#include "antilag_system_state_generated.h"
#include "closed_loop_idle_generated.h"
#include "vvt_generated.h"
#include "mc33810_state_generated.h"
#include <livedata_board_extra.h>

template<>
const output_channels_s* getLiveData(size_t) {
	return &engine->outputChannels;
}

template<>
const knock_controller_s* getLiveData(size_t) {
	return &engine->module<KnockController>().unmock();
}

template<>
const tcu_controller_s* getLiveData(size_t) {
#if EFI_TCU
  GearControllerBase *gearController = engine->gearController;
	return gearController == nullptr ? nullptr : gearController->transmissionController;
#else
		return nullptr;
#endif // EFI_TCU
}

template<>
const high_pressure_fuel_pump_s* getLiveData(size_t) {
#if EFI_HPFP
	return &engine->module<HpfpController>().unmock();
#else
		return nullptr; // explicit null to confirm that this struct is handled
#endif // EFI_HPFP
}

template<>
const launch_control_state_s* getLiveData(size_t) {
#if EFI_LAUNCH_CONTROL
	return &engine->launchController;
#else
	return nullptr;
#endif
}

template<>
const shift_torque_reduction_state_s* getLiveData(size_t) {
#if EFI_LAUNCH_CONTROL
	return &engine->shiftTorqueReductionController;
#else
	return nullptr;
#endif
}

template<>
const antilag_system_state_s* getLiveData(size_t) {
#if EFI_ANTILAG_SYSTEM
	return &engine->antilagController;
#else
	return nullptr;
#endif
}

template<>
const injector_model_s* getLiveData(size_t) {
	return &engine->module<InjectorModelPrimary>().unmock();
}

template<>
const boost_control_s* getLiveData(size_t) {
#if EFI_BOOST_CONTROL
	return &engine->module<BoostController>().unmock();
#else
	return nullptr;
#endif
}

template<>
const ac_control_s* getLiveData(size_t) {
	return &engine->module<AcController>().unmock();
}

template<>
const fuel_computer_s* getLiveData(size_t) {
#if EFI_ENGINE_CONTROL
	return &engine->fuelComputer;
#else
	return nullptr;
#endif
}

#ifdef MODULE_FAN_CONTROL
template<>
const fan_control_s* getLiveData(size_t idx) {
	switch (idx) {
		case 0: return &engine->module<FanControl1>().unmock();
		case 1: return &engine->module<FanControl2>().unmock();
		default: return nullptr;
	}
}
#endif

#ifdef MODULE_FUEL_PUMP
template<>
const fuel_pump_control_s* getLiveData(size_t) {
	return &engine->module<FuelPumpController>().unmock();
}
#endif

template<>
const main_relay_s* getLiveData(size_t) {
	return &engine->module<MainRelayController>().unmock();
}

template<>
const engine_state_s* getLiveData(size_t) {
	return &engine->engineState;
}

template<>
const prime_injection_s* getLiveData(size_t) {
	return &engine->module<PrimeController>().unmock();
}

template<>
const tps_accel_state_s* getLiveData(size_t) {
	return &engine->module<TpsAccelEnrichment>().unmock();
}

template<>
const nitrous_control_state_s* getLiveData(size_t) {
#if EFI_LAUNCH_CONTROL
    return &engine->module<NitrousController>().unmock();
#else
    return nullptr;
#endif // EFI_LAUNCH_CONTROL
}

template<>
const dc_motors_s* getLiveData(size_t) {
	return &engine->dc_motors;
}

template<>
const trigger_central_s* getLiveData(size_t) {
#if EFI_SHAFT_POSITION_INPUT
	return &engine->triggerCentral;
#else
	return nullptr;
#endif
}

template<>
const trigger_state_s* getLiveData(size_t idx) {
#if EFI_SHAFT_POSITION_INPUT
	switch (idx) {
		case 0: return &engine->triggerCentral.triggerState;
		case 1: return &engine->triggerCentral.vvtState[0][0];
		case 2: return &engine->triggerCentral.vvtState[0][1];
		case 3: return &engine->triggerCentral.vvtState[1][0];
		case 4: return &engine->triggerCentral.vvtState[1][1];
		default: return nullptr;
	}
#else
	UNUSED(idx);
	return nullptr;
#endif
}

template<>
const vvt_s* getLiveData(size_t idx) {
#if EFI_VVT_PID
	switch (idx) {
		case 0: return &engine->module<VvtController1>().unmock();
		case 1: return &engine->module<VvtController2>().unmock();
		case 2: return &engine->module<VvtController3>().unmock();
		case 3: return &engine->module<VvtController4>().unmock();
		default: return nullptr;
	}
#else
	UNUSED(idx);
	return nullptr;
#endif
}

template<>
const trigger_state_primary_s* getLiveData(size_t) {
#if EFI_SHAFT_POSITION_INPUT
	return &engine->triggerCentral.triggerState;
#else
	return nullptr;
#endif
}

template<>
const wall_fuel_state_s* getLiveData(size_t) {
#if EFI_ENGINE_CONTROL
	return &engine->injectionEvents.elements[0].getWallFuel();
#else
	return nullptr;
#endif
}

template<>
const idle_state_s* getLiveData(size_t) {
#if EFI_IDLE_CONTROL
	return &engine->module<IdleController>().unmock();
#else
	return nullptr;
#endif
}

template<>
const ignition_state_s* getLiveData(size_t) {
	return &engine->ignitionState;
}

template<>
const sent_state_s* getLiveData(size_t) {
#if EFI_SENT_SUPPORT
	return &engine->sent_state;
#else
	return nullptr;
#endif
}

template<>
const closed_loop_idle_s* getLiveData(size_t) {
#if EFI_IDLE_CONTROL
	return &engine->m_ltit;
#else
	return nullptr;
#endif
}

template<>
const throttle_model_s* getLiveData(size_t) {
#if EFI_IDLE_CONTROL
	return &engine->module<ThrottleModel>().unmock();
#else
	return nullptr;
#endif
}

template<>
const lambda_monitor_s* getLiveData(size_t) {
#if EFI_SHAFT_POSITION_INPUT
	return &engine->lambdaMonitor;
#else
	return nullptr;
#endif
}

#ifndef BOARD_MC33810_COUNT
	#define BOARD_MC33810_COUNT 0
#endif

template<>
const mc33810_state_s* getLiveData(size_t idx) {
#if (BOARD_MC33810_COUNT > 0)
	return mc33810getLiveData(idx);
#else
	UNUSED(idx);
	return nullptr;
#endif
}

static const FragmentEntry fragments[] = {
// This header is generated - do not edit by hand!
#include "live_data_fragments.h"
};

FragmentList getLiveDataFragments() {
	return { fragments, efi::size(fragments) };
}

template<>
const long_term_fuel_trim_state_s* getLiveData(size_t) {
#if EFI_LTFT_CONTROL
	engine->module<LongTermFuelTrim>()->onLiveDataRead();
	return &engine->module<LongTermFuelTrim>().unmock();
#else
	return nullptr;
#endif
}

template<>
const short_term_fuel_trim_state_s* getLiveData(size_t) {
#if EFI_LTFT_CONTROL
	return &engine->module<ShortTermFuelTrim>().unmock();
#else
	return nullptr;
#endif
}

template<>
const live_data_example_s* getLiveData(size_t) {
#if EFI_LTFT_CONTROL
	return &engine->module<ExampleModule>().unmock();
#else
	return nullptr;
#endif
}

template<>
const vvl_controller_state_s* getLiveData(size_t) {
#if MODULE_VVL_CONTROLLER
	return &engine->module<VvlController>().unmock();
#else
	return nullptr;
#endif
}

template<>
const live_data_rotational_idle_s* getLiveData(size_t) {
#if ROTATIONAL_IDLE_CONTROLLER
	return &engine->rotationalIdleController;
#else
	return nullptr;
#endif
}