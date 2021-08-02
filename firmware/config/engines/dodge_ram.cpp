/*
 * @file dodge_ram.cpp
 *
 * set engine_type 31
 *
 * @date Apr 22, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "dodge_ram.h"
#include "custom_engine.h"
#include "allsensors.h"
#include "engine_math.h"

void setDodgeRam1996(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->trigger.type = TT_DODGE_RAM;
	engineConfiguration->injector.flow = 243.6; // 23.2lb/h
	engineConfiguration->map.sensor.type = MT_DODGE_NEON_2003;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	//Base engine setting
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->specs.displacement = 5.2;

	engineConfiguration->triggerInputPins[0] = GPIOC_6;
	engineConfiguration->triggerInputPins[1] = GPIOA_5;

	engineConfiguration->injectionPins[0] = GPIOE_2;
	engineConfiguration->injectionPins[1] = GPIOB_9;
	engineConfiguration->injectionPins[2] = GPIOD_5;
	engineConfiguration->injectionPins[3] = GPIOB_8;

	engineConfiguration->injectionPins[4] = GPIOB_7;
	engineConfiguration->injectionPins[5] = GPIOE_3;
	engineConfiguration->injectionPins[6] = GPIOE_4;
	engineConfiguration->injectionPins[7] = GPIOD_3;

	engineConfiguration->ignitionPins[0] = GPIOC_9;
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->mainRelayPin = GPIOE_5;
	engineConfiguration->fuelPumpPin = GPIOE_6;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;

	engineConfiguration->engineChartSize = 450;


	// set global_trigger_offset_angle -50
	engineConfiguration->globalTriggerAngleOffset = -50;

	// set_global_fuel_correction 1

	// set_whole_timing_map 10
	setWholeTimingTable_d(10 PASS_CONFIG_PARAMETER_SUFFIX);


	engineConfiguration->alternatorControlPin = GPIOD_7;
	engineConfiguration->alternatorControl.pFactor = 22;

	engineConfiguration->idle.solenoidPin = GPIOC_13;
	engineConfiguration->idle.solenoidFrequency = 300;

	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
//	engineConfiguration->vbattDividerCoeff = ((float) (8.93 + 41.27)) / 8.93 * 2;
	engineConfiguration->vbattDividerCoeff = 9.6;

	setDodgeSensor(&engineConfiguration->clt, 2700);
	setDodgeSensor(&engineConfiguration->iat, 2700);

	engineConfiguration->useStepperIdle = true;
}
