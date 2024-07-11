/**
 * @file    engine_controller.cpp
 * @brief   Controllers package entry point code
 *
 *
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "pch.h"


#include "trigger_central.h"
#include "script_impl.h"
#include "idle_thread.h"
#include "hardware.h"
#include "advance_map.h"
#include "main_trigger_callback.h"
#include "flash_main.h"
#include "bench_test.h"
#include "mmc_card.h"
#include "electronic_throttle.h"
#include "trigger_emulator_algo.h"
#include "map_averaging.h"
#include "high_pressure_fuel_pump.h"
#include "malfunction_central.h"
#include "malfunction_indicator.h"
#include "speed_density.h"
#include "local_version_holder.h"
#include "alternator_controller.h"
#include "can_bench_test.h"
#include "engine_emulator.h"
#include "fuel_math.h"
#include "spark_logic.h"
#include "status_loop.h"
#include "aux_valves.h"
#include "accelerometer.h"
#include "vvt.h"
#include "boost_control.h"
#include "launch_control.h"
#include "tachometer.h"
#include "speedometer.h"
#include "gppwm.h"
#include "date_stamp.h"
#include "rusefi_lua.h"
#include "buttonshift.h"
#include "start_stop.h"
#include "dynoview.h"
#include "vr_pwm.h"
#include "adc_subscription.h"
#include "gc_generic.h"

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif /* EFI_SENSOR_CHART */

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif /* EFI_TUNER_STUDIO */

#if EFI_LOGIC_ANALYZER
#include "logic_analyzer.h"
#endif /* EFI_LOGIC_ANALYZER */

#if HAL_USE_ADC
#include "AdcDevice.h"
#endif /* HAL_USE_ADC */

#if defined(EFI_BOOTLOADER_INCLUDE_CODE)
#include "bootloader/bootloader.h"
#endif /* EFI_BOOTLOADER_INCLUDE_CODE */

#include "periodic_task.h"


#if ! EFI_UNIT_TEST
#include "init.h"
#include "mpu_util.h"
#endif /* EFI_UNIT_TEST */

#if !EFI_UNIT_TEST

/**
 * Would love to pass reference to configuration object into constructor but C++ does allow attributes after parenthesized initializer
 */
Engine ___engine CCM_OPTIONAL;

#else // EFI_UNIT_TEST

Engine * engine;

#endif /* EFI_UNIT_TEST */


void initDataStructures() {
#if EFI_ENGINE_CONTROL
	initFuelMap();
	initSpeedDensity();
	IgnitionEventList &events = engine->ignitionEvents;
	for (size_t i=0;i<efi::size(events.elements);i++) {
	  // above-zero value helps distinguish events
	  events.elements[i].sparkCounter = 1;
	}
  // above-zero value helps distinguish events
	engine->engineState.globalSparkCounter = 1;
#endif // EFI_ENGINE_CONTROL
}

#if !EFI_UNIT_TEST

static void doPeriodicSlowCallback();

class PeriodicFastController : public PeriodicTimerController {
	void PeriodicTask() override {
		engine->periodicFastCallback();
	}

	int getPeriodMs() override {
		return FAST_CALLBACK_PERIOD_MS;
	}
};

class PeriodicSlowController : public PeriodicTimerController {
	void PeriodicTask() override {
		doPeriodicSlowCallback();
	}

	int getPeriodMs() override {
		// no reason to have this configurable, looks like everyone is happy with 20Hz
		return SLOW_CALLBACK_PERIOD_MS;
	}
};

static PeriodicFastController fastController;
static PeriodicSlowController slowController;

class EngineStateBlinkingTask : public PeriodicTimerController {
	int getPeriodMs() override {
		return 50;
	}

	void PeriodicTask() override {
#if EFI_SHAFT_POSITION_INPUT
		bool is_running = engine->rpmCalculator.isRunning();
#else
		bool is_running = false;
#endif /* EFI_SHAFT_POSITION_INPUT */

		if (is_running) {
			// blink in running mode
			enginePins.runningLedPin.toggle();
		} else {
			bool is_cranking = engine->rpmCalculator.isCranking();
			enginePins.runningLedPin.setValue(is_cranking);
		}
	}
};

static EngineStateBlinkingTask engineStateBlinkingTask;

static void resetAccel() {
	engine->tpsAccelEnrichment.resetAE();
#if EFI_ENGINE_CONTROL
	for (size_t i = 0; i < efi::size(engine->injectionEvents.elements); i++)
	{
		engine->injectionEvents.elements[i].getWallFuel().resetWF();
	}
#endif // EFI_ENGINE_CONTROL
}

static void doPeriodicSlowCallback() {
#if EFI_SHAFT_POSITION_INPUT
	efiAssertVoid(ObdCode::CUSTOM_ERR_6661, getCurrentRemainingStack() > 64, "lowStckOnEv");

	slowStartStopButtonCallback();

	engine->rpmCalculator.onSlowCallback();
	if (engine->rpmCalculator.isStopped()) {
		resetAccel();
	}

	if (engine->versionForConfigurationListeners.isOld(engine->getGlobalConfigurationVersion())) {
		updateAccelParameters();
	}
#endif /* EFI_SHAFT_POSITION_INPUT */

	engine->periodicSlowCallback();

#if EFI_SHAFT_POSITION_INPUT
	if (engine->triggerCentral.directSelfStimulation || engine->rpmCalculator.isStopped()) {
		/**
		 * rusEfi usually runs on hardware which halts execution while writing to internal flash, so we
		 * postpone writes to until engine is stopped. Writes in case of self-stimulation are fine.
		 *
		 * todo: allow writing if 2nd bank of flash is used
		 */
#if EFI_CONFIGURATION_STORAGE
		writeToFlashIfPending();
#endif /* EFI_CONFIGURATION_STORAGE */
	}
#else /* if EFI_SHAFT_POSITION_INPUT */
	#if EFI_CONFIGURATION_STORAGE
		writeToFlashIfPending();
	#endif /* EFI_CONFIGURATION_STORAGE */
#endif /* EFI_SHAFT_POSITION_INPUT */

#if EFI_TCU
	if (engineConfiguration->tcuEnabled && engineConfiguration->gearControllerMode != GearControllerMode::None) {
		if (engine->gearController == NULL) {
			initGearController();
		} else if (engine->gearController->getMode() != engineConfiguration->gearControllerMode) {
			initGearController();
		}
		engine->gearController->update();
	}
#endif // EFI_TCU

	tryResetWatchdog();
}

void initPeriodicEvents() {
	slowController.start();
	fastController.start();
}

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer) {
#if HAL_USE_ADC
	if (!isAdcChannelValid(hwChannel)) {
		strcpy(buffer, "NONE");
	} else {
		strcpy(buffer, portname(getAdcChannelPort(msg, hwChannel)));
		itoa10(&buffer[2], getAdcChannelPin(hwChannel));
	}
#else
	strcpy(buffer, "NONE");
#endif /* HAL_USE_ADC */
	return buffer;
}

#if HAL_USE_ADC
extern AdcDevice fastAdc;
#endif /* HAL_USE_ADC */

#if EFI_PROD_CODE
static void printSensorInfo() {
#if HAL_USE_ADC
	// Print info about analog mappings
	AdcSubscription::PrintInfo();
#endif // HAL_USE_ADC

	// Print info about all sensors
	Sensor::showAllSensorInfo();
}
#endif // EFI_PROD_CODE

#define isOutOfBounds(offset) ((offset<0) || (offset) >= (int) sizeof(engine_configuration_s))

static void getShort(int offset) {
	if (isOutOfBounds(offset))
		return;
	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
	uint16_t value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	efiPrintf("short%s%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void getByte(int offset) {
	if (isOutOfBounds(offset))
		return;
	uint8_t *ptr = (uint8_t *) (&((char *) engineConfiguration)[offset]);
	uint8_t value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	efiPrintf("byte%s%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void setBit(const char *offsetStr, const char *bitStr, const char *valueStr) {
	int offset = atoi(offsetStr);
	if (absI(offset) == absI(ATOI_ERROR_CODE)) {
		efiPrintf("invalid offset [%s]", offsetStr);
		return;
	}
	if (isOutOfBounds(offset)) {
		return;
	}
	int bit = atoi(bitStr);
	if (absI(bit) == absI(ATOI_ERROR_CODE)) {
		efiPrintf("invalid bit [%s]", bitStr);
		return;
	}
	int value = atoi(valueStr);
	if (absI(value) == absI(ATOI_ERROR_CODE)) {
		efiPrintf("invalid value [%s]", valueStr);
		return;
	}
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	*ptr ^= (-value ^ *ptr) & (1 << bit);
	/**
	 * this response is part of rusEfi console API
	 */
	efiPrintf("bit%s%d/%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, bit, value);
	incrementGlobalConfigurationVersion("setBit");
}

static void setShort(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
	*ptr = (uint16_t) value;
	getShort(offset);
	incrementGlobalConfigurationVersion("setShort");
}

static void setByte(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	uint8_t *ptr = (uint8_t *) (&((char *) engineConfiguration)[offset]);
	*ptr = (uint8_t) value;
	getByte(offset);
	incrementGlobalConfigurationVersion("setByte");
}

static void getBit(int offset, int bit) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	int value = (*ptr >> bit) & 1;
	/**
	 * this response is part of rusEfi console API
	 */
	efiPrintf("bit%s%d/%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, bit, value);
}

static void getInt(int offset) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	int value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	efiPrintf("int%s%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void setInt(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	*ptr = value;
	getInt(offset);
	incrementGlobalConfigurationVersion("setInt");
}

static void getFloat(int offset) {
	if (isOutOfBounds(offset))
		return;
	float *ptr = (float *) (&((char *) engineConfiguration)[offset]);
	float value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	efiPrintf("float%s%d is %.5f", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void setFloat(const char *offsetStr, const char *valueStr) {
	int offset = atoi(offsetStr);
	if (absI(offset) == absI(ATOI_ERROR_CODE)) {
		efiPrintf("invalid offset [%s]", offsetStr);
		return;
	}
	if (isOutOfBounds(offset))
		return;
	float value = atoff(valueStr);
	if (cisnan(value)) {
		efiPrintf("invalid value [%s]", valueStr);
		return;
	}
	float *ptr = (float *) (&((char *) engineConfiguration)[offset]);
	*ptr = value;
	getFloat(offset);
	incrementGlobalConfigurationVersion("setFloat");
}

static void initConfigActions() {
	addConsoleActionSS("set_float", (VoidCharPtrCharPtr) setFloat);
	addConsoleActionII("set_int", (VoidIntInt) setInt);
	addConsoleActionII("set_short", (VoidIntInt) setShort);
	addConsoleActionII("set_byte", (VoidIntInt) setByte);
	addConsoleActionSSS("set_bit", setBit);

	addConsoleActionI("get_float", getFloat);
	addConsoleActionI("get_int", getInt);
	addConsoleActionI("get_short", getShort);
	addConsoleActionI("get_byte", getByte);
	addConsoleActionII("get_bit", getBit);
}
#endif /* EFI_UNIT_TEST */

// one-time start-up
// this method is used by real firmware and simulator and unit test
void commonInitEngineController() {
#if EFI_PROD_CODE
	addConsoleAction("sensorinfo", printSensorInfo);
	addConsoleAction("reset_accel", resetAccel);
#endif /* EFI_PROD_CODE */

#if EFI_SIMULATOR || EFI_UNIT_TEST
	printf("commonInitEngineController\n");
#endif

#if !EFI_UNIT_TEST
	initConfigActions();
#endif /* EFI_UNIT_TEST */

#if EFI_ENGINE_CONTROL
	/**
	 * This has to go after 'enginePins.startPins()' in order to
	 * properly detect un-assigned output pins
	 */
	prepareOutputSignals();

	engine->injectionEvents.addFuelEvents();
#endif // EFI_ENGINE_CONTROL

#if EFI_SENSOR_CHART
	initSensorChart();
#endif /* EFI_SENSOR_CHART */

#if EFI_PROD_CODE || EFI_SIMULATOR
	initSettings();

	if (hasFirmwareError()) {
		return;
	}
#endif

#if ! EFI_UNIT_TEST && EFI_ENGINE_CONTROL
	initBenchTest();
#endif /* ! EFI_UNIT_TEST && EFI_ENGINE_CONTROL */

#if EFI_ALTERNATOR_CONTROL
	initAlternatorCtrl();
#endif /* EFI_ALTERNATOR_CONTROL */

#if EFI_VVT_PID
	initVvtActuators();
#endif /* EFI_VVT_PID */

#if EFI_MALFUNCTION_INDICATOR
	initMalfunctionIndicator();
#endif /* EFI_MALFUNCTION_INDICATOR */

#if !EFI_UNIT_TEST
	// This is tested independently - don't configure sensors for tests.
	// This lets us selectively mock them for each test.
	initNewSensors();
#endif /* EFI_UNIT_TEST */

	initSensors();

	initAccelEnrichment();

	initScriptImpl();

	initGpPwm();

#if EFI_IDLE_CONTROL
	startIdleThread();
#endif /* EFI_IDLE_CONTROL */

#if EFI_TCU
	initGearController();
#endif

	initButtonDebounce();

#if EFI_ELECTRONIC_THROTTLE_BODY
	initElectronicThrottle();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_MAP_AVERAGING
	if (engineConfiguration->isMapAveragingEnabled) {
		initMapAveraging();
	}
#endif /* EFI_MAP_AVERAGING */

#if EFI_BOOST_CONTROL
	initBoostCtrl();
#endif /* EFI_BOOST_CONTROL */

#if EFI_LAUNCH_CONTROL
	initLaunchControl();
#endif

  initIgnitionAdvanceControl();

#if EFI_UNIT_TEST
	engine->rpmCalculator.Register();
#endif /* EFI_UNIT_TEST */

#if (EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT) || EFI_SIMULATOR || EFI_UNIT_TEST
	initAuxValves();
#endif /* EFI_ENGINE_CONTROL */

	initTachometer();
	initSpeedometer();
}

PUBLIC_API_WEAK bool validateBoardConfig() {
  return true;
}

// Returns false if there's an obvious problem with the loaded configuration
bool validateConfigOnStartUpOrBurn() {
  if (!validateBoardConfig()) {
    return false;
  }
	if (engineConfiguration->cylindersCount > MAX_CYLINDER_COUNT) {
		criticalError("Invalid cylinder count: %d", engineConfiguration->cylindersCount);
		return false;
	}
	if (engineConfiguration->adcVcc > 5.0f || engineConfiguration->adcVcc < 1.0f) {
    criticalError("Invalid adcVcc: %f", engineConfiguration->adcVcc);
		return false;
	}
	if (engineConfiguration->mapExpAverageAlpha <= 0 || engineConfiguration->mapExpAverageAlpha > 1) {
	  engineConfiguration->mapExpAverageAlpha = 1;
	}

	ensureArrayIsAscending("Injector deadtime", engineConfiguration->injector.battLagCorrBins);

#if EFI_ENGINE_CONTROL
	// Fueling
	{
		ensureArrayIsAscending("VE load", config->veLoadBins);
		ensureArrayIsAscending("VE RPM", config->veRpmBins);

		ensureArrayIsAscending("Lambda/AFR load", config->lambdaLoadBins);
		ensureArrayIsAscending("Lambda/AFR RPM", config->lambdaRpmBins);

		ensureArrayIsAscending("Fuel CLT mult", config->cltFuelCorrBins);
		ensureArrayIsAscending("Fuel IAT mult", config->iatFuelCorrBins);

		ensureArrayIsAscending("Injection phase load", config->injPhaseLoadBins);
		ensureArrayIsAscending("Injection phase RPM", config->injPhaseRpmBins);

		ensureArrayIsAscendingOrDefault("Fuel Level Sensor", config->fuelLevelBins);
		ensureArrayIsAscendingOrDefault("Fuel Trim Rpm", config->fuelTrimRpmBins);
		ensureArrayIsAscendingOrDefault("Fuel Trim Load", config->fuelTrimLoadBins);

		ensureArrayIsAscendingOrDefault("TC slip", engineConfiguration->tractionControlSlipBins);
		ensureArrayIsAscendingOrDefault("TC speed", engineConfiguration->tractionControlSpeedBins);

		ensureArrayIsAscending("TPS/TPS AE from", config->tpsTpsAccelFromRpmBins);
		ensureArrayIsAscending("TPS/TPS AE to", config->tpsTpsAccelToRpmBins);

		ensureArrayIsAscendingOrDefault("TPS TPS RPM correction", config->tpsTspCorrValuesBins);

		ensureArrayIsAscendingOrDefault("Staging Load", config->injectorStagingLoadBins);
		ensureArrayIsAscendingOrDefault("Staging RPM", config->injectorStagingRpmBins);
	}

	// Ignition
	{
		ensureArrayIsAscending("Dwell RPM", config->sparkDwellRpmBins);

		ensureArrayIsAscending("Ignition load", config->ignitionLoadBins);
		ensureArrayIsAscending("Ignition RPM", config->ignitionRpmBins);
		ensureArrayIsAscendingOrDefault("Ign Trim Rpm", config->ignTrimRpmBins);
   		ensureArrayIsAscendingOrDefault("Ign Trim Load", config->ignTrimLoadBins);

		ensureArrayIsAscending("Ignition CLT corr", config->cltTimingBins);

		ensureArrayIsAscending("Ignition IAT corr IAT", config->ignitionIatCorrTempBins);
		ensureArrayIsAscending("Ignition IAT corr Load", config->ignitionIatCorrLoadBins);
	}

	ensureArrayIsAscendingOrDefault("Map estimate TPS", config->mapEstimateTpsBins);
	ensureArrayIsAscendingOrDefault("Map estimate RPM", config->mapEstimateRpmBins);
#endif // EFI_ENGINE_CONTROL

	ensureArrayIsAscendingOrDefault("Script Curve 1", config->scriptCurve1Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 2", config->scriptCurve2Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 3", config->scriptCurve3Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 4", config->scriptCurve4Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 5", config->scriptCurve5Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 6", config->scriptCurve6Bins);

// todo: huh? why does this not work on CI?	ensureArrayIsAscendingOrDefault("Dwell Correction Voltage", engineConfiguration->dwellVoltageCorrVoltBins);

	ensureArrayIsAscending("MAF transfer function", config->mafDecodingBins);

	// Cranking tables
	ensureArrayIsAscending("Cranking fuel mult", config->crankingFuelBins);
	ensureArrayIsAscending("Cranking duration", config->crankingCycleBins);
	ensureArrayIsAscending("Cranking Fuel CLT", config->crankingCycleFuelCltBins);
	ensureArrayIsAscending("Cranking TPS", config->crankingTpsBins);

	// Idle tables
	ensureArrayIsAscending("Idle target RPM", config->cltIdleRpmBins);
	ensureArrayIsAscending("Idle warmup mult", config->cltIdleCorrBins);
	ensureArrayIsAscendingOrDefault("Idle coasting RPM", config->iacCoastingRpmBins);
	ensureArrayIsAscendingOrDefault("Idle VE RPM", config->idleVeRpmBins);
	ensureArrayIsAscendingOrDefault("Idle VE Load", config->idleVeLoadBins);
	ensureArrayIsAscendingOrDefault("Idle timing", config->idleAdvanceBins);

	for (size_t index = 0; index < efi::size(engineConfiguration->vrThreshold); index++) {
		auto& cfg = engineConfiguration->vrThreshold[index];

		if (cfg.pin == Gpio::Unassigned) {
			continue;
		}
		ensureArrayIsAscending("VR threshold", cfg.rpmBins);
	}

#if EFI_BOOST_CONTROL
	// Boost
	ensureArrayIsAscending("Boost control TPS", config->boostTpsBins);
	ensureArrayIsAscending("Boost control RPM", config->boostRpmBins);
#endif // EFI_BOOST_CONTROL

#if EFI_ANTILAG_SYSTEM
	// ALS
	ensureArrayIsAscendingOrDefault("ign ALS TPS", config->alsIgnRetardLoadBins);
	ensureArrayIsAscendingOrDefault("ign ALS RPM", config->alsIgnRetardrpmBins);
	ensureArrayIsAscendingOrDefault("fuel ALS TPS", config->alsFuelAdjustmentLoadBins);
	ensureArrayIsAscendingOrDefault("fuel ALS RPM", config->alsFuelAdjustmentrpmBins);
#endif // EFI_ANTILAG_SYSTEM

#if EFI_ELECTRONIC_THROTTLE_BODY
	// ETB
	ensureArrayIsAscending("Pedal map pedal", config->pedalToTpsPedalBins);
	ensureArrayIsAscending("Pedal map RPM", config->pedalToTpsRpmBins);
#endif // EFI_ELECTRONIC_THROTTLE_BODY

	if (isGdiEngine()) {
		ensureArrayIsAscending("HPFP compensation", config->hpfpCompensationRpmBins);
		ensureArrayIsAscending("HPFP deadtime", config->hpfpDeadtimeVoltsBins);
		ensureArrayIsAscending("HPFP lobe profile", config->hpfpLobeProfileQuantityBins);
		ensureArrayIsAscending("HPFP target rpm", config->hpfpTargetRpmBins);
		ensureArrayIsAscending("HPFP target load", config->hpfpTargetLoadBins);
	}

	// VVT
	if (isBrainPinValid(engineConfiguration->camInputs[0])) {
		ensureArrayIsAscending("VVT intake load", config->vvtTable1LoadBins);
		ensureArrayIsAscending("VVT intake RPM", config->vvtTable1RpmBins);
	}

#if CAM_INPUTS_COUNT != 1
	if (isBrainPinValid(engineConfiguration->camInputs[1])) {
		ensureArrayIsAscending("VVT exhaust load", config->vvtTable2LoadBins);
		ensureArrayIsAscending("VVT exhaust RPM", config->vvtTable2RpmBins);
	}
#endif

	return true;
}

#if !EFI_UNIT_TEST

void commonEarlyInit() {
	// Start this early - it will start LED blinking and such
	startStatusThreads();

#if EFI_SHAFT_POSITION_INPUT
	// todo: figure out better startup logic
	initTriggerCentral();
#endif /* EFI_SHAFT_POSITION_INPUT */

	/**
	 * Initialize hardware drivers
	 */
	initHardware();

	initQcBenchControls();

#if EFI_FILE_LOGGING
	initMmcCard();
#endif /* EFI_FILE_LOGGING */

#if EFI_ENGINE_EMULATOR
	initEngineEmulator();
#endif

#if EFI_LUA
	startLua();
#endif // EFI_LUA

#if EFI_CAN_SERIAL
	// needs to be called after initCan() inside initHardware()
	startCanConsole();
#endif /* EFI_CAN_SERIAL */

#if HW_CHECK_ALWAYS_STIMULATE
	// we need a special binary for final assembly check. We cannot afford to require too much software or too many steps
	// to be executed at the place of assembly
	enableTriggerStimulator(/*incGlobalConfiguration*/false);
#endif // HW_CHECK_ALWAYS_STIMULATE
}

// one-time start-up
void initRealHardwareEngineController() {
	commonInitEngineController();
	initWarningRunningPins();

#if EFI_LOGIC_ANALYZER
	if (engineConfiguration->isWaveAnalyzerEnabled) {
		initWaveAnalyzer();
	}
#endif /* EFI_LOGIC_ANALYZER */

	if (hasFirmwareError()) {
		return;
	}

	engineStateBlinkingTask.start();

	initVrThresholdPwm();
}

/**
 * these two variables are here only to let us know how much RAM is available, also these
 * help to notice when RAM usage goes up - if a code change adds to RAM usage these variables would fail
 * linking process which is the way to raise the alarm
 *
 * You get "cannot move location counter backwards" linker error when you run out of RAM. When you run out of RAM you shall reduce these
 * UNUSED_SIZE constants.
 */
#ifndef RAM_UNUSED_SIZE
#define RAM_UNUSED_SIZE 17400
#endif
#ifndef CCM_UNUSED_SIZE
#define CCM_UNUSED_SIZE 512
#endif
static volatile char UNUSED_RAM_SIZE[RAM_UNUSED_SIZE];
static volatile char UNUSED_CCM_SIZE[CCM_UNUSED_SIZE] CCM_OPTIONAL;

/**
 * See also SIGNATURE_HASH
 */
int getRusEfiVersion(void) {
	if (UNUSED_RAM_SIZE[0] != 0)
		return 123; // this is here to make the compiler happy about the unused array
	if (UNUSED_CCM_SIZE[0] * 0 != 0)
		return 3211; // this is here to make the compiler happy about the unused array
#if defined(EFI_BOOTLOADER_INCLUDE_CODE)
	// make bootloader code happy too
	if (initBootloader() != 0)
		return 123;
#endif /* EFI_BOOTLOADER_INCLUDE_CODE */
	return VCS_DATE;
}
#endif /* EFI_UNIT_TEST */
