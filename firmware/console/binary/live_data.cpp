#include "pch.h"

#include "tunerstudio.h"

template <typename TStruct>
const TStruct* getLiveDataAddr();

template<>
const output_channels_s* getLiveDataAddr() {
	return &engine->outputChannels;
}

template<>
const high_pressure_fuel_pump_s* getLiveDataAddr() {
	return &engine->module<HpfpController>().unmock();
}

template<>
const launch_control_state_s* getLiveDataAddr() {
	return &engine->launchController;
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
const fan_control_s* getLiveDataAddr() {
	return &engine->fan1;
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
const trigger_state_s* getLiveDataAddr() {
	return &engine->triggerCentral.triggerState;
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
const electronic_throttle_s* getLiveDataAddr() {
	// todo: figure out how to handle two units?
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
