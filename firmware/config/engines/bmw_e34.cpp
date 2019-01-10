/**
 * @file bmw_e34.cpp
 *
 * set engine_type 25
 *
 * Coil pairs: 1-6, 2-5, 3-4
 *
 * Injector groups: 1,2,3 & 4,5,6
 *
 * @date Nov 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "bmw_e34.h"
#include "thermistors.h"
#include "engine_math.h"

EXTERN_ENGINE
;

void setBmwE34(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// chartsize 450
	engineConfiguration->engineChartSize = 450;

//	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
//	engineConfiguration->trigger.type = TT_ONE_PLUS_TOOTHED_WHEEL_60_2;
//	engineConfiguration->injectionMode = IM_SEQUENTIAL;
//	boardConfiguration->triggerInputPins[0] = GPIOC_6;
//	boardConfiguration->triggerInputPins[1] = GPIOA_5;

//Base engine setting
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.displacement = 2.91;
	engineConfiguration->specs.firingOrder = FO_1_5_3_6_2_4;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->twoWireBatchInjection = true;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	boardConfiguration->tachOutputPin = GPIOC_8;

	// Trigger configuration
//	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	// set global_trigger_offset_angle 84
	engineConfiguration->globalTriggerAngleOffset = 84;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 30;
	engineConfiguration->trigger.customSkippedToothCount = 1;


	// Injection settings
	setFlatInjectorLag(1.15 PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->injector.flow = 750;

	// General settings
	boardConfiguration->tunerStudioSerialSpeed = 57600;
	engineConfiguration->rpmHardLimit = 7000;
	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER_SUFFIX);

	// for best performance at high RPM, we need to turn off 'Engine Sniffer' and 'Sensor Sniffer'
	boardConfiguration->sensorChartMode = SC_OFF;
	engineConfiguration->isEngineChartEnabled = false;

	engineConfiguration->isCylinderCleanupEnabled = false;
	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->isIgnitionEnabled = true;

	setConstantDwell(3 PASS_ENGINE_PARAMETER_SUFFIX); // a bit shorter dwell
	engineConfiguration->ignMathCalculateAtIndex = 14;

	engineConfiguration->mapAveragingSchedulingAtIndex = 6;

	// Cranking
	engineConfiguration->cranking.rpm = 600;
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->cranking.baseFuel = 7;
	// enable constant_dwell
	engineConfiguration->useConstantDwellDuringCranking = true;
	engineConfiguration->ignitionDwellForCrankingMs = 6;

	// Inputs configuration
	engineConfiguration->analogInputDividerCoefficient = 1.52;
	engineConfiguration->vbattDividerCoeff = 5.33;
	engineConfiguration->vbattAdcChannel = EFI_ADC_15; // PC5

	engineConfiguration->tpsAdcChannel = EFI_ADC_3;


	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;

	setWholeTimingTable(25 PASS_ENGINE_PARAMETER_SUFFIX);

	boardConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;

//	bc->isFastAdcEnabled = true;

	boardConfiguration->injectionPinMode = OM_INVERTED;
	boardConfiguration->injectionPins[0] = GPIOB_8; // #1
	boardConfiguration->injectionPins[1] = GPIOE_2; // #2
	boardConfiguration->injectionPins[2] = GPIOE_3; // #3
	boardConfiguration->injectionPins[3] = GPIOE_4; // #4
	boardConfiguration->injectionPins[4] = GPIOE_5; // #5
	boardConfiguration->injectionPins[5] = GPIOE_6; // #6

	boardConfiguration->ignitionPinMode = OM_INVERTED;

	boardConfiguration->ignitionPins[0] = GPIOB_5; // #1
	boardConfiguration->ignitionPins[2] = GPIOB_6; // #3
	boardConfiguration->ignitionPins[4] = GPIOB_7; // #5

	boardConfiguration->canRxPin = GPIO_UNASSIGNED;
	boardConfiguration->canTxPin = GPIO_UNASSIGNED;

	boardConfiguration->triggerErrorPin = GPIO_UNASSIGNED;

	// clutch up
	boardConfiguration->clutchUpPin = GPIOD_3;
	boardConfiguration->clutchUpPinMode = PI_PULLUP;

	// fuel pump
	boardConfiguration->fuelPumpPin = GPIOD_4;

	// idle
	boardConfiguration->idle.solenoidPin = GPIOC_14;
	boardConfiguration->idle.solenoidPinMode = OM_INVERTED;
	boardConfiguration->idle.solenoidFrequency = 300;
	boardConfiguration->manIdlePosition = 50; // set_idle_pwm 50

	// disable sd_card
	boardConfiguration->sdCardCsPin = GPIO_UNASSIGNED;
	boardConfiguration->is_enabled_spi_2 = false;
	boardConfiguration->is_enabled_spi_3 = false;
	boardConfiguration->max31855spiDevice = SPI_NONE;

	// turbocharger boost control solenoid: TODO output: GPIOE_6
	// water injection #1 TODO GPIOD_7
	// water injection #2 TODO GPIOE_2

	/**
	 * emulating the 60-0 trigger takes some resources, let's keep it slow by default
	 * rpm 200
	 */
	boardConfiguration->triggerSimulatorFrequency = 200;

	// Configurating sensors:

	// map
	engineConfiguration->map.sensor.type = MT_MPX4250;

	// thermistors
	setThermistorConfiguration(&engineConfiguration->clt, -10, 9300, 20, 2500, 80, 335);
	engineConfiguration->iat.config.bias_resistor = 2200;
	setThermistorConfiguration(&engineConfiguration->iat, -10, 9300, 20, 2500, 80, 335);
	engineConfiguration->clt.config.bias_resistor = 2200;

//	/**
//	 * This saves a couple of ticks in trigger emulation methods
//	 * TODO: add some smart logic to detect unneeded trigger simulation pins?
//	 * TODO: but probably not worth it
//	 */
//	bc->triggerSimulatorPins[1] = GPIO_UNASSIGNED;

	boardConfiguration->triggerSimulatorPins[0] = GPIOD_1;
	boardConfiguration->triggerSimulatorPins[1] = GPIOD_2;
	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
}
