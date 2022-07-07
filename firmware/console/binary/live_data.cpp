#include "pch.h"

#include "live_data.h"

#include "tunerstudio.h"
#include "wideband_state_generated.h"
#include "electronic_throttle_generated.h"
#include "knock_controller_generated.h"

template<>
const output_channels_s* getLiveDataAddr() {
	return &engine->outputChannels;
}

template<>
const knock_controller_s* getLiveDataAddr() {
	return &engine->module<KnockController>().unmock();
}

template<>
const high_pressure_fuel_pump_s* getLiveDataAddr() {
#if EFI_HPFP
	return &engine->module<HpfpController>().unmock();
#else
		return nullptr; // explicit null to confirm that this struct is handled
#endif // EFI_HPFP
}

template<>
const launch_control_state_s* getLiveDataAddr() {
#if EFI_LAUNCH_CONTROL
	return &engine->launchController;
#else
	return nullptr;
#endif
}

template<>
const injector_model_s* getLiveDataAddr() {
	return &engine->module<InjectorModel>().unmock();
}

template<>
const boost_control_s* getLiveDataAddr() {
#if EFI_BOOST_CONTROL
	return &engine->boostController;
#else
	return nullptr;
#endif
}

template<>
const ac_control_s* getLiveDataAddr() {
	return &engine->module<AcController>().unmock();
}

template<>
const fan_control_s* getLiveDataAddr(size_t idx) {
	switch (idx) {
		case 0: return &engine->fan1;
		case 1: return &engine->fan2;
		default: return nullptr;
	}
}

template<>
const fuel_pump_control_s* getLiveDataAddr() {
	return &engine->module<FuelPumpController>().unmock();
}

template<>
const main_relay_s* getLiveDataAddr() {
	return &engine->module<MainRelayController>().unmock();
}

template<>
const engine_state_s* getLiveDataAddr() {
	return &engine->engineState;
}

template<>
const tps_accel_state_s* getLiveDataAddr() {
	return &engine->tpsAccelEnrichment;
}

template<>
const trigger_central_s* getLiveDataAddr() {
	return &engine->triggerCentral;
}

template<>
const trigger_state_s* getLiveDataAddr(size_t idx) {
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
const wall_fuel_state_s* getLiveDataAddr() {
	return &engine->injectionEvents.elements[0].wallFuel;
}

template<>
const idle_state_s* getLiveDataAddr() {
	return &engine->module<IdleController>().unmock();
}

template<>
const ignition_state_s* getLiveDataAddr() {
	return &engine->ignitionState;
}

template<>
const electronic_throttle_s* getLiveDataAddr(size_t) {
	// return engine->etbControllers[0];
	return nullptr;
}

#if EFI_UNIT_TEST
FragmentList getLiveDataFragments() {
	return { nullptr, 0 };
}
#else

static const FragmentEntry fragments[] = {
// This header is generated - do not edit by hand!
#include "live_data_fragments.h"
};

FragmentList getLiveDataFragments() {
	return { fragments, efi::size(fragments) };
}

#endif
