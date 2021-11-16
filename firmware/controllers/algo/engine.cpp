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
#include "os_util.h"
#include "os_access.h"
#include "aux_valves.h"
#include "map_averaging.h"
#include "fsio_impl.h"
#include "perf_trace.h"
#include "backup_ram.h"
#include "idle_thread.h"
#include "idle_hardware.h"
#include "gppwm.h"
#include "tachometer.h"
#include "dynoview.h"
#include "boost_control.h"
#include "fan_control.h"
#include "ac_control.h"
#include "vr_pwm.h"
#if EFI_MC33816
 #include "mc33816.h"
#endif // EFI_MC33816

#if EFI_PROD_CODE
#include "trigger_emulator_algo.h"
#include "bench_test.h"
#else
#define isRunningBenchTest() true
#endif /* EFI_PROD_CODE */

#if (BOARD_TLE8888_COUNT > 0)
#include "gpio/tle8888.h"
#endif

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern int waveChartUsedSize;
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

FsioState::FsioState() {
#if EFI_ENABLE_ENGINE_WARNING
	isEngineWarning = FALSE;
#endif
#if EFI_ENABLE_CRITICAL_ENGINE_STOP
	isCriticalEngineCondition = FALSE;
#endif
}

void Engine::resetEngineSnifferIfInTestMode() {
#if EFI_ENGINE_SNIFFER
	if (isFunctionalTestMode) {
		// TODO: what is the exact reasoning for the exact engine sniffer pause time I wonder
		waveChart.pauseEngineSnifferUntilNt = getTimeNowNt() + MS2NT(300);
		waveChart.reset();
	}
#endif /* EFI_ENGINE_SNIFFER */
}

/**
 * VVT decoding delegates to universal trigger decoder. Here we map vvt_mode_e into corresponding trigger_type_e
 */
trigger_type_e getVvtTriggerType(vvt_mode_e vvtMode) {
	switch (vvtMode) {
	case VVT_2JZ:
		return TT_VVT_JZ;
	case VVT_MIATA_NB2:
		return TT_VVT_MIATA_NB2;
	case VVT_BOSCH_QUICK_START:
		return TT_VVT_BOSCH_QUICK_START;
	case VVT_HONDA_K:
	case VVT_TOYOTA_4_1:
	case VVT_FIRST_HALF:
	case VVT_SECOND_HALF:
		return TT_ONE;
	case VVT_FORD_ST170:
		return TT_FORD_ST170;
	case VVT_BARRA_3_PLUS_1:
		return TT_VVT_BARRA_3_PLUS_1;
	case VVT_NISSAN_VQ:
		return TT_VVT_NISSAN_VQ35;
	case VVT_NISSAN_MR:
		return TT_NISSAN_MR18_CAM_VVT;
	default:
		firmwareError(OBD_PCM_Processor_Fault, "getVvtTriggerType for %s", getVvt_mode_e(vvtMode));
		return TT_ONE; // we have to return something for the sake of -Werror=return-type
	}
}

static void initVvtShape(int camIndex, TriggerState &initState) {
	vvt_mode_e vvtMode = engineConfiguration->vvtMode[camIndex];
	TriggerWaveform *shape = &ENGINE(triggerCentral).vvtShape[camIndex];

	// not ideas but good for now code
	ENGINE(triggerCentral).vvtState[0][0].name = "vvt00";
	ENGINE(triggerCentral).vvtState[0][1].name = "vvt01";
	ENGINE(triggerCentral).vvtState[1][0].name = "vvt10";
	ENGINE(triggerCentral).vvtState[1][1].name = "vvt11";

	if (vvtMode != VVT_INACTIVE) {
		trigger_config_s config;
		// todo: should 'vvtWithRealDecoder' be used here?
		ENGINE(triggerCentral).vvtTriggerType[camIndex] = config.type = getVvtTriggerType(vvtMode);

		shape->initializeTriggerWaveform(
				engineConfiguration->ambiguousOperationMode,
				CONFIG(vvtCamSensorUseRise), &config);

		shape->initializeSyncPoint(initState,
				engine->vvtTriggerConfiguration[camIndex],
				config);
	}

}

void Engine::initializeTriggerWaveform() {
	static TriggerState initState;

	// Re-read config in case it's changed
	primaryTriggerConfiguration.update();
	for (int camIndex = 0;camIndex < CAMS_PER_BANK;camIndex++) {
		vvtTriggerConfiguration[camIndex].update();
	}

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	// we have a confusing threading model so some synchronization would not hurt
	chibios_rt::CriticalSectionLocker csl;

	TRIGGER_WAVEFORM(initializeTriggerWaveform(
			engineConfiguration->ambiguousOperationMode,
			engineConfiguration->useOnlyRisingEdgeForTrigger, &engineConfiguration->trigger));

	/**
	 * this is only useful while troubleshooting a new trigger shape in the field
	 * in very VERY rare circumstances
	 */
	if (CONFIG(overrideTriggerGaps)) {
		int gapIndex = 0;
		for (;gapIndex<=CONFIG(overrideTriggerGaps);gapIndex++) {
			float gapOverrideFrom = CONFIG(triggerGapOverrideFrom[gapIndex]);
			float gapOverrideTo = CONFIG(triggerGapOverrideTo[gapIndex]);
			TRIGGER_WAVEFORM(setTriggerSynchronizationGap3(/*gapIndex*/gapIndex, gapOverrideFrom, gapOverrideTo));
		}
		for (;gapIndex<GAP_TRACKING_LENGTH;gapIndex++) {
			ENGINE(triggerCentral.triggerShape).syncronizationRatioFrom[gapIndex] = NAN;
			ENGINE(triggerCentral.triggerShape).syncronizationRatioTo[gapIndex] = NAN;
		}
	}

	if (!TRIGGER_WAVEFORM(shapeDefinitionError)) {
		/**
	 	 * 'initState' instance of TriggerState is used only to initialize 'this' TriggerWaveform instance
	 	 * #192 BUG real hardware trigger events could be coming even while we are initializing trigger
	 	 */
		calculateTriggerSynchPoint(ENGINE(triggerCentral.triggerShape),
				initState);

		ENGINE(triggerCentral.triggerState).name = "TRG";
		engine->engineCycleEventCount = TRIGGER_WAVEFORM(getLength());
	}


	for (int camIndex = 0;camIndex < CAMS_PER_BANK;camIndex++) {
		initVvtShape(camIndex, initState);
	}


	if (!TRIGGER_WAVEFORM(shapeDefinitionError)) {
		prepareOutputSignals();
	}
#endif /* EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT */
}

static void cylinderCleanupControl() {
#if EFI_ENGINE_CONTROL
	bool newValue;
	if (engineConfiguration->isCylinderCleanupEnabled) {
		newValue = !engine->rpmCalculator.isRunning() && Sensor::getOrZero(SensorType::DriverThrottleIntent) > CLEANUP_MODE_TPS;
	} else {
		newValue = false;
	}
	if (newValue != engine->isCylinderCleanupMode) {
		engine->isCylinderCleanupMode = newValue;
		efiPrintf("isCylinderCleanupMode %s", boolToString(newValue));
	}
#endif
}

#if ANALOG_HW_CHECK_MODE
static void assertCloseTo(const char * msg, float actual, float expected) {
	if (actual < 0.75 * expected || actual > 1.25 * expected) {
		firmwareError(OBD_PCM_Processor_Fault, "%s analog input validation failed %f vs %f", msg, actual, expected);
	}
}
#endif // ANALOG_HW_CHECK_MODE

void Engine::periodicSlowCallback() {
	ScopePerf perf(PE::EnginePeriodicSlowCallback);

	// Re-read config in case it's changed
	primaryTriggerConfiguration.update();
	for (int camIndex = 0;camIndex < CAMS_PER_BANK;camIndex++) {
		vvtTriggerConfiguration[camIndex].update();
	}

	watchdog();
	updateSlowSensors();
	checkShutdown();

	updateVrPwm();

#if EFI_FSIO
	runFsio();
#else
	runHardcodedFsio();
#endif /* EFI_FSIO */

	bool acActive = acState.updateAc();
	updateFans(acActive);

	updateGppwm();

	updateIdleControl();

#if EFI_BOOST_CONTROL
	updateBoostControl();
#endif // EFI_BOOST_CONTROL

	cylinderCleanupControl();

	standardAirCharge = getStandardAirCharge();

#if (BOARD_TLE8888_COUNT > 0)
	tle8888startup();
#endif

#if EFI_DYNO_VIEW
	updateDynoView();
#endif

	slowCallBackWasInvoked = true;

#if EFI_PROD_CODE
	void baroLps25Update();
	baroLps25Update();
#endif // EFI_PROD_CODE

#if ANALOG_HW_CHECK_MODE
	efiAssertVoid(OBD_PCM_Processor_Fault, isAdcChannelValid(CONFIG(clt).adcChannel), "No CLT setting");
	efitimesec_t secondsNow = getTimeNowSeconds();
	if (secondsNow > 2 && secondsNow < 180) {
		assertCloseTo("RPM", Sensor::get(SensorType::Rpm).Value, HW_CHECK_RPM);
	} else if (!hasFirmwareError() && secondsNow > 180) {
		static bool isHappyTest = false;
		if (!isHappyTest) {
			setTriggerEmulatorRPM(5 * HW_CHECK_RPM);
			efiPrintf("TEST PASSED");
			isHappyTest = true;
		}
	}
	assertCloseTo("clt", Sensor::get(SensorType::Clt).Value, 49.3);
	assertCloseTo("iat", Sensor::get(SensorType::Iat).Value, 73.2);
	assertCloseTo("aut1", Sensor::get(SensorType::AuxTemp1).Value, 13.8);
	assertCloseTo("aut2", Sensor::get(SensorType::AuxTemp2).Value, 6.2);
#endif // ANALOG_HW_CHECK_MODE
}


#if (BOARD_TLE8888_COUNT > 0)
extern float vBattForTle8888;
#endif /* BOARD_TLE8888_COUNT */

/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 * See also periodicFastCallback
 */
void Engine::updateSlowSensors() {
	updateSwitchInputs();

#if EFI_ENGINE_CONTROL
	int rpm = GET_RPM();
	isEngineChartEnabled = CONFIG(isEngineChartEnabled) && rpm < CONFIG(engineSnifferRpmThreshold);
	sensorChartMode = rpm < CONFIG(sensorSnifferRpmThreshold) ? CONFIG(sensorChartMode) : SC_OFF;

	engineState.updateSlowSensors();

#if (BOARD_TLE8888_COUNT > 0)
	// nasty value injection into C driver which would not be able to access Engine class
	vBattForTle8888 = Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE);
#endif /* BOARD_TLE8888_COUNT */

#if EFI_MC33816
	initMc33816IfNeeded();
#endif // EFI_MC33816
#endif
}

void Engine::updateSwitchInputs() {
#if EFI_GPIO_HARDWARE
	// this value is not used yet
	if (isBrainPinValid(CONFIG(clutchDownPin))) {
		engine->clutchDownState = CONFIG(clutchDownPinInverted) ^ efiReadPin(CONFIG(clutchDownPin));
	}
	if (hasAcToggle()) {
		bool result = getAcToggle();
		if (engine->acSwitchState != result) {
			engine->acSwitchState = result;
			engine->acSwitchLastChangeTime = getTimeNowUs();
		}
		engine->acSwitchState = result;
	}
	if (isBrainPinValid(CONFIG(clutchUpPin))) {
		engine->clutchUpState = CONFIG(clutchUpPinInverted) ^ efiReadPin(CONFIG(clutchUpPin));
	}
	if (isBrainPinValid(CONFIG(throttlePedalUpPin))) {
		engine->idle.throttlePedalUpState = efiReadPin(CONFIG(throttlePedalUpPin));
	}

	if (isBrainPinValid(engineConfiguration->brakePedalPin)) {
		engine->brakePedalState = efiReadPin(engineConfiguration->brakePedalPin);
	}
#endif // EFI_GPIO_HARDWARE
}

void Engine::onTriggerSignalEvent() {
	isSpinning = true;
}

Engine::Engine() {
	reset();
}

/**
 * @see scheduleStopEngine()
 * @return true if there is a reason to stop engine
 */
bool Engine::needToStopEngine(efitick_t nowNt) const {
	return stopEngineRequestTimeNt != 0 &&
			nowNt - stopEngineRequestTimeNt	< 3 * NT_PER_SECOND;
}

int Engine::getGlobalConfigurationVersion(void) const {
	return globalConfigurationVersion;
}

void Engine::reset() {
	/**
	 * it's important for fixAngle() that engineCycle field never has zero
	 */
	engineCycle = getEngineCycle(FOUR_STROKE_CRANK_SENSOR);
	memset(&ignitionPin, 0, sizeof(ignitionPin));
}


/**
 * Here we have a bunch of stuff which should invoked after configuration change
 * so that we can prepare some helper structures
 */
void Engine::preCalculate() {
#if EFI_TUNER_STUDIO
	// we take 2 bytes of crc32, no idea if it's right to call it crc16 or not
	// we have a hack here - we rely on the fact that engineMake is the first of three relevant fields
	tsOutputChannels.engineMakeCodeNameCrc16 = crc32(engineConfiguration->engineMake, 3 * VEHICLE_INFO_SIZE);

	// we need and can empty warning message for CRC purposes
	memset(config->warning_message, 0, sizeof(error_message_t));
	tsOutputChannels.tuneCrc16 = crc32(config, sizeof(persistent_config_s));
#endif /* EFI_TUNER_STUDIO */
}

#if EFI_SHAFT_POSITION_INPUT
void Engine::OnTriggerStateDecodingError() {
	warning(CUSTOM_SYNC_COUNT_MISMATCH, "trigger not happy current %d/%d/%d expected %d/%d/%d",
			triggerCentral.triggerState.currentCycle.eventCount[0],
			triggerCentral.triggerState.currentCycle.eventCount[1],
			triggerCentral.triggerState.currentCycle.eventCount[2],
			TRIGGER_WAVEFORM(getExpectedEventCount(0)),
			TRIGGER_WAVEFORM(getExpectedEventCount(1)),
			TRIGGER_WAVEFORM(getExpectedEventCount(2)));
	triggerCentral.triggerState.setTriggerErrorState();


	triggerCentral.triggerState.totalTriggerErrorCounter++;
	if (CONFIG(verboseTriggerSynchDetails) || (triggerCentral.triggerState.someSortOfTriggerError && !CONFIG(silentTriggerError))) {
#if EFI_PROD_CODE
		efiPrintf("error: synchronizationPoint @ index %d expected %d/%d/%d got %d/%d/%d",
				triggerCentral.triggerState.currentCycle.current_index,
				TRIGGER_WAVEFORM(getExpectedEventCount(0)),
				TRIGGER_WAVEFORM(getExpectedEventCount(1)),
				TRIGGER_WAVEFORM(getExpectedEventCount(2)),
				triggerCentral.triggerState.currentCycle.eventCount[0],
				triggerCentral.triggerState.currentCycle.eventCount[1],
				triggerCentral.triggerState.currentCycle.eventCount[2]);
#endif /* EFI_PROD_CODE */
	}

}

void Engine::OnTriggerStateProperState(efitick_t nowNt) {
	rpmCalculator.setSpinningUp(nowNt);
}

void Engine::OnTriggerSynchronizationLost() {
	// Needed for early instant-RPM detection
	rpmCalculator.setStopSpinning();
}

void Engine::OnTriggerInvalidIndex(int currentIndex) {
	// let's not show a warning if we are just starting to spin
	if (GET_RPM() != 0) {
		warning(CUSTOM_SYNC_ERROR, "sync error: index #%d above total size %d", currentIndex, triggerCentral.triggerShape.getSize());
		triggerCentral.triggerState.setTriggerErrorState();
	}
}

void Engine::OnTriggerSyncronization(bool wasSynchronized) {
	// We only care about trigger shape once we have synchronized trigger. Anything could happen
	// during first revolution and it's fine
	if (wasSynchronized) {
		/**
	 	 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
	 	 */
		bool isDecodingError = triggerCentral.triggerState.validateEventCounters(triggerCentral.triggerShape);

		enginePins.triggerDecoderErrorPin.setValue(isDecodingError);

		// 'triggerStateListener is not null' means we are running a real engine and now just preparing trigger shape
		// that's a bit of a hack, a sweet OOP solution would be a real callback or at least 'needDecodingErrorLogic' method?
		if (isDecodingError) {
			OnTriggerStateDecodingError();
		}

		engine->triggerErrorDetection.add(isDecodingError);

		if (triggerCentral.isTriggerDecoderError()) {
			warning(CUSTOM_OBD_TRG_DECODING, "trigger decoding issue. expected %d/%d/%d got %d/%d/%d",
					TRIGGER_WAVEFORM(getExpectedEventCount(0)),
					TRIGGER_WAVEFORM(getExpectedEventCount(1)),
					TRIGGER_WAVEFORM(getExpectedEventCount(2)),
					triggerCentral.triggerState.currentCycle.eventCount[0],
					triggerCentral.triggerState.currentCycle.eventCount[1],
					triggerCentral.triggerState.currentCycle.eventCount[2]);
		}
	}

}
#endif

void Engine::injectEngineReferences() {
	primaryTriggerConfiguration.update();
	for (int camIndex = 0;camIndex < CAMS_PER_BANK;camIndex++) {
		vvtTriggerConfiguration[camIndex].update();
	}
}

void Engine::setConfig() {
	efi::clear(config);

	injectEngineReferences();
}

void Engine::watchdog() {
#if EFI_ENGINE_CONTROL
	if (isRunningPwmTest)
		return;
	if (!isSpinning) {
		if (!isRunningBenchTest() && enginePins.stopPins()) {
			// todo: make this a firmwareError assuming functional tests would run
			warning(CUSTOM_ERR_2ND_WATCHDOG, "Some pins were turned off by 2nd pass watchdog");
		}
		return;
	}

	/**
	 * todo: better watch dog implementation should be implemented - see
	 * http://sourceforge.net/p/rusefi/tickets/96/
	 */
	float secondsSinceTriggerEvent = engine->triggerCentral.getTimeSinceTriggerEvent(getTimeNowNt());

	if (secondsSinceTriggerEvent < 0.5f) {
		// Engine moved recently, no need to safe pins.
		return;
	}
	isSpinning = false;
	ignitionEvents.isReady = false;
#if EFI_PROD_CODE || EFI_SIMULATOR
	efiPrintf("engine has STOPPED");
	efiPrintf("templog engine has STOPPED %f", secondsSinceTriggerEvent);
	triggerInfo();
#endif

	enginePins.stopPins();
#endif
}

void Engine::checkShutdown() {
#if EFI_MAIN_RELAY_CONTROL
	// if we are already in the "ignition_on" mode, then do nothing
	if (ignitionOnTimeNt > 0) {
		return;
	}

	// here we are in the shutdown (the ignition is off) or initial mode (after the firmware fresh start)
	const efitick_t engineStopWaitTimeoutUs = 500000LL;	// 0.5 sec
	// in shutdown mode, we need a small cooldown time between the ignition off and on
	if (stopEngineRequestTimeNt == 0 || (getTimeNowNt() - stopEngineRequestTimeNt) > US2NT(engineStopWaitTimeoutUs)) {
		// if the ignition key is turned on again,
		// we cancel the shutdown mode, but only if all shutdown procedures are complete
		const float vBattThresholdOn = 8.0f;
		if ((Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE) > vBattThresholdOn) && !isInShutdownMode()) {
			ignitionOnTimeNt = getTimeNowNt();
			stopEngineRequestTimeNt = 0;
			efiPrintf("Ignition voltage detected! Cancel the engine shutdown!");
		}
	}
#endif /* EFI_MAIN_RELAY_CONTROL */
}

bool Engine::isInMainRelayBench() {
	if (mainRelayBenchStartNt == 0) {
		return false;
	}
	return (getTimeNowNt() - mainRelayBenchStartNt) < NT_PER_SECOND;
}

bool Engine::isInShutdownMode() const {
#if EFI_MAIN_RELAY_CONTROL
	// if we are in "ignition_on" mode and not in shutdown mode
	if (stopEngineRequestTimeNt == 0 && ignitionOnTimeNt > 0) {
		const float vBattThresholdOff = 5.0f;
		// start the shutdown process if the ignition voltage dropped low
		if (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE) <= vBattThresholdOff) {
			scheduleStopEngine();
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

bool Engine::isMainRelayEnabled() const {
#if EFI_MAIN_RELAY_CONTROL
	return enginePins.mainRelay.getLogicValue();
#else
	// if no main relay control, we assume it's always turned on
	return true;
#endif /* EFI_MAIN_RELAY_CONTROL */
}


float Engine::getTimeIgnitionSeconds(void) const {
	// return negative if the ignition is turned off
	if (ignitionOnTimeNt == 0)
		return -1;
	float numSeconds = (float)NT2US(getTimeNowNt() - ignitionOnTimeNt) / US_PER_SECOND_F;
	return numSeconds;
}

injection_mode_e Engine::getCurrentInjectionMode() {
	return rpmCalculator.isCranking() ? CONFIG(crankingInjectionMode) : CONFIG(injectionMode);
}

// see also in TunerStudio project '[doesTriggerImplyOperationMode] tag
static bool doesTriggerImplyOperationMode(trigger_type_e type) {
	return type != TT_TOOTHED_WHEEL
			&& type != TT_ONE
			&& type != TT_ONE_PLUS_ONE
			&& type != TT_3_1_CAM
			&& type != TT_36_2_2_2
			&& type != TT_TOOTHED_WHEEL_60_2
			&& type != TT_TOOTHED_WHEEL_36_1;
}

operation_mode_e Engine::getOperationMode() {
	/**
	 * here we ignore user-provided setting for well known triggers.
	 * For instance for Miata NA, there is no reason to allow user to set FOUR_STROKE_CRANK_SENSOR
	 */
	return doesTriggerImplyOperationMode(engineConfiguration->trigger.type) ? triggerCentral.triggerShape.getOperationMode() : engineConfiguration->ambiguousOperationMode;
}

/**
 * The idea of this method is to execute all heavy calculations in a lower-priority thread,
 * so that trigger event handler/IO scheduler tasks are faster.
 */
void Engine::periodicFastCallback() {
	ScopePerf pc(PE::EnginePeriodicFastCallback);

#if EFI_MAP_AVERAGING
	refreshMapAveragingPreCalc();
#endif

	engineState.periodicFastCallback();

	knockController.periodicFastCallback();

	tachSignalCallback();
}

void doScheduleStopEngine() {
	efiPrintf("Starting doScheduleStopEngine");
	engine->stopEngineRequestTimeNt = getTimeNowNt();
	engine->ignitionOnTimeNt = 0;
	// todo: initiate stepper motor parking
	// make sure we have stored all the info
#if EFI_PROD_CODE
	//todo: FIX kinetis build with this line
	//backupRamFlush();
#endif // EFI_PROD_CODE
}
