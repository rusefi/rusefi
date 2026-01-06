/**
 * @file	engine_configuration.cpp
 * @brief	Utility method related to the engine configuration data structure.
 *
 * @date Nov 22, 2013
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
 *
 */

#include "pch.h"
#include "transition_events.h"
#include "speed_density.h"
#include "flash_main.h"

#include "bench_test.h"

#if EFI_ONBOARD_MEMS
#include "accelerometer.h"
#endif // EFI_ONBOARD_MEMS

#include "defaults.h"

#include "custom_engine.h"

#include "boost_control.h"
#include "engine_configuration_defaults.h"
#if EFI_IDLE_CONTROL
#include "idle_thread.h"
#endif /* EFI_IDLE_CONTROL */

#if EFI_ALTERNATOR_CONTROL
#include "alternator_controller.h"
#endif

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif

#include "hardware.h"

#if EFI_PROD_CODE
#include "board.h"
#endif /* EFI_PROD_CODE */

#if EFI_EMULATE_POSITION_SENSORS
#include "trigger_emulator_algo.h"
#endif /* EFI_EMULATE_POSITION_SENSORS */

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

#include "board_overrides.h"

#define TS_DEFAULT_SPEED 38400

std::optional<setup_custom_board_overrides_type> custom_board_DefaultConfiguration;
std::optional<setup_custom_board_overrides_type> custom_board_ConfigOverrides;
std::optional<setup_custom_board_config_type> custom_board_OnConfigurationChange;

/**
 * Current engine configuration. On firmware start we assign empty configuration, then
 * we copy actual configuration after reading settings from flash.
 * This is useful to compare old/current (activeConfiguration) and new/future (engineConfiguration) configurations in order to apply new settings.
 *
 * todo: place this field next to 'engineConfiguration'?
 */
static bool hasRememberedConfiguration = false;
#if EFI_ACTIVE_CONFIGURATION_IN_FLASH
#include "flash_int.h"
engine_configuration_s & activeConfiguration = reinterpret_cast<persistent_config_container_s*>(getFlashAddrFirstCopy())->persistentConfiguration.engineConfiguration;
// we cannot use this activeConfiguration until we call rememberCurrentConfiguration()
bool isActiveConfigurationVoid = true;
#else
static engine_configuration_s activeConfigurationLocalStorage;
engine_configuration_s & activeConfiguration = activeConfigurationLocalStorage;
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */

void rememberCurrentConfiguration() {
#if ! EFI_ACTIVE_CONFIGURATION_IN_FLASH
	memcpy(&activeConfiguration, engineConfiguration, sizeof(engine_configuration_s));
#else
	isActiveConfigurationVoid = false;
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */
    hasRememberedConfiguration = true;
}

static void fillAfterString(char *string, int size) {
	// we have to reset bytes after \0 symbol in order to calculate correct tune CRC from MSQ file
	for (int i = std::strlen(string) + 1; i < size; i++) {
		string[i] = 0;
	}
}

static void wipeStrings() {
	fillAfterString(engineConfiguration->engineMake, sizeof(vehicle_info_t));
	fillAfterString(engineConfiguration->engineCode, sizeof(vehicle_info_t));
	fillAfterString(engineConfiguration->vehicleName, sizeof(vehicle_info_t));
	fillAfterString(engineConfiguration->vinNumber, sizeof(vin_number_t));
}

void onBurnRequest() {
  onTransitionEvent(TransitionEvent::BurnRequest);
	wipeStrings();

	incrementGlobalConfigurationVersion("burn");
}

/**
 * this hook is about https://wiki.rusefi.com/Custom-Firmware and https://wiki.rusefi.com/Canned-Tune-Process
 * todo: why two hooks? is one already dead?
 */
void boardBeforeTuneDefaults() {
  // placeholder
}

void boardOnConfigurationChange(engine_configuration_s* /*previousConfiguration*/) {
  // placeholder
}

/**
 * this is the top-level method which should be called in case of any changes to engine configuration
 * online tuning of most values in the maps does not count as configuration change, but 'Burn' command does
 *
 * this method is NOT currently invoked on ECU start - actual user input has to happen!
 * See 'preCalculate' or 'startHardware' which are invoked BOTH on start and configuration change
 */
void incrementGlobalConfigurationVersion(const char * msg) {
  onTransitionEvent(TransitionEvent::GlobalConfigurationVersion);
    assertStackVoid("increment", ObdCode::STACK_USAGE_MISC, EXPECTED_REMAINING_STACK);
    if (!hasRememberedConfiguration) {
        criticalError("too early to invoke incrementGlobalConfigurationVersion %s", msg);
    }
	engine->globalConfigurationVersion++;
#if EFI_DETAILED_LOGGING
	efiPrintf("set globalConfigurationVersion=%d", globalConfigurationVersion);
#endif /* EFI_DETAILED_LOGGING */

	applyNewHardwareSettings();

	call_board_override(custom_board_OnConfigurationChange, &activeConfiguration);

	engine->preCalculate();

#if EFI_ELECTRONIC_THROTTLE_BODY
	onConfigurationChangeElectronicThrottleCallback(&activeConfiguration);
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_ENGINE_CONTROL && EFI_PROD_CODE
	onConfigurationChangeBenchTest();
#endif

#if EFI_SHAFT_POSITION_INPUT
	onConfigurationChangeTriggerCallback();
#endif /* EFI_SHAFT_POSITION_INPUT */
#if EFI_EMULATE_POSITION_SENSORS && ! EFI_UNIT_TEST
	onConfigurationChangeRpmEmulatorCallback(&activeConfiguration);
#endif /* EFI_EMULATE_POSITION_SENSORS */

	engine->engineModules.apply_all([](auto & m) {
			m.onConfigurationChange(&activeConfiguration);
		});
	rememberCurrentConfiguration();
}

/**
 * @brief Sets the same dwell time across the whole getRpm() range
 * set dwell X
 */
void setConstantDwell(floatms_t dwellMs) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		config->sparkDwellRpmBins[i] = 1000 * i;
	}
	setArrayValues(config->sparkDwellValues, dwellMs);
}

void setFuelTablesLoadBin(float minValue, float maxValue) {
	setLinearCurve(config->injPhaseLoadBins, minValue, maxValue, 1);
	setLinearCurve(config->veLoadBins, minValue, maxValue, 1);
	setLinearCurve(config->lambdaLoadBins, minValue, maxValue, 1);
}

void setWholeIatCorrTimingTable(float value) {
	setTable(config->ignitionIatCorrTable, value);
}

/**
 * See also crankingTimingAngle
 */
void setWholeTimingTable(angle_t value) {
	setTable(config->ignitionTable, value);
}

#if EFI_ENGINE_CONTROL
namespace {
    void initTemperatureCurve(
        float * const bins,
        float * const values,
        const int size,
        const float defaultValue,
        const float initialTemperature = -40.0f,
        const float temperatureStep = 10.0f
    ) {
        for (int i = 0; i < size; i++) {
            bins[i] = initialTemperature + i * temperatureStep;
            values[i] = defaultValue; // this correction is a multiplier
        }
    }

    void initBoostTemperatureCurve(float* const bins, float* const values, const float defaultValue) {
        initTemperatureCurve(bins, values, BOOST_CURVE_SIZE, defaultValue, 20.0f, 20.0f);
    }
}
#endif // EFI_ENGINE_CONTROL

void prepareVoidConfiguration(engine_configuration_s *p_engineConfiguration) {
	criticalAssertVoid(p_engineConfiguration != nullptr, "ec NULL");
	efi::clear(p_engineConfiguration);

	p_engineConfiguration->clutchDownPinMode = PI_PULLUP;
	p_engineConfiguration->clutchUpPinMode = PI_PULLUP;
	p_engineConfiguration->brakePedalPinMode = PI_PULLUP;
}

void setDefaultBasePins() {
#if EFI_PROD_CODE
	// call overrided board-specific serial configuration setup, if needed (for custom boards only)
	// needed also by bootloader code
	setPinConfigurationOverrides();
#endif /* EFI_PROD_CODE */

	// set UART pads configuration based on the board
// needed also by bootloader code
#ifdef TS_SECONDARY_UxART_PORT
	engineConfiguration->binarySerialTxPin = Gpio::C10;
	engineConfiguration->binarySerialRxPin = Gpio::C11;
#endif // TS_SECONDARY_UxART_PORT

	engineConfiguration->tunerStudioSerialSpeed = TS_DEFAULT_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = 115200;
}

// needed also by bootloader code
// at the moment bootloader does NOT really need SD card, this is a step towards future bootloader SD card usage
void setDefaultSdCardParameters() {
	engineConfiguration->isSdCardEnabled = true;
}

#if EFI_ENGINE_CONTROL

/**
 * see also setTargetRpmCurve()
 */
static void setDefaultIdleSpeedTarget() {
#if CLT_CURVE_SIZE == 16
	copyArray(config->cltIdleRpmBins, {  -30, - 20,  -10,    0,   10,   20,   30,   40,   50,  60,  70,  80,  90, 100 , 110,  120 });
	copyArray(config->cltIdleRpm,     { 1350, 1350, 1300, 1200, 1150, 1100, 1050, 1000, 1000, 950, 950, 930, 900, 900, 1000, 1100 });
#endif // CLT_CURVE_SIZE
}
#endif // EFI_ENGINE_CONTROL

/**
 * see also setDefaultIdleSpeedTarget()
 */
void setTargetRpmCurve(float rpm) {
	setLinearCurve(config->cltIdleRpmBins, CLT_CURVE_RANGE_FROM, 140, 10);
	setLinearCurve(config->cltIdleRpm, rpm, rpm, 10);
}

static void setDefaultGppwmParameters() {
	// Same config for all channels
	for (size_t i = 0; i < efi::size(engineConfiguration->gppwm); i++) {
		auto& cfg = engineConfiguration->gppwm[i];
		chsnprintf(engineConfiguration->gpPwmNote[i], sizeof(engineConfiguration->gpPwmNote[0]), "GPPWM%d", i);

		// Set default axes
		cfg.loadAxis = GPPWM_Zero;
		cfg.rpmAxis = GPPWM_Rpm;

		cfg.pin = Gpio::Unassigned;
		cfg.dutyIfError = 0;
		cfg.onAboveDuty = 60;
		cfg.offBelowDuty = 50;
		cfg.pwmFrequency = 250;

		for (size_t j = 0; j < efi::size(cfg.loadBins); j++) {
			uint8_t z = j * 100 / (efi::size(cfg.loadBins) - 1);
			cfg.loadBins[j] = z;

			// Fill some values in the table
			for (size_t k = 0; k < efi::size(cfg.rpmBins); k++) {
				cfg.table[j][k] = z;
			}

		}

		for (size_t j = 0; j < efi::size(cfg.rpmBins); j++) {
			cfg.rpmBins[j] = 1000 * j;
		}
	}
}

static void setDefaultBoostOpenLoopParameters() {
	engineConfiguration->boostOpenLoopYAxis = GPPWM_Tps;
}

#if EFI_ENGINE_CONTROL
static void setDefaultEngineNoiseTable() {
	setRpmTableBin(config->knockNoiseRpmBins);

	engineConfiguration->knockDetectionWindowStart = 15.0 + 5.0;
	engineConfiguration->knockSamplingDuration = 45;

	setArrayValues(config->knockBaseNoise, -20);
}
#endif // EFI_ENGINE_CONTROL

static void setDefaultCanSettings() {
	// OBD-II default rate is 500kbps
	engineConfiguration->canBaudRate = B500KBPS;
	engineConfiguration->can2BaudRate = B500KBPS;
#if (EFI_CAN_BUS_COUNT >= 3)
	engineConfiguration->can3BaudRate = B500KBPS;
#endif

	engineConfiguration->canSleepPeriodMs = 50;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canVssScaling = 1.0f;

	// Don't enable, but set default address
	engineConfiguration->verboseCanBaseAddress = CAN_DEFAULT_BASE;
}

static void setDefaultScriptParameters() {
	setLinearCurve(config->scriptTable1LoadBins, 20, 120, 10);
	setRpmTableBin(config->scriptTable1RpmBins);
	setLinearCurve(config->scriptTable2LoadBins, 20, 120, 10);
	setRpmTableBin(config->scriptTable2RpmBins);
	setLinearCurve(config->scriptTable3LoadBins, 20, 120, 10);
	setRpmTableBin(config->scriptTable3RpmBins);
	setLinearCurve(config->scriptTable4LoadBins, 20, 120, 10);
	setRpmTableBin(config->scriptTable4RpmBins);
}

static void setDefaultIdleOpenLoopParameters() {
	setRpmTableBin(config->rpmIdleCorrBins);
	setLinearCurve(config->cltIdleCorrBins, CLT_CURVE_RANGE_FROM, 140, 10);
	for (size_t i = 0; i < CLT_IDLE_TABLE_RPM_SIZE; i++) {
		setLinearCurve(config->cltIdleCorrTable[i], 75.0, 50, 5);
	}
}

/**
 * @brief	Global default engine configuration
 * This method sets the global engine configuration defaults. These default values are then
 * overridden by engine-specific defaults and the settings are saved in flash memory.
 *
 * This method is invoked only when new configuration is needed:
 *  * recently re-flashed chip
 *  * flash version of configuration failed CRC check or appears to be older then FLASH_DATA_VERSION
 *  * 'rewriteconfig' command
 *  * 'set engine_type X' command
 *
 * This method should only change the state of the configuration data structure but should NOT change the state of
 * anything else.
 *
 * This method should NOT be setting any default pinout
 */
static void setDefaultEngineConfiguration() {
#if (! EFI_UNIT_TEST)
	efi::clear(persistentState.persistentConfiguration);
#endif
	prepareVoidConfiguration(engineConfiguration);

#if EFI_BOOST_CONTROL
    setDefaultBoostParameters();
#endif

  setDefaultCanSettings();

	engineConfiguration->sdCardLogFrequency = 50;

	setDefaultGppwmParameters();
	setDefaultBoostOpenLoopParameters();
	setDefaultScriptParameters();

#if EFI_ENGINE_CONTROL
	setDefaultBaseEngine();
	setDefaultFuel();
	setDefaultIgnition();
	setDefaultCranking();

	// VVT closed loop, totally random values!
	engineConfiguration->auxPid[0].pFactor = 2;
	engineConfiguration->auxPid[0].iFactor = 0.005;
	engineConfiguration->auxPid[0].dFactor = 0;
	engineConfiguration->auxPid[0].offset = 33;
	engineConfiguration->auxPid[0].minValue = 10;
	engineConfiguration->auxPid[0].maxValue = 90;

	engineConfiguration->vvtOutputFrequency = DEFAULT_SOLENOID_FREQUENCY; // VVT solenoid control

	engineConfiguration->isCylinderCleanupEnabled = true;

	engineConfiguration->auxPid[0].minValue = 10;
	engineConfiguration->auxPid[0].maxValue = 90;
	engineConfiguration->auxPid[1].minValue = 10;
	engineConfiguration->auxPid[1].maxValue = 90;

	engineConfiguration->turboSpeedSensorMultiplier = 1;

#if EFI_IDLE_CONTROL
	setDefaultIdleParameters();
#endif /* EFI_IDLE_CONTROL */

#if EFI_ELECTRONIC_THROTTLE_BODY
	setDefaultEtbParameters();
	setDefaultEtbBiasCurve();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

	setBosch0280218037();

	engineConfiguration->mapMinBufferLength = 1;
	engineConfiguration->vvtActivationDelayMs = 6000;

	engineConfiguration->startCrankingDuration = 3;

	engineConfiguration->maxAcRpm = 5000;
	engineConfiguration->maxAcClt = 100;
	engineConfiguration->maxAcTps = 75;

	initTemperatureCurve(IAT_FUEL_CORRECTION_CURVE, 1);

	initBoostTemperatureCurve(config->cltBoostCorrBins, config->cltBoostCorr, 1.0f);
	initBoostTemperatureCurve(config->iatBoostCorrBins, config->iatBoostCorr, 1.0f);
	initBoostTemperatureCurve(config->cltBoostAdderBins, config->cltBoostAdder, 0.0f);
	initBoostTemperatureCurve(config->iatBoostAdderBins, config->iatBoostAdder, 0.0f);

	engineConfiguration->alternatorControl.minValue = 0;
	engineConfiguration->alternatorControl.maxValue = 90;

	setLinearCurve(config->scriptCurve1Bins, 0, 100, 1);
	setLinearCurve(config->scriptCurve1, 0, 100, 1);

	setLinearCurve(config->scriptCurve2Bins, 0, 100, /*precision*/1);
	setLinearCurve(config->scriptCurve2, 30, 170, 1);

	setLinearCurve(config->scriptCurve3Bins, 0, 100, 1);
	setLinearCurve(config->scriptCurve4Bins, 0, 100, 1);
	setLinearCurve(config->scriptCurve5Bins, 0, 100, 1);
	setLinearCurve(config->scriptCurve6Bins, 0, 100, 1);

	setLinearCurve(config->alsIgnRetardLoadBins, 2, 10, /*precision*/1);
	setRpmTableBin(config->alsIgnRetardrpmBins);
	setLinearCurve(config->alsFuelAdjustmentLoadBins, 2, 10, /*precision*/1);
	setRpmTableBin(config->alsFuelAdjustmentrpmBins);
	setLinearCurve(config->fuelLevelBins, 0, 5);


	setRpmTableBin(engineConfiguration->map.samplingAngleBins);
	setLinearCurve(engineConfiguration->map.samplingAngle, 100, 130, 1);
	setRpmTableBin(engineConfiguration->map.samplingWindowBins);
	setLinearCurve(engineConfiguration->map.samplingWindow, 50, 50, 1);

#if VVT_TABLE_SIZE == 8
	setLinearCurve(config->vvtTable1LoadBins, 20, 120, 10);
	setLinearCurve(config->vvtTable2LoadBins, 20, 120, 10);
#else
	setLinearCurve(config->vvtTable1LoadBins, 20, 120, 5);
	setLinearCurve(config->vvtTable2LoadBins, 20, 120, 5);
#endif
	setRpmTableBin(config->vvtTable1RpmBins);
	setRpmTableBin(config->vvtTable2RpmBins);

	setDefaultEngineNoiseTable();

    // is this same old setCommonNTCSensor?
	engineConfiguration->clt.config = {0, 23.8889, 48.8889, 9500, 2100, 1000, 1500};

    setCommonNTCSensorParameters(&engineConfiguration->iat);

	// wow unit tests have much cooler setDefaultLaunchParameters method
	engineConfiguration->launchRpm = 3000;
// 	engineConfiguration->launchTimingRetard = 10;
	engineConfiguration->launchRpmWindow = 500;
    engineConfiguration->launchSpeedThreshold = 30;

	engineConfiguration->engineSnifferRpmThreshold = 2500;

	/**
	 * Idle control defaults
	 */
	setDefaultIdleSpeedTarget();
	//	setTargetRpmCurve(1200);

	engineConfiguration->idleRpmPid.pFactor = 0.05;
	engineConfiguration->idleRpmPid.iFactor = 0.002;

	engineConfiguration->idleRpmPid.minValue = -20;
	engineConfiguration->idleRpmPid.maxValue = 20;
	/**
	 * between variation between different sensor and weather and fabrication tolerance
	 * five percent looks like a safer default
	 */
	engineConfiguration->idlePidDeactivationTpsThreshold = 5;

	engineConfiguration->idle.solenoidFrequency = DEFAULT_SOLENOID_FREQUENCY;
	// set idle_position 50
	setDefaultIdleOpenLoopParameters();
//	engineConfiguration->idleMode = IM_AUTO;
	engineConfiguration->idleMode = idle_mode_e::IM_MANUAL;

	engineConfiguration->useStepperIdle = false;

	setLinearCurve(config->iacCoastingRpmBins, 0, 8000, 1);

#if !EFI_UNIT_TEST
  // todo: remove from *engine* defaults, move into boards?
	engineConfiguration->analogInputDividerCoefficient = 2;
#endif


	setTPS1Calibration(convertVoltageTo10bitADC(0),
			convertVoltageTo10bitADC(5),
			convertVoltageTo10bitADC(5),
			convertVoltageTo10bitADC(0));

	engineConfiguration->tps2Min = convertVoltageTo10bitADC(0);
	engineConfiguration->tps2Max = convertVoltageTo10bitADC(5);
	engineConfiguration->tps2SecondaryMin = convertVoltageTo10bitADC(5);
	engineConfiguration->tps2SecondaryMax = convertVoltageTo10bitADC(0);

	engineConfiguration->idlePositionMin = PACK_MULT_VOLTAGE * 0;
	engineConfiguration->idlePositionMax = PACK_MULT_VOLTAGE * 5;
	engineConfiguration->wastegatePositionClosedVoltage = 0.0;
	engineConfiguration->wastegatePositionOpenedVoltage = 5.0;
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	engineConfiguration->oilPressure.v1 = 0.5f;
	engineConfiguration->oilPressure.v2 = 4.5f;
	engineConfiguration->oilPressure.value1 = 0;
	engineConfiguration->oilPressure.value2 = 689.476f;	// 100psi = 689.476kPa

	engineConfiguration->mapLowValueVoltage = 0;
	// todo: start using this for custom MAP
	engineConfiguration->mapHighValueVoltage = 5;

	engineConfiguration->cylinderBore = 87.5;

	setBoschHDEV_5_injectors();

	setEgoSensor(ES_14Point7_Free);

	// todo: remove from *engine* defaults, move into boards?
	engineConfiguration->adcVcc = 3.0;

	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->baroSensor.type = MT_CUSTOM;
	engineConfiguration->baroSensor.lowValue = 0;
	engineConfiguration->baroSensor.highValue = 500;

#if EFI_PROD_CODE
	engineConfiguration->engineChartSize = 300;
#else
	// need more events for automated test
	engineConfiguration->engineChartSize = 400;
#endif

#if EFI_PROD_CODE || EFI_SIMULATOR
	// some tests broke with map averaging, see https://github.com/rusefi/rusefi/issues/7868
	engineConfiguration->isMapAveragingEnabled = true;
#endif
	engineConfiguration->isWaveAnalyzerEnabled = true;

	engineConfiguration->acIdleRpmTarget = 900;
	engineConfiguration->acDelay = engine_configuration_defaults::AC_DELAY;
    engineConfiguration->minAcPressure = engine_configuration_defaults::MIN_AC_PRESSURE;
    engineConfiguration->maxAcPressure = engine_configuration_defaults::MAX_AC_PRESSURE;
	engineConfiguration->acPressureEnableHyst = engine_configuration_defaults::AC_PRESSURE_ENABLE_HYST;
	engineConfiguration->acIdleExtraOffset = 15;

    engineConfiguration->nitrousMinimumTps = engine_configuration_defaults::NITROUS_MINIMUM_TPS;
    engineConfiguration->nitrousMinimumClt = engine_configuration_defaults::NITROUS_MINIMUM_CLT;
    engineConfiguration->nitrousMaximumAfr = engine_configuration_defaults::NITROUS_MAXIMUM_AFR;
    engineConfiguration->nitrousActivationRpm = engine_configuration_defaults::NITROUS_ACTIVATION_RPM;
    engineConfiguration->nitrousDeactivationRpm = engine_configuration_defaults::NITROUS_DEACTIVATION_RPM;
    engineConfiguration->nitrousDeactivationRpmWindow = engine_configuration_defaults::NITROUS_DEACTIVATION_RPM_WINDOW;

	engineConfiguration->triggerSimulatorRpm = DEFAULT_SELT_STIM_RPM;
	engineConfiguration->simulatorCamPosition[0] = DEFAULT_SELT_STIM_VVT0;

	engineConfiguration->alternatorPwmFrequency = DEFAULT_SOLENOID_FREQUENCY;

	engineConfiguration->isAlternatorControlEnabled = false;

	engineConfiguration->driveWheelRevPerKm = 1000;
	engineConfiguration->finalGearRatio = 1;
	engineConfiguration->vssGearRatio = 3.73;
	engineConfiguration->vssToothCount = 21;

	engineConfiguration->mapErrorDetectionTooLow = 5;
	// todo: default limits should be hard-coded for each sensor type
	// https://github.com/rusefi/rusefi/issues/4030
	engineConfiguration->mapErrorDetectionTooHigh = 410;

	setLinearCurve(config->throttleEstimateEffectiveAreaBins, 0, 100);
#endif // EFI_ENGINE_CONTROL
	// Allow custom default_script.lua to be provided by BOARDINC
	// see https://gcc.gnu.org/onlinedocs/gcc-2.95.3/cpp_1.html#SEC6
	#include <default_script.lua>
}

#if defined(STM32F7) && defined(HARDWARE_CI)
// part of F7 drama looks like we are having a hard time erasing configuration on HW CI :(
#define IGNORE_FLASH_CONFIGURATION true
#endif

// by default, do not ignore config from flash! use it!
#ifndef IGNORE_FLASH_CONFIGURATION
#define IGNORE_FLASH_CONFIGURATION false
#endif

void loadConfiguration() {

#if ! EFI_ACTIVE_CONFIGURATION_IN_FLASH
	// Clear the active configuration so that registered output pins (etc) detect the change on startup and init properly
	prepareVoidConfiguration(&activeConfiguration);
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */

	/* If board have any storage */
#if EFI_CONFIGURATION_STORAGE
	if (IGNORE_FLASH_CONFIGURATION) {
		engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
		resetConfigurationExt(engineConfiguration->engineType);
		writeToFlashNow();
	} else {
		// this call reads configuration from flash memory or sets default configuration
		// if flash state does not look right.
		readFromFlash();
	}
#else
	// This board doesn't load configuration, initialize the default
	engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
	resetConfigurationExt(engineConfiguration->engineType);
#endif /* EFI_CONFIGURATION_STORAGE */

	// Force any board configuration options that humans shouldn't be able to change
	call_board_override(custom_board_ConfigOverrides);
}

void resetConfigurationExt(configuration_callback_t boardCallback, engine_type_e engineType) {
	enginePins.reset(); // that's mostly important for functional tests
	/**
	 * Let's apply global defaults first
	 */
	setDefaultEngineConfiguration();

	/**
	 * custom board engine defaults. Yes, this overlaps with (older) engine_type_e approach.
	 */
	call_board_override(custom_board_BeforeTuneDefaults);

	// set initial pin groups
	setDefaultBasePins();

	if (boardCallback != nullptr) {
		boardCallback(engineConfiguration);
	}

#if EFI_PROD_CODE
	// call board-specific configuration setup, if needed (for custom boards only)
	call_board_override(custom_board_DefaultConfiguration);
	call_board_override(custom_board_ConfigOverrides);
#endif // EFI_PROD_CODE

	engineConfiguration->engineType = engineType;
	applyEngineType(engineType);
	applyNonPersistentConfiguration();
}

void emptyCallbackWithConfiguration(engine_configuration_s * p_engineConfiguration) {
	UNUSED(p_engineConfiguration);
}

void resetConfigurationExt(engine_type_e engineType) {
	resetConfigurationExt(&emptyCallbackWithConfiguration, engineType);
}

void applyNonPersistentConfiguration() {
#if EFI_PROD_CODE
	efiAssertVoid(ObdCode::CUSTOM_APPLY_STACK, hasLotsOfRemainingStack(), "apply c");
	efiPrintf("applyNonPersistentConfiguration()");
#endif

#if EFI_ENGINE_CONTROL
	engine->updateTriggerConfiguration();
#endif // EFI_ENGINE_CONTROL
}

void setCamOperationMode() {
	engineConfiguration->skippedWheelOnCam = true;
}

void setCrankOperationMode() {
	// this is related to 'setDefaultBaseEngine' having 'skippedWheelOnCam = true' which is a weird fact by itself
	engineConfiguration->skippedWheelOnCam = false;
}

void commonFrankensoAnalogInputs() {
	/**
	 * VBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
}

void setBoardDefaultConfiguration() {
  // custom_board_DefaultConfiguration
}
void setBoardConfigOverrides() {
  // time to force migration to custom_board_ConfigOverrides
}

PUBLIC_API_WEAK int hackHellenBoardId(int detectedId) { return detectedId; }

PUBLIC_API_WEAK void onBoardStandBy() { }

PUBLIC_API_WEAK_SOMETHING_WEIRD int getBoardMetaOutputsCount() { return 0; }
// default implementation: treat all outputs as low side
PUBLIC_API_WEAK int getBoardMetaLowSideOutputsCount() { return getBoardMetaOutputsCount(); }
PUBLIC_API_WEAK Gpio* getBoardMetaOutputs() { return nullptr; }
PUBLIC_API_WEAK int getBoardMetaDcOutputsCount() { return 0; }
