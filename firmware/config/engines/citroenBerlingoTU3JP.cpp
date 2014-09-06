/**
 * @file	citroenBerlingoTU3JP.cpp
 *
 * CITROEN_TU3JP: engine_type 15
 * Board pin out for Frankenstein
 *
 * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
 *
 *
 * @date Apr 15, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "thermistors.h"
#include "citroenBerlingoTU3JP.h"

void setCitroenBerlingoTU3JPConfiguration(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->engineType = CITROEN_TU3JP;

	// base engine setting
	engineConfiguration->triggerConfig.triggerType = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 1.390;
	engineConfiguration->firingOrder = FO_1_THEN_3_THEN_4_THEN2;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;

	// rpm limiter
	engineConfiguration->rpmHardLimit = 5000;

	/**
	 * Outputs
	 */
	// Frankenstain low out #3: PE6  injector 1-4
	// Frankenstain low out #4: PC13 injector 2-3

	boardConfiguration->injectionPins[0] = GPIOE_6;
	boardConfiguration->injectionPins[1] = GPIOC_13;
	boardConfiguration->injectionPins[2] = GPIO_NONE;
	boardConfiguration->injectionPins[3] = GPIO_NONE;

//	boardConfiguration->ignitionPins[0] = GPIO_NONE;
//	boardConfiguration->ignitionPins[1] = GPIO_NONE;
//	boardConfiguration->ignitionPins[2] = GPIO_NONE;
//	boardConfiguration->ignitionPins[3] = GPIO_NONE;

	boardConfiguration->fuelPumpPin = GPIOB_9;
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

//	boardConfiguration->o2heaterPin = GPIOC_13;
//	boardConfiguration->logicAnalyzerPins[1] = GPIO_NONE;

	/**
	 * Inputs
	 */

	// See https://docs.google.com/spreadsheet/ccc?key=0Arl1FeMZcfisdEdGdUlHdWh6cVBoSzFIbkxqa1QtZ3c
	// Frankenstein analog input #1: PA1 adc1   *2.9    *0.6
	// Frankenstein analog input #2: PA3 adc3   *2.9    *1.0
	// Frankenstein analog input #3: PC3 adc13  *2.5    *0.005
	// Frankenstein analog input #4: PC1 adc11  *0.05   *1.5
	// Frankenstein analog input #5: PA0 adc0   *0.05   *0.01
	// Frankenstein analog input #6: PC2 adc12  *0.06   *1.2
	// Frankenstein analog input #7: PA4 adc4   *3.0    *0.4
	// Frankenstein analog input #8: PA2 adc2   *2.9    *0.9
	// Frankenstein analog input #9: PA6 adc6   *3.0    *6.0
	// Frankenstein analog input #10: PA7 adc7  *2.9    *2.35
	// Frankenstein analog input #11: PC4 adc14 *0.00   *0.00
	// Frankenstein analog input #12: PC5 adc15 *0.00   *0.00

	/**
	 * MAP <need tune>
	 */
	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->map.sensor.sensorType = MT_CUSTOM;
	engineConfiguration->map.sensor.customValueAt0 = 20;
	engineConfiguration->map.sensor.customValueAt5 = 200;
	/**
	 * TPS <need tune>
	 */
	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 750;
	/**
	 * IAT
	 */
	engineConfiguration->iatAdcChannel = EFI_ADC_13;
	setThermistorConfiguration(&engineConfiguration->iatThermistorConf, -20.0, 15600.0, 23.0, 2250.0, 92.0, 240.0);
	engineConfiguration->iatThermistorConf.bias_resistor = 2700; // same as OEM ECU
	/**
	* CLT
	*/
	engineConfiguration->cltAdcChannel = EFI_ADC_15;
	setThermistorConfiguration(&engineConfiguration->cltThermistorConf, -20.0, 28680.0, 25.0, 2796.0, 100.0, 177.0);
	engineConfiguration->cltThermistorConf.bias_resistor = 2700; // eah winter is hard, sensor as LADA Samara :)
	/**
	 * vBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;
	engineConfiguration->vbattDividerCoeff = ((float) (2.7 + 10)) / 2.7 * 2;
}

