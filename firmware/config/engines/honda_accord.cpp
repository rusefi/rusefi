/**
 * @file	honda_accord.cpp
 *
 * 1995 Honda Accord EX
 * http://rusefi.com/wiki/index.php?title=Vehicle:Honda_Accord_1995
 * http://rusefi.com/forum/viewtopic.php?f=3&t=621
 *
 * engine_type 6
 *
 * @date Jan 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine_configuration.h"
#include "trigger_decoder.h"

void setHondaAccordConfiguration(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->engineType = HONDA_ACCORD;

	engineConfiguration->map.sensor.sensorType = MT_HONDA3BAR;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 2.156;

	// Keihin 06164-P0A-A00
	engineConfiguration->injectorFlow = 248;


	engineConfiguration->algorithm = LM_SPEED_DENSITY;


	engineConfiguration->crankingSettings.coolantTempMaxC = 65; // 8ms at 65C
	engineConfiguration->crankingSettings.fuelAtMaxTempMs = 8;

	engineConfiguration->crankingSettings.coolantTempMinC = 0; // 20ms at 0C
	engineConfiguration->crankingSettings.fuelAtMinTempMs = 15;


	memset(boardConfiguration->adcHwChannelEnabled, 0, sizeof(boardConfiguration->adcHwChannelEnabled));
	boardConfiguration->adcHwChannelEnabled[0] = ADC_FAST; // ADC0 - PA0 - MAP
	boardConfiguration->adcHwChannelEnabled[1] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[2] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[3] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[4] = ADC_SLOW;

	boardConfiguration->adcHwChannelEnabled[6] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[7] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[11] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[12] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[13] = ADC_SLOW;


	engineConfiguration->map.sensor.sensorType = MT_MPX4250;
	engineConfiguration->map.sensor.hwChannel = 0;


}

