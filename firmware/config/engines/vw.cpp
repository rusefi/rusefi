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
void setVwAba() {
	setFrankensoConfiguration();

	setWholeTimingTable_d(20);
	// set cranking_timing_angle 10
	engineConfiguration->crankingTimingAngle = 10;

	// set global_trigger_offset_angle 93
	engineConfiguration->globalTriggerAngleOffset = 93;


	setCrankOperationMode();
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_60_2;

	engineConfiguration->mafAdcChannel = EFI_ADC_1;


	//Base engine setting
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 2.0;
	engineConfiguration->injector.flow = 320; // 30lb/h
	// set algorithm 3
	setAlgorithm(LM_SPEED_DENSITY);
	engineConfiguration->map.sensor.type = MT_GM_3_BAR;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	engineConfiguration->ignitionPins[0] = Gpio::E14; // Frankenso high side - pin 1G
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;

	engineConfiguration->wastegatePositionSensor = EFI_ADC_4; // PA4

	float mapRange = 110;

	setEgoSensor(ES_PLX);
	setFuelTablesLoadBin(20, mapRange);
	setLinearCurve(config->ignitionLoadBins, 20, mapRange);

	engineConfiguration->isSdCardEnabled = false;
	engineConfiguration->tpsMin = 740;
	engineConfiguration->tpsMax = 135;
}

void setHellen121Vag_5_cyl() {
	engineConfiguration->cylindersCount = 5;
	engineConfiguration->displacement = 2.5;
	engineConfiguration->firingOrder = FO_1_2_4_5_3;
}

void setHellen121Vag_vr6_cyl() {
	engineConfiguration->cylindersCount = 6;
	engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
}

void setHellen121Vag_v6_cyl() {
	engineConfiguration->cylindersCount = 6;
	engineConfiguration->displacement = 2.7;

	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = Gpio::A7; // 87a

	engineConfiguration->firingOrder = FO_1_4_3_6_2_5;
}

void setHellen121Vag_8_cyl() {
	engineConfiguration->cylindersCount = 8;
	engineConfiguration->displacement = 4.2;
	engineConfiguration->firingOrder = FO_1_5_4_8_6_3_7_2;
}
