/**
 * @file    ford_aspire.cpp
 * @brief	1996 Ford Aspire default engine configuration
 *
 * FORD_ASPIRE_1996 = 3
 * set engine_type 3
 *
 * @date Sep 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


#include "ford_aspire.h"

#include "allsensors.h"
#include "advance_map.h"

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const int8_t default_aspire_timing_table[16][16] = {
/* RPM					800.000000	1213.333374	1626.666748	2040.000000	2453.333496	2866.666748	3280.000000	3693.333496	4106.666992	4520.000000	4933.333496	5346.666992	5760.000000	6173.333496	6586.666992	7000.000000*/
/* Load 1.200000 */{	-0,	+7,	+16,	+23,	+29,	+31,	+32,	+30,	+30,	+26,	+24,	+25,	+25,	+25,	+25,	+25},
/* Load 1.413333 */{	-0,	+7,	+16,	+23,	+29,	+31,	+32,	+30,	+30,	+26,	+24,	+24,	+29,	+25,	+25,	+25},
/* Load 1.626667 */{	-0,	+7,	+16,	+23,	+29,	+31,	+32,	+30,	+30,	+26,	+24,	+25,	+24,	+26,	+26,	+26},
/* Load 1.840000 */{	-0,	+7,	+16,	+23,	+29,	+30,	+32,	+30,	+30,	+26,	+24,	+24,	+25,	+35,	+35,	+35},
/* Load 2.053333 */{	-0,	+7,	+16,	+23,	+29,	+31,	+32,	+30,	+30,	+25,	+24,	+24,	+24,	+24,	+24,	+24},
/* Load 2.266667 */{	+1,	+7,	+16,	+23,	+29,	+31,	+32,	+30,	+29,	+26,	+24,	+27,	+25,	+25,	+25,	+25},
/* Load 2.480000 */{	-1,	+10,	+16,	+22,	+28,	+30,	+32,	+30,	+30,	+26,	+24,	+24,	+24,	+25,	+25,	+25},
/* Load 2.693333 */{	-9,	+10,	+11,	+19,	+25,	+27,	+32,	+30,	+30,	+27,	+25,	+22,	+24,	+25,	+25,	+25},
/* Load 2.906667 */{	-9,	-5,	-2,	+17,	+17,	+20,	+25,	+27,	+27,	+27,	+25,	+25,	+26,	+27,	+27,	+27},
/* Load 3.120000 */{	-9,	-5,	-2,	+0,	+12,	+16,	+21,	+27,	+25,	+24,	+24,	+25,	+26,	+31,	+31,	+31},
/* Load 3.333333 */{	-9,	-5,	-2,	+0,	+2,	+3,	+11,	+18,	+21,	+21,	+21,	+22,	+25,	+25,	+25,	+25},
/* Load 3.546667 */{	-9,	-5,	-2,	-0,	+3,	+3,	+6,	+7,	+19,	+20,	+19,	+21,	+21,	+21,	+21,	+21},
/* Load 3.760000 */{	-9,	-5,	-2,	+0,	+2,	+3,	+6,	+8,	+8,	+9,	+11,	+20,	+20,	+20,	+20,	+20},
/* Load 3.973333 */{	-9,	-5,	-2,	-0,	+2,	+3,	+6,	+8,	+8,	+8,	+11,	+11,	+13,	+12,	+12,	+12},
/* Load 4.186667 */{	-9,	-5,	-2,	-0,	+2,	+3,	+6,	+6,	+8,	+9,	+11,	+11,	+11,	+12,	+12,	+12},
/* Load 4.400000 */{	-0,	-5,	-0,	-0,	-0,	-0,	-1,	+7,	-1,	-1,	-2,	-2,	-2,	+12,	+12,	+12}
};
#endif

static void setDefaultAspireMaps(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setTimingLoadBin(1.2, 4.4 PASS_CONFIG_PARAMETER_SUFFIX);
	setTimingRpmBin(800, 7000 PASS_CONFIG_PARAMETER_SUFFIX);

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	copyTable(config->ignitionTable, default_aspire_timing_table);
#endif
}

void setFordAspireEngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 750;

	engineConfiguration->rpmHardLimit = 7000;

	/**
	 * 18K Ohm @ -20C
	 * 2.1K Ohm @ 24C
	 * 1K Ohm @ 49C
	 */
	// that's my custom resistor value!
	engineConfiguration->clt.config = {-20, 23.8889, 48.8889, 18000, 2100, 1000, 3300};

//	engineConfiguration->ignitionPinMode = OM_INVERTED;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 1.3;
	// Denso 195500-2110
	engineConfiguration->injector.flow = 119.8;

	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->globalTriggerAngleOffset = 175;
	engineConfiguration->extraInjectionOffset = 54 + 360;

	setDefaultAspireMaps(PASS_CONFIG_PARAMETER_SIGNATURE);
	// set_cranking_rpm 550
	engineConfiguration->cranking.rpm = 550;
	// set cranking_timing_angle 37
	engineConfiguration->crankingTimingAngle = -37;

	setSingleCoilDwell(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->trigger.type = TT_FORD_ASPIRE;

	engineConfiguration->triggerInputDebugPins[0] = GPIOC_15;

	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;

	engineConfiguration->HD44780width = 20;
	engineConfiguration->HD44780height = 4;

	// Frankenstein analog input #1: adc1
	// Frankenstein analog input #2: adc3
	// Frankenstein analog input #3: adc13
	// Frankenstein analog input #4: adc11
	// Frankenstein analog input #5: adc
	// Frankenstein analog input #6: adc
	// Frankenstein analog input #7: adc
	// Frankenstein analog input #8: adc
	// Frankenstein analog input #9: adc
	// Frankenstein analog input #10: adc
	// Frankenstein analog input #11: adc
	// Frankenstein analog input #12: adc


	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
//	engineConfiguration->iat.adcChannel =

	engineConfiguration->map.sensor.type = MT_DENSO183;

	engineConfiguration->engineSnifferRpmThreshold = 13000;
	engineConfiguration->sensorSnifferRpmThreshold = 13000;
}
