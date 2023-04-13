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
#include "advance_map.h"
#include "main_trigger_callback.h"
#include "flash_main.h"
#include "bench_test.h"
#include "electronic_throttle.h"
#include "map_averaging.h"
#include "high_pressure_fuel_pump.h"
#include "malfunction_central.h"
#include "malfunction_indicator.h"
#include "speed_density.h"
#include "local_version_holder.h"
#include "alternator_controller.h"
#include "fuel_math.h"
#include "spark_logic.h"
#include "accelerometer.h"
#include "vvt.h"
#include "boost_control.h"
#include "launch_control.h"
#include "tachometer.h"
#include "gppwm.h"
#include "date_stamp.h"
#include "buttonshift.h"
#include "start_stop.h"
#include "dynoview.h"
#include "vr_pwm.h"
#include "adc_subscription.h"

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
#include "AdcConfiguration.h"
#endif /* HAL_USE_ADC */

#if defined(EFI_BOOTLOADER_INCLUDE_CODE)
#include "bootloader/bootloader.h"
#endif /* EFI_BOOTLOADER_INCLUDE_CODE */

#include "periodic_task.h"


#if ! EFI_UNIT_TEST
#include "init.h"
#endif /* EFI_UNIT_TEST */

#if EFI_PROD_CODE
#include "pwm_tester.h"
#endif /* EFI_PROD_CODE */

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
			int is_cranking = engine->rpmCalculator.isCranking();
			enginePins.runningLedPin.setValue(is_cranking);
		}
	}
};

static EngineStateBlinkingTask engineStateBlinkingTask;

static void resetAccel() {
	engine->tpsAccelEnrichment.resetAE();

	for (size_t i = 0; i < efi::size(engine->injectionEvents.elements); i++)
	{
		engine->injectionEvents.elements[i].wallFuel.resetWF();
	}
}

static void doPeriodicSlowCallback() {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	efiAssertVoid(ObdCode::CUSTOM_ERR_6661, getCurrentRemainingStack() > 64, "lowStckOnEv");

	slowStartStopButtonCallback();

	engine->rpmCalculator.onSlowCallback();

	if (engine->triggerCentral.directSelfStimulation || engine->rpmCalculator.isStopped()) {
		/**
		 * rusEfi usually runs on hardware which halts execution while writing to internal flash, so we
		 * postpone writes to until engine is stopped. Writes in case of self-stimulation are fine.
		 *
		 * todo: allow writing if 2nd bank of flash is used
		 */
#if EFI_INTERNAL_FLASH
		writeToFlashIfPending();
#endif /* EFI_INTERNAL_FLASH */
	}

	if (engine->rpmCalculator.isStopped()) {
		resetAccel();
	}

	if (engine->versionForConfigurationListeners.isOld(engine->getGlobalConfigurationVersion())) {
		updateAccelParameters();
	}

	engine->periodicSlowCallback();
#endif /* if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT */

#if EFI_TCU
	if (engineConfiguration->tcuEnabled && engineConfiguration->gearControllerMode != GearControllerMode::None) {
		if (engine->gearController == NULL) {
			initGearController();
		} else if (engine->gearController->getMode() != engineConfiguration->gearControllerMode) {
			initGearController();
		}
		engine->gearController->update();
	}
#endif

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

static void printSensorInfo() {
#if HAL_USE_ADC
	// Print info about analog mappings
	AdcSubscription::PrintInfo();
#endif // HAL_USE_ADC

	// Print info about all sensors
	Sensor::showAllSensorInfo();
}

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
	incrementGlobalConfigurationVersion();
}

static void setShort(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
	*ptr = (uint16_t) value;
	getShort(offset);
	incrementGlobalConfigurationVersion();
}

static void setByte(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	uint8_t *ptr = (uint8_t *) (&((char *) engineConfiguration)[offset]);
	*ptr = (uint8_t) value;
	getByte(offset);
	incrementGlobalConfigurationVersion();
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
	incrementGlobalConfigurationVersion();
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
	incrementGlobalConfigurationVersion();
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

// this method is used by real firmware and simulator and unit test
void commonInitEngineController() {
	initInterpolation();

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
	initStartStopButton();

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

#if EFI_UNIT_TEST
	engine->rpmCalculator.Register();
#endif /* EFI_UNIT_TEST */

	initTachometer();
}

// Returns false if there's an obvious problem with the loaded configuration
bool validateConfig() {
	if (engineConfiguration->cylindersCount > MAX_CYLINDER_COUNT) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Invalid cylinder count: %d", engineConfiguration->cylindersCount);
		return false;
	}

	ensureArrayIsAscending("Batt Lag", engineConfiguration->injector.battLagCorrBins);

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

		ensureArrayIsAscending("TPS/TPS AE from", config->tpsTpsAccelFromRpmBins);
		ensureArrayIsAscending("TPS/TPS AE to", config->tpsTpsAccelToRpmBins);

		ensureArrayIsAscendingOrDefault("TPS TPS RPM correction", engineConfiguration->tpsTspCorrValuesBins);
	}

	// Ignition
	{
		ensureArrayIsAscending("Dwell RPM", config->sparkDwellRpmBins);

		ensureArrayIsAscending("Ignition load", config->ignitionLoadBins);
		ensureArrayIsAscending("Ignition RPM", config->ignitionRpmBins);

		ensureArrayIsAscending("Ignition CLT corr", config->cltTimingBins);

		ensureArrayIsAscending("Ignition IAT corr IAT", config->ignitionIatCorrTempBins);
		ensureArrayIsAscending("Ignition IAT corr Load", config->ignitionIatCorrLoadBins);
	}

	ensureArrayIsAscendingOrDefault("Map estimate TPS", config->mapEstimateTpsBins);
	ensureArrayIsAscendingOrDefault("Map estimate RPM", config->mapEstimateRpmBins);

	ensureArrayIsAscendingOrDefault("Script Curve 1", config->scriptCurve1Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 2", config->scriptCurve2Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 3", config->scriptCurve3Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 4", config->scriptCurve4Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 5", config->scriptCurve5Bins);
	ensureArrayIsAscendingOrDefault("Script Curve 6", config->scriptCurve6Bins);

// todo: huh? why does this not work on CI?	ensureArrayIsAscendingOrDefault("Dwell Correction Voltage", engineConfiguration->dwellVoltageCorrVoltBins);

	ensureArrayIsAscending("MAF decoding", config->mafDecodingBins);

	// Cranking tables
	ensureArrayIsAscending("Cranking fuel mult", config->crankingFuelBins);
	ensureArrayIsAscending("Cranking duration", config->crankingCycleBins);
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
		ensureArrayIsAscending("VR Bins", cfg.rpmBins);
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

	// ETB
	ensureArrayIsAscending("Pedal map pedal", config->pedalToTpsPedalBins);
	ensureArrayIsAscending("Pedal map RPM", config->pedalToTpsRpmBins);

	if (engineConfiguration->hpfpCamLobes > 0) {
		ensureArrayIsAscending("HPFP compensation", engineConfiguration->hpfpCompensationRpmBins);
		ensureArrayIsAscending("HPFP deadtime", engineConfiguration->hpfpDeadtimeVoltsBins);
		ensureArrayIsAscending("HPFP lobe profile", engineConfiguration->hpfpLobeProfileQuantityBins);
		ensureArrayIsAscending("HPFP target rpm", engineConfiguration->hpfpTargetRpmBins);
		ensureArrayIsAscending("HPFP target load", engineConfiguration->hpfpTargetLoadBins);
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

void initEngineController() {
	addConsoleAction("sensorinfo", printSensorInfo);

#if EFI_PROD_CODE && EFI_ENGINE_CONTROL
	initBenchTest();
#endif /* EFI_PROD_CODE && EFI_ENGINE_CONTROL */

	commonInitEngineController();

#if EFI_LOGIC_ANALYZER
	if (engineConfiguration->isWaveAnalyzerEnabled) {
		initWaveAnalyzer();
	}
#endif /* EFI_LOGIC_ANALYZER */

	if (hasFirmwareError()) {
		return;
	}

	engineStateBlinkingTask.start();

	initVrPwm();

#if EFI_PWM_TESTER
	initPwmTester();
#endif /* EFI_PWM_TESTER */

#if EFI_ALTERNATOR_CONTROL
	initAlternatorCtrl();
#endif /* EFI_ALTERNATOR_CONTROL */

#if EFI_AUX_PID
	initVvtActuators();
#endif /* EFI_AUX_PID */

#if EFI_MALFUNCTION_INDICATOR
	initMalfunctionIndicator();
#endif /* EFI_MALFUNCTION_INDICATOR */

#if EFI_PROD_CODE
	addConsoleAction("reset_accel", resetAccel);
#endif /* EFI_PROD_CODE */

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
#define RAM_UNUSED_SIZE 30000
#endif
#ifndef CCM_UNUSED_SIZE
#define CCM_UNUSED_SIZE 512
#endif
static char UNUSED_RAM_SIZE[RAM_UNUSED_SIZE];
static char UNUSED_CCM_SIZE[CCM_UNUSED_SIZE] CCM_OPTIONAL;

/**
 * See also GIT_HASH
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
