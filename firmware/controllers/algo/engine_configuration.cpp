/**
 * @file	engine_configuration.cpp
 * @brief	Utility method related to the engine configuration data structure.
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#include "main.h"
#include "engine_configuration.h"
#include "fsio_impl.h"
#include "allsensors.h"
#include "interpolation.h"
#include "trigger_decoder.h"
#include "engine_math.h"
#include "speed_density.h"
#include "advance_map.h"

#include "hip9011_lookup.h"
#include "accelerometer.h"

#include "custom_engine.h"
#include "engine_template.h"
#include "acura_rsx.h"
#include "audi_aan.h"
#include "bmw_e34.h"

#include "dodge_neon.h"
#include "dodge_ram.h"
#include "dodge_stratus.h"

#include "ford_aspire.h"
#include "ford_fiesta.h"
#include "ford_1995_inline_6.h"

#include "snow_blower.h"
#include "nissan_primera.h"
#include "honda_accord.h"
#include "GY6_139QMB.h"

#include "mazda_miata.h"
#include "mazda_miata_nb.h"
#include "mazda_miata_1_6.h"
#include "mazda_miata_vvt.h"
#include "mazda_323.h"
#include "mazda_626.h"

#include "saturn_ion.h"
#include "MiniCooperR50.h"
#include "citroenBerlingoTU3JP.h"
#include "rover_v8.h"
#include "mitsubishi.h"
#include "prometheus.h"
#include "subaru.h"
#include "test_engine.h"
#include "sachs.h"
#include "gm_2_2.h"
#include "vw.h"
#include "daihatsu.h"
#include "chevrolet_camaro_4.h"
#include "suzuki_vitara.h"
#include "chevrolet_c20_1973.h"
#include "toyota_jzs147.h"
#include "ford_festiva.h"
#include "lada_kalina.h"
#include "geo_storm.h"
#include "zil130.h"
#include "honda_600.h"

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "electronic_throttle.h"
#include "idle_thread.h"
#include "alternatorController.h"
#include "hardware.h"
#include "board.h"
#include "HIP9011.h"
#endif /* EFI_PROD_CODE */

#if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
#include "trigger_emulator.h"
#endif /* EFI_EMULATE_POSITION_SENSORS */

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
#include "tunerstudio.h"
#endif

EXTERN_ENGINE;

//#define TS_DEFAULT_SPEED 115200
#define TS_DEFAULT_SPEED 38400

#define xxxxx 0

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

static volatile int globalConfigurationVersion = 0;

/**
 * Current engine configuration. On firmware start we assign empty configuration, then
 * we copy actual configuration after reading settings.
 * This is useful to compare old and new configurations in order to apply new settings.
 *
 * todo: place this field next to 'engineConfiguration'?
 */
engine_configuration_s activeConfiguration;

extern engine_configuration_s *engineConfiguration;

void rememberCurrentConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	memcpy(&activeConfiguration, engineConfiguration, sizeof(engine_configuration_s));
}

/**
 * This counter is incremented every time user adjusts ECU parameters online (either via dev console or other
 * tuning software)
 */
int getGlobalConfigurationVersion(void) {
	return globalConfigurationVersion;
}

extern LoggingWithStorage sharedLogger;

/**
 * this is the top-level method which should be called in case of any changes to engine configuration
 * online tuning of most values in the maps does not count as configuration change, but 'Burn' command does
 */
void incrementGlobalConfigurationVersion(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	globalConfigurationVersion++;
#if EFI_DEFAILED_LOGGING
	scheduleMsg(&sharedLogger, "set globalConfigurationVersion=%d", globalConfigurationVersion);
#endif /* EFI_DEFAILED_LOGGING */
/**
 * All these callbacks could be implemented as listeners, but these days I am saving RAM
 */
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	applyNewHardwareSettings();
#endif /* EFI_PROD_CODE */
	engine->preCalculate();
#if EFI_ALTERNATOR_CONTROL || defined(__DOXYGEN__)
	onConfigurationChangeAlternatorCallback(&activeConfiguration);
#endif /* EFI_ALTERNATOR_CONTROL */
#if EFI_ELECTRONIC_THROTTLE_BODY || defined(__DOXYGEN__)
	onConfigurationChangeElectronicThrottleCallback(&activeConfiguration);
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
	onConfigurationChangeIdleCallback(&activeConfiguration);
#endif /* EFI_IDLE_CONTROL */

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	onConfigurationChangeTriggerCallback(&activeConfiguration PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_SHAFT_POSITION_INPUT */
#if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
	onConfigurationChangeRpmEmulatorCallback(&activeConfiguration);
#endif /* EFI_EMULATE_POSITION_SENSORS */

	onConfigurationChangeFsioCallback(&activeConfiguration PASS_ENGINE_PARAMETER_SUFFIX);

	rememberCurrentConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
}

/**
 * @brief Sets the same dwell time across the whole getRpm() range
 */
void setConstantDwell(floatms_t dwellMs DECLARE_ENGINE_PARAMETER_SUFFIX) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		engineConfiguration->sparkDwellRpmBins[i] = 1000 * i;
	}
	setLinearCurve(engineConfiguration->sparkDwellValues, DWELL_CURVE_SIZE, dwellMs, dwellMs, 0.01);
}

void setAfrMap(afr_table_t table, float value) {
	for (int l = 0; l < FUEL_LOAD_COUNT; l++) {
		for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
			table[l][rpmIndex] = (int)(value * AFR_STORAGE_MULT);
		}
	}
}

void setMap(fuel_table_t table, float value) {
	for (int l = 0; l < FUEL_LOAD_COUNT; l++) {
		for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
			table[l][rpmIndex] = value;
		}
	}
}

static void setWholeVEMap(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMap(config->veTable, value);
}

void setWholeFuelMap(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMap(config->fuelTable, value);
}

void setWholeIgnitionIatCorr(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMap(config->ignitionIatCorrTable, value);
}

void setFuelTablesLoadBin(float minValue, float maxValue DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setLinearCurve(config->injPhaseLoadBins, FUEL_LOAD_COUNT, minValue, maxValue, 1);
	setLinearCurve(config->veLoadBins, FUEL_LOAD_COUNT, minValue, maxValue, 1);
	setLinearCurve(config->afrLoadBins, FUEL_LOAD_COUNT, minValue, maxValue, 1);
}

void setTimingMap(ignition_table_t map, float value) {
	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
		for (int r = 0; r < IGN_RPM_COUNT; r++) {
			map[l][r] = value;
		}
	}
}

void setWholeIatCorrTimingTable(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setTimingMap(config->ignitionIatCorrTable, value);
}

/**
 * See also crankingTimingAngle
 */
void setWholeTimingTable(angle_t value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setTimingMap(config->ignitionTable, value);
}

static void initTemperatureCurve(float *bins, float *values, int size, float defaultValue) {
	for (int i = 0; i < size; i++) {
		bins[i] = -40 + i * 10;
		values[i] = defaultValue; // this correction is a multiplier
	}
}

static void setDefaultFsioParameters(engine_configuration_s *engineConfiguration) {
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
	for (int i = 0; i < AUX_PID_COUNT; i++) {
		engineConfiguration->auxPidPins[i] = GPIO_UNASSIGNED;
	}
	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		boardConfiguration->fsioOutputPins[i] = GPIO_UNASSIGNED;
		boardConfiguration->fsioDigitalInputs[i] = GPIO_UNASSIGNED;
		engineConfiguration->fsioInputModes[i] = PI_DEFAULT;
	}
	for (int i = 0; i < FSIO_ANALOG_INPUT_COUNT ; i++) {
		engineConfiguration->fsioAdc[i] = EFI_ADC_NONE;
	}
}

void prepareVoidConfiguration(engine_configuration_s *engineConfiguration) {
	memset(engineConfiguration, 0, sizeof(engine_configuration_s));
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;

	setDefaultFsioParameters(engineConfiguration);

	disableLCD(boardConfiguration);

	engineConfiguration->camInput = GPIO_UNASSIGNED;
	boardConfiguration->triggerInputPins[0] = GPIO_UNASSIGNED;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;

	for (int i = 0; i < SERVO_COUNT; i++) {
		engineConfiguration->servoOutputPins[i] = GPIO_UNASSIGNED;
	}

	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT; i++) {
		engineConfiguration->auxValves[i] = GPIO_UNASSIGNED;
	}
	engineConfiguration->LIS302DLCsPin = GPIO_UNASSIGNED;
	engineConfiguration->flexFuelSensor = GPIO_UNASSIGNED;
	engineConfiguration->test557pin = GPIO_UNASSIGNED;

	boardConfiguration->joystickCenterPin = GPIO_UNASSIGNED;
	boardConfiguration->joystickAPin = GPIO_UNASSIGNED;
	boardConfiguration->joystickBPin = GPIO_UNASSIGNED;
	boardConfiguration->joystickCPin = GPIO_UNASSIGNED;
	boardConfiguration->joystickDPin = GPIO_UNASSIGNED;

	boardConfiguration->frequencyReportingMapInputPin = GPIO_UNASSIGNED;

	engineConfiguration->sdCardSpiDevice = SPI_NONE;
	boardConfiguration->sdCardCsPin = GPIO_UNASSIGNED;
	engineConfiguration->accelerometerSpiDevice = SPI_DEVICE_1;

	engineConfiguration->cj125ua = EFI_ADC_NONE;
	engineConfiguration->cj125ur = EFI_ADC_NONE;

	boardConfiguration->idle.stepperDirectionPin = GPIO_UNASSIGNED;
	engineConfiguration->stepperDirectionPinMode = OM_DEFAULT;
	boardConfiguration->idle.stepperStepPin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePinMode = OM_DEFAULT;

	engineConfiguration->dizzySparkOutputPin = GPIO_UNASSIGNED;

	boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->acRelayPinMode = OM_DEFAULT;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	setDefaultAlternatorParameters();
#endif
#if EFI_ELECTRONIC_THROTTLE_BODY || defined(__DOXYGEN__)
	setDefaultEtbParameters();
#endif
#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
	setDefaultIdleParameters();
#endif
	boardConfiguration->wboHeaterPin = GPIO_UNASSIGNED;
	boardConfiguration->cj125CsPin = GPIO_UNASSIGNED;

	boardConfiguration->hip9011CsPin = GPIO_UNASSIGNED;

	boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->mainRelayPinMode = OM_DEFAULT;
	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	boardConfiguration->idle.solenoidPinMode = OM_DEFAULT;
	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
	boardConfiguration->etbControlPin1 = GPIO_UNASSIGNED;
	boardConfiguration->etbControlPin2 = GPIO_UNASSIGNED;
	boardConfiguration->etbDirectionPin1 = GPIO_UNASSIGNED;
	boardConfiguration->etbDirectionPin2 = GPIO_UNASSIGNED;
	boardConfiguration->o2heaterPin = GPIO_UNASSIGNED;

	boardConfiguration->tachOutputPin = GPIO_UNASSIGNED;

	boardConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;
	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	boardConfiguration->fanPin = GPIO_UNASSIGNED;
	boardConfiguration->fanPinMode = OM_DEFAULT;

	boardConfiguration->clutchDownPin = GPIO_UNASSIGNED;
	boardConfiguration->clutchDownPinMode = PI_PULLUP;
	boardConfiguration->clutchUpPin = GPIO_UNASSIGNED;
	boardConfiguration->clutchUpPinMode = PI_PULLUP;
	engineConfiguration->brakePedalPin = GPIO_UNASSIGNED;
	engineConfiguration->brakePedalPinMode = PI_PULLUP;

	boardConfiguration->gps_rx_pin = GPIO_UNASSIGNED;
	boardConfiguration->gps_tx_pin = GPIO_UNASSIGNED;

	for (int i = 0;i < INJECTION_PIN_COUNT;i++) {
		boardConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}
	boardConfiguration->injectionPinMode = OM_DEFAULT;

	for (int i = 0;i < IGNITION_PIN_COUNT;i++) {
		boardConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	}
	boardConfiguration->ignitionPinMode = OM_DEFAULT;
}

void setDefaultBasePins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->fatalErrorPin = GPIOD_14;
	engineConfiguration->warninigPin = GPIOD_13;
	engineConfiguration->configResetPin = GPIOB_1;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// call overrided board-specific serial configuration setup, if needed (for custom boards only)
	// needed also by bootloader code
	setPinConfigurationOverrides();
#endif
}

// needed also by bootloader code
void setDefaultSerialParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	boardConfiguration->startConsoleInBinaryMode = true;
	boardConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOC_10;
	engineConfiguration->binarySerialRxPin = GPIOC_11;
	engineConfiguration->consoleSerialTxPin = GPIOC_10;
	engineConfiguration->consoleSerialRxPin = GPIOC_11;
	boardConfiguration->tunerStudioSerialSpeed = TS_DEFAULT_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = 115200;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// call overrided board-specific serial configuration setup, if needed (for custom boards only)
	setSerialConfigurationOverrides();
#endif
}

// needed also by bootloader code
void setDefaultSdCardParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	boardConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	boardConfiguration->sdCardCsPin = GPIOD_4;
	boardConfiguration->isSdCardEnabled = true;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// call overrided board-specific SD card configuration setup, if needed (for custom boards only)
	setSdCardConfigurationOverrides();
#endif
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

static void setDefaultWarmupIdleCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	initTemperatureCurve(CLT_MANUAL_IDLE_CORRECTION, PERCENT_MULT);

	float baseIdle = 30;

	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -40, 150);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -30, 150);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -20, 40.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION, -10, 40.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,   0, 40.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  10, 40.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  20, 40.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  30, 40.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  40, 40.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  50, 37.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  60, 35.0 / baseIdle * 100);
	setCurveValue(CLT_MANUAL_IDLE_CORRECTION,  70, 33.0 / baseIdle * 100);
}

static void setDefaultWarmupFuelEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	initTemperatureCurve(WARMUP_CLT_EXTRA_FUEL_CURVE, PERCENT_MULT);

	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, -40, 150);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, -30, 150);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, -20, 142);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, -10, 136);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 0, 128);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 10, 119);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 20, 112);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 30, 110);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 40, 106);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 50, 106);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 60, 103);
	setCurveValue(WARMUP_CLT_EXTRA_FUEL_CURVE, 70, 101);
}

static void setDefaultFuelCutParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	boardConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
}

static void setDefaultCrankingSettings(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setLinearCurve(engineConfiguration->crankingTpsCoef, CRANKING_CURVE_SIZE, 1, 1, 1);
	setLinearCurve(engineConfiguration->crankingTpsBins, CRANKING_CURVE_SIZE, 0, 100, 1);

	config->crankingFuelCoef[0] = 5; // base cranking fuel adjustment coefficient
	config->crankingFuelBins[0] = -20; // temperature in C
	config->crankingFuelCoef[1] = 3.7;
	config->crankingFuelBins[1] = -10;
	config->crankingFuelCoef[2] = 2.6;
	config->crankingFuelBins[2] = 5;
	config->crankingFuelCoef[3] = 2.4;
	config->crankingFuelBins[3] = 20;
	config->crankingFuelCoef[4] = 2.1;
	config->crankingFuelBins[4] = 35;
	config->crankingFuelCoef[5] = 1.8;
	config->crankingFuelBins[5] = 50;
	config->crankingFuelCoef[6] = 1.1;
	config->crankingFuelBins[6] = 65;
	config->crankingFuelCoef[7] = 1;
	config->crankingFuelBins[7] = 90;

	config->crankingCycleCoef[0] = 1.5;
	config->crankingCycleBins[0] = 4;

	config->crankingCycleCoef[1] = 1.35;
	config->crankingCycleBins[1] = 8;

	config->crankingCycleCoef[2] = 1.05;
	config->crankingCycleBins[2] = 12;

	config->crankingCycleCoef[3] = 0.75;
	config->crankingCycleBins[3] = 16;

	config->crankingCycleCoef[4] = 0.5;
	config->crankingCycleBins[4] = 74;
	config->crankingCycleCoef[5] = 0.5;
	config->crankingCycleBins[5] = 75;
	config->crankingCycleCoef[6] = 0.5;
	config->crankingCycleBins[6] = 76;
	config->crankingCycleCoef[7] = 0.5;
	config->crankingCycleBins[7] = 77;

	engineConfiguration->crankingAdvance[0] = 0;
	engineConfiguration->crankingAdvanceBins[0] = 0;
	engineConfiguration->crankingAdvance[1] = 0;
	engineConfiguration->crankingAdvanceBins[0] = 200;
	engineConfiguration->crankingAdvance[2] = 0;
	engineConfiguration->crankingAdvanceBins[0] = 400;
	engineConfiguration->crankingAdvance[3] = 0;
	engineConfiguration->crankingAdvanceBins[0] = 1000;

}

/**
 * see also setTargetRpmCurve()
 */
static void setDefaultIdleSpeedTarget(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setLinearCurve(engineConfiguration->cltIdleRpmBins, CLT_CURVE_SIZE, -40, 140, 10);

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
	boardConfiguration->idle.stepperDirectionPin = GPIOE_10;
	boardConfiguration->idle.stepperStepPin = GPIOE_12;
	engineConfiguration->stepperEnablePin = GPIOE_14;
	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->idleStepperTotalSteps = 150;
}

static void setCanDefaults(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	boardConfiguration->canDeviceMode = CD_USE_CAN2;
	boardConfiguration->canTxPin = GPIOB_6;
	boardConfiguration->canRxPin = GPIOB_12;

	engineConfiguration->canSleepPeriod = 50;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;
}

/**
 * see also setDefaultIdleSpeedTarget()
 */
void setTargetRpmCurve(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setLinearCurve(engineConfiguration->cltIdleRpmBins, CLT_CURVE_SIZE, -40, 90, 10);
	setLinearCurve(engineConfiguration->cltIdleRpm, CLT_CURVE_SIZE, rpm, rpm, 10);
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
 */
void setDefaultConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if (! EFI_UNIT_TEST) || defined(__DOXYGEN__)
	memset(&persistentState.persistentConfiguration, 0, sizeof(persistentState.persistentConfiguration));
#endif
	prepareVoidConfiguration(engineConfiguration);

	boardConfiguration->mafSensorType = Bosch0280218037;
	setBosch0280218037(config);

	setBosch02880155868(PASS_ENGINE_PARAMETER_SIGNATURE);

	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		config->fsioFormulas[i][0] = 0;
	}


	boardConfiguration->mapMinBufferLength = 1;

	engineConfiguration->idlePidRpmDeadZone = 50;
	engineConfiguration->startOfCrankingPrimingPulse = 0;

	engineConfiguration->acCutoffLowRpm = 700;
	engineConfiguration->acCutoffHighRpm = 5000;

	engineConfiguration->postCrankingTargetClt = 25;
	engineConfiguration->postCrankingDurationSec = 2;

	initTemperatureCurve(IAT_FUEL_CORRECTION_CURVE, 1);

	engineConfiguration->tachPulseDuractionMs = 4;
	engineConfiguration->tachPulseTriggerIndex = 4;

	engineConfiguration->auxPid[0].minValue = 10;
	engineConfiguration->auxPid[0].maxValue = 90;

	engineConfiguration->etb.minValue = 0;
	engineConfiguration->etb.maxValue = 99;

	engineConfiguration->alternatorControl.minValue = 10;
	engineConfiguration->alternatorControl.maxValue = 90;

	setLinearCurve(engineConfiguration->cltTimingBins, CLT_TIMING_CURVE_SIZE, -40, 120, 1);
	setLinearCurve(engineConfiguration->cltTimingExtra, CLT_TIMING_CURVE_SIZE, 0, 0, 1);

	setLinearCurve(engineConfiguration->fsioCurve1Bins, FSIO_CURVE_16, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve1, FSIO_CURVE_16, 0, 100, 1);

	setLinearCurve(engineConfiguration->fsioCurve2Bins, FSIO_CURVE_16, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve2, FSIO_CURVE_16, 30, 170, 1);

	setLinearCurve(engineConfiguration->fsioCurve3Bins, FSIO_CURVE_8, 0, 100, 1);
	setLinearCurve(engineConfiguration->fsioCurve4Bins, FSIO_CURVE_8, 0, 100, 1);


	setDefaultWarmupIdleCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	setDefaultWarmupFuelEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE);

	setDefaultFuelCutParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	setMazdaMiataNbTpsTps(PASS_ENGINE_PARAMETER_SIGNATURE);

	setConstantDwell(4 PASS_ENGINE_PARAMETER_SUFFIX); // 4ms is global default dwell
	// disable constant_dwell
	engineConfiguration->useConstantDwellDuringCranking = false;
	engineConfiguration->ignitionDwellForCrankingMs = 6;

	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER_SUFFIX);
	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER_SUFFIX);
	setTimingLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER_SUFFIX);
	setTimingRpmBin(800, 7000 PASS_ENGINE_PARAMETER_SUFFIX);

	setLinearCurve(engineConfiguration->map.samplingAngleBins, MAP_ANGLE_SIZE, 800, 7000, 1);
	setLinearCurve(engineConfiguration->map.samplingAngle, MAP_ANGLE_SIZE, 100, 130, 1);
	setLinearCurve(engineConfiguration->map.samplingWindowBins, MAP_ANGLE_SIZE, 800, 7000, 1);
	setLinearCurve(engineConfiguration->map.samplingWindow, MAP_ANGLE_SIZE, 50, 50, 1);

	// set_whole_timing_map 3
	setWholeFuelMap(3 PASS_ENGINE_PARAMETER_SUFFIX);
	setAfrMap(config->afrTable, 14.7);

	setDefaultVETable(PASS_ENGINE_PARAMETER_SIGNATURE);

	setMap(config->injectionPhase, -180);
	setRpmTableBin(config->injPhaseRpmBins, FUEL_RPM_COUNT);
	setFuelTablesLoadBin(10, 160 PASS_ENGINE_PARAMETER_SUFFIX);
	setDefaultIatTimingCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	setLinearCurve(engineConfiguration->mapAccelTaperBins, FSIO_TABLE_8, 0, 32, 4);
	setLinearCurve(engineConfiguration->mapAccelTaperMult, FSIO_TABLE_8, 1, 1, 1);

	setLinearCurve(config->tpsTpsAccelFromRpmBins, FSIO_TABLE_8, 0, 100, 10);
	setLinearCurve(config->tpsTpsAccelToRpmBins, FSIO_TABLE_8, 0, 100, 10);

	setLinearCurve(config->fsioTable1LoadBins, FSIO_TABLE_8, 20, 120, 10);
	setRpmTableBin(config->fsioTable1RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable2LoadBins, FSIO_TABLE_8, 20, 120, 10);
	setRpmTableBin(config->fsioTable2RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable3LoadBins, FSIO_TABLE_8, 20, 120, 10);
	setRpmTableBin(config->fsioTable3RpmBins, FSIO_TABLE_8);
	setLinearCurve(config->fsioTable4LoadBins, FSIO_TABLE_8, 20, 120, 10);
	setRpmTableBin(config->fsioTable4RpmBins, FSIO_TABLE_8);

	initEngineNoiseTable(PASS_ENGINE_PARAMETER_SIGNATURE);

	setThermistorConfiguration(&engineConfiguration->clt, 0, 9500, 23.8889, 2100, 48.8889, 1000);
	engineConfiguration->clt.config.bias_resistor = 1500;

	setThermistorConfiguration(&engineConfiguration->iat, 32, 9500, 75, 2100, 120, 1000);
// todo: this value is way off! I am pretty sure temp coeffs are off also
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	engineConfiguration->warningPeriod = 10;
#else
	engineConfiguration->warningPeriod = 0;
#endif /* EFI_PROD_CODE */

	engineConfiguration->step1rpm = 3000;
 	engineConfiguration->step1timing = 10;
	engineConfiguration->step1RpmWindow = 500;
	engineConfiguration->step1fuelCutEnable = false;
	engineConfiguration->step1SparkCutEnable = false;
	engineConfiguration->step1RpmWindow = 500;

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

	engineConfiguration->noAccelAfterHardLimitPeriodSecs = 3;

	setDefaultCrankingSettings(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->warmupTargetAfrBins[0] = -12;
	engineConfiguration->warmupTargetAfr[0] = 12.3;
	engineConfiguration->warmupTargetAfrBins[1] = 0;
	engineConfiguration->warmupTargetAfr[1] = 13;
	engineConfiguration->warmupTargetAfrBins[2] = 30;
	engineConfiguration->warmupTargetAfr[2] = 13.8;
	engineConfiguration->warmupTargetAfrBins[3] = 60;
	engineConfiguration->warmupTargetAfr[3] = 14.5;

	engineConfiguration->fuelClosedLoopCorrectionEnabled = false;
	engineConfiguration->fuelClosedLoopCltThreshold = 70;
	engineConfiguration->fuelClosedLoopRpmThreshold = 900;
	engineConfiguration->fuelClosedLoopTpsThreshold = 80;
	boardConfiguration->fuelClosedLoopAfrLowThreshold = 10.3;
	engineConfiguration->fuelClosedLoopAfrHighThreshold = 19.8;
	engineConfiguration->fuelClosedLoopPid.pFactor = -0.1;

	/**
	 * Idle control defaults
	 */
	setDefaultIdleSpeedTarget(PASS_ENGINE_PARAMETER_SIGNATURE);
	//	setTargetRpmCurve(1200 PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->idleRpmPid.pFactor = 0.05;
	engineConfiguration->idleRpmPid.iFactor = 0.002;

	engineConfiguration->idleRpmPid.minValue = 5;
	engineConfiguration->idleRpmPid.maxValue = 95;
	boardConfiguration->idlePidDeactivationTpsThreshold = 2;

	boardConfiguration->idle.solenoidFrequency = 200;
	// set idle_position 50
	boardConfiguration->manIdlePosition = 50;
	engineConfiguration->crankingIACposition = 50;
//	engineConfiguration->idleMode = IM_AUTO;
	engineConfiguration->idleMode = IM_MANUAL;

	boardConfiguration->useStepperIdle = false;

	setDefaultStepperIdleParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	/**
	 * Cranking defaults
	 */
	engineConfiguration->startUpFuelPumpDuration = 4;
	engineConfiguration->cranking.baseFuel = 5;
	engineConfiguration->crankingChargeAngle = 70;


	engineConfiguration->timingMode = TM_DYNAMIC;
	engineConfiguration->fixedModeTiming = 50;

	engineConfiguration->analogInputDividerCoefficient = 2;

	// performance optimization
	boardConfiguration->sensorChartMode = SC_OFF;

	engineConfiguration->storageMode = MS_AUTO;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;

	engineConfiguration->pedalPositionChannel = EFI_ADC_NONE;

	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->globalTriggerAngleOffset = 0;
	engineConfiguration->extraInjectionOffset = 0;
	engineConfiguration->ignitionOffset = 0;
	engineConfiguration->overrideCrankingIgnition = true;
	engineConfiguration->sensorChartFrequency = 20;

	engineConfiguration->fuelAlgorithm = LM_PLAIN_MAF;

	engineConfiguration->vbattDividerCoeff = ((float) (15 + 65)) / 15;

	engineConfiguration->fanOnTemperature = 95;
	engineConfiguration->fanOffTemperature = 91;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	engineConfiguration->oilPressure.hwChannel = EFI_ADC_NONE;
	
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

	engineConfiguration->displayMode = DM_HD44780;

	engineConfiguration->logFormat = LF_NATIVE;
	engineConfiguration->directSelfStimulation = false;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;

	engineConfiguration->HD44780width = 20;
	engineConfiguration->HD44780height = 4;

	engineConfiguration->vRefAdcChannel = EFI_ADC_NONE;
	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->iat.adcChannel = EFI_ADC_7;
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->cylinderBore = 87.5;

	setEgoSensor(ES_14Point7_Free PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->globalFuelCorrection = 1;
	engineConfiguration->adcVcc = 3.0;

	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->baroSensor.type = MT_CUSTOM;
	engineConfiguration->baroSensor.lowValue = 0;
	engineConfiguration->baroSensor.highValue = 500;

	engineConfiguration->isEngineChartEnabled = true;

	engineConfiguration->useOnlyRisingEdgeForTrigger = false;


	engineConfiguration->acSwitchAdc = EFI_ADC_NONE;

	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;



#if EFI_PROD_CODE || defined(__DOXYGEN__)
	engineConfiguration->engineChartSize = 300;
#else
	// need more events for automated test
	engineConfiguration->engineChartSize = 400;
#endif

	engineConfiguration->primingSquirtDurationMs = 5;

	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isCylinderCleanupEnabled = false; // this feature is evil if one does not have TPS, better turn off by default
	engineConfiguration->secondTriggerChannelEnabled = true;

	engineConfiguration->isMapAveragingEnabled = true;
	engineConfiguration->isTunerStudioEnabled = true;
	engineConfiguration->isWaveAnalyzerEnabled = true;

	engineConfiguration->debugMode = DBG_ALTERNATOR_PID;

	engineConfiguration->acIdleRpmBump = 200;
	engineConfiguration->knockDetectionWindowStart = 35;
	engineConfiguration->knockDetectionWindowEnd = 135;

	engineConfiguration->fuelLevelSensor = EFI_ADC_NONE;
	boardConfiguration->fuelLevelEmptyTankVoltage = 0;
	boardConfiguration->fuelLevelFullTankVoltage = 5;

	engineConfiguration->hipOutputChannel = EFI_ADC_NONE;

	/**
	 * this is RPM. 10000 rpm is only 166Hz, 800 rpm is 13Hz
	 */
	boardConfiguration->triggerSimulatorFrequency = 1200;

	boardConfiguration->triggerErrorPin = GPIO_UNASSIGNED;

	boardConfiguration->max31855spiDevice = SPI_NONE;
	for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
		boardConfiguration->max31855_cs[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->alternatorPwmFrequency = 300;

	engineConfiguration->communicationPin = GPIOD_15; // blue LED on discovery
	engineConfiguration->runningPin = GPIOD_12; // greeb LED on discovery
	setDefaultBasePins(PASS_ENGINE_PARAMETER_SIGNATURE);
	
	setDefaultSerialParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	boardConfiguration->triggerSimulatorPins[0] = GPIOD_1;
	boardConfiguration->triggerSimulatorPins[1] = GPIOD_2;
	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;

	boardConfiguration->triggerSimulatorPinModes[0] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;


	strcpy(config->timingMultiplier, "1");
	strcpy(config->timingAdditive, "0");

	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_2;
	engineConfiguration->cj125SpiDevice = SPI_DEVICE_2;

	engineConfiguration->isAlternatorControlEnabled = true;

//	boardConfiguration->gps_rx_pin = GPIOB_7;
//	boardConfiguration->gps_tx_pin = GPIOB_6;

	boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIOA_5;
	boardConfiguration->logicAnalyzerPins[0] = GPIO_UNASSIGNED;
	boardConfiguration->logicAnalyzerPins[1] = GPIO_UNASSIGNED; // GPIOE_5 is a popular option (if available)
	boardConfiguration->logicAnalyzerPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->logicAnalyzerPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
	engineConfiguration->vehicleSpeedCoef = 1.0f;

	boardConfiguration->logicAnalyzerMode[0] = false;
	boardConfiguration->logicAnalyzerMode[1] = false;

	engineConfiguration->mapErrorDetectionTooLow = 5;
	engineConfiguration->mapErrorDetectionTooHigh = 250;

	boardConfiguration->idleThreadPeriod = 100;
	boardConfiguration->consoleLoopPeriod = 200;
	boardConfiguration->lcdThreadPeriod = 300;
	boardConfiguration->generalPeriodicThreadPeriod = 50;
	boardConfiguration->useLcdScreen = true;

	boardConfiguration->boardTestModeJumperPin = GPIOB_0;

	setCanDefaults(PASS_ENGINE_PARAMETER_SIGNATURE);


	// set this to SPI_DEVICE_3 to enable stimulation
	boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
	boardConfiguration->digitalPotentiometerChipSelect[0] = GPIOD_7;
	boardConfiguration->digitalPotentiometerChipSelect[1] = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[2] = GPIOD_5;
	boardConfiguration->digitalPotentiometerChipSelect[3] = GPIO_UNASSIGNED;

	boardConfiguration->is_enabled_spi_1 = false;
	boardConfiguration->is_enabled_spi_2 = false;
	boardConfiguration->is_enabled_spi_3 = true;

	boardConfiguration->spi1mosiPin = GPIOB_5;
	boardConfiguration->spi1misoPin = GPIOB_4;
	boardConfiguration->spi1sckPin = GPIOB_3; // please note that this pin is also SWO/SWD - Single Wire debug Output

#if EFI_MEMS || defined(__DOXYGEN__)
	// this would override some values from above
	configureAccelerometerPins(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif

	boardConfiguration->spi2mosiPin = GPIOB_15;
	boardConfiguration->spi2misoPin = GPIOB_14;
	boardConfiguration->spi2sckPin = GPIOB_13;

	boardConfiguration->spi3mosiPin = GPIOB_5;
	boardConfiguration->spi3misoPin = GPIOB_4;
	boardConfiguration->spi3sckPin = GPIOB_3;

	boardConfiguration->hip9011Gain = 1;
#if EFI_HIP_9011 || defined(__DOXYGEN__)
	setHip9011FrankensoPinout();
#endif

	setDefaultSdCardParameters(PASS_ENGINE_PARAMETER_SIGNATURE);
	
	boardConfiguration->isFastAdcEnabled = true;
	boardConfiguration->isEngineControlEnabled = true;

	boardConfiguration->isVerboseAlternator = false;

	engineConfiguration->warmupAfrPid.offset = 1;
	engineConfiguration->warmupAfrThreshold = 60;

	engineConfiguration->engineLoadAccelLength = 6;
	engineConfiguration->engineLoadAccelEnrichmentThreshold = 5; // kPa
	engineConfiguration->engineLoadAccelEnrichmentMultiplier = 0; // todo: improve implementation and re-enable by default

	engineConfiguration->tpsAccelLength = 12;
	engineConfiguration->tpsAccelEnrichmentThreshold = 40; // TPS % change, per engine cycle

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// call overrided board-specific configuration setup, if needed (for custom boards only)
	setBoardConfigurationOverrides();
#endif

	/**
	 * to test:
	 * set_fsio_setting 1 5000
	 * set_fsio_output_pin 1 PE3
	 * set debug_mode 23
	 * writeconfig
	 * <reboot ECU>
	 * fsioinfo
	 */
	boardConfiguration->fsio_setting[0] = 5000;
	// simple warning light as default configuration
	// set_fsio_expression 1 "rpm > fsio_setting(1)"
	setFsio(0, GPIO_UNASSIGNED, RPM_ABOVE_USER_SETTING_1 PASS_ENGINE_PARAMETER_SUFFIX);

}

void resetConfigurationExt(Logging * logger, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX) {
	enginePins.reset(); // that's mostly important for functional tests
	/**
	 * Let's apply global defaults first
	 */
	setDefaultConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
#if EFI_SIMULATOR || defined(__DOXYGEN__)
	engineConfiguration->directSelfStimulation = true;
#endif /* */
	engineConfiguration->engineType = engineType;

	/**
	 * And override them with engine-specific defaults
	 */
	switch (engineType) {
	case CUSTOM_ENGINE:
		setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case ACURA_RSX:
		setAcuraRSX(engineConfiguration);
		break;
#if EFI_SUPPORT_DODGE_NEON || defined(__DOXYGEN__)
	case DODGE_NEON_1995:
		setDodgeNeon1995EngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case DODGE_NEON_2003_CAM:
		setDodgeNeonNGCEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case DODGE_NEON_2003_CRANK:
		setDodgeNeonNGCEngineConfigurationCrankBased(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case LADA_KALINA:
		setLadaKalina(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;

#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE || defined(__DOXYGEN__)
	case FORD_ASPIRE_1996:
		setFordAspireEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)
	case FORD_FIESTA:
		setFordFiestaDefaultEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA || defined(__DOXYGEN__)
	case NISSAN_PRIMERA:
		setNissanPrimeraEngineConfiguration(engineConfiguration);
		break;
#endif
	case HONDA_ACCORD_CD:
		setHondaAccordConfigurationThreeWires(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case ZIL_130:
		setZil130(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MIATA_NA_1_6:
	case MAZDA_MIATA_NA8:
		setMiataNA_1_6_Configuration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case TEST_CIVIC_4_0_BOTH:
		setHondaCivic4_0_both(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case TEST_CIVIC_4_0_RISE:
		setHondaCivic4_0_rise(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case HONDA_ACCORD_CD_TWO_WIRES:
		setHondaAccordConfiguration1_24(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case HONDA_ACCORD_1_24_SHIFTED:
		setHondaAccordConfiguration1_24_shifted(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case FRANKENSO_QA_ENGINE:
		setFrankensoBoardTestConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case HONDA_ACCORD_CD_DIP:
		setHondaAccordConfigurationDip(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MITSU_4G93:
		setMitsubishiConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
#if EFI_SUPPORT_1995_FORD_INLINE_6 || defined(__DOXYGEN__)
	case FORD_INLINE_6_1995:
		setFordInline6(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
#endif /* EFI_SUPPORT_1995_FORD_INLINE_6 */
	case GY6_139QMB:
		setGy6139qmbDefaultEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case HONDA_600:
		setHonda600(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_NB1:
		setMazdaMiataNb1EngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MAZDA_323:
		setMazda323EngineConfiguration(engineConfiguration);
		break;
	case MAZDA_626:
		setMazda626EngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case SATURN_ION_2004:
		setSaturnIonEngineConfiguration(engineConfiguration);
		break;
	case SUZUKI_VITARA:
		setSuzukiVitara(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MINI_COOPER_R50:
		setMiniCooperR50(engineConfiguration);
		break;
	case FORD_ESCORT_GT:
		setFordEscortGt(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MIATA_1990:
		setMiata1990(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MIATA_1994_DEVIATOR:
		setMiata1994_d(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MIATA_1994_SPAGS:
		setMiata1994_s(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MIATA_1996:
		setMiata1996(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case CITROEN_TU3JP:
		setCitroenBerlingoTU3JPConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case ROVER_V8:
		setRoverv8(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case SUBARU_2003_WRX:
		setSubaru2003Wrx(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case BMW_E34:
		setBmwE34(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case GM_2_2:
		setGm2_2(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case DODGE_RAM:
		setDodgeRam1996(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case DODGE_STRATUS:
		setDodgeStratus(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case VW_ABA:
		setVwAba(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
#if EFI_UNIT_TEST
	case TEST_ISSUE_366_BOTH:
		setTestEngineIssue366both(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case TEST_ISSUE_366_RISE:
		setTestEngineIssue366rise(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
#else
	case TEST_ISSUE_366_BOTH:
	case TEST_ISSUE_366_RISE:
#endif

	case TEST_ENGINE:
		setTestEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_2003:
		setMazdaMiata2003EngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_2003_NA_RAIL:
		setMazdaMiata2003EngineConfigurationNaFuelRail(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case MAZDA_MIATA_2003_BOARD_TEST:
		setMazdaMiata2003EngineConfigurationBoardTest(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case PROMETHEUS_DEFAULTS:
		setPrometheusDefaults(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case TEST_ENGINE_VVT:
		setTestVVTEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case SACHS:
		setSachs(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case DAIHATSU:
		setDaihatsu(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case CAMARO_4:
		setCamaro4(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case CHEVY_C20_1973:
		set1973c20(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case TOYOTA_2JZ_GTE_VVTi:
		setToyota_2jz_vics(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case TOYOTA_JZS147:
		setToyota_jzs147EngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;
	case GEO_STORM:
		setGeoStormConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
		break;

	default:
		warning(CUSTOM_UNEXPECTED_ENGINE_TYPE, "Unexpected engine type: %d", engineType);
	}
	applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER_SUFFIX);
	// todo: eliminate triggerShape.operationMode?
	if (engineConfiguration->operationMode != TRIGGER_SHAPE(getOperationMode())) {
		warning(CUSTOM_ERR_OP_MODE, "operationMode %d/trigger mismatch %d",
				engineConfiguration->operationMode,
				TRIGGER_SHAPE(getOperationMode()));
	}

#if EFI_TUNER_STUDIO
	syncTunerStudioCopy();
#endif
}

void validateConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->adcVcc > 5.0f || engineConfiguration->adcVcc < 1.0f) {
		engineConfiguration->adcVcc = 3.0f;
	}

}

void applyNonPersistentConfiguration(Logging * logger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 256, "apply c");
	scheduleMsg(logger, "applyNonPersistentConfiguration()");
#endif

	assertEngineReference();

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	engine->triggerCentral.triggerShape.initializeTriggerShape(logger PASS_ENGINE_PARAMETER_SUFFIX);
#endif

#if EFI_FSIO || defined(__DOXYGEN__)
	applyFsioConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

void prepareShapes(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine->injectionEvents.addFuelEvents(PASS_ENGINE_PARAMETER_SIGNATURE);
}

#endif

float getRpmMultiplier(operation_mode_e mode) {
	if (mode == FOUR_STROKE_CAM_SENSOR) {
		return 0.5;
	} else if (mode == FOUR_STROKE_CRANK_SENSOR) {
		return 1;
	}
	return 1;
}

void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode) {
	engineConfiguration->operationMode = mode;
}

operation_mode_e getOperationMode(engine_configuration_s const *engineConfiguration) {
	return engineConfiguration->operationMode;
}

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration) {
	/**
	 * VBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
}

void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration) {
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
	boardConfiguration->joystickCenterPin = GPIOC_8;
	boardConfiguration->joystickAPin = GPIOD_10;
	boardConfiguration->joystickBPin = GPIO_UNASSIGNED;
	boardConfiguration->joystickCPin = GPIO_UNASSIGNED;
	boardConfiguration->joystickDPin = GPIOD_11;
}

void copyTargetAfrTable(fuel_table_t const source, afr_table_t destination) {
	// todo: extract a template!
	for (int loadIndex = 0; loadIndex < FUEL_LOAD_COUNT; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
			destination[loadIndex][rpmIndex] = AFR_STORAGE_MULT * source[loadIndex][rpmIndex];
		}
	}
}

void copyFuelTable(fuel_table_t const source, fuel_table_t destination) {
	// todo: extract a template!
	for (int loadIndex = 0; loadIndex < FUEL_LOAD_COUNT; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < FUEL_RPM_COUNT; rpmIndex++) {
			destination[loadIndex][rpmIndex] = source[loadIndex][rpmIndex];
		}
	}
}

void copyTimingTable(ignition_table_t const source, ignition_table_t destination) {
	// todo: extract a template!
	for (int k = 0; k < IGN_LOAD_COUNT; k++) {
		for (int rpmIndex = 0; rpmIndex < IGN_RPM_COUNT; rpmIndex++) {
			destination[k][rpmIndex] = source[k][rpmIndex];
		}
	}
}

