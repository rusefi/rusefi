/**
 * @file	sachs.cpp
 *
 * set engine_type 29
 * http://rusefi.com/forum/viewtopic.php?f=3&t=396
 *
 * @date Jan 26, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "sachs.h"
#include "allsensors.h"
#include "engine_math.h"
#include "table_helper.h"

void setSachs(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->specs.displacement = 0.1; // 100cc
	engineConfiguration->specs.cylindersCount = 1;

	setOperationMode(engineConfiguration, TWO_STROKE);
	engineConfiguration->specs.firingOrder = FO_1;
	engineConfiguration->engineChartSize = 400;

	 // set injection_offset 0
	engineConfiguration->extraInjectionOffset = 0;

	setEgoSensor(ES_Innovate_MTX_L PASS_CONFIG_PARAMETER_SUFFIX);

	/**
	 * 50/2 trigger
	 */
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 50;
	engineConfiguration->trigger.customSkippedToothCount = 2;

	engineConfiguration->useSerialPort = false;

	// Frankenstein analog input #1: PA1 adc1 MAP
	// Frankenstein analog input #2: PA3 adc3 TPS
	// Frankenstein analog input #3: PC3 adc13 IAT
	// Frankenstein analog input #4: PC1 adc11 CLT
	// Frankenstein analog input #5: PA0 adc0 O2
	// Frankenstein analog input #6: PC2 adc12
	// Frankenstein analog input #7: PA4 adc4
	// Frankenstein analog input #8: PA2 adc2
	// Frankenstein analog input #9: PA6 adc6
	// Frankenstein analog input #10: PA7 adc7
	// Frankenstein analog input #11: PC4 adc14
	// Frankenstein analog input #12: PC5 adc15

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;

	/**
	 * TPS 0% 0.9v
	 * TPS 100% 2.34v
	 */
	engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);


	// Frankenstein: low side - out #1: PC14
	// Frankenstein: low side - out #2: PC15
	// Frankenstein: low side - out #3: PE6
	// Frankenstein: low side - out #4: PC13
	// Frankenstein: low side - out #5: PE4
	// Frankenstein: low side - out #6: PE5
	// Frankenstein: low side - out #7: PE2
	// Frankenstein: low side - out #8: PE3
	// Frankenstein: low side - out #9: PE0
	// Frankenstein: low side - out #10: PE1
	// Frankenstein: low side - out #11: PB8
	// Frankenstein: low side - out #12: PB9

	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;

	engineConfiguration->injectionPins[0] = GPIOC_15;

	engineConfiguration->fuelPumpPin = GPIOE_6;

	// todo: extract a method? figure out something smarter
	setTimingRpmBin(800, 15000 PASS_CONFIG_PARAMETER_SUFFIX);
	setLinearCurve(config->veRpmBins, 15000, 7000, 1);
	setLinearCurve(config->lambdaRpmBins, 15000, 7000, 1);

	engineConfiguration->hasFrequencyReportingMapSensor = true;
	engineConfiguration->frequencyReportingMapInputPin = GPIOC_6;
	engineConfiguration->mapFrequency100Kpa = 159;
	engineConfiguration->mapFrequency0Kpa = 80;
}
