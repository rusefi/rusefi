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
#include "main_trigger_callback.h"
#include "flash_main.h"
#include "bench_test.h"
#include "mmc_card.h"
#include "electronic_throttle.h"
#include "trigger_emulator_algo.h"
#include "high_pressure_fuel_pump.h"
#include "malfunction_central.h"
#include "speed_density.h"
#include "local_version_holder.h"
#include "alternator_controller.h"
#include "can_bench_test.h"
#include "engine_emulator.h"
#include "fuel_math.h"
#include "defaults.h"
#include "spark_logic.h"
#include "status_loop.h"
#include "aux_valves.h"
#include "accelerometer.h"
#include "vvt.h"
#include "boost_control.h"
#include "launch_control.h"
#include "speedometer.h"
#include "gppwm.h"
#include "date_stamp.h"
#include "rusefi_lua.h"
#include "lua_heap.h"
#include "buttonshift.h"
#include "start_stop.h"
#include "dynoview.h"
#include "vr_pwm.h"
#include "adc_subscription.h"
#include "gc_generic.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif /* EFI_TUNER_STUDIO */

#if EFI_LOGIC_ANALYZER
#include "logic_analyzer.h"
#endif /* EFI_LOGIC_ANALYZER */

#include "periodic_task.h"
#include "board_overrides.h"

#ifdef MODULE_MAP_AVERAGING
#include "map_averaging.h"
#endif

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

using namespace rusefi::stringutil;

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
		return static_cast<int>(FAST_CALLBACK_PERIOD_MS);
	}
};

class PeriodicSlowController : public PeriodicTimerController {
	void PeriodicTask() override {
		doPeriodicSlowCallback();
	}

	int getPeriodMs() override {
		// no reason to have this configurable, looks like everyone is happy with 20Hz
		return static_cast<int>(SLOW_CALLBACK_PERIOD_MS);
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
	engine->module<TpsAccelEnrichment>()->resetAE();

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
#endif /* EFI_SHAFT_POSITION_INPUT */

	engine->periodicSlowCallback();

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

#if EFI_PROD_CODE
	// single-shot reset all counter after 5 second of happines
	static unsigned int slow_counter = static_cast<unsigned int>(5 * 1000 / SLOW_CALLBACK_PERIOD_MS);
	if ((slow_counter) && (--slow_counter == 0)) {
		errorHandlerResetCounters();
	}
#endif // EFI_PROD_CODE
}

void initPeriodicEvents() {
	slowController.start();
	fastController.start();
}

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer, size_t bufferSize) {
#if HAL_USE_ADC
	if (!isAdcChannelValid(hwChannel)) {
		snprintf(buffer, bufferSize, "NONE");
	} else if (isAdcChannelOffChip(hwChannel)) {
		snprintf(buffer, bufferSize, "External ADC%d", hwChannel - EFI_ADC_0);
	} else {
		const char *name = portname(getAdcChannelPort(msg, hwChannel));
		snprintf(buffer, bufferSize, "%s%d", name ? name : "null", getAdcChannelPin(hwChannel));
	}
#else
  UNUSED(msg);
  UNUSED(hwChannel);
	snprintf(buffer, bufferSize, "NONE");
#endif /* HAL_USE_ADC */
	return buffer;
}

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
	if (std::isnan(value)) {
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

#if !EFI_UNIT_TEST
	// This is tested independently - don't configure sensors for tests.
	// This lets us selectively mock them for each test.
	initNewSensors();
#endif /* EFI_UNIT_TEST */

	initSensors();

	initAccelEnrichment();

	initScriptImpl();

	initGpPwm();

#ifdef MODULE_FUEL_PUMP
	initFuelPump();
#endif

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

#if EFI_MAP_AVERAGING && defined (MODULE_MAP_AVERAGING)
	engine->module<MapAveragingModule>()->init();
#else
	efiPrintf("No MapAveraging support!");
#endif /* EFI_MAP_AVERAGING */

#if EFI_BOOST_CONTROL
	initBoostCtrl();
#endif /* EFI_BOOST_CONTROL */

#if EFI_LAUNCH_CONTROL
	initLaunchControl();
#endif

#if EFI_ENGINE_CONTROL
  initIgnitionAdvanceControl();
#endif // EFI_ENGINE_CONTROL

#if EFI_UNIT_TEST
	// [tag:trigger_rpm_startup] General algorithm tests need RPM without a pin.
	// TriggerRpmStartup removes this registration before testing hardware startup.
	engine->rpmCalculator.Register();
#endif /* EFI_UNIT_TEST */

#if EFI_AUX_VALVES
	initAuxValves();
#endif /* EFI_AUX_VALVES */

#ifdef MODULE_TACHOMETER
	engine->module<TachometerModule>()->init();
#endif

	initSpeedometer();

#if EFI_ENGINE_CONTROL
	initStft();
#endif // EFI_ENGINE_CONTROL
#if EFI_LTFT_CONTROL
	initLtft();
#endif
}

bool validateBoardConfig() {
  // todo: remove placeholder June 2026
  return true;
}

static bool validateGdi() {
	auto lobes = engineConfiguration->hpfpCamLobes;
	if (!lobes) {
		return true;
	}
#if EFI_HPFP
	int expectedLastLobeProfileAngle = 360 / lobes;
  float actualLastAngle = config->hpfpLobeProfileAngle[efi::size(config->hpfpLobeProfileAngle) - 1];
	if (expectedLastLobeProfileAngle != actualLastAngle) {
		criticalError("Last HPFP angle expected %d got %f", expectedLastLobeProfileAngle, actualLastAngle);
		return false;
	}
#endif // EFI_HPFP
	return true;
}

struct AxisOrderValidation {
	bool valid = true;

	template<typename T, int N>
	void check(const char* name, const T (&axis)[N]) {
		valid = valid && ensureArrayIsAscending(name, axis);
	}

	template<typename T, int N>
	void checkOrDefault(const char* name, const T (&axis)[N]) {
		valid = valid && ensureArrayIsAscendingOrDefault(name, axis);
	}
};

/**
 * Validate the current tune at start-up and before a TunerStudio Burn. The
 * checks do not modify the tune; repairs belong in applyDefaultsOrFixAfterBurn().
 *
 * @param previousConfiguration nullptr at start-up, otherwise the last applied configuration.
 * @param applyFixes Run the defaults/repair step. Extra-page Burn only validates, so it
 * does not change the main settings page before the client's CRC check.
 * @return false if any check rejects the tune. Start-up then skips engine-controller
 * initialization; Burn neither applies hardware changes nor queues the save.
 *
 * Invalid scalar settings report a critical error and return immediately. Table
 * checks report the first broken axis and also reject the tune [tag:table_axis_order].
 * When a table is only used with a specific setting, guard its checks with that
 * setting (see the MAF curve) so unused data does not block the ECU.
 *
 * The result describes the current tune, not the latched fault state. A corrected
 * tune can pass and be saved; after a critical fault, reboot to resume engine control.
 */
static bool validateConfig(const engine_configuration_s* previousConfiguration, bool applyFixes = true) {
#if !EFI_ENGINE_CONTROL
  UNUSED(applyFixes);
#endif
  if (!get_board_override_result(custom_board_validateConfig, true, previousConfiguration)) {
    return false;
  }
#if defined(HW_HELLEN_UAEFI)
  // todo: make this board-specific validation callback!
  if (!pickEtbOrStepper()) {
    return false;
  }
#endif
  if (!validateGdi()) {
    return false;
  }
#if EFI_ELECTRONIC_THROTTLE_BODY
  if (engineConfiguration->etbMinimumPosition + 1 >= engineConfiguration->etbMaximumPosition) {
		criticalError("Broken ETB min/max %d %d",
		  engineConfiguration->etbMinimumPosition,
		  engineConfiguration->etbMaximumPosition);
		return false;
  }
#endif

  if (engineConfiguration->enableSoftwareKnock && engineConfiguration->knockFrequency != 0 && engineConfiguration->knockFrequency < 100) {
    // todo: migrate from hard error to soft error
		criticalError("knock frequency setting uses HZ not KHz: %f", engineConfiguration->knockFrequency);
		return false;
  }

#ifndef EFI_SOFTWARE_KNOCK
  if (engineConfiguration->enableSoftwareKnock) {
    // [tag:user_facing_ini_labels] Match the Knock sensing control in TunerStudio.
    criticalError("Knock sensing is enabled but this firmware does not support software knock");
    return false;
  }
#endif // EFI_SOFTWARE_KNOCK

#if EFI_ENGINE_CONTROL
  if (applyFixes) {
    applyDefaultsOrFixAfterBurn(previousConfiguration);
  }
	if (!(engineConfiguration->injector.flow > 0) || !std::isfinite(engineConfiguration->injector.flow)) {
		criticalError("Primary injector flow must be positive");
		return false;
	}
	if (engineConfiguration->enableStagedInjection &&
		(!(engineConfiguration->injectorSecondary.flow > 0) || !std::isfinite(engineConfiguration->injectorSecondary.flow))) {
		criticalError("Secondary injector flow must be positive when staged injection is enabled");
		return false;
	}
	if (engineConfiguration->cylindersCount > MAX_CYLINDER_COUNT) {
		// [tag:user_facing_ini_labels] Match the engine setting label in tunerstudio.template.ini.
		criticalError("Invalid Number of cylinders: %d", engineConfiguration->cylindersCount);
		return false;
	}
#endif // EFI_ENGINE_CONTROL

#if EFI_PROD_CODE && (BOARD_MC33810_COUNT > 0)
    float maxConfiguredCorr = config->dwellVoltageCorrValues[0];
    for (size_t i = 0;i<efi::size(config->dwellVoltageCorrValues);i++) {
        maxConfiguredCorr = std::max(maxConfiguredCorr, (float)config->dwellVoltageCorrValues[i]);
    }
    float maxConfiguredDwell = config->sparkDwellValues[0];
    for (size_t i = 0;i<efi::size(config->sparkDwellValues);i++) {
        maxConfiguredDwell = std::max(maxConfiguredDwell, (float)config->sparkDwellValues[i]);
    }
    int maxAllowedDwell = getMc33810maxDwellTimer(engineConfiguration->mc33810maxDwellTimer);
        if (maxConfiguredCorr * maxConfiguredDwell > maxAllowedDwell) {
            criticalError("Dwell=%.2f/corr=%.2f while 33810 limit %d", maxConfiguredDwell, maxConfiguredCorr, maxAllowedDwell);
            return false;
        }

#endif // EFI_PROD_CODE && (BOARD_MC33810_COUNT > 0)
	if (engineConfiguration->adcVcc > 5.0f || engineConfiguration->adcVcc < 1.0f) {
		// [tag:user_facing_ini_labels] Match the hardware setting label in tunerstudio.template.ini.
    criticalError("Invalid ADC vRef voltage: %f", engineConfiguration->adcVcc);
		return false;
	}

	// [tag:table_axis_order] Report the first problem and refuse the tune on boot or Burn.
	AxisOrderValidation axes;
#if EFI_ENGINE_CONTROL
	axes.check("Injector deadtime vBATT", engineConfiguration->injector.battLagCorrBattBins);
	axes.check("Injector deadtime Pressure", engineConfiguration->injector.battLagCorrPressBins);

	// Fueling
	{
		axes.check("VE load", config->veLoadBins);
		axes.check("VE RPM", config->veRpmBins);

		axes.check("Second VE load", secondTablesGetState()->secondVeLoadBins);
		axes.check("Second VE RPM", secondTablesGetState()->secondVeRpmBins);

		axes.check("Lambda/AFR load", config->lambdaLoadBins);
		axes.check("Lambda/AFR RPM", config->lambdaRpmBins);

		axes.check("Fuel CLT mult", config->cltFuelCorrBins);
		axes.check("Fuel IAT mult", config->iatFuelCorrBins);

		axes.checkOrDefault("Boost CLT mult", config->cltBoostCorrBins);
		axes.checkOrDefault("Boost IAT mult", config->iatBoostCorrBins);

		axes.check("Injection phase load", config->injPhaseLoadBins);
		axes.check("Injection phase RPM", config->injPhaseRpmBins);

		axes.checkOrDefault("Fuel Level Sensor", config->fuelLevelBins);

		axes.checkOrDefault("STFT Rpm", config->fuelTrimRpmBins);
		axes.checkOrDefault("STFT Load", config->fuelTrimLoadBins);

		axes.checkOrDefault("knock rpm", config->maxKnockRetardRpmBins);
		axes.checkOrDefault("knock tps", config->maxKnockRetardLoadBins);

		axes.checkOrDefault("TC slip", engineConfiguration->tractionControlSlipBins);
		axes.checkOrDefault("TC speed", engineConfiguration->tractionControlSpeedBins);

		axes.check("TPS/TPS AE from", config->tpsTpsAccelFromRpmBins);
		axes.check("TPS/TPS AE to", config->tpsTpsAccelToRpmBins);

		axes.checkOrDefault("TPS TPS RPM correction", config->tpsTspCorrValuesBins);

		axes.checkOrDefault("Staging Load", config->injectorStagingLoadBins);
		axes.checkOrDefault("Staging RPM", config->injectorStagingRpmBins);
	}

	// Ignition
	{
		axes.check("Dwell RPM", config->sparkDwellRpmBins);

		axes.check("Ignition load", config->ignitionLoadBins);
		axes.check("Ignition RPM", config->ignitionRpmBins);

		axes.check("Second Ignition load", secondTablesGetState()->secondIgnitionLoadBins);
		axes.check("Second Ignition RPM", secondTablesGetState()->secondIgnitionRpmBins);
		axes.checkOrDefault("Ign Trim Rpm", config->ignTrimRpmBins);
		axes.checkOrDefault("Ign Trim Load", config->ignTrimLoadBins);

		axes.check("Ignition CLT corr CLT", config->ignitionCltCorrTempBins);
		axes.check("Ignition CLT corr Load", config->ignitionCltCorrLoadBins);

		axes.check("Ignition IAT corr IAT", config->ignitionIatCorrTempBins);
		axes.check("Ignition IAT corr Load", config->ignitionIatCorrLoadBins);
	}

	axes.checkOrDefault("Map estimate TPS", config->mapEstimateTpsBins);
	axes.checkOrDefault("Map estimate RPM", config->mapEstimateRpmBins);
#endif // EFI_ENGINE_CONTROL

	axes.checkOrDefault("Script Curve 1", config->scriptCurve1Bins);
	axes.checkOrDefault("Script Curve 2", config->scriptCurve2Bins);
	axes.checkOrDefault("Script Curve 3", config->scriptCurve3Bins);
	axes.checkOrDefault("Script Curve 4", config->scriptCurve4Bins);
	axes.checkOrDefault("Script Curve 5", config->scriptCurve5Bins);
	axes.checkOrDefault("Script Curve 6", config->scriptCurve6Bins);

// todo: huh? why does this not work on CI?	axes.checkOrDefault("Dwell Correction Voltage", engineConfiguration->dwellVoltageCorrVoltBins);

#if EFI_ENGINE_CONTROL
	// Both analog MAF inputs use this curve, regardless of the fuel algorithm.
	if (isAdcChannelValid(engineConfiguration->mafAdcChannel) || isAdcChannelValid(engineConfiguration->maf2AdcChannel)) {
		axes.check("MAF transfer function", config->mafDecodingBins);
	}

	// Cranking tables
	axes.check("Cranking fuel mult", config->crankingFuelBins);
	axes.check("Cranking duration", config->crankingCycleBins);
	axes.check("Cranking Fuel CLT", config->crankingCycleFuelCltBins);
	axes.check("Cranking TPS", config->crankingTpsBins);

	// Idle tables
	axes.check("Idle target RPM", config->cltIdleRpmBins);
	axes.check("Idle warmup mult CLT", config->cltIdleCorrBins);
	axes.check("Idle warmup mult RPM", config->rpmIdleCorrBins);
#endif // EFI_ENGINE_CONTROL
	axes.checkOrDefault("Idle coasting RPM", config->iacCoastingRpmBins);
	axes.checkOrDefault("Idle VE RPM", config->idleVeRpmBins);
	axes.checkOrDefault("Idle VE Load", config->idleVeLoadBins);
	axes.checkOrDefault("Idle timing", config->idleAdvanceBins);

	for (size_t index = 0; index < efi::size(engineConfiguration->vrThreshold); index++) {
		auto& cfg = engineConfiguration->vrThreshold[index];

		if (cfg.pin == Gpio::Unassigned) {
			continue;
		}
		axes.check("VR threshold", cfg.rpmBins);
	}

#if EFI_BOOST_CONTROL
	// Boost
	axes.check("Boost control Load [open loop]", config->boostOpenLoopLoadBins);
	axes.check("Boost control Load [closed loop]", config->boostClosedLoopLoadBins);
	axes.check("Boost control RPM [open+closed loop]", config->boostRpmBins);
#endif // EFI_BOOST_CONTROL

#if EFI_ANTILAG_SYSTEM
	// ALS
	axes.checkOrDefault("ign ALS TPS", config->alsIgnRetardLoadBins);
	axes.checkOrDefault("ign ALS RPM", config->alsIgnRetardrpmBins);
	axes.checkOrDefault("fuel ALS TPS", config->alsFuelAdjustmentLoadBins);
	axes.checkOrDefault("fuel ALS RPM", config->alsFuelAdjustmentrpmBins);
#endif // EFI_ANTILAG_SYSTEM

#if EFI_ELECTRONIC_THROTTLE_BODY
	// ETB
	axes.check("Pedal map pedal", config->pedalToTpsPedalBins);
	axes.check("Pedal map RPM", config->pedalToTpsRpmBins);
#endif // EFI_ELECTRONIC_THROTTLE_BODY

	if (isGdiEngine()) {
		axes.check("HPFP compensation", config->hpfpCompensationRpmBins);
		axes.check("HPFP deadtime", config->hpfpDeadtimeVoltsBins);
		axes.check("HPFP lobe profile", config->hpfpLobeProfileQuantityBins);
		axes.check("HPFP target rpm", config->hpfpTargetRpmBins);
		axes.check("HPFP target load", config->hpfpTargetLoadBins);

		axes.check("HPFP fuel mass compensation fuel pressure", config->hpfpFuelMassCompensationFuelPressure);
		axes.check("HPFP fuel mass compensation fuel mass", config->hpfpFuelMassCompensationFuelMass);

	}

	// VVT
	if (isBrainPinValid(engineConfiguration->camInputs[0])) {
		axes.check("VVT intake load", config->vvtTable1LoadBins);
		axes.check("VVT intake RPM", config->vvtTable1RpmBins);
	}

#if CAM_INPUTS_COUNT != 1
	if (isBrainPinValid(engineConfiguration->camInputs[1])) {
		axes.check("VVT exhaust load", config->vvtTable2LoadBins);
		axes.check("VVT exhaust RPM", config->vvtTable2RpmBins);
	}
#endif

	if (engineConfiguration->enableOilPressureProtect) {
		axes.check("Oil pressure protection", config->minimumOilPressureBins);
	}


	return axes.valid;
}

bool validateConfigOnStartUpOrBurn() {
	// start-up: activeConfiguration is still void-prepared, there is no meaningful previous configuration
	return validateConfig(nullptr);
}

bool validateConfigOnStartUpOrBurn(bool isRunningOnBurn) {
	ConfigurationWizard::onConfigOnStartUpOrBurn(isRunningOnBurn);
	// burn: activeConfiguration still holds the configuration the ECU is currently running on
	return validateConfig(&activeConfiguration);
}

bool validateConfigForSave() {
	return validateConfig(&activeConfiguration, false);
}

#if !EFI_UNIT_TEST

void commonEarlyInit() {
	// note: LED blinking is started even earlier, see startStatusThreads() call in runRusEfi()

#if EFI_SHAFT_POSITION_INPUT
	// todo: figure out better startup logic
	initTriggerCentral();
#endif /* EFI_SHAFT_POSITION_INPUT */

	/**
	 * Initialize hardware drivers
	 */
	initHardware();

#if EFI_PROD_CODE
	// Hardware is initialized: now latch the critical error for a watchdog reset, if there was one.
	// Doing it before initHardware() would leave SPI/pins unconfigured and initMmc() would refuse
	// the card, so the SD report file could never be written.
	errorHandlerRaiseWatchdogResetError();
#endif // EFI_PROD_CODE

	initQcBenchControls();

#if EFI_FILE_LOGGING
	initMmcCard();
#endif /* EFI_FILE_LOGGING */

#if EFI_ENGINE_EMULATOR
	initEngineEmulator();
#endif

#if EFI_LUA
	luaHeapInit();
	startLua();
#endif // EFI_LUA

#if EFI_CAN_SERIAL
	// needs to be called after initCan() inside initHardware()
	startCanConsole();
#endif /* EFI_CAN_SERIAL */

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
 * See also SIGNATURE_HASH
 */
int getRusEfiVersion() {
	return VCS_DATE;
}
#endif /* EFI_UNIT_TEST */

std::optional<custom_validate_config_type> custom_board_validateConfig;
std::optional<custom_fix_configuration_type> custom_board_fix_configuration;
