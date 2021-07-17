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

#include "os_access.h"
#include "fsio_impl.h"
#include "allsensors.h"
#include "interpolation.h"
#include "speed_density.h"
#include "advance_map.h"
#include "flash_main.h"

#include "hip9011_logic.h"

#if EFI_MEMS
#include "accelerometer.h"
#endif

#include "defaults.h"
#include "custom_engine.h"
#include "engine_template.h"
#include "bmw_m73.h"

#include "dodge_neon.h"
#include "dodge_ram.h"

#include "ford_aspire.h"
#include "ford_1995_inline_6.h"

#include "nissan_primera.h"
#include "nissan_vq.h"
#include "honda_accord.h"
#include "GY6_139QMB.h"

#include "mazda_miata.h"
#include "mazda_miata_1_6.h"
#include "mazda_miata_na8.h"
#include "mazda_miata_nb.h"
#include "mazda_miata_vvt.h"
#include "mazda_626.h"
#include "m111.h"

#include "citroenBerlingoTU3JP.h"
#include "mitsubishi.h"
#include "subaru.h"
#include "test_engine.h"
#include "sachs.h"
#include "vw.h"
#include "me7pnp.h"
#include "vw_b6.h"
#include "chevrolet_camaro_4.h"
#include "toyota_jzs147.h"
#include "ford_festiva.h"
#include "lada_kalina.h"
#include "honda_600.h"
#include "boost_control.h"
#if EFI_IDLE_CONTROL
#include "idle_thread.h"
#endif /* EFI_IDLE_CONTROL */

#if EFI_ALTERNATOR_CONTROL
#include "alternator_controller.h"
#endif

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif

#if EFI_HIP_9011
#include "hip9011.h"
#endif

#include "hardware.h"

#if EFI_PROD_CODE
#include "init.h"
#include "board.h"
#endif /* EFI_PROD_CODE */

#if EFI_EMULATE_POSITION_SENSORS
#include "trigger_emulator_algo.h"
#endif /* EFI_EMULATE_POSITION_SENSORS */

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

//#define TS_DEFAULT_SPEED 115200
#define TS_DEFAULT_SPEED 38400

/**
 * Current engine configuration. On firmware start we assign empty configuration, then
 * we copy actual configuration after reading settings.
 * This is useful to compare old and new configurations in order to apply new settings.
 *
 * todo: place this field next to 'engineConfiguration'?
 */
#if EFI_ACTIVE_CONFIGURATION_IN_FLASH
#include "flash_int.h"
engine_configuration_s & activeConfiguration = reinterpret_cast<persistent_config_container_s*>(getFlashAddrFirstCopy())->persistentConfiguration.engineConfiguration;
// we cannot use this activeConfiguration until we call rememberCurrentConfiguration()
bool isActiveConfigurationVoid = true;
#else
static engine_configuration_s activeConfigurationLocalStorage;
engine_configuration_s & activeConfiguration = activeConfigurationLocalStorage;
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */

extern engine_configuration_s *engineConfiguration;

void rememberCurrentConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if ! EFI_ACTIVE_CONFIGURATION_IN_FLASH
	memcpy(&activeConfiguration, engineConfiguration, sizeof(engine_configuration_s));
#else
	isActiveConfigurationVoid = false;
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */
}

static void wipeString(char *string, int size) {
	// we have to reset bytes after \0 symbol in order to calculate correct tune CRC from MSQ file
	for (int i = strlen(string) + 1; i < size; i++) {
		string[i] = 0;
	}
}

static void wipeStrings(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	wipeString(engineConfiguration->engineMake, sizeof(vehicle_info_t));
	wipeString(engineConfiguration->engineCode, sizeof(vehicle_info_t));
	wipeString(engineConfiguration->vehicleName, sizeof(vehicle_info_t));

	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		wipeString(config->fsioFormulas[i], sizeof(le_formula_t));
	}
}

void onBurnRequest(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	wipeStrings(PASS_ENGINE_PARAMETER_SIGNATURE);

	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

/**
 * this is the top-level method which should be called in case of any changes to engine configuration
 * online tuning of most values in the maps does not count as configuration change, but 'Burn' command does
 *
 * this method is NOT currently invoked on ECU start - actual user input has to happen!
 * See preCalculate which is invoked BOTH on start and configuration change
 */
void incrementGlobalConfigurationVersion(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ENGINE(globalConfigurationVersion++);
#if EFI_DEFAILED_LOGGING
	efiPrintf("set globalConfigurationVersion=%d", globalConfigurationVersion);
#endif /* EFI_DEFAILED_LOGGING */

	applyNewHardwareSettings(PASS_ENGINE_PARAMETER_SIGNATURE);

/**
 * All these callbacks could be implemented as listeners, but these days I am saving RAM
 */
#if EFI_PROD_CODE
	reconfigureSensors();
#endif /* EFI_PROD_CODE */
	engine->preCalculate(PASS_ENGINE_PARAMETER_SIGNATURE);
#if EFI_ALTERNATOR_CONTROL
	onConfigurationChangeAlternatorCallback(&activeConfiguration);
#endif /* EFI_ALTERNATOR_CONTROL */

#if EFI_BOOST_CONTROL
	onConfigurationChangeBoostCallback(&activeConfiguration);
#endif
#if EFI_ELECTRONIC_THROTTLE_BODY
	onConfigurationChangeElectronicThrottleCallback(&activeConfiguration);
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_IDLE_CONTROL && ! EFI_UNIT_TEST
	onConfigurationChangeIdleCallback(&activeConfiguration);
#endif /* EFI_IDLE_CONTROL */

#if EFI_SHAFT_POSITION_INPUT
	onConfigurationChangeTriggerCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* EFI_SHAFT_POSITION_INPUT */
#if EFI_EMULATE_POSITION_SENSORS && ! EFI_UNIT_TEST
	onConfigurationChangeRpmEmulatorCallback(&activeConfiguration);
#endif /* EFI_EMULATE_POSITION_SENSORS */

#if EFI_FSIO
	onConfigurationChangeFsioCallback(&activeConfiguration PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */
	rememberCurrentConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
}

/**
 * @brief Sets the same dwell time across the whole getRpm() range
 * set dwell X
 */
void setConstantDwell(floatms_t dwellMs DECLARE_CONFIG_PARAMETER_SUFFIX) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		engineConfiguration->sparkDwellRpmBins[i] = 1000 * i;
	}
	setArrayValues(engineConfiguration->sparkDwellValues, dwellMs);
}

void setWholeIgnitionIatCorr(float value DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setTable(config->ignitionIatCorrTable, value);
}

void setFuelTablesLoadBin(float minValue, float maxValue DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setLinearCurve(config->injPhaseLoadBins, minValue, maxValue, 1);
	setLinearCurve(config->veLoadBins, minValue, maxValue, 1);
	setLinearCurve(config->lambdaLoadBins, minValue, maxValue, 1);
}

void setWholeIatCorrTimingTable(float value DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setTable(config->ignitionIatCorrTable, value);
}

/**
 * See also crankingTimingAngle
 */
void setWholeTimingTable_d(angle_t value DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setTable(config->ignitionTable, value);
}

static void initTemperatureCurve(float *bins, float *values, int size, float defaultValue) {
	for (int i = 0; i < size; i++) {
		bins[i] = -40 + i * 10;
		values[i] = defaultValue; // this correction is a multiplier
	}
}

void prepareVoidConfiguration(engine_configuration_s *engineConfiguration) {
	efiAssertVoid(OBD_PCM_Processor_Fault, engineConfiguration != NULL, "ec NULL");
	efi::clear(engineConfiguration);

	engineConfiguration->clutchDownPinMode = PI_PULLUP;
	engineConfiguration->clutchUpPinMode = PI_PULLUP;
	engineConfiguration->brakePedalPinMode = PI_PULLUP;
}

void setDefaultBasePins(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
#ifdef EFI_WARNING_PIN
	engineConfiguration->warningLedPin = EFI_WARNING_PIN;
#else
	engineConfiguration->warningLedPin = GPIOD_13; // orange LED on discovery
#endif


#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = GPIOD_15; // blue LED on discovery
#endif
#ifdef EFI_RUNNING_PIN
	engineConfiguration->runningLedPin = EFI_RUNNING_PIN;
#else
	engineConfiguration->runningLedPin = GPIOD_12; // green LED on discovery
#endif

#if EFI_PROD_CODE
	// call overrided board-specific serial configuration setup, if needed (for custom boards only)
	// needed also by bootloader code
	setPinConfigurationOverrides();
#endif /* EFI_PROD_CODE */

	// set UART pads configuration based on the board
// needed also by bootloader code
	engineConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOC_10;
	engineConfiguration->binarySerialRxPin = GPIOC_11;
	engineConfiguration->tunerStudioSerialSpeed = TS_DEFAULT_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = 115200;

#if EFI_PROD_CODE
	// call overrided board-specific serial configuration setup, if needed (for custom boards only)
	setSerialConfigurationOverrides();
#endif /* EFI_PROD_CODE */
}

// needed also by bootloader code
// at the moment bootloader does NOT really need SD card, this is a step towards future bootloader SD card usage
void setDefaultSdCardParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	engineConfiguration->sdCardCsPin = GPIOD_4;
	engineConfiguration->isSdCardEnabled = true;

#if EFI_PROD_CODE
	// call overrided board-specific SD card configuration setup, if needed (for custom boards only)
	setSdCardConfigurationOverrides();
#endif /* EFI_PROD_CODE */
}

static void setDefaultWarmupIdleCorrection(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	initTemperatureCurve(CLT_MANUAL_IDLE_CORRECTION, 1.0);

	float baseIdle = 30;

	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -40, 1.5);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -30, 1.5);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -20, 40.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -10, 40.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,   0, 40.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  10, 40.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  20, 40.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  30, 40.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  40, 40.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  50, 37.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  60, 35.0 / baseIdle);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  70, 33.0 / baseIdle);
}

/**
 * see also setTargetRpmCurve()
 */
static void setDefaultIdleSpeedTarget(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setLinearCurve(engineConfiguration->cltIdleRpmBins, CLT_CURVE_RANGE_FROM, 140, 10);

	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, -30, 1350);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, -20, 1300);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, -10, 1200);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 0, 1150);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 10, 1100);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 20, 1050);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 30, 1000);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 40, 1000);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 50, 950);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 60, 950);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 70, 930);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 80, 900);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 90, 900);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 100, 1000);
	setCurveValue(engineConfiguration->cltIdleRpmBins, engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, 110, 1100);
}

static void setDefaultStepperIdleParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->idle.stepperDirectionPin = GPIOE_10;
	engineConfiguration->idle.stepperStepPin = GPIOE_12;
	engineConfiguration->stepperEnablePin = GPIOE_14;
	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->idleStepperTotalSteps = 150;
}

static void setCanFrankensoDefaults(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->canTxPin = GPIOB_6;
	engineConfiguration->canRxPin = GPIOB_12;
}

/**
 * see also setDefaultIdleSpeedTarget()
 */
void setTargetRpmCurve(int rpm DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setLinearCurve(engineConfiguration->cltIdleRpmBins, CLT_CURVE_RANGE_FROM, 90, 10);
	setLinearCurve(engineConfiguration->cltIdleRpm, rpm, rpm, 10);
}

void setDefaultGppwmParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// Same config for all channels
	for (size_t i = 0; i < efi::size(CONFIG(gppwm)); i++) {
		auto& cfg = CONFIG(gppwm)[i];

		cfg.pin = GPIO_UNASSIGNED;
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
			cfg.rpmBins[j] = 1000 * j / RPM_1_BYTE_PACKING_MULT;
		}
	}
}

void setDefaultEngineNoiseTable(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setRpmTableBin(engineConfiguration->knockNoiseRpmBins, ENGINE_NOISE_CURVE_SIZE);

	engineConfiguration->knockNoise[0] = 2; // 800
	engineConfiguration->knockNoise[1] = 2; // 1700
	engineConfiguration->knockNoise[2] = 2; // 2600
	engineConfiguration->knockNoise[3] = 2; // 3400
	engineConfiguration->knockNoise[4] = 2; // 4300
	engineConfiguration->knockNoise[5] = 2; // 5200
	engineConfiguration->knockNoise[6] = 2; // 6100
	engineConfiguration->knockNoise[7] = 2; // 7000
}

static void setHip9011FrankensoPinout(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	/**
	 * SPI on PB13/14/15
	 */
	//	CONFIG(hip9011CsPin) = GPIOD_0; // rev 0.1

	CONFIG(isHip9011Enabled) = true;
	engineConfiguration->hip9011PrescalerAndSDO = HIP_8MHZ_PRESCALER; // 8MHz chip
	CONFIG(is_enabled_spi_2) = true;
	// todo: convert this to rusEfi, hardware-independent enum
#if EFI_PROD_CODE
#ifdef EFI_HIP_CS_PIN
	CONFIG(hip9011CsPin) = EFI_HIP_CS_PIN;
#else
	CONFIG(hip9011CsPin) = GPIOB_0; // rev 0.4
#endif
	CONFIG(hip9011CsPinMode) = OM_OPENDRAIN;

	CONFIG(hip9011IntHoldPin) = GPIOB_11;
	CONFIG(hip9011IntHoldPinMode) = OM_OPENDRAIN;

	engineConfiguration->spi2SckMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi2MosiMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi2MisoMode = PO_PULLUP; // 32
#endif /* EFI_PROD_CODE */

	engineConfiguration->hip9011Gain = 1;
	engineConfiguration->knockVThreshold = 4;
	engineConfiguration->maxKnockSubDeg = 20;


	if (!CONFIG(useTpicAdvancedMode)) {
	    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
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
static void setDefaultEngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if (! EFI_UNIT_TEST)
	efi::clear(persistentState.persistentConfiguration);
#endif
	prepareVoidConfiguration(engineConfiguration);

	setDefaultBaseEngine(PASS_CONFIG_PARAMETER_SIGNATURE);
	setDefaultFuel(PASS_CONFIG_PARAMETER_SIGNATURE);
	setDefaultIgnition(PASS_CONFIG_PARAMETER_SIGNATURE);
	setDefaultCranking(PASS_CONFIG_PARAMETER_SIGNATURE);

#if EFI_IDLE_CONTROL
	setDefaultIdleParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_IDLE_CONTROL */

#if EFI_ELECTRONIC_THROTTLE_BODY
	setDefaultEtbParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
	setDefaultEtbBiasCurve(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */
#if EFI_BOOST_CONTROL
    setDefaultBoostParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif

    // OBD-II default rate is 500kbps
    CONFIG(canBaudRate) = B500KBPS;

	CONFIG(mafSensorType) = Bosch0280218037;
	setBosch0280218037(config);

	engineConfiguration->canSleepPeriodMs = 50;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = true;

	// Don't enable, but set default address
	engineConfiguration->verboseCanBaseAddress = CAN_DEFAULT_BASE;

	engineConfiguration->sdCardPeriodMs = 50;

	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		config->fsioFormulas[i][0] = 0;
	}

	CONFIG(mapMinBufferLength) = 1;
	
	CONFIG(startCrankingDuration) = 3;

	engineConfiguration->idlePidRpmDeadZone = 50;
	engineConfiguration->startOfCrankingPrimingPulse = 0;

	engineConfiguration->maxAcRpm = 5000;
	engineConfiguration->maxAcClt = 100;
	engineConfiguration->maxAcTps = 75;

	initTemperatureCurve(IAT_FUEL_CORRECTION_CURVE, 1);

	engineConfiguration->auxPid[0].minValue = 10;
	engineConfiguration->auxPid[0].maxValue = 90;

	engineConfiguration->alternatorControl.minValue = 10;
	engineConfiguration->alternatorControl.maxValue = 90;

	setLinearCurve(engineConfiguration->fsioCurve1Bins, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve1, 0, 100, 1);

	setLinearCurve(engineConfiguration->fsioCurve2Bins, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve2, 30, 170, 1);

	setLinearCurve(engineConfiguration->fsioCurve3Bins, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve4Bins, 0, 100, 1);

#if EFI_ENGINE_CONTROL
	setDefaultWarmupIdleCorrection(PASS_CONFIG_PARAMETER_SIGNATURE);

	setLinearCurve(engineConfiguration->map.samplingAngleBins, 800, 7000, 1);
	setLinearCurve(engineConfiguration->map.samplingAngle, 100, 130, 1);
	setLinearCurve(engineConfiguration->map.samplingWindowBins, 800, 7000, 1);
	setLinearCurve(engineConfiguration->map.samplingWindow, 50, 50, 1);

	setLinearCurve(config->vvtTable1LoadBins, 20, 120, 10);
	setRpmTableBin(config->vvtTable1RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->vvtTable2LoadBins, 20, 120, 10);
	setRpmTableBin(config->vvtTable2RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable1LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable1RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable2LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable2RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable3LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable3RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable4LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable4RpmBins, FSIO_TABLE_8);

	setDefaultEngineNoiseTable(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->clt.config = {0, 23.8889, 48.8889, 9500, 2100, 1000, 1500};

// todo: this value is way off! I am pretty sure temp coeffs are off also
	engineConfiguration->iat.config = {32, 75, 120, 9500, 2100, 1000, 2700};

	engineConfiguration->launchRpm = 3000;
 	engineConfiguration->launchTimingRetard = 10;
	engineConfiguration->launchTimingRpmRange = 500;
	engineConfiguration->launchSparkCutEnable = false;
	engineConfiguration->launchFuelCutEnable = false;
	engineConfiguration->hardCutRpmRange = 500;

	engineConfiguration->slowAdcAlpha = 0.33333;
	engineConfiguration->engineSnifferRpmThreshold = 2500;
	engineConfiguration->sensorSnifferRpmThreshold = 2500;

	engineConfiguration->noAccelAfterHardLimitPeriodSecs = 3;

	/**
	 * Idle control defaults
	 */
	setDefaultIdleSpeedTarget(PASS_ENGINE_PARAMETER_SIGNATURE);
	//	setTargetRpmCurve(1200 PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->idleRpmPid.pFactor = 0.05;
	engineConfiguration->idleRpmPid.iFactor = 0.002;

	engineConfiguration->idleRpmPid.minValue = 0;
	engineConfiguration->idleRpmPid.maxValue = 99;
	engineConfiguration->idlePidDeactivationTpsThreshold = 2;

	engineConfiguration->idle.solenoidFrequency = 200;
	// set idle_position 50
	engineConfiguration->manIdlePosition = 50;
//	engineConfiguration->idleMode = IM_AUTO;
	engineConfiguration->idleMode = IM_MANUAL;

	engineConfiguration->useStepperIdle = false;

	setDefaultStepperIdleParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	setDefaultGppwmParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

#if !EFI_UNIT_TEST
	engineConfiguration->analogInputDividerCoefficient = 2;
#endif

	// performance optimization
	engineConfiguration->sensorChartMode = SC_OFF;


	engineConfiguration->extraInjectionOffset = 0;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(0);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(5);
	engineConfiguration->tps1SecondaryMin = convertVoltageTo10bitADC(0);
	engineConfiguration->tps1SecondaryMax = convertVoltageTo10bitADC(5);
	engineConfiguration->tps2Min = convertVoltageTo10bitADC(0);
	engineConfiguration->tps2Max = convertVoltageTo10bitADC(5);
	engineConfiguration->tps2SecondaryMin = convertVoltageTo10bitADC(0);
	engineConfiguration->tps2SecondaryMax = convertVoltageTo10bitADC(5);
	engineConfiguration->idlePositionMin = PACK_MULT_VOLTAGE * 0;
	engineConfiguration->idlePositionMax = PACK_MULT_VOLTAGE * 5;
	engineConfiguration->wastegatePositionMin = PACK_MULT_VOLTAGE * 0;
	engineConfiguration->wastegatePositionMax = PACK_MULT_VOLTAGE * 5;
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	engineConfiguration->oilPressure.v1 = 0.5f;
	engineConfiguration->oilPressure.v2 = 4.5f;
	engineConfiguration->oilPressure.value1 = 0;
	engineConfiguration->oilPressure.value2 = 689.476f;	// 100psi = 689.476kPa

	engineConfiguration->mapLowValueVoltage = 0;
	// todo: start using this for custom MAP
	engineConfiguration->mapHighValueVoltage = 5;

	engineConfiguration->logFormat = LF_NATIVE;

	engineConfiguration->HD44780width = 20;
	engineConfiguration->HD44780height = 4;

	engineConfiguration->cylinderBore = 87.5;

	setEgoSensor(ES_14Point7_Free PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->globalFuelCorrection = 1;
	engineConfiguration->adcVcc = 3.0;

	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->baroSensor.type = MT_CUSTOM;
	engineConfiguration->baroSensor.lowValue = 0;
	engineConfiguration->baroSensor.highValue = 500;

	engineConfiguration->isEngineChartEnabled = true;

#if EFI_PROD_CODE
	engineConfiguration->engineChartSize = 300;
#else
	// need more events for automated test
	engineConfiguration->engineChartSize = 400;
#endif

	engineConfiguration->primingSquirtDurationMs = 5;

	engineConfiguration->isMapAveragingEnabled = true;
	engineConfiguration->isWaveAnalyzerEnabled = true;

	engineConfiguration->debugMode = DBG_ALTERNATOR_PID;

	engineConfiguration->acIdleRpmBump = 200;

	/* these two are used for HIP9011 only
	 * Currently this is offset from fire event, not TDC */
	/* TODO: convert to offset from TDC */
	engineConfiguration->knockDetectionWindowStart = 15.0 + 5.0;
	engineConfiguration->knockDetectionWindowEnd = 15.0 + 45.0;

	/**
	 * this is RPM. 10000 rpm is only 166Hz, 800 rpm is 13Hz
	 */
	engineConfiguration->triggerSimulatorFrequency = 1200;

	engineConfiguration->alternatorPwmFrequency = 300;

	strcpy(config->timingMultiplier, "1");
	strcpy(config->timingAdditive, "0");

	engineConfiguration->cj125isUaDivided = true;

	engineConfiguration->isAlternatorControlEnabled = false;

	engineConfiguration->vehicleSpeedCoef = 1.0f;


	engineConfiguration->mapErrorDetectionTooLow = 5;
	engineConfiguration->mapErrorDetectionTooHigh = 250;

	engineConfiguration->useLcdScreen = true;

	engineConfiguration->hip9011Gain = 1;

	engineConfiguration->isEngineControlEnabled = true;
#endif // EFI_ENGINE_CONTROL
#if EFI_FSIO
	/**
	 * to test:
	 * set_fsio_setting 1 5000
	 * set_fsio_output_pin 1 PE3
	 * set debug_mode 23
	 * writeconfig
	 * <reboot ECU>
	 * fsioinfo
	 */
	engineConfiguration->fsio_setting[0] = 5000;
	// simple warning light as default configuration
	// set_fsio_expression 1 "rpm > fsio_setting(1)"
	setFsio(0, GPIO_UNASSIGNED, RPM_ABOVE_USER_SETTING_1 PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */

	strncpy(config->luaScript, "function onTick()\nend", efi::size(config->luaScript));
}

/**
 * @brief	Hardware board-specific default configuration (GPIO pins, ADC channels, SPI configs etc.)
 */
void setDefaultFrankensoConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

	setCanFrankensoDefaults(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->iat.adcChannel = EFI_ADC_7;
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->accelerometerSpiDevice = SPI_DEVICE_1;
	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_2;
	engineConfiguration->cj125SpiDevice = SPI_DEVICE_2;

//	engineConfiguration->gps_rx_pin = GPIOB_7;
//	engineConfiguration->gps_tx_pin = GPIOB_6;

	engineConfiguration->triggerSimulatorPins[0] = GPIOD_1;
	engineConfiguration->triggerSimulatorPins[1] = GPIOD_2;

	engineConfiguration->triggerInputPins[0] = GPIOC_6;
	engineConfiguration->triggerInputPins[1] = GPIOA_5;

	// set this to SPI_DEVICE_3 to enable stimulation
	//engineConfiguration->digitalPotentiometerSpiDevice = SPI_DEVICE_3;
	engineConfiguration->digitalPotentiometerChipSelect[0] = GPIOD_7;
	engineConfiguration->digitalPotentiometerChipSelect[1] = GPIO_UNASSIGNED;
	engineConfiguration->digitalPotentiometerChipSelect[2] = GPIOD_5;
	engineConfiguration->digitalPotentiometerChipSelect[3] = GPIO_UNASSIGNED;

	engineConfiguration->spi1mosiPin = GPIOB_5;
	engineConfiguration->spi1misoPin = GPIOB_4;
	engineConfiguration->spi1sckPin = GPIOB_3; // please note that this pin is also SWO/SWD - Single Wire debug Output

	engineConfiguration->spi2mosiPin = GPIOB_15;
	engineConfiguration->spi2misoPin = GPIOB_14;
	engineConfiguration->spi2sckPin = GPIOB_13;

	engineConfiguration->spi3mosiPin = GPIOB_5;
	engineConfiguration->spi3misoPin = GPIOB_4;
	engineConfiguration->spi3sckPin = GPIOB_3;
	
	// set optional subsystem configs
#if EFI_MEMS
	// this would override some values from above
	configureAccelerometerPins(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_MEMS */

#if EFI_HIP_9011
	setHip9011FrankensoPinout(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_HIP_9011 */

#if EFI_FILE_LOGGING
	setDefaultSdCardParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_FILE_LOGGING */

	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = true;
}

#ifdef CONFIG_RESET_SWITCH_PORT
// this pin is not configurable at runtime so that we have a reliable way to reset configuration
#define SHOULD_IGNORE_FLASH() (palReadPad(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN) == 0)
#else
#define SHOULD_IGNORE_FLASH() (false)
#endif // CONFIG_RESET_SWITCH_PORT

// by default, do not ignore config from flash! use it!
#ifndef IGNORE_FLASH_CONFIGURATION
#define IGNORE_FLASH_CONFIGURATION false
#endif

void loadConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#ifdef CONFIG_RESET_SWITCH_PORT
	// initialize the reset pin if necessary
	palSetPadMode(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN, PAL_MODE_INPUT_PULLUP);
#endif /* CONFIG_RESET_SWITCH_PORT */

#if ! EFI_ACTIVE_CONFIGURATION_IN_FLASH
	// Clear the active configuration so that registered output pins (etc) detect the change on startup and init properly
	prepareVoidConfiguration(&activeConfiguration);
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */

#if EFI_INTERNAL_FLASH
	if (SHOULD_IGNORE_FLASH() || IGNORE_FLASH_CONFIGURATION) {
		engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
		resetConfigurationExt(engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
		writeToFlashNow();
	} else {
		// this call reads configuration from flash memory or sets default configuration
		// if flash state does not look right.
		readFromFlash();
	}
#else // not EFI_INTERNAL_FLASH
	// This board doesn't load configuration, initialize the default
	engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
	resetConfigurationExt(engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_INTERNAL_FLASH */

	// Force any board configuration options that humans shouldn't be able to change
	setBoardConfigOverrides();
}

void resetConfigurationExt(configuration_callback_t boardCallback, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX) {
	enginePins.reset(); // that's mostly important for functional tests
	/**
	 * Let's apply global defaults first
	 */
	setDefaultEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

	// set initial pin groups
	setDefaultBasePins(PASS_CONFIG_PARAMETER_SIGNATURE);

	if (boardCallback != nullptr) {
		boardCallback(engineConfiguration);
	}

#if EFI_PROD_CODE
	// call overrided board-specific configuration setup, if needed (for custom boards only)
	setBoardDefaultConfiguration();
	setBoardConfigOverrides();
#endif

	engineConfiguration->engineType = engineType;

	/**
	 * And override them with engine-specific defaults
	 */
	switch (engineType) {
	case MICRO_RUS_EFI:
// todo: is it time to replace MICRO_RUS_EFI, PROTEUS, PROMETHEUS_DEFAULTS with MINIMAL_PINS? maybe rename MINIMAL_PINS to DEFAULT?
	case PROTEUS_DEFAULTS:
	case PROMETHEUS_DEFAULTS:
	case HELLEN_128_MERCEDES:
	case MINIMAL_PINS:
		// all basic settings are already set in prepareVoidConfiguration(), no need to set anything here
		// nothing to do - we do it all in setBoardDefaultConfiguration
		break;
	case TEST_ENGINE:
		setTestEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#if EFI_UNIT_TEST
	case TEST_ISSUE_366_BOTH:
		setTestEngineIssue366both(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_ISSUE_366_RISE:
		setTestEngineIssue366rise(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_ISSUE_898:
		setIssue898(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#endif // EFI_UNIT_TEST
#if HW_MICRO_RUSEFI
	case MRE_M111:
		setM111EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case SUBARUEJ20G_DEFAULTS:
	case MRE_SUBARU_EJ18:
		setSubaruEJ18_MRE(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_BOARD_OLD_TEST:
		mreBoardOldTest(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_BOARD_NEW_TEST:
		mreBoardNewTest(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case BMW_M73_MRE:
	case BMW_M73_MRE_SLAVE:
		setEngineBMW_M73_microRusEfi(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_MIATA_NA6_VAF:
		setMiataNA6_VAF_MRE(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_MIATA_94_MAP:
		setMiata94_MAP_MRE(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_MIATA_NA6_MAP:
		setMiataNA6_MAP_MRE(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_MIATA_NB2_MAP:
		setMiataNB2_MRE_MAP(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_MIATA_NB2_MAF:
		setMiataNB2_MRE_MAF(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_MIATA_NB2_ETB:
		setMiataNB2_MRE_ETB(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_BODY_CONTROL:
		mreBCM(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#endif // HW_MICRO_RUSEFI
#if HW_PROTEUS
	case PROTEUS_QC_TEST_BOARD:
		proteusBoardTest(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case BMW_M73_PROTEUS:
		setEngineBMW_M73_Proteus(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MIATA_PROTEUS_TCU:
		setMiataNB2_Proteus_TCU(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case PROTEUS_MIATA_NB2:
		setMiataNB2_ProteusEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#ifdef HARDWARE_CI
	case PROTEUS_ANALOG_PWM_TEST:
		setProteusAnalogPwmTest(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#endif // HARDWARE_CI
#endif // HW_PROTEUS
#if HW_HELLEN
	case HELLEN_NB2:
		setMiataNB2_Hellen72(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HELLEN72_ETB:
		setHellen72etb(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HELLEN_121_NISSAN:
		setHellen121nissan(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HELLEN_121_VAG_5_CYL:
	    setHellen121Vag_5_cyl(PASS_CONFIG_PARAMETER_SIGNATURE);
        break;
	case HELLEN_121_VAG_V6_CYL:
	    setHellen121Vag_v6_cyl(PASS_CONFIG_PARAMETER_SIGNATURE);
        break;
	case HELLEN_121_VAG_VR6_CYL:
	    setHellen121Vag_vr6_cyl(PASS_CONFIG_PARAMETER_SIGNATURE);
        break;
	case HELLEN_121_VAG_8_CYL:
	    setHellen121Vag_8_cyl(PASS_CONFIG_PARAMETER_SIGNATURE);
        break;
	case HELLEN_121_VAG:
	case HELLEN_55_BMW:
	case HELLEN_88_BMW:
	case HELLEN_134_BMW:
	case HELLEN_154_VAG:
		break;
	case HELLEN_NA6:
	case HELLEN_NA94:
		setHellenNA6(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#endif // HW_HELLEN
#if HW_FRANKENSO
	case DEFAULT_FRANKENSO:
		setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case FRANKENSO_QA_ENGINE:
		setFrankensoBoardTestConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case BMW_M73_F:
		setEngineBMW_M73_Frankenso(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case BMW_M73_M:
		setEngineBMW_M73_Manhattan(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case DODGE_NEON_1995:
		setDodgeNeon1995EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case DODGE_NEON_2003_CRANK:
		setDodgeNeonNGCEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case LADA_KALINA:
		setLadaKalina(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case FORD_ASPIRE_1996:
		setFordAspireEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case NISSAN_PRIMERA:
		setNissanPrimeraEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HONDA_ACCORD_CD:
		setHondaAccordConfigurationThreeWires(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MIATA_NA6_MAP:
		setMiataNA6_MAP_Frankenso(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MIATA_NA6_VAF:
		setMiataNA6_VAF_Frankenso(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case ETB_BENCH_ENGINE:
		setEtbTestConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TLE8888_BENCH_ENGINE:
		setTle8888TestConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_NA8:
		setMazdaMiataNA8Configuration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_CIVIC_4_0_BOTH:
		setHondaCivic4_0_both(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_CIVIC_4_0_RISE:
		setHondaCivic4_0_rise(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HONDA_ACCORD_CD_TWO_WIRES:
		setHondaAccordConfiguration1_24(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HONDA_ACCORD_1_24_SHIFTED:
		setHondaAccordConfiguration1_24_shifted(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HONDA_ACCORD_CD_DIP:
		setHondaAccordConfigurationDip(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MITSU_4G93:
		setMitsubishiConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case FORD_INLINE_6_1995:
		setFordInline6(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case GY6_139QMB:
		setGy6139qmbDefaultEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case HONDA_600:
		setHonda600(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_NB1:
		setMazdaMiataNb1EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MAZDA_626:
		setMazda626EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case FORD_ESCORT_GT:
		setFordEscortGt(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MIATA_1990:
		setMiata1990(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MIATA_1996:
		setMiata1996(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case CITROEN_TU3JP:
		setCitroenBerlingoTU3JPConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case SUBARU_2003_WRX:
		setSubaru2003Wrx(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case DODGE_RAM:
		setDodgeRam1996(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case VW_B6:
		setVwPassatB6(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case VW_ABA:
		setVwAba(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_2003:
		setMazdaMiata2003EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_2003_NA_RAIL:
		setMazdaMiata2003EngineConfigurationNaFuelRail(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_2003_BOARD_TEST:
		setMazdaMiata2003EngineConfigurationBoardTest(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_ENGINE_VVT:
		setTestVVTEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case SACHS:
		setSachs(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case CAMARO_4:
		setCamaro4(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TOYOTA_2JZ_GTE_VVTi:
		setToyota_2jz_vics(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TOYOTA_JZS147:
		setToyota_jzs147EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case VAG_18_TURBO:
		vag_18_Turbo(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_33816:
		setTest33816EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#endif // HW_FRANKENSO
#ifdef HW_SUBARU_EG33
	case SUBARUEG33_DEFAULTS:
		setSubaruEG33Defaults(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#endif //HW_SUBARU_EG33
	default:
		firmwareError(CUSTOM_UNEXPECTED_ENGINE_TYPE, "Unexpected engine type: %d", engineType);
	}
	applyNonPersistentConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

#if EFI_TUNER_STUDIO
	syncTunerStudioCopy();
#endif /* EFI_TUNER_STUDIO */
}

void emptyCallbackWithConfiguration(engine_configuration_s * engineConfiguration) {
	UNUSED(engineConfiguration);
}

void resetConfigurationExt(engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX) {
	resetConfigurationExt(&emptyCallbackWithConfiguration, engineType PASS_ENGINE_PARAMETER_SUFFIX);
}

void validateConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->adcVcc > 5.0f || engineConfiguration->adcVcc < 1.0f) {
		engineConfiguration->adcVcc = 3.0f;
	}
	engine->preCalculate(PASS_ENGINE_PARAMETER_SIGNATURE);

	/**
	 * TunerStudio text tune files convert negative zero into positive zero so to keep things consistent we should avoid
	 * negative zeros altogether. Unfortunately default configuration had one and here we are mitigating that.
	 */
	for (int i = 0;i < CLT_CURVE_SIZE;i++) {
		engineConfiguration->cltIdleRpmBins[i] = fixNegativeZero(engineConfiguration->cltIdleRpmBins[i]);
	}
}

void applyNonPersistentConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	efiAssertVoid(CUSTOM_APPLY_STACK, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "apply c");
	efiPrintf("applyNonPersistentConfiguration()");
#endif

	assertEngineReference();

#if EFI_ENGINE_CONTROL
	ENGINE(initializeTriggerWaveform(PASS_ENGINE_PARAMETER_SIGNATURE));
#endif // EFI_ENGINE_CONTROL

#if EFI_FSIO
	applyFsioConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif // EFI_FSIO
}

#if EFI_ENGINE_CONTROL

void prepareShapes(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine->injectionEvents.addFuelEvents(PASS_ENGINE_PARAMETER_SIGNATURE);
}

#endif

void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode) {
	engineConfiguration->ambiguousOperationMode = mode;
}

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration) {
	/**
	 * VBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
}

void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration) {
	
	engineConfiguration->joystickCenterPin = GPIOC_8;
	engineConfiguration->joystickAPin = GPIOD_10;
	engineConfiguration->joystickBPin = GPIO_UNASSIGNED;
	engineConfiguration->joystickCPin = GPIO_UNASSIGNED;
	engineConfiguration->joystickDPin = GPIOD_11;
}

// These symbols are weak so that a board_configuration.cpp file can override them
__attribute__((weak)) void setBoardDefaultConfiguration(void) { }
__attribute__((weak)) void setBoardConfigOverrides(void) { }
