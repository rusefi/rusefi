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
	engineConfiguration->globalTriggerAngleOffset = 144;
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 1.360;
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
	// Frankenstein analog input #1: PA1 adc1	MAP
	// Frankenstein analog input #2: PA3 adc3	TPS
	// Frankenstein analog input #3: PC3 adc13	IAT
	// Frankenstein analog input #4: PC1 adc11	CLT
	// Frankenstein analog input #5: PA0 adc0	vBatt
	// Frankenstein analog input #6: PC2 adc12
	// Frankenstein analog input #7: PA4 adc4
	// Frankenstein analog input #8: PA2 adc2
	// Frankenstein analog input #9: PA6 adc6
	// Frankenstein analog input #10: PA7 adc7
	// Frankenstein analog input #11: PC4 adc14
	// Frankenstein analog input #12: PC5 adc15

	/**
	 * MAP <BOSCH 0 261 230 057>
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_1;
	engineConfiguration->map.sensor.sensorType = MT_CUSTOM;
	engineConfiguration->map.sensor.customValueAt0 = 10;
	engineConfiguration->map.sensor.customValueAt5 = 110;
	/**
	 * TPS <MAGNETI MARELLI>
	 */
	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
	engineConfiguration->tpsMin = 433;
	engineConfiguration->tpsMax = 3248;
	/**
	 * IAT <OEM ECU>
	 */
	engineConfiguration->iatAdcChannel = EFI_ADC_13;
	setThermistorConfiguration(&engineConfiguration->iatThermistorConf, -20.0, 15600.0, 23.0, 2250.0, 92.0, 240.0);
	engineConfiguration->iatThermistorConf.bias_resistor = 2700;
	/**
	* CLT <LADA Samara>
	*/
	engineConfiguration->cltAdcChannel = EFI_ADC_11;
	setThermistorConfiguration(&engineConfiguration->cltThermistorConf, -20.0, 28680.0, 25.0, 2796.0, 100.0, 177.0);
	engineConfiguration->cltThermistorConf.bias_resistor = 2700;
	/**
	 * vBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;
	engineConfiguration->vbattDividerCoeff = ((float) (2.7 + 10)) / 2.7 * 2;
}
