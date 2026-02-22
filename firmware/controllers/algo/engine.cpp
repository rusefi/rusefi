/**
 * @file	engine.cpp
 *
 *
 * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
 * express myself in C/C++. I am open for suggestions :)
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_central.h"
#include "fuel_math.h"
#include "advance_map.h"
#include "speed_density.h"
#include "advance_map.h"
#include "init.h"

#include "aux_valves.h"
#include "perf_trace.h"
#include "backup_ram.h"
#include "idle_thread.h"
#include "idle_hardware.h"
#include "gppwm.h"
#include "speedometer.h"
#include "dynoview.h"
#include "boost_control.h"
#include "ac_control.h"
#include "vr_pwm.h"
#include "max3185x.h"
#if EFI_MC33816
 #include "mc33816.h"
#endif // EFI_MC33816

#include "bench_test.h"

#if EFI_PROD_CODE
#include "trigger_emulator_algo.h"
#endif /* EFI_PROD_CODE */

#if (BOARD_TLE8888_COUNT > 0)
#include "gpio/tle8888.h"
#endif

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern int waveChartUsedSize;
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

void Engine::resetEngineSnifferIfInTestMode() {
#if EFI_ENGINE_SNIFFER
	if (isFunctionalTestMode) {
		// TODO: what is the exact reasoning for the exact engine sniffer pause time I wonder
		waveChart.pauseEngineSnifferUntilNt = getTimeNowNt() + MS2NT(300);
		waveChart.reset();
	}
#endif /* EFI_ENGINE_SNIFFER */
}

PUBLIC_API_WEAK trigger_type_e getCustomVvtTriggerType(vvt_mode_e vvtMode) {
		criticalError("Broken VVT mode maybe corrupted calibration %d: %s", vvtMode, getVvt_mode_e(vvtMode));
		return trigger_type_e::TT_HALF_MOON; // we have to return something for the sake of -Werror=return-type
}

// todo: move this method from engine.cpp already?
/**
 * VVT decoding delegates to universal trigger decoder. Here we map vvt_mode_e into corresponding trigger_type_e
 */
trigger_type_e getVvtTriggerType(vvt_mode_e vvtMode) {
	switch (vvtMode) {
	case VVT_CUSTOM_1:
	case VVT_CUSTOM_2:
	case VVT_INACTIVE:
	  // hold on, what? 'VVT_INACTIVE' means TT_HALF_MOON?!
		return trigger_type_e::TT_HALF_MOON;
	case VVT_TOYOTA_3_TOOTH:
		return trigger_type_e::TT_VVT_TOYOTA_3_TOOTH;
	case VVT_MIATA_NB:
		return trigger_type_e::TT_VVT_MIATA_NB;
	case VVT_BOSCH_QUICK_START:
		return trigger_type_e::TT_VVT_BOSCH_QUICK_START;
	case VVT_HONDA_K_EXHAUST:
	    return trigger_type_e::TT_HONDA_K_CAM_4_1;
	case VVT_HONDA_K_INTAKE:
	case VVT_SINGLE_TOOTH:
	case VVT_MAP_V_TWIN:
		return trigger_type_e::TT_HALF_MOON;
	case VVT_FORD_ST170:
		return trigger_type_e::TT_FORD_ST170;
	case VVT_BARRA_3_PLUS_1:
		return trigger_type_e::TT_VVT_BARRA_3_PLUS_1;
	case VVT_FORD_COYOTE:
	    return trigger_type_e::TT_VVT_FORD_COYOTE;
	case VVT_DEV:
	    return trigger_type_e::TT_DEV;
	case VVT_MAZDA_SKYACTIV:
	    return trigger_type_e::TT_VVT_MAZDA_SKYACTIV;
	case VVT_MAZDA_L:
		return trigger_type_e::TT_VVT_MAZDA_L;
	case VVT_NISSAN_VQ:
		return trigger_type_e::TT_VVT_NISSAN_VQ35;
	case VVT_TOYOTA_4_1:
		return trigger_type_e::TT_VVT_TOYOTA_4_1;
	case VVT_MITSUBISHI_4G69:
	    return trigger_type_e::TT_VVT_MITSUBISHI_4G69;
	case VVT_MITSUBISHI_3A92:
		return trigger_type_e::TT_VVT_MITSUBISHI_3A92;
	case VVT_MITSUBISHI_6G72:
	    return trigger_type_e::TT_VVT_MITSU_6G72;
	case VVT_HONDA_CBR_600:
	    return trigger_type_e::TT_HONDA_CBR_600;
	case VVT_CHRYSLER_PHASER:
		return trigger_type_e::TT_CHRYSLER_PHASER;
	case VVT_TOYOTA_3TOOTH_UZ:
		return trigger_type_e::TT_TOYOTA_3_TOOTH_UZ;
	case VVT_NISSAN_MR:
		return trigger_type_e::TT_NISSAN_MR18_CAM_VVT;
	case VVT_BMW_N63TU:
	case VVT_MITSUBISHI_4G63:
		return trigger_type_e::TT_MITSU_4G63_CAM;
	case VVT_HR12DDR_IN:
	    return trigger_type_e::TT_NISSAN_HR_CAM_IN;
	case VVT_SUBARU_7TOOTH:
			return trigger_type_e::TT_VVT_SUBARU_7_WITHOUT_6;
	default:
	  return getCustomVvtTriggerType(vvtMode);
	}
}

void Engine::updateTriggerConfiguration() {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	// we have a confusing threading model so some synchronization would not hurt
	chibios_rt::CriticalSectionLocker csl;

	engine->triggerCentral.applyShapesConfiguration();

	if (!engine->triggerCentral.triggerShape.shapeDefinitionError) {
		prepareOutputSignals();
	}
#endif /* EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT */
}

#include "board_overrides.h"

std::optional<setup_custom_board_overrides_type> custom_board_periodicSlowCallback;
std::optional<setup_custom_board_overrides_type> custom_board_periodicFastCallback;

void boardPeriodicSlowCallback() {
  // placeholder to force upgrade
}

void boardPeriodicFastCallback() {
  // placeholder to force upgrade
}

void Engine::periodicSlowCallback() {
	ScopePerf perf(PE::EnginePeriodicSlowCallback);

#if EFI_SHAFT_POSITION_INPUT
	// Re-read config in case it's changed
	triggerCentral.primaryTriggerConfiguration.update();
	for (int camIndex = 0;camIndex < CAMS_PER_BANK;camIndex++) {
		triggerCentral.vvtTriggerConfiguration[camIndex].update();
	}

  getEngineState()->heaterControlEnabled = engineConfiguration->forceO2Heating || engine->rpmCalculator.isRunning();
	enginePins.o2heater.setValue(getEngineState()->heaterControlEnabled);
	enginePins.starterRelayDisable.setValue(Sensor::getOrZero(SensorType::Rpm) < engineConfiguration->cranking.rpm);
#endif // EFI_SHAFT_POSITION_INPUT

	efiWatchdog();
	updateSlowSensors();
	checkShutdown();

	module<TpsAccelEnrichment>()->onNewValue(Sensor::getOrZero(SensorType::Tps1));

	updateVrThresholdPwm();

	updateGppwm();

	engine->engineModules.apply_all([](auto & m) { m.onSlowCallback(); });

#if (BOARD_TLE8888_COUNT > 0)
	tle8888startup();
#endif

#if EFI_DYNO_VIEW
	updateDynoView();
#endif

	// TODO: move to sensor_checker.cpp?
	if ((engine->rpmCalculator.isCranking()) && (Sensor::getOrZero(SensorType::BatteryVoltage) < 7)) {
		// undervoltage crancking!
		if (getEngineState()->undervoltageCrankingTimer.getElapsedSeconds() > 1) {
			warningTsReport(ObdCode::OBD_System_Voltage_Low, "Cranking on low battery!");
		}
	} else {
		getEngineState()->undervoltageCrankingTimer.reset();
	}

	slowCallBackWasInvoked = true;

#if EFI_PROD_CODE
	void baroLps25Update();
	baroLps25Update();
#endif // EFI_PROD_CODE
  boardPeriodicSlowCallback();
  call_board_override(custom_board_periodicSlowCallback);
}

/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 * See also periodicFastCallback
 */
void Engine::updateSlowSensors() {
	updateSwitchInputs();

#if EFI_PROD_CODE
	// todo: extract method? do better? see https://github.com/rusefi/rusefi/issues/7511 for details
	engine->module<InjectorModelSecondary>()->updateState();
	engine->module<InjectorModelPrimary>()->updateState();
#endif // EFI_PROD_CODE

#if EFI_SHAFT_POSITION_INPUT
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	triggerCentral.isEngineSnifferEnabled = rpm < engineConfiguration->engineSnifferRpmThreshold;
#endif // EFI_SHAFT_POSITION_INPUT
}

bool getClutchDownState() {
#if EFI_GPIO_HARDWARE
	if (isBrainPinValid(engineConfiguration->clutchDownPin)) {
		return efiReadPin(engineConfiguration->clutchDownPin, engineConfiguration->clutchDownPinMode);
	}
#endif // EFI_GPIO_HARDWARE
	// todo: boolean sensors should leverage sensor framework #6342
	return engine->engineState.lua.clutchDownState;
}

static bool getClutchUpState() {
#if EFI_GPIO_HARDWARE
	if (isBrainPinValid(engineConfiguration->clutchUpPin)) {
		return efiReadPin(engineConfiguration->clutchUpPin, engineConfiguration->clutchUpPinMode);
	}
#endif // EFI_GPIO_HARDWARE
	// todo: boolean sensors should leverage sensor framework #6342
	return engine->engineState.lua.clutchUpState;
}

bool getBrakePedalState() {
#if EFI_GPIO_HARDWARE
	if (isBrainPinValid(engineConfiguration->brakePedalPin)) {
		return efiReadPin(engineConfiguration->brakePedalPin, engineConfiguration->brakePedalPinMode);
	}
#endif // EFI_GPIO_HARDWARE
	// todo: boolean sensors should leverage sensor framework #6342
	return engine->engineState.lua.brakePedalState;
}


void Engine::updateSwitchInputs() {
	// this value is not used yet
  engine->engineState.clutchDownState = getClutchDownState();
	engine->clutchUpSwitchedState.update(getClutchUpState());
	engine->brakePedalSwitchedState.update(getBrakePedalState());
#if EFI_GPIO_HARDWARE
	{
		bool currentState;
		if (hasAcToggle()) {
			currentState = getAcToggle();
#ifdef EFI_KLINE
		} else if (engineConfiguration->hondaK) {
extern bool kAcRequestState;
		    currentState = kAcRequestState;
#endif // EFI_KLINE
		} else {
			currentState = engine->engineState.lua.acRequestState;
		}
		AcController & acController = engine->module<AcController>().unmock();
		if (engine->acButtonSwitchedState.update(currentState)) {
			acController.timeSinceStateChange.reset();
		}
	}

	pokeAuxDigital();

#endif // EFI_GPIO_HARDWARE
}

Engine::Engine() {
	// Everything else has default initializers setup in generated file
	engineState.lua.fuelMult = 1;
	ignitionState.luaTimingMult = 1;
}

int Engine::getGlobalConfigurationVersion() const {
	return globalConfigurationVersion;
}

void Engine::reset() {
	/**
	 * it's important for wrapAngle() that engineCycle field never has zero
	 */
	engineState.engineCycle = getEngineCycle(FOUR_STROKE_CRANK_SENSOR);
	resetLua();
}

void Engine::resetLua() {
	// todo: https://github.com/rusefi/rusefi/issues/4308
	engineState.lua = {};
	engineState.lua.fuelAdd = 0;
	engineState.lua.fuelMult = 1;
	engineState.lua.luaDisableEtb = false;
	engineState.lua.luaIgnCut = false;
	engineState.lua.luaFuelCut = false;
	engineState.lua.engineTorque = NAN;
	engineState.lua.disableDecelerationFuelCutOff = false;
#if EFI_BOOST_CONTROL
	module<BoostController>().unmock().resetLua();
#endif // EFI_BOOST_CONTROL
	ignitionState.luaTimingAdd = 0;
	ignitionState.luaTimingMult = 1;
#if EFI_IDLE_CONTROL
	module<IdleController>().unmock().luaAdd = 0;
#endif // EFI_IDLE_CONTROL
}

/**
 * Here we have a bunch of stuff which should invoked after configuration change
 * so that we can prepare some helper structures
 */
void Engine::preCalculate() {
#if EFI_TUNER_STUDIO
	// we take 2 bytes of crc32, no idea if it's right to call it crc16 or not
	// we have a hack here - we rely on the fact that engineMake is the first of three relevant fields
	engine->outputChannels.engineMakeCodeNameCrc16 = crc32(engineConfiguration->engineMake, 3 * VEHICLE_INFO_SIZE);

	engine->outputChannels.tuneCrc16 = crc32(config, sizeof(persistent_config_s));
#endif /* EFI_TUNER_STUDIO */
}

#if EFI_SHAFT_POSITION_INPUT
void Engine::OnTriggerStateProperState(efitick_t nowNt, size_t triggerStateIndex) {
	UNUSED(triggerStateIndex);

	rpmCalculator.setSpinningUp(nowNt);
}

TriggerStateListener* Engine::nextListener() {
  return secondListener;
}

void Engine::OnTriggerSynchronizationLost() {
	// Needed for early instant-RPM detection
	rpmCalculator.setStopSpinning();

	triggerCentral.triggerState.resetState();
	triggerCentral.instantRpm.resetInstantRpm();

	for (size_t i = 0; i < efi::size(triggerCentral.vvtState); i++) {
		for (size_t j = 0; j < efi::size(triggerCentral.vvtState[0]); j++) {
			triggerCentral.vvtState[i][j].resetState();
		}
	}
}

void Engine::OnTriggerSynchronization(bool wasSynchronized, bool isDecodingError) {
	// TODO: this logic probably shouldn't be part of engine.cpp

	// We only care about trigger shape once we have synchronized trigger. Anything could happen
	// during first revolution and it's fine
	if (wasSynchronized) {
		enginePins.triggerDecoderErrorPin.setValue(isDecodingError);

		// 'triggerStateListener is not null' means we are running a real engine and now just preparing trigger shape
		// that's a bit of a hack, a sweet OOP solution would be a real callback or at least 'needDecodingErrorLogic' method?
		if (isDecodingError) {
#if EFI_PROD_CODE
			if (engineConfiguration->verboseTriggerSynchDetails || (triggerCentral.triggerState.someSortOfTriggerError() && !engineConfiguration->silentTriggerError)) {
				efiPrintf("error: synchronizationPoint @ index %lu expected %d/%d got %d/%d",
						triggerCentral.triggerState.currentCycle.current_index,
						triggerCentral.triggerShape.getExpectedEventCount(TriggerWheel::T_PRIMARY),
						triggerCentral.triggerShape.getExpectedEventCount(TriggerWheel::T_SECONDARY),
						triggerCentral.triggerState.currentCycle.eventCount[0],
						triggerCentral.triggerState.currentCycle.eventCount[1]);
			}
#endif /* EFI_PROD_CODE */
		}

		engine->triggerCentral.triggerErrorDetection.add(isDecodingError);
	}

}
#endif

void Engine::injectEngineReferences() {
#if EFI_SHAFT_POSITION_INPUT
	triggerCentral.primaryTriggerConfiguration.update();
	for (int camIndex = 0;camIndex < CAMS_PER_BANK;camIndex++) {
		triggerCentral.vvtTriggerConfiguration[camIndex].update();
	}
#endif // EFI_SHAFT_POSITION_INPUT
}

void Engine::setConfig() {
#if !EFI_UNIT_TEST
// huh should this be happy?  static_assert(config != nullptr);
#endif
	efi::clear(config);

	injectEngineReferences();
}

/**
 * This code asserts that we do not have unexpected gaps in time flow with the exception of internal flash burn.
 */
static void assertTimeIsLinear() {
#if ! EFI_UNIT_TEST
	static efitimems_t mostRecentMs = 0;
	efitimems_t msNow = getTimeNowMs();
	if (engineConfiguration->watchOutForLinearTime && engine->configBurnTimer.hasElapsedSec(5)) {
		if (mostRecentMs != 0) {
			efitimems_t gapInMs = msNow - mostRecentMs;
			// todo: lower gapInMs threshold?
			if (gapInMs > 200) {
				firmwareError(ObdCode::RUNTIME_CRITICAL_WATCH_DOG_SECONDS, "gap in time: mostRecentMs %lumS, now=%lumS, gap=%lumS",
					mostRecentMs, msNow, gapInMs);
			}
		}
	}
	mostRecentMs = msNow;
#endif
}

void Engine::efiWatchdog() {
    assertTimeIsLinear();
	if (isRunningPwmTest) {
		return;
	}

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	if (module<PrimeController>()->isPriming() || triggerCentral.engineMovedRecently()) {
        // do not invoke check in priming or if engine moved recently, no need to assert safe pin state.
		return;
	}

	if (!triggerCentral.isSpinningJustForWatchdog) {
		if (!isRunningBenchTest() && enginePins.stopPins()) {
			// todo: make this a firmwareError assuming functional tests would run
			warning(ObdCode::CUSTOM_ERR_2ND_WATCHDOG, "Some pins were turned off by 2nd pass watchdog");
		}
		return;
	}

	/**
	 * todo: better watch dog implementation should be implemented - see
	 * http://sourceforge.net/p/rusefi/tickets/96/
	 */
	triggerCentral.isSpinningJustForWatchdog = false;
    onEngineHasStopped();
#endif // EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
}

void Engine::onEngineHasStopped() {
#if EFI_ENGINE_CONTROL
	ignitionEvents.isReady = false;
#endif // EFI_ENGINE_CONTROL

#if EFI_PROD_CODE || EFI_SIMULATOR
	efiPrintf("Engine has stopped spinning.");
#endif

    // this invocation should be the last layer of defence in terms of making sure injectors/coils are not active
	enginePins.stopPins();
}

void Engine::checkShutdown() {
#if EFI_MAIN_RELAY_CONTROL
	// if we are already in the "ignition_on" mode, then do nothing
/* this logic is not alive
	if (ignitionOnTimeNt > 0) {
		return;
	}
todo: move to shutdown_controller.cpp
*/

	// here we are in the shutdown (the ignition is off) or initial mode (after the firmware fresh start)
/* this needs work or tests
	const efitick_t engineStopWaitTimeoutUs = 500000LL;	// 0.5 sec
	// in shutdown mode, we need a small cooldown time between the ignition off and on
todo: move to shutdown_controller.cpp
	if (stopEngineRequestTimeNt == 0 || (getTimeNowNt() - stopEngineRequestTimeNt) > US2NT(engineStopWaitTimeoutUs)) {
		// if the ignition key is turned on again,
		// we cancel the shutdown mode, but only if all shutdown procedures are complete
		const float vBattThresholdOn = 8.0f;
		// we fallback into zero instead of VBAT_FALLBACK_VALUE because it's not safe to false-trigger the "ignition on" event,
		// and we want to turn on the main relay only when 100% sure.
		if ((Sensor::getOrZero(SensorType::BatteryVoltage) > vBattThresholdOn) && !isInShutdownMode()) {
			ignitionOnTimeNt = getTimeNowNt();
			efiPrintf("Ignition voltage detected!");
			if (stopEngineRequestTimeNt != 0) {
				efiPrintf("Cancel the engine shutdown!");
				stopEngineRequestTimeNt = 0;
			}
		}
	}
*/
#endif /* EFI_MAIN_RELAY_CONTROL */
}

bool Engine::isInShutdownMode() const {
	// TODO: this logic is currently broken
#if 0 && EFI_MAIN_RELAY_CONTROL && EFI_PROD_CODE
	// if we are in "ignition_on" mode and not in shutdown mode
	if (stopEngineRequestTimeNt == 0 && ignitionOnTimeNt > 0) {
		const float vBattThresholdOff = 5.0f;
		// start the shutdown process if the ignition voltage dropped low
		if (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE) <= vBattThresholdOff) {
			doScheduleStopEngine();
		}
	}

	// we are not in the shutdown mode?
	if (stopEngineRequestTimeNt == 0) {
		return false;
	}

	const efitick_t turnOffWaitTimeoutNt = NT_PER_SECOND;
	// We don't want any transients to step in, so we wait at least 1 second whatever happens.
	// Also it's good to give the stepper motor some time to start moving to the initial position (or parking)
	if ((getTimeNowNt() - stopEngineRequestTimeNt) < turnOffWaitTimeoutNt)
		return true;

	const efitick_t engineSpinningWaitTimeoutNt = 5 * NT_PER_SECOND;
	// The engine is still spinning! Give it some time to stop (but wait no more than 5 secs)
	if (isSpinning && (getTimeNowNt() - stopEngineRequestTimeNt) < engineSpinningWaitTimeoutNt)
		return true;

	// The idle motor valve is still moving! Give it some time to park (but wait no more than 10 secs)
	// Usually it can move to the initial 'cranking' position or zero 'parking' position.
	const efitick_t idleMotorWaitTimeoutNt = 10 * NT_PER_SECOND;
	if (isIdleMotorBusy() && (getTimeNowNt() - stopEngineRequestTimeNt) < idleMotorWaitTimeoutNt)
		return true;
#endif /* EFI_MAIN_RELAY_CONTROL */
	return false;
}

injection_mode_e getCurrentInjectionMode() {
	return getEngineRotationState()->isCranking() ? engineConfiguration->crankingInjectionMode : engineConfiguration->injectionMode;
}

/**
 * The idea of this method is to execute all heavy calculations in a lower-priority thread,
 * so that trigger event handler/IO scheduler tasks are faster.
 */
void Engine::periodicFastCallback() {
	ScopePerf pc(PE::EnginePeriodicFastCallback);

	boardPeriodicFastCallback();
	call_board_override(custom_board_periodicFastCallback);


	engineState.periodicFastCallback();

	speedoUpdate();

	engineModules.apply_all([](auto & m) { m.onFastCallback(); });
}

void Engine::onEngineStopped() {
	engineModules.apply_all([](auto& m) { m.onEngineStop(); });
}

EngineRotationState * getEngineRotationState() {
	return &engine->rpmCalculator;
}

EngineState * getEngineState() {
	return &engine->engineState;
}

TunerStudioOutputChannels *getTunerStudioOutputChannels() {
	return &engine->outputChannels;
}

Scheduler *getScheduler() {
	return &engine->scheduler;
}

#if EFI_SHAFT_POSITION_INPUT
TriggerCentral * getTriggerCentral() {
	return &engine->triggerCentral;
}
#endif // EFI_SHAFT_POSITION_INPUT

#if EFI_ENGINE_CONTROL
LimpManager * getLimpManager() {
	return &engine->module<LimpManager>().unmock();
}

FuelSchedule *getFuelSchedule() {
	return &engine->injectionEvents;
}

IgnitionEventList *getIgnitionEvents() {
	return &engine->ignitionEvents;
}
#endif // EFI_ENGINE_CONTROL
