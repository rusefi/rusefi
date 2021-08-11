/*
 * @file vw.cpp
 *
 * set engine_type 32
 *
 * @date May 24, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "vw.h"
#include "custom_engine.h"

// VW_ABA
// set engine_type 32
void setVwAba(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	setWholeTimingTable_d(20 PASS_CONFIG_PARAMETER_SUFFIX);
	// set cranking_timing_angle 10
	engineConfiguration->crankingTimingAngle = 10;

	engineConfiguration->isCylinderCleanupEnabled = true;

	// set global_trigger_offset_angle 93
	engineConfiguration->globalTriggerAngleOffset = 93;


	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	engineConfiguration->mafAdcChannel = EFI_ADC_1;


	//Base engine setting
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2.0;
	engineConfiguration->injector.flow = 320; // 30lb/h
	// set algorithm 3
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);
	engineConfiguration->map.sensor.type = MT_GM_3_BAR;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	engineConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->idlePositionSensor = EFI_ADC_3; // PA3
	engineConfiguration->wastegatePositionSensor = EFI_ADC_4; // PA4

	float mapRange = 110;

	setEgoSensor(ES_PLX PASS_CONFIG_PARAMETER_SUFFIX);
	setFuelTablesLoadBin(20, mapRange PASS_CONFIG_PARAMETER_SUFFIX);
	setTimingLoadBin(20, mapRange PASS_CONFIG_PARAMETER_SUFFIX);

	CONFIG(isSdCardEnabled) = false;
	engineConfiguration->tpsMin = 740;
	engineConfiguration->tpsMax = 135;
}

void setHellen121Vag_5_cyl(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 5;
	engineConfiguration->specs.displacement = 2.5;
	engineConfiguration->specs.firingOrder = FO_1_2_4_5_3;
}

void setHellen121Vag_vr6_cyl(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_5_3_6_2_4;
}

void setHellen121Vag_v6_cyl(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.displacement = 2.7;

	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = GPIOA_7; // 87a

	engineConfiguration->specs.firingOrder = FO_1_4_3_6_2_5;
}

void setHellen121Vag_8_cyl(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.displacement = 4.2;
	engineConfiguration->specs.firingOrder = FO_1_5_4_8_6_3_7_2;
}
