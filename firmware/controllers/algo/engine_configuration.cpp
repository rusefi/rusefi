/**
 * @file	engine_configuration.cpp
 * @brief	Utility method related to the engine configuration data structure.
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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

#include "custom_engine.h"
#include "acura_rsx.h"
#include "audi_aan.h"
#include "bmw_e34.h"
#include "dodge_neon.h"
#include "ford_aspire.h"
#include "ford_fiesta.h"
#include "ford_1995_inline_6.h"
#include "snow_blower.h"
#include "nissan_primera.h"
#include "honda_accord.h"
#include "GY6_139QMB.h"
#include "mazda_miata_nb.h"
#include "mazda_323.h"
#include "mazda_626.h"
#include "saturn_ion.h"
#include "MiniCooperR50.h"
#include "mazda_miata.h"
#include "citroenBerlingoTU3JP.h"
#include "rover_v8.h"
#include "mitsubishi.h"
#include "subaru.h"
#include "test_engine.h"
#include "sachs.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

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

int getGlobalConfigurationVersion(void) {
	return globalConfigurationVersion;
}

void incrementGlobalConfigurationVersion(void) {
	globalConfigurationVersion++;
	// todo invoke engine->configurationListeners.
}

/**
 * @brief Sets the same dwell time across the whole getRpm() range
 */
void setConstantDwell(engine_configuration_s *engineConfiguration, float dwellMs) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		engineConfiguration->sparkDwellBins[i] = 1000 * i;
		engineConfiguration->sparkDwell[i] = dwellMs;
	}
}

void setMap(fuel_table_t table, float value) {
	for (int l = 0; l < FUEL_LOAD_COUNT; l++) {
		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
			table[l][r] = value;
		}
	}
}

static void setWholeVEMap(engine_configuration_s *engineConfiguration, float value) {
	setMap(engineConfiguration->veTable, value);
}

void setWholeFuelMap(engine_configuration_s *engineConfiguration, float value) {
	setMap(engineConfiguration->fuelTable, value);
}

void setWholeTimingTable(engine_configuration_s *engineConfiguration, float value) {
	// todo: table helper?
	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
		for (int r = 0; r < IGN_RPM_COUNT; r++) {
			engineConfiguration->ignitionTable[l][r] = value;
		}
	}
}

/**
 * @brief	Global default engine configuration
 * This method sets the default global engine configuration. These values are later overridden by engine-specific defaults
 * and the settings saves in flash memory.
 */
void setDefaultConfiguration(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	memset(engineConfiguration, 0, sizeof(engine_configuration_s));
	memset(boardConfiguration, 0, sizeof(board_configuration_s));

	setDetaultVETable(engineConfiguration);

	setBosch0280218037(engineConfiguration);

	engineConfiguration->injector.lag = 1.0;

	engineConfiguration->acCutoffLowRpm = 700;
	engineConfiguration->acCutoffHighRpm = 5000;


	for (int i = 0; i < IAT_CURVE_SIZE; i++) {
		engineConfiguration->iatFuelCorrBins[i] = -40 + i * 10;
		engineConfiguration->iatFuelCorr[i] = 1; // this correction is a multiplier
	}

	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		engineConfiguration->cltFuelCorrBins[i] = -40 + i * 10;
		engineConfiguration->cltFuelCorr[i] = 1; // this correction is a multiplier
	}

	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -30, 1.5);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -20, 1.42);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -10, 1.36);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 0, 1.28);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 10, 1.19);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 20, 1.12);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 30, 1.10);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 40, 1.06);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 50, 1.06);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 60, 1.03);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 70, 1.01);

	for (int i = 0; i < VBAT_INJECTOR_CURVE_SIZE; i++) {
		engineConfiguration->injector.battLagCorrBins[i] = 12 - VBAT_INJECTOR_CURVE_SIZE / 2 + i;
		engineConfiguration->injector.battLagCorr[i] = 0; // zero extra time by default
	}

	setConstantDwell(engineConfiguration, 4); // 4ms is global default dwell
	engineConfiguration->useConstantDwellDuringCranking = false;

	setFuelLoadBin(engineConfiguration, 1.2, 4.4);
	setFuelRpmBin(engineConfiguration, 800, 7000);
	setTimingLoadBin(engineConfiguration, 1.2, 4.4);
	setTimingRpmBin(engineConfiguration, 800, 7000);

	setTableBin2(engineConfiguration->map.samplingAngleBins, MAP_ANGLE_SIZE, 800, 7000, 1);
	setTableBin2(engineConfiguration->map.samplingAngle, MAP_ANGLE_SIZE, 100, 130, 1);
	setTableBin2(engineConfiguration->map.samplingWindowBins, MAP_ANGLE_SIZE, 800, 7000, 1);
	setTableBin2(engineConfiguration->map.samplingWindow, MAP_ANGLE_SIZE, 50, 50, 1);

	// set_whole_timing_map 3
	setWholeFuelMap(engineConfiguration, 3);
	setWholeVEMap(engineConfiguration, 0.8);
	setMap(engineConfiguration->afrTable, 14.7);
	setMap(engineConfiguration->injectionPhase, 0);

	setThermistorConfiguration(&engineConfiguration->clt, 0, 9500, 23.8889, 2100, 48.8889, 1000);
	engineConfiguration->clt.bias_resistor = 1500;

	setThermistorConfiguration(&engineConfiguration->iat, 32, 9500, 75, 2100, 120, 1000);
// todo: this value is way off! I am pretty sure temp coeffs are off also
	engineConfiguration->iat.bias_resistor = 2700;

	engineConfiguration->rpmHardLimit = 7000;
	engineConfiguration->cranking.rpm = 550;

	engineConfiguration->crankingFuelCoef[0] = 5; // base cranking fuel adjustment coefficient
	engineConfiguration->crankingFuelBins[0] = -20; // temperature in C
	engineConfiguration->crankingFuelCoef[1] = 3.7;
	engineConfiguration->crankingFuelBins[1] = -10;
	engineConfiguration->crankingFuelCoef[2] = 2.6;
	engineConfiguration->crankingFuelBins[2] = 5;
	engineConfiguration->crankingFuelCoef[3] = 2.4;
	engineConfiguration->crankingFuelBins[3] = 20;
	engineConfiguration->crankingFuelCoef[4] = 2.1;
	engineConfiguration->crankingFuelBins[4] = 35;
	engineConfiguration->crankingFuelCoef[5] = 1.8;
	engineConfiguration->crankingFuelBins[5] = 50;
	engineConfiguration->crankingFuelCoef[6] = 1.5;
	engineConfiguration->crankingFuelBins[6] = 65;
	engineConfiguration->crankingFuelCoef[7] = 1;
	engineConfiguration->crankingFuelBins[7] = 90;

	engineConfiguration->crankingCycleCoef[0] = 2;
	engineConfiguration->crankingCycleBins[0] = 10;
	engineConfiguration->crankingCycleCoef[1] = 1;
	engineConfiguration->crankingCycleBins[1] = 31;
	engineConfiguration->crankingCycleCoef[2] = 1;
	engineConfiguration->crankingCycleBins[2] = 52;
	engineConfiguration->crankingCycleCoef[3] = 0.5;
	engineConfiguration->crankingCycleBins[3] = 73;
	engineConfiguration->crankingCycleCoef[4] = 0.5;
	engineConfiguration->crankingCycleBins[4] = 74;
	engineConfiguration->crankingCycleCoef[5] = 0.5;
	engineConfiguration->crankingCycleBins[5] = 75;
	engineConfiguration->crankingCycleCoef[6] = 0.5;
	engineConfiguration->crankingCycleBins[6] = 76;
	engineConfiguration->crankingCycleCoef[7] = 0.5;
	engineConfiguration->crankingCycleBins[7] = 77;

	engineConfiguration->cranking.baseFuel = 5;

	engineConfiguration->analogInputDividerCoefficient = 2;

	engineConfiguration->crankingChargeAngle = 70;
	engineConfiguration->timingMode = TM_DYNAMIC;
	engineConfiguration->fixedModeTiming = 50;

	// performance optimization
	boardConfiguration->analogChartMode = AC_OFF;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_4;

	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->globalTriggerAngleOffset = 0;
	engineConfiguration->injectionAngle = 0;
	engineConfiguration->ignitionBaseAngle = 0;
	engineConfiguration->overrideCrankingIgnition = true;
	engineConfiguration->analogChartFrequency = 20;

	engineConfiguration->algorithm = LM_PLAIN_MAF;

	engineConfiguration->vbattDividerCoeff = ((float) (15 + 65)) / 15;

	engineConfiguration->fanOnTemperature = 75;
	engineConfiguration->fanOffTemperature = 70;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
	engineConfiguration->tpsErrorLowValue = convertVoltageTo10bitADC(0.2);
	engineConfiguration->tpsErrorHighValue = convertVoltageTo10bitADC(6);

	engineConfiguration->canNbcType = CAN_BUS_NBC_BMW;
	engineConfiguration->canSleepPeriod = 50;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = false;

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2;
	/**
	 * By the way http://users.erols.com/srweiss/tableifc.htm has a LOT of data
	 */
	engineConfiguration->injector.flow = 200;

	engineConfiguration->displayMode = DM_HD44780;

	engineConfiguration->logFormat = LF_NATIVE;
	engineConfiguration->directSelfStimulation = false;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;

	engineConfiguration->HD44780width = 20;
	engineConfiguration->HD44780height = 4;

	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
	engineConfiguration->vbattAdcChannel = EFI_ADC_5;
	engineConfiguration->cltAdcChannel = EFI_ADC_6;
	engineConfiguration->iatAdcChannel = EFI_ADC_7;
	engineConfiguration->mafAdcChannel = EFI_ADC_0;
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->cylinderBore = 87.5;

	initEgoSensor(&engineConfiguration->afr, ES_BPSX_D1);

	engineConfiguration->globalFuelCorrection = 1;

	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->baroSensor.type = MT_CUSTOM;
	engineConfiguration->baroSensor.valueAt0 = 0;
	engineConfiguration->baroSensor.valueAt5 = 500;

	engineConfiguration->diffLoadEnrichmentCoef = 1;

	engineConfiguration->hasMapSensor = true;
	engineConfiguration->hasMafSensor = true;
	engineConfiguration->hasTpsSensor = true;
	engineConfiguration->hasAfrSensor = true;
	engineConfiguration->hasCltSensor = true;
	engineConfiguration->hasBaroSensor = false;
	engineConfiguration->isDigitalChartEnabled = true;

	boardConfiguration->idleSolenoidFrequency = 200;
	boardConfiguration->idleSolenoidPwm = 0.5;
	engineConfiguration->targetIdleRpm = 1200;
//	engineConfiguration->idleMode = IM_AUTO;
	engineConfiguration->idleMode = IM_MANUAL;

	boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->acRelayPinMode = OM_DEFAULT;

	boardConfiguration->alternatorControlPin = GPIO_UNASSIGNED;
	boardConfiguration->alternatorControlPinMode = OM_DEFAULT;

	engineConfiguration->acSwitchAdc = EFI_ADC_NONE;

#if EFI_PROD_CODE
	engineConfiguration->digitalChartSize = 300;
#else
	// need more events for automated test
	engineConfiguration->digitalChartSize = 400;
#endif

	engineConfiguration->engineCycle = 720;

	engineConfiguration->primingSquirtDurationMs = 5;

	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isCylinderCleanupEnabled = true;
	engineConfiguration->secondTriggerChannelEnabled = true;

	engineConfiguration->isMapAveragingEnabled = true;
	engineConfiguration->isMilEnabled = true;
	engineConfiguration->isFuelPumpEnabled = true;
	engineConfiguration->isTunerStudioEnabled = true;
	engineConfiguration->isWaveAnalyzerEnabled = true;
	engineConfiguration->isIdleThreadEnabled = true;

	engineConfiguration->acIdleRpmBump = 200;
	engineConfiguration->knockDetectionWindowStart = 35;
	engineConfiguration->knockDetectionWindowEnd = 135;

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
	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
		boardConfiguration->fsioPins[i] = GPIO_UNASSIGNED;
		boardConfiguration->le_formulas[i][0] = 0;
		engineConfiguration->fsioInputs[i] = GPIO_UNASSIGNED;
		engineConfiguration->fsioInputModes[i] = PI_DEFAULT;
	}
	for (int i = 0; i < JOYSTICK_PIN_COUNT; i++) {
		boardConfiguration->joystickPins[i] = GPIO_UNASSIGNED;
	}

	boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->mainRelayPinMode = OM_DEFAULT;
	boardConfiguration->idleValvePin = GPIO_UNASSIGNED;
	boardConfiguration->idleValvePinMode = OM_DEFAULT;
	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
	boardConfiguration->electronicThrottlePin1 = GPIO_UNASSIGNED;
	boardConfiguration->o2heaterPin = GPIO_UNASSIGNED;

	boardConfiguration->injectionPins[0] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPinMode = OM_DEFAULT;


	boardConfiguration->ignitionPins[0] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[6] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[7] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[10] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[11] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	boardConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;
	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	boardConfiguration->fanPin = GPIO_UNASSIGNED;
	boardConfiguration->fanPinMode = OM_DEFAULT;

	boardConfiguration->clutchDownPin = GPIO_UNASSIGNED;
	boardConfiguration->clutchDownPinMode = PI_PULLUP;
	engineConfiguration->clutchUpPin = GPIO_UNASSIGNED;
	engineConfiguration->clutchUpPinMode = PI_PULLUP;

	boardConfiguration->triggerSimulatorPins[0] = GPIOD_1;
	boardConfiguration->triggerSimulatorPins[1] = GPIOD_2;
	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;

	boardConfiguration->triggerSimulatorPinModes[0] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;

	setFrankenstein_01_LCD(boardConfiguration);

	strcpy(engineConfiguration->timingMultiplier, "1");
	strcpy(engineConfiguration->timingAdditive, "0");

	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_2;

	engineConfiguration->isGpsEnabled = false;
	boardConfiguration->gps_rx_pin = GPIOB_7;
	boardConfiguration->gps_tx_pin = GPIOB_6;

	memset(boardConfiguration->adcHwChannelEnabled, 0, sizeof(boardConfiguration->adcHwChannelEnabled));
	boardConfiguration->adcHwChannelEnabled[0] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[1] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[2] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[3] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[4] = ADC_FAST;
	// boardConfiguration->adcHwChannelEnabled[5] = ADC_SLOW; PA5 is also timer input
	boardConfiguration->adcHwChannelEnabled[6] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[7] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[11] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[12] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[13] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[14] = ADC_SLOW;

	boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIOA_5;
	boardConfiguration->logicAnalyzerPins[0] = GPIOA_8;
	boardConfiguration->logicAnalyzerPins[1] = GPIO_UNASSIGNED; // GPIOE_5 is a popular option (if available)
	boardConfiguration->logicAnalyzerPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->logicAnalyzerPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
	engineConfiguration->vehicleSpeedCoef = 100;

	boardConfiguration->logicAnalyzerMode[0] = false;
	boardConfiguration->logicAnalyzerMode[1] = false;

	boardConfiguration->idleThreadPeriod = 100;
	boardConfiguration->consoleLoopPeriod = 200;
	boardConfiguration->lcdThreadPeriod = 300;
	boardConfiguration->tunerStudioThreadPeriod = 300;
	boardConfiguration->generalPeriodicThreadPeriod = 50;

	boardConfiguration->boardTestModeJumperPin = GPIOB_0;

	boardConfiguration->canDeviceMode = CD_USE_CAN2;
	boardConfiguration->canTxPin = GPIOB_0;
	boardConfiguration->canRxPin = GPIOB_12;

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
	boardConfiguration->spi1sckPin = GPIOB_3;

	boardConfiguration->spi2mosiPin = GPIOB_15;
	boardConfiguration->spi2misoPin = GPIOB_14;
	boardConfiguration->spi2sckPin = GPIOB_13;

	boardConfiguration->spi3mosiPin = GPIOB_5;
	boardConfiguration->spi3misoPin = GPIOB_4;
	boardConfiguration->spi3sckPin = GPIOB_3;

	boardConfiguration->hip9011Gain = 1;

	boardConfiguration->isSdCardEnabled = true;
	boardConfiguration->isFastAdcEnabled = false;
	boardConfiguration->isEngineControlEnabled = true;

	boardConfiguration->tunerStudioSerialSpeed = TS_DEFAULT_SPEED;
}

void resetConfigurationExt(Logging * logger, engine_type_e engineType, Engine *engine) {
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
	/**
	 * Let's apply global defaults first
	 */
	setDefaultConfiguration(engineConfiguration, boardConfiguration);
#if EFI_SIMULATOR
	engineConfiguration->directSelfStimulation = true;
#endif /* */
	engineConfiguration->engineType = engineType;
	engineConfiguration->headerMagicValue = HEADER_MAGIC_NUMBER;
	/**
	 * And override them with engine-specific defaults
	 */
	switch (engineType) {
	case CUSTOM_ENGINE:
		setCustomEngineConfiguration(engineConfiguration);
		break;
	case ACURA_RSX:
		setAcuraRSX(engineConfiguration);
		break;
#if EFI_SUPPORT_DODGE_NEON || defined(__DOXYGEN__)
	case DODGE_NEON_1995:
		setDodgeNeon1995EngineConfiguration(engineConfiguration, boardConfiguration);
		break;
	case DODGE_NEON_2003:
		setDodgeNeonNGCEngineConfiguration(engineConfiguration, boardConfiguration);
		break;
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE || defined(__DOXYGEN__)
	case FORD_ASPIRE_1996:
		setFordAspireEngineConfiguration(engineConfiguration, boardConfiguration);
		break;
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)
	case FORD_FIESTA:
		setFordFiestaDefaultEngineConfiguration(engineConfiguration);
		break;
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA || defined(__DOXYGEN__)
	case NISSAN_PRIMERA:
		setNissanPrimeraEngineConfiguration(engineConfiguration);
		break;
#endif
	case HONDA_ACCORD_CD:
		setHondaAccordConfigurationThreeWires(engineConfiguration, boardConfiguration);
		break;
	case HONDA_ACCORD_CD_TWO_WIRES:
		setHondaAccordConfigurationTwoWires(engineConfiguration, boardConfiguration);
		break;
	case HONDA_ACCORD_CD_DIP:
		setHondaAccordConfigurationDip(engineConfiguration, boardConfiguration);
		break;
	case MITSU_4G93:
		setMitsubishiConfiguration(engineConfiguration, boardConfiguration);
		break;
#if EFI_SUPPORT_1995_FORD_INLINE_6 || defined(__DOXYGEN__)
	case FORD_INLINE_6_1995:
		setFordInline6(engineConfiguration, boardConfiguration);
		break;
#endif /* EFI_SUPPORT_1995_FORD_INLINE_6 */
	case GY6_139QMB:
		setGy6139qmbDefaultEngineConfiguration(engineConfiguration);
		break;
	case MAZDA_MIATA_NB:
		setMazdaMiataNbEngineConfiguration(engineConfiguration);
		break;
	case MAZDA_323:
		setMazda323EngineConfiguration(engineConfiguration);
		break;
	case MAZDA_626:
		setMazda626EngineConfiguration(engineConfiguration);
		break;
	case SATURN_ION_2004:
		setSaturnIonEngineConfiguration(engineConfiguration);
		break;
	case MINI_COOPER_R50:
		setMiniCooperR50(engineConfiguration);
		break;
	case FORD_ESCORT_GT:
		setFordEscortGt(engineConfiguration);
		break;
	case MIATA_1990:
		setMiata1990(engineConfiguration);
		break;
	case MIATA_1994_DEVIATOR:
		setMiata1994_d(engineConfiguration);
		break;
	case MIATA_1994_SPAGS:
		setMiata1994_s(engineConfiguration);
		break;
	case MIATA_1996:
		setMiata1996(engineConfiguration);
		break;
	case CITROEN_TU3JP:
		setCitroenBerlingoTU3JPConfiguration(engineConfiguration);
		break;
	case ROVER_V8:
		setRoverv8(engineConfiguration);
		break;
	case SUBARU_2003_WRX:
		setSubaru2003Wrx(engineConfiguration);
		break;
	case BMW_E34:
		setBmwE34(engineConfiguration);
		break;
	case TEST_ENGINE:
		setTestEngineConfiguration(engineConfiguration);
		break;
	case SACHS:
		setSachs(engineConfiguration);
		break;
	default:
		firmwareError("Unexpected engine type: %d", engineType);
	}
	applyNonPersistentConfiguration(logger, engine);

#if EFI_TUNER_STUDIO
	syncTunerStudioCopy();
#endif
}

engine_configuration2_s::engine_configuration2_s() {
}

void applyNonPersistentConfiguration(Logging * logger, Engine *engine) {
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
// todo: this would require 'initThermistors() to re-establish a reference, todo: fix
//	memset(engineConfiguration2, 0, sizeof(engine_configuration2_s));
#if EFI_PROD_CODE
	scheduleMsg(logger, "applyNonPersistentConfiguration()");
#endif
#if EFI_ENGINE_CONTROL
	initializeTriggerShape(logger, engineConfiguration, engine);
#endif
	if (engine->triggerShape.getSize() == 0) {
		firmwareError("triggerShape size is zero");
		return;
	}
	if (engine->triggerShape.getSize() == 0) {
		firmwareError("shaftPositionEventCount is zero");
		return;
	}
	engine->engineCycleEventCount = engine->triggerShape.getLength();
#if EFI_FSIO || defined(__DOXYGEN__)
	applyFsioConfiguration(PASS_ENGINE_PARAMETER_F);
#endif
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

void prepareShapes(Engine *engine) {
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	engine_configuration2_s *engineConfiguration2 = engine->engineConfiguration2;
	prepareOutputSignals(PASS_ENGINE_PARAMETER_F);

	// todo: looks like this is here only for unit tests. todo: remove
	initializeIgnitionActions(0, 0, &engineConfiguration2->ignitionEvents[0] PASS_ENGINE_PARAMETER);
}

#endif

void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode) {
	if (mode == FOUR_STROKE_CAM_SENSOR) {
		engineConfiguration->rpmMultiplier = 0.5;
	} else if (mode == FOUR_STROKE_CRANK_SENSOR) {
		engineConfiguration->rpmMultiplier = 1;
	}
}

operation_mode_e getOperationMode(engine_configuration_s const *engineConfiguration) {
	if (engineConfiguration->rpmMultiplier == 1)
		return FOUR_STROKE_CRANK_SENSOR;
	return FOUR_STROKE_CAM_SENSOR;
}

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration) {
	/**
	 * VBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
	engineConfiguration->bc.adcHwChannelEnabled[14] = ADC_SLOW;
}

void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration) {
	engineConfiguration->isJoystickEnabled = true;
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
	boardConfiguration->joystickCenterPin = GPIOC_8;
	boardConfiguration->joystickAPin = GPIOD_10;
	boardConfiguration->joystickBPin = GPIO_UNASSIGNED;
	boardConfiguration->joystickCPin = GPIOD_9;
	boardConfiguration->joystickDPin = GPIOD_11;
}

void copyFuelTable(fuel_table_t const source, fuel_table_t destination) {
	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
			destination[k][r] = source[k][r];
		}
	}
}

void copyTimingTable(ignition_table_t const source, ignition_table_t destination) {
	for (int k = 0; k < IGN_LOAD_COUNT; k++) {
		for (int r = 0; r < IGN_RPM_COUNT; r++) {
			destination[k][r] = source[k][r];
		}
	}
}

