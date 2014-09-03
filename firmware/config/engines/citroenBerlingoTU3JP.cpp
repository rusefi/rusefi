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

	// set thermistor
	setThermistorConfiguration(&engineConfiguration->cltThermistorConf, -40.0, 100700.0, 25.0, 2796.0, 100.0, 177.0);
	engineConfiguration->cltThermistorConf.bias_resistor = 2700; // eah winter is hard, sensor as LADA Samara :)

	setThermistorConfiguration(&engineConfiguration->iatThermistorConf, -20.0, 15600.0, 23.0, 2250.0, 92.0, 240.0);
	engineConfiguration->iatThermistorConf.bias_resistor = 2700; // same as OEM ECU

	/**
	 * Outputs
	 */
	// Frankenstain low out #3: PE6  injector 1-4
	// Frankenstain low out #4: PC13 injector 2-3

//	boardConfiguration->o2heaterPin = GPIOC_13;

	boardConfiguration->injectionPins[0] = GPIOE_6;
	boardConfiguration->injectionPins[1] = GPIOC_13;
	boardConfiguration->injectionPins[2] = GPIO_NONE;
	boardConfiguration->injectionPins[3] = GPIO_NONE;

//	boardConfiguration->ignitionPins[0] = GPIOE_NONE;
//	boardConfiguration->ignitionPins[1] = GPIO_NONE;
//	boardConfiguration->ignitionPins[2] = GPIO_NONE;
//	boardConfiguration->ignitionPins[3] = GPIO_NONE;

	boardConfiguration->fuelPumpPin = GPIOE_3; // temporary value pin
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

//	boardConfiguration->logicAnalyzerPins[1] = GPIO_NONE;

	// Frankenstein analog input #1: adc1
	// Frankenstein analog input #2: adc3
	// Frankenstein analog input #3: adc13
	// Frankenstein analog input #4: adc11
	// todo: see https://docs.google.com/spreadsheet/ccc?key=0Arl1FeMZcfisdEdGdUlHdWh6cVBoSzFIbkxqa1QtZ3c
	// Frankenstein analog input #5: adc
	// Frankenstein analog input #6: adc
	// Frankenstein analog input #7: adc
	// Frankenstein analog input #8: adc
	// Frankenstein analog input #9: adc
	// Frankenstein analog input #10: adc
	// Frankenstein analog input #11: adc
	// Frankenstein analog input #12: adc
	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
	engineConfiguration->cltAdcChannel = EFI_ADC_11;

	engineConfiguration->map.sensor.sensorType = MT_CUSTOM;
	engineConfiguration->map.sensor.customValueAt0 = 20;
	engineConfiguration->map.sensor.customValueAt5 = 200;
}

