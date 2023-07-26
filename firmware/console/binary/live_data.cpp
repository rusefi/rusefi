#include "pch.h"

#include "live_data.h"

#include "tunerstudio.h"
#include "wideband_state_generated.h"
#include "electronic_throttle_impl.h"
#include "knock_controller_generated.h"
#include "fuel_computer.h"
#include "antilag_system_state_generated.h"
#include "vvt_generated.h"

template<>
const output_channels_s* getLiveData(size_t) {
	return &engine->outputChannels;
}

template<>
const knock_controller_s* getLiveData(size_t) {
	return &engine->module<KnockController>().unmock();
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
const antilag_system_state_s* getLiveData(size_t) {
#if EFI_ANTILAG_SYSTEM
	return &engine->antilagController;
#else
	return nullptr;
#endif
}

template<>
const injector_model_s* getLiveData(size_t) {
	return &engine->module<InjectorModel>().unmock();
}

template<>
const boost_control_s* getLiveData(size_t) {
#if EFI_BOOST_CONTROL
	return &engine->boostController;
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
	return &engine->fuelComputer;
}

template<>
const fan_control_s* getLiveData(size_t idx) {
	switch (idx) {
		case 0: return &engine->module<FanControl1>().unmock();
		case 1: return &engine->module<FanControl2>().unmock();
		default: return nullptr;
	}
}

template<>
const fuel_pump_control_s* getLiveData(size_t) {
	return &engine->module<FuelPumpController>().unmock();
}

template<>
const main_relay_s* getLiveData(size_t) {
	return &engine->module<MainRelayController>().unmock();
}

template<>
const engine_state_s* getLiveData(size_t) {
	return &engine->engineState;
}

template<>
const tps_accel_state_s* getLiveData(size_t) {
	return &engine->tpsAccelEnrichment;
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
	return nullptr;
#endif
}

template<>
const vvt_s* getLiveData(size_t idx) {
#if EFI_AUX_PID
	switch (idx) {
		case 0: return &engine->module<VvtController1>().unmock();
		case 1: return &engine->module<VvtController2>().unmock();
		case 2: return &engine->module<VvtController3>().unmock();
		case 3: return &engine->module<VvtController4>().unmock();
		default: return nullptr;
	}
#else
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
	return &engine->injectionEvents.elements[0].wallFuel;
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
	return &engine->sent_state;
}

template<>
const throttle_model_s* getLiveData(size_t) {
	return &engine->module<ThrottleModel>().unmock();
}

template<>
const lambda_monitor_s* getLiveData(size_t) {
	return &engine->lambdaMonitor;
}

static const FragmentEntry fragments[] = {
// This header is generated - do not edit by hand!
#include "live_data_fragments.h"
};

FragmentList getLiveDataFragments() {
	return { fragments, efi::size(fragments) };
}
