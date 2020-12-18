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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "bmw_e34.h"
#include "thermistors.h"
#include "engine_math.h"

EXTERN_CONFIG
;

void setBmwE34(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
	// chartsize 450
	engineConfiguration->engineChartSize = 450;

//	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
//	engineConfiguration->trigger.type = TT_ONE_PLUS_TOOTHED_WHEEL_60_2;
//	engineConfiguration->injectionMode = IM_SEQUENTIAL;
//	engineConfiguration->triggerInputPins[0] = GPIOC_6;
//	engineConfiguration->triggerInputPins[1] = GPIOA_5;

//Base engine setting
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.displacement = 2.91;
	engineConfiguration->specs.firingOrder = FO_1_5_3_6_2_4;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->twoWireBatchInjection = true;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	engineConfiguration->tachOutputPin = GPIOC_8;

	// Trigger configuration
//	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	// set global_trigger_offset_angle 84
	engineConfiguration->globalTriggerAngleOffset = 84;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 30;
	engineConfiguration->trigger.customSkippedToothCount = 1;


	// Injection settings
	setFlatInjectorLag(1.15 PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->injector.flow = 750;

	// General settings
	engineConfiguration->tunerStudioSerialSpeed = 57600;
	engineConfiguration->rpmHardLimit = 7000;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	// for best performance at high RPM, we need to turn off 'Engine Sniffer' and 'Sensor Sniffer'
	engineConfiguration->sensorChartMode = SC_OFF;
	engineConfiguration->isEngineChartEnabled = false;

	engineConfiguration->isCylinderCleanupEnabled = false;
	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->isIgnitionEnabled = true;

	setConstantDwell(3 PASS_CONFIG_PARAMETER_SUFFIX); // a bit shorter dwell
	engineConfiguration->ignMathCalculateAtIndex = 14;

	engineConfiguration->mapAveragingSchedulingAtIndex = 6;

	// Cranking
	engineConfiguration->cranking.rpm = 600;
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	// enable constant_dwell
	engineConfiguration->useConstantDwellDuringCranking = true;
	engineConfiguration->ignitionDwellForCrankingMs = 6;

	// Inputs configuration
	engineConfiguration->analogInputDividerCoefficient = 1.52;
	engineConfiguration->vbattDividerCoeff = 5.33;
	engineConfiguration->vbattAdcChannel = EFI_ADC_15; // PC5

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;


	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;

	setWholeTimingTable(25);

	engineConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;

	engineConfiguration->canRxPin = GPIO_UNASSIGNED;
	engineConfiguration->canTxPin = GPIO_UNASSIGNED;

	engineConfiguration->triggerErrorPin = GPIO_UNASSIGNED;

	// clutch up
	engineConfiguration->clutchUpPin = GPIOD_3;
	engineConfiguration->clutchUpPinMode = PI_PULLUP;

	// fuel pump
	engineConfiguration->fuelPumpPin = GPIOD_4;

	// idle
	engineConfiguration->idle.solenoidPin = GPIOC_14;

	engineConfiguration->idle.solenoidPinMode = OM_INVERTED;

	engineConfiguration->idle.solenoidFrequency = 300;
	engineConfiguration->manIdlePosition = 50; // set_idle_pwm 50


	// turbocharger boost control solenoid: TODO output: GPIOE_6
	// water injection #1 TODO GPIOD_7
	// water injection #2 TODO GPIOE_2

	/**
	 * emulating the 60-0 trigger takes some resources, let's keep it slow by default
	 * rpm 200
	 */
	engineConfiguration->triggerSimulatorFrequency = 200;

	// Configurating sensors:

	// map
	engineConfiguration->map.sensor.type = MT_MPX4250;

	// thermistors
	engineConfiguration->clt.config = {-10, 20, 80, 9300, 2500, 335, 2200};
	engineConfiguration->iat.config = {-10, 20, 80, 9300, 2500, 335, 2200};
}
