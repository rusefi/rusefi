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

#include "global.h"
#include "os_access.h"
#include "engine_configuration.h"
#include "fsio_impl.h"
#include "allsensors.h"
#include "interpolation.h"
#include "engine_math.h"
#include "speed_density.h"
#include "advance_map.h"
#include "sensor.h"

#include "hip9011_lookup.h"
#if EFI_MEMS
#include "accelerometer.h"
#endif

#include "custom_engine.h"
#include "engine_template.h"
#include "bmw_e34.h"
#include "bmw_m73.h"

#include "dodge_neon.h"
#include "dodge_ram.h"

#include "ford_aspire.h"
#include "ford_fiesta.h"
#include "ford_1995_inline_6.h"

#include "nissan_primera.h"
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
#include "rover_v8.h"
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

#if EFI_PROD_CODE
#include "init.h"
#include "hardware.h"
#include "board.h"
#endif /* EFI_PROD_CODE */

#if EFI_EMULATE_POSITION_SENSORS
#include "trigger_emulator_algo.h"
#endif /* EFI_EMULATE_POSITION_SENSORS */

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

EXTERN_ENGINE;

//#define TS_DEFAULT_SPEED 115200
#define TS_DEFAULT_SPEED 38400

#define xxxxx 0

#if 0
static fuel_table_t alphaNfuel = {
		{/*0  engineLoad=0.00*/   /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*1  engineLoad=6.66*/   /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*2  engineLoad=13.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*3  engineLoad=20.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*4  engineLoad=26.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*5  engineLoad=33.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*6  engineLoad=40.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*7  engineLoad=46.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*8  engineLoad=53.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*9  engineLoad=60.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*10 engineLoad=66.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*11 engineLoad=73.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*12 engineLoad=80.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*13 engineLoad=86.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*14 engineLoad=93.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*15 engineLoad=100.00*/ /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx}
		};
#endif

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

extern LoggingWithStorage sharedLogger;

static void wipeString(char *string, int size) {
	// we have to reset bytes after \0 symbol in order to calculate correct tune CRC from MSQ file
	for (int i = strlen(string) + 1; i < size; i++) {
		// todo: open question if it's worth replacing for loop with a memset. would a memset be much faster?
		// do we care about performance here?
		string[i] = 0;
	}
}

void wipeStrings(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
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
	scheduleMsg(&sharedLogger, "set globalConfigurationVersion=%d", globalConfigurationVersion);
#endif /* EFI_DEFAILED_LOGGING */
/**
 * All these callbacks could be implemented as listeners, but these days I am saving RAM
 */
#if EFI_PROD_CODE
	applyNewHardwareSettings();
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
#if EFI_EMULATE_POSITION_SENSORS
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
	setLinearCurve(engineConfiguration->sparkDwellValues, dwellMs, dwellMs, 0.01);
}

void setLambdaMap(lambda_table_t table, float value) {
	for (int l = 0; l < FUEL_LOAD_COUNT; l++) {
		for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
			table[l][rpmIndex] = (int)(value * PACK_MULT_LAMBDA_CFG);
		}
	}
}

// todo: make this a template
void setMap(fuel_table_t table, float value) {
	for (int l = 0; l < FUEL_LOAD_COUNT; l++) {
		for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
			table[l][rpmIndex] = value;
		}
	}
}

void setWholeIgnitionIatCorr(float value DECLARE_CONFIG_PARAMETER_SUFFIX) {
#if (IGN_LOAD_COUNT == FUEL_LOAD_COUNT) && (IGN_RPM_COUNT == FUEL_RPM_COUNT)
	// todo: make setMap a template
	setMap(config->ignitionIatCorrTable, value);
#else
	UNUSED(value);
#endif
}

void setFuelTablesLoadBin(float minValue, float maxValue DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setLinearCurve(config->injPhaseLoadBins, minValue, maxValue, 1);
	setLinearCurve(config->veLoadBins, minValue, maxValue, 1);
	setLinearCurve(config->lambdaLoadBins, minValue, maxValue, 1);
}

void setTimingMap(ignition_table_t map, float value) {
	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
		for (int r = 0; r < IGN_RPM_COUNT; r++) {
			map[l][r] = value;
		}
	}
}

void setWholeIatCorrTimingTable(float value DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setTimingMap(config->ignitionIatCorrTable, value);
}

/**
 * See also crankingTimingAngle
 */
void setWholeTimingTable_d(angle_t value DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setTimingMap(config->ignitionTable, value);
}

static void initTemperatureCurve(float *bins, float *values, int size, float defaultValue) {
	for (int i = 0; i < size; i++) {
		bins[i] = -40 + i * 10;
		values[i] = defaultValue; // this correction is a multiplier
	}
}

void prepareVoidConfiguration(engine_configuration_s *engineConfiguration) {
	efiAssertVoid(OBD_PCM_Processor_Fault, engineConfiguration != NULL, "ec NULL");
	memset(engineConfiguration, 0, sizeof(engine_configuration_s));

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


// todo: move injector calibration somewhere else?
// todo: add a enum? if we have enough data?
static void setBosch02880155868(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// http://www.boschdealer.com/specsheets/0280155868cs.jpg
	engineConfiguration->injector.battLagCorrBins[0] = 6;
	engineConfiguration->injector.battLagCorr[0] = 3.371;

	engineConfiguration->injector.battLagCorrBins[1] = 8;
	engineConfiguration->injector.battLagCorr[1] = 1.974;

	engineConfiguration->injector.battLagCorrBins[2] = 10;
	engineConfiguration->injector.battLagCorr[2] = 1.383;

	engineConfiguration->injector.battLagCorrBins[3] = 11;
	engineConfiguration->injector.battLagCorr[3] = 1.194;

	engineConfiguration->injector.battLagCorrBins[4] = 12;
	engineConfiguration->injector.battLagCorr[4] = 1.04;

	engineConfiguration->injector.battLagCorrBins[5] = 13;
	engineConfiguration->injector.battLagCorr[5] = 0.914;

	engineConfiguration->injector.battLagCorrBins[6] = 14;
	engineConfiguration->injector.battLagCorr[6] = 0.797;

	engineConfiguration->injector.battLagCorrBins[7] = 15;
	engineConfiguration->injector.battLagCorr[7] = 0.726;
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

static void setDefaultWarmupFuelEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	static const float bins[] =
	{
		-40,
		-30,
		-20,
		-10,
		0,
		10,
		20,
		30,
		40,
		50,
		60,
		70,
		80,
		90,
		100,
		110
	};

	copyArray(config->cltFuelCorrBins, bins);

	static const float values[] =
	{
		1.50,
		1.50,
		1.42,
		1.36,
		1.28,
		1.19,
		1.12,
		1.10,
		1.06,
		1.06,
		1.03,
		1.01,
		1,
		1,
		1,
		1
	};

	copyArray(config->cltFuelCorr, values);
}

static void setDefaultFuelCutParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutMap = 30;
	engineConfiguration->coastingFuelCutClt = 30;
}

static void setDefaultCrankingSettings(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	CONFIG(useTLE8888_cranking_hack) = true;

	setLinearCurve(engineConfiguration->crankingTpsCoef, /*from*/1, /*to*/1, 1);
	setLinearCurve(engineConfiguration->crankingTpsBins, 0, 100, 1);

	setLinearCurve(config->cltCrankingCorrBins, CLT_CURVE_RANGE_FROM, 100, 1);
	setLinearCurve(config->cltCrankingCorr, 1.0, 1.0, 1);

	// Cranking temperature compensation
	static const float crankingCoef[] = {
		2.8,
		2.2,
		1.8,
		1.5,
		1.0,
		1.0,
		1.0,
		1.0
	};
	copyArray(config->crankingFuelCoef, crankingCoef);

	// Deg C
	static const float crankingBins[] = {
		-20,
		-10,
		5,
		30,
		35,
		50,
		65,
		90
	};
	copyArray(config->crankingFuelBins, crankingBins);

	// Cranking cycle compensation

	static const float crankingCycleCoef[] = {
		1.5,
		1.35,
		1.05,
		0.75,
		0.5,
		0.5,
		0.5,
		0.5
	};
	copyArray(config->crankingCycleCoef, crankingCycleCoef);

	static const float crankingCycleBins[] = {
		4,
		8,
		12,
		16,
		74,
		75,
		76,
		77
	};
	copyArray(config->crankingCycleBins, crankingCycleBins);

	// Cranking ignition timing
	static const float advanceValues[] = { 0, 0, 0, 0 };
	copyArray(engineConfiguration->crankingAdvance, advanceValues);

	static const float advanceBins[] = { 0, 200, 400, 1000 };
	copyArray(engineConfiguration->crankingAdvanceBins, advanceBins);
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

int getTargetRpmForIdleCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// error is already reported, let's take the value at 0C since that should be a nice high idle
	float clt = Sensor::get(SensorType::Clt).value_or(0);

	int targetRpm = interpolate2d("cltRpm", clt, CONFIG(cltIdleRpmBins), CONFIG(cltIdleRpm));

	return targetRpm + engine->fsioState.fsioIdleTargetRPMAdjustment;
}

void setDefaultMultisparkParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// 1ms spark + 2ms dwell
	engineConfiguration->multisparkSparkDuration = 1000;
	engineConfiguration->multisparkDwell = 2000;

	// Conservative defaults - probably won't blow up coils
	engineConfiguration->multisparkMaxRpm = 1500;
	engineConfiguration->multisparkMaxExtraSparkCount = 2;
	engineConfiguration->multisparkMaxSparkingAngle = 30;
}

void setDefaultStftSettings(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	auto& cfg = CONFIG(stft);

	// Default to disabled
	CONFIG(fuelClosedLoopCorrectionEnabled) = false;

	// Default to proportional mode (for wideband sensors)
	CONFIG(stftIgnoreErrorMagnitude) = false;

	// 60 second startup delay - some O2 sensors are slow to warm up.
	cfg.startupDelay = 60;

	// Only correct in [12.0, 17.0]
	cfg.minAfr = 120;
	cfg.maxAfr = 170;

	// Above 60 deg C
	cfg.minClt = 60;

	// 0.5% deadband
	cfg.deadband = 5;

	// Sensible region defaults
	cfg.maxIdleRegionRpm = 1000 / RPM_1_BYTE_PACKING_MULT;
	cfg.maxOverrunLoad = 35;
	cfg.minPowerLoad = 85;

	// Sensible cell defaults
	for (size_t i = 0; i < efi::size(cfg.cellCfgs); i++) {
		// 30 second time constant - nice and slow
		cfg.cellCfgs[i].timeConstant = 30 * 10;

		/// Allow +-5%
		cfg.cellCfgs[i].maxAdd = 5;
		cfg.cellCfgs[i].maxRemove = -5;
	}
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
	memset(&persistentState.persistentConfiguration, 0, sizeof(persistentState.persistentConfiguration));
#endif
	prepareVoidConfiguration(engineConfiguration);

#if EFI_ALTERNATOR_CONTROL
	setDefaultAlternatorParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_ALTERNATOR_CONTROL */

#if EFI_IDLE_CONTROL
	setDefaultIdleParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_IDLE_CONTROL */

#if !EFI_UNIT_TEST
	// todo: this is a reasonable default for what kinds of engines exactly?
	engineConfiguration->wwaeTau = 0.3;
	engineConfiguration->wwaeBeta = 0.3;
#endif // EFI_UNIT_TEST

#if EFI_ELECTRONIC_THROTTLE_BODY
	setDefaultEtbParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
	setDefaultEtbBiasCurve(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */
#if EFI_BOOST_CONTROL
    setDefaultBoostParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif

    CONFIG(tachPulsePerRev) = 1;

    // OBD-II default rate is 500kbps
    CONFIG(canBaudRate) = B500KBPS;

	CONFIG(mafSensorType) = Bosch0280218037;
	setBosch0280218037(config);

	setBosch02880155868(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->canSleepPeriodMs = 50;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;

	// Don't enable, but set default address
	engineConfiguration->verboseCanBaseAddress = CAN_DEFAULT_BASE;

	engineConfiguration->sdCardPeriodMs = 50;

	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		config->fsioFormulas[i][0] = 0;
	}

	CONFIG(mapMinBufferLength) = 1;

	CONFIG(startCrankingDuration) = 3;

	CONFIG(compressionRatio) = 9;

	engineConfiguration->idlePidRpmDeadZone = 50;
	engineConfiguration->startOfCrankingPrimingPulse = 0;

	engineConfiguration->acCutoffLowRpm = 700;
	engineConfiguration->acCutoffHighRpm = 5000;

	engineConfiguration->postCrankingDurationSec = 2;

	initTemperatureCurve(IAT_FUEL_CORRECTION_CURVE, 1);

	engineConfiguration->tachPulseDuractionMs = 4;

	engineConfiguration->auxPid[0].minValue = 10;
	engineConfiguration->auxPid[0].maxValue = 90;

	engineConfiguration->alternatorControl.minValue = 10;
	engineConfiguration->alternatorControl.maxValue = 90;

	setLinearCurve(engineConfiguration->cltTimingBins, CLT_CURVE_RANGE_FROM, 120, 1);
	setLinearCurve(engineConfiguration->cltTimingExtra, 0, 0, 1);

	setLinearCurve(engineConfiguration->fsioCurve1Bins, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve1, 0, 100, 1);

	setLinearCurve(engineConfiguration->fsioCurve2Bins, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve2, 30, 170, 1);

	setLinearCurve(engineConfiguration->fsioCurve3Bins, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve4Bins, 0, 100, 1);

#if EFI_ENGINE_CONTROL
	setDefaultWarmupIdleCorrection(PASS_CONFIG_PARAMETER_SIGNATURE);

	setDefaultWarmupFuelEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE);

	setDefaultFuelCutParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	setMazdaMiataNbTpsTps(PASS_CONFIG_PARAMETER_SIGNATURE);

	/**
	 * 4ms is global default dwell for the whole RPM range
	 * if you only have one coil and many cylinders or high RPM you would need lower value at higher RPM
	 */
	setConstantDwell(4 PASS_CONFIG_PARAMETER_SUFFIX);
	/**
	 * Use angle-based duration during cranking
	 * this is equivalent to 'disable cranking_constant_dwell' console command
	 */
	engineConfiguration->useConstantDwellDuringCranking = true;
	engineConfiguration->ignitionDwellForCrankingMs = 6;

	setTimingLoadBin(1.2, 4.4 PASS_CONFIG_PARAMETER_SUFFIX);
	setTimingRpmBin(800, 7000 PASS_CONFIG_PARAMETER_SUFFIX);

	setLinearCurve(engineConfiguration->map.samplingAngleBins, 800, 7000, 1);
	setLinearCurve(engineConfiguration->map.samplingAngle, 100, 130, 1);
	setLinearCurve(engineConfiguration->map.samplingWindowBins, 800, 7000, 1);
	setLinearCurve(engineConfiguration->map.samplingWindow, 50, 50, 1);

	setLambdaMap(config->lambdaTable, 1.0f);
	engineConfiguration->stoichRatioPrimary = 14.7f * PACK_MULT_AFR_CFG;

	setDefaultVETable(PASS_ENGINE_PARAMETER_SIGNATURE);

#if (IGN_LOAD_COUNT == FUEL_LOAD_COUNT) && (IGN_RPM_COUNT == FUEL_RPM_COUNT)
	// todo: make setMap a template
	setMap(config->injectionPhase, -180);
#endif
	setRpmTableBin(config->injPhaseRpmBins, FUEL_RPM_COUNT);
	setFuelTablesLoadBin(10, 160 PASS_CONFIG_PARAMETER_SUFFIX);
	setDefaultIatTimingCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	setLinearCurve(engineConfiguration->mapAccelTaperBins, 0, 32, 4);
	setLinearCurve(engineConfiguration->mapAccelTaperMult, 1, 1, 1);

	setLinearCurve(config->tpsTpsAccelFromRpmBins, 0, 100, 10);
	setLinearCurve(config->tpsTpsAccelToRpmBins, 0, 100, 10);

	setLinearCurve(config->fsioTable1LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable1RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable2LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable2RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable3LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable3RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable4LoadBins, 20, 120, 10);
	setRpmTableBin(config->fsioTable4RpmBins, FSIO_TABLE_8);

	initEngineNoiseTable(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->clt.config = {0, 23.8889, 48.8889, 9500, 2100, 1000, 1500};

// todo: this value is way off! I am pretty sure temp coeffs are off also
	engineConfiguration->iat.config = {32, 75, 120, 9500, 2100, 1000, 2700};

#if EFI_PROD_CODE
	engineConfiguration->warningPeriod = 10;
#else
	engineConfiguration->warningPeriod = 0;
#endif /* EFI_PROD_CODE */

	engineConfiguration->launchRpm = 3000;
 	engineConfiguration->launchTimingRetard = 10;
	engineConfiguration->launchTimingRpmRange = 500;
	engineConfiguration->launchSparkCutEnable = false;
	engineConfiguration->launchFuelCutEnable = false;
	engineConfiguration->hardCutRpmRange = 500;

	engineConfiguration->slowAdcAlpha = 0.33333;
	engineConfiguration->engineSnifferRpmThreshold = 2500;
	engineConfiguration->sensorSnifferRpmThreshold = 2500;
	engineConfiguration->rpmHardLimit = 7000;
	engineConfiguration->cranking.rpm = 550;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->cutSparkOnHardLimit = true;


	engineConfiguration->tChargeMinRpmMinTps = 0.25;
	engineConfiguration->tChargeMinRpmMaxTps = 0.25;
	engineConfiguration->tChargeMaxRpmMinTps = 0.25;
	engineConfiguration->tChargeMaxRpmMaxTps = 0.9;
	engineConfiguration->tChargeMode = TCHARGE_MODE_RPM_TPS;
	engineConfiguration->tChargeAirCoefMin = 0.098f;
	engineConfiguration->tChargeAirCoefMax = 0.902f;
	engineConfiguration->tChargeAirFlowMax = 153.6f;
	engineConfiguration->tChargeAirIncrLimit = 1.0f;
	engineConfiguration->tChargeAirDecrLimit = 12.5f;

	engineConfiguration->noAccelAfterHardLimitPeriodSecs = 3;

	setDefaultCrankingSettings(PASS_ENGINE_PARAMETER_SIGNATURE);

	setDefaultStftSettings(PASS_ENGINE_PARAMETER_SIGNATURE);

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
	engineConfiguration->crankingIACposition = 50;
//	engineConfiguration->idleMode = IM_AUTO;
	engineConfiguration->idleMode = IM_MANUAL;

	engineConfiguration->useStepperIdle = false;

	setDefaultStepperIdleParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	/**
	 * Cranking defaults
	 */
	engineConfiguration->startUpFuelPumpDuration = 4;
	engineConfiguration->cranking.baseFuel = 27;
	engineConfiguration->crankingChargeAngle = 70;


	engineConfiguration->timingMode = TM_DYNAMIC;
	engineConfiguration->fixedModeTiming = 50;

	setDefaultMultisparkParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	setDefaultGppwmParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

#if !EFI_UNIT_TEST
	engineConfiguration->analogInputDividerCoefficient = 2;
#endif

	// performance optimization
	engineConfiguration->sensorChartMode = SC_OFF;

	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->globalTriggerAngleOffset = 0;
	engineConfiguration->extraInjectionOffset = 0;
	engineConfiguration->ignitionOffset = 0;
	engineConfiguration->sensorChartFrequency = 20;

	engineConfiguration->fuelAlgorithm = LM_SPEED_DENSITY;

	engineConfiguration->vbattDividerCoeff = ((float) (15 + 65)) / 15;

	engineConfiguration->fanOnTemperature = 95;
	engineConfiguration->fanOffTemperature = 91;

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

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2;
	/**
	 * By the way http://users.erols.com/srweiss/tableifc.htm has a LOT of data
	 */
	engineConfiguration->injector.flow = 200;

	engineConfiguration->mapLowValueVoltage = 0;
	// todo: start using this for custom MAP
	engineConfiguration->mapHighValueVoltage = 5;

	engineConfiguration->logFormat = LF_NATIVE;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;

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

	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	// Default this to on - if you want to diagnose, turn it off.
	engineConfiguration->silentTriggerError = true;

#if EFI_PROD_CODE
	engineConfiguration->engineChartSize = 300;
#else
	// need more events for automated test
	engineConfiguration->engineChartSize = 400;
#endif

	engineConfiguration->primingSquirtDurationMs = 5;

	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isCylinderCleanupEnabled = false; // this feature is evil if one does not have TPS, better turn off by default

	engineConfiguration->isMapAveragingEnabled = true;
	engineConfiguration->isWaveAnalyzerEnabled = true;

	engineConfiguration->debugMode = DBG_ALTERNATOR_PID;

	engineConfiguration->acIdleRpmBump = 200;
	engineConfiguration->knockDetectionWindowStart = 35;
	engineConfiguration->knockDetectionWindowEnd = 135;

	engineConfiguration->fuelLevelEmptyTankVoltage = 0;
	engineConfiguration->fuelLevelFullTankVoltage = 5;

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

	engineConfiguration->engineLoadAccelLength = 6;
	engineConfiguration->engineLoadAccelEnrichmentThreshold = 5; // kPa
	engineConfiguration->engineLoadAccelEnrichmentMultiplier = 0; // todo: improve implementation and re-enable by default

	engineConfiguration->tpsAccelLength = 12;
	engineConfiguration->tpsAccelEnrichmentThreshold = 40; // TPS % change, per engine cycle
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

	//engineConfiguration->logicAnalyzerPins[1] = GPIOE_5; // GPIOE_5 is a popular option (if available)


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
	setHip9011FrankensoPinout();
#endif /* EFI_HIP_9011 */

#if EFI_FILE_LOGGING
	setDefaultSdCardParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_FILE_LOGGING */

	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = true;
}

void resetConfigurationExt(Logging * logger, configuration_callback_t boardCallback, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX) {
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
	setBoardConfigurationOverrides();
#endif

	engineConfiguration->engineType = engineType;

	/**
	 * And override them with engine-specific defaults
	 */
	switch (engineType) {
	case MICRO_RUS_EFI:
// todo: is it time to replace MICRO_RUS_EFI, PROTEUS, PROMETHEUS_DEFAULTS with MINIMAL_PINS? maybe rename MINIMAL_PINS to DEFAULT?
	case PROTEUS:
	case PROMETHEUS_DEFAULTS:
	case ZIL_130:
	case MINIMAL_PINS:
		// all basic settings are already set in prepareVoidConfiguration(), no need to set anything here
		// nothing to do - we do it all in setBoardConfigurationOverrides
		break;
	case MIATA_PROTEUS_TCU:
		setMiataNB2_Proteus_TCU(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case PROTEUS_MIATA_NB2:
		setMiataNB2_ProteusEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_M111:
		setM111EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_BOARD_OLD_TEST:
		mreBoardOldTest(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_BOARD_NEW_TEST:
		mreBoardNewTest(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_ENGINE:
		setTestEngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case SUBARUEJ20G_DEFAULTS:
	case MRE_SUBARU_EJ18:
		setSubaruEJ18_MRE(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#if EFI_UNIT_TEST
	case TEST_ISSUE_366_BOTH:
		setTestEngineIssue366both(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case TEST_ISSUE_366_RISE:
		setTestEngineIssue366rise(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case ISSUE_898:
		setIssue898(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#endif // EFI_UNIT_TEST
	case BMW_M73_PROTEUS:
		setEngineBMW_M73_Proteus(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
#if EFI_INCLUDE_ENGINE_PRESETS
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
	case DODGE_NEON_1995:
		setDodgeNeon1995EngineConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case MRE_BODY_CONTROL:
		mreBCM(PASS_CONFIG_PARAMETER_SIGNATURE);
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
	case FORD_FIESTA:
		setFordFiestaDefaultEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
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
	case ROVER_V8:
		setRoverv8(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case SUBARU_2003_WRX:
		setSubaru2003Wrx(PASS_CONFIG_PARAMETER_SIGNATURE);
		break;
	case BMW_E34:
		setBmwE34(PASS_CONFIG_PARAMETER_SIGNATURE);
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
#endif // EFI_INCLUDE_ENGINE_PRESETS
	default:
		firmwareError(CUSTOM_UNEXPECTED_ENGINE_TYPE, "Unexpected engine type: %d", engineType);
	}
	applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER_SUFFIX);

#if EFI_TUNER_STUDIO
	syncTunerStudioCopy();
#endif /* EFI_TUNER_STUDIO */
}

void emptyCallbackWithConfiguration(engine_configuration_s * engineConfiguration) {
	UNUSED(engineConfiguration);
}

void resetConfigurationExt(Logging * logger, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX) {
	resetConfigurationExt(logger, &emptyCallbackWithConfiguration, engineType PASS_ENGINE_PARAMETER_SUFFIX);
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

void applyNonPersistentConfiguration(Logging * logger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE
	efiAssertVoid(CUSTOM_APPLY_STACK, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "apply c");
	scheduleMsg(logger, "applyNonPersistentConfiguration()");
#endif

	assertEngineReference();

#if EFI_ENGINE_CONTROL
	ENGINE(initializeTriggerWaveform(logger PASS_ENGINE_PARAMETER_SUFFIX));
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

float getRpmMultiplier(operation_mode_e mode) {
	if (mode == FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR) {
		return 2;
	} else if (mode == FOUR_STROKE_CAM_SENSOR) {
		return 0.5;
	} else if (mode == FOUR_STROKE_CRANK_SENSOR) {
		return 1;
	}
	return 1;
}

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

static const ConfigOverrides defaultConfigOverrides{};
// This symbol is weak so that a board_configuration.cpp file can override it
__attribute__((weak)) const ConfigOverrides& getConfigOverrides() {
	return defaultConfigOverrides;
}
