/**
 * @file	mazda_626.cpp
 * MAZDA_626
 * set_engine_type 28
 *
 * @date Jan 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "mazda_626.h"
#include "engine_math.h"
#include "honda_accord.h"
#include "custom_engine.h"
#include "allsensors.h"

EXTERN_ENGINE;

void setMazda626EngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);

	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
	engineConfiguration->trigger.type = TT_MAZDA_DOHC_1_4;

	engineConfiguration->digitalChartSize = 150;

	boardConfiguration->analogChartMode = AC_TRIGGER;
	engineConfiguration->analogChartFrequency = 2;

	// set_global_trigger_offset_angle -42
	engineConfiguration->globalTriggerAngleOffset = -42;

	// set_cranking_timing_angle 15
	engineConfiguration->crankingTimingAngle = 15;
	engineConfiguration->crankingChargeAngle = 70;

	// set_whole_fuel_map 9
	setWholeFuelMap(engineConfiguration, 9);

// set_whole_timing_map 10
	setWholeTimingTable(engineConfiguration, 10);

	// http://i.imgur.com/fclVzvu.jpg
	setCommonNTCSensor(&engineConfiguration->clt);
	// http://i.imgur.com/2hI67yW.jpg
	setThermistorConfiguration(&engineConfiguration->iat, 20, 33000, 50, 9000, 90.0, 2700);

	commonFrankensoAnalogInputs(engineConfiguration);
	engineConfiguration->hasTpsSensor = false;
//	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->mafAdcChannel = EFI_ADC_0;

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;


// todo	engineConfiguration->afr.hwChannel = EFI_ADC_3;
	initEgoSensor(&engineConfiguration->afr, ES_Innovate_MTX_L);
}
