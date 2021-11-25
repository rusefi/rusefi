/**
 * @file	citroenBerlingoTU3JP.cpp
 *
 * CITROEN_TU3JP
 * set engine_type 15
 * Board pin out for Frankenstein
 *
 * @date Apr 15, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "citroenBerlingoTU3JP.h"
#include "custom_engine.h"

void setCitroenBerlingoTU3JPConfiguration() {
	setDefaultFrankensoConfiguration();

	/**
	 * Base engine setting
	 */
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->globalTriggerAngleOffset = 114;
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 1.360;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->rpmHardLimit = 5000;
	engineConfiguration->cranking.rpm = 600;

//	memcpy(config->ve2RpmBins, rpmSteps, sizeof(rpmSteps));

	/**
	 * Algorithm Alpha-N setting
	 */
	setAlgorithm(LM_ALPHA_N);
	setTimingRpmBin(800, 7000);

	/**
	 * Outputs
	 */

	// Frankenstein lo-side output #1: PC14 Igniter 1-4
	// Frankenstein lo-side output #2: PC15 Igniter 2-3
	// Frankenstein lo-side output #3: PE6  Injector 1-4
	// Frankenstein lo-side output #4: PC13 Injector 2-3
	// Frankenstein lo-side output #5: PE4
	// Frankenstein lo-side output #6: PE5
	// Frankenstein lo-side output #7: PE2
	// Frankenstein lo-side output #8: PE3	Tachometer
	// Frankenstein lo-side output #9: PE0	Fan
	// Frankenstein lo-side output #10: PE1	MIL
	// Frankenstein lo-side output #11: PB8	Main relay
	// Frankenstein lo-side output #12: PB9	Fuel pump

	engineConfiguration->ignitionPins[0] = GPIOC_14;
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOC_15;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->injector.flow = 137; //SIEMENS DEKA VAZ20734
	engineConfiguration->injectionPins[0] = GPIOE_6;
	engineConfiguration->injectionPins[1] = GPIOC_13;
	engineConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->fanPin = GPIOE_0;
	engineConfiguration->fanPinMode = OM_DEFAULT;
	engineConfiguration->fanOffTemperature = 95;
	engineConfiguration->fanOnTemperature = 99;

	engineConfiguration->malfunctionIndicatorPin = GPIOE_1;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	engineConfiguration->mainRelayPin = GPIOB_8;

	engineConfiguration->fuelPumpPin = GPIOB_9;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	setFrankenso_01_LCD(engineConfiguration);


//	engineConfiguration->o2heaterPin = GPIOC_13;
//	engineConfiguration->logicAnalyzerPins[1] = GPIO_UNASSIGNED;

	/**
	 * Inputs
	 */

	// See https://docs.google.com/spreadsheet/ccc?key=0Arl1FeMZcfisdEdGdUlHdWh6cVBoSzFIbkxqa1QtZ3c
	// Frankenstein analog input #1: PA1		adc1	MAP
	// Frankenstein analog input #2: PA3		adc3	TPS
	// Frankenstein analog input #3: PC3		adc13	IAT
	// Frankenstein analog input #4: PC1		adc11	CLT
	// Frankenstein analog input #5: PA0		adc0	vBatt
	// Frankenstein analog input #6: PC2		adc12	WBO
	// Frankenstein analog input #7: PA4		adc4
	// Frankenstein analog input #8: PA2		adc2
	// Frankenstein analog input #9: PA6		adc6
	// Frankenstein analog input #10: PA7		adc7
	// Frankenstein analog input #11: PC4		adc14
	// Frankenstein analog input #12: PC5|PA8	adc15	Speed Sensor

	/**
	 * MAP <BOSCH 0 261 230 057>
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_1;
	engineConfiguration->map.sensor.type = MT_CUSTOM;
	engineConfiguration->map.sensor.lowValue = 10;
	engineConfiguration->map.sensor.highValue = 105;
	engineConfiguration->mapHighValueVoltage = 5;
	/**
	 * TPS <MAGNETI MARELLI>
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->tpsMax = 102; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMin = 825; // convert 12to10 bit (ADC/4)
	/**
	 * IAT <OEM ECU>
	 */
	engineConfiguration->iat.adcChannel = EFI_ADC_13;
	engineConfiguration->iat.config = {-20.0, 23.0, 92.0, 15600.0, 2250.0, 240.0, 2660};
	/**
	* CLT <LADA Samara>
	*/
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
	engineConfiguration->clt.config = {-20.0, 25.0, 100.0, 28680.0, 2796.0, 177.0, 2660};
	/**
	 * vBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;
	engineConfiguration->vbattDividerCoeff = ((float) (2.6 + 10.1)) / 2.6 * 2;
	/**
	* WBO Innovate LC-1
	*/
	engineConfiguration->afr.hwChannel = EFI_ADC_12;
}
