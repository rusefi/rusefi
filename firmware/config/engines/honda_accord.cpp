/**
 * @file	honda_accord.cpp
 *
 * 1995 Honda Accord EX
 * http://rusefi.com/wiki/index.php?title=Vehicle:Honda_Accord_1995
 * http://rusefi.com/forum/viewtopic.php?f=3&t=621
 *
 * engine_type 6
 * engine_type 17
 *
 * @date Jan 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "trigger_decoder.h"
#include "thermistors.h"
#include "honda_accord.h"
#include "engine_math.h"

void setFrankenso_01_LCD(board_configuration_s *boardConfiguration) {
	boardConfiguration->HD44780_rs = GPIOE_7;
	boardConfiguration->HD44780_e = GPIOE_9;
	boardConfiguration->HD44780_db4 = GPIOE_11;
	boardConfiguration->HD44780_db5 = GPIOE_13;
	boardConfiguration->HD44780_db6 = GPIOE_15;
	boardConfiguration->HD44780_db7 = GPIOB_10;
}

static void setHondaAccordConfigurationCommon(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->map.sensor.sensorType = MT_DENSO183;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->injectionMode = IM_BATCH;

	engineConfiguration->idleMode = IM_MANUAL;

	engineConfiguration->HD44780height = 4;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 2.156;

	// Keihin 06164-P0A-A00
	engineConfiguration->injectorFlow = 248;

//	engineConfiguration->algorithm = LM_SPEED_DENSITY;
	// I want to start with a simple Alpha-N
	engineConfiguration->algorithm = LM_TPS;
	setFuelLoadBin(engineConfiguration, 0, 100);
	setTimingLoadBin(engineConfiguration, 0, 100);

	engineConfiguration->crankingSettings.coolantTempMaxC = 65; // 8ms at 65C
	engineConfiguration->crankingSettings.fuelAtMaxTempMs = 8;

	engineConfiguration->crankingSettings.coolantTempMinC = 0; // 20ms at 0C
	engineConfiguration->crankingSettings.fuelAtMinTempMs = 15;

	/**
	 * 18K Ohm @ -20C
	 * 2.1K Ohm @ 24C
	 * 100 Ohm @ 120C
	 */
	setThermistorConfiguration(&engineConfiguration->cltThermistorConf, -20.0, 18000.0, 23.8889, 2100.0, 120.0, 100.0);
	engineConfiguration->cltThermistorConf.bias_resistor = 1500; // same as OEM ECU

	setThermistorConfiguration(&engineConfiguration->iatThermistorConf, -20.0, 18000.0, 23.8889, 2100.0, 120.0, 100.0);
	engineConfiguration->iatThermistorConf.bias_resistor = 1500; // same as OEM ECU

	// set_cranking_charge_angle 35
	engineConfiguration->crankingChargeAngle = 70;
	// set_cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 45;

	// set_global_trigger_offset_angle 34
	engineConfiguration->globalTriggerAngleOffset = 34;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 2000
	engineConfiguration->crankingSettings.crankingRpm = 500;


	// set_ignition_offset 350
//	engineConfiguration->ignitionOffset = 350;
	 // set_injection_offset 510
//	engineConfiguration->injectionOffset = 510;


	/**
	 * ADC inputs:
	 *
	 * Inp1/ADC12 PC2: CLT
	 * Inp2/ADC11 PC1: AIT/IAT
	 * Inp3/ADC0 PA0: MAP
	 * Inp4/ADC13 PC3: AFR
	 * Inp6/ADC1 PA1: TPS
	 * Inp12/ADC14 PC4: VBatt
	 */
	memset(boardConfiguration->adcHwChannelEnabled, 0, sizeof(boardConfiguration->adcHwChannelEnabled));
	boardConfiguration->adcHwChannelEnabled[0] = ADC_FAST; // ADC0 - PA0 - MAP
	boardConfiguration->adcHwChannelEnabled[1] = ADC_SLOW; // TPS
	boardConfiguration->adcHwChannelEnabled[2] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[3] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[4] = ADC_SLOW;

	boardConfiguration->adcHwChannelEnabled[6] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[11] = ADC_SLOW; // IAT
	boardConfiguration->adcHwChannelEnabled[12] = ADC_SLOW; // CLT
	boardConfiguration->adcHwChannelEnabled[13] = ADC_SLOW; // AFR
	boardConfiguration->adcHwChannelEnabled[14] = ADC_SLOW; // VBatt

	/**
	 * D14/W10 O2 Sensor
	 */
	engineConfiguration->afrSensor.afrAdcChannel = EFI_ADC_13;

	/**
	 * VBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
	engineConfiguration->vbattDividerCoeff = ((float) (8.2 + 33)) / 8.2 * 2;

	//	todo engineConfiguration->afrSensor.afrAdcChannel = 14;


	/**
	 * MAP D17/W5
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;


	/**
	 * TPS D11/W11
	 */
	engineConfiguration->tpsAdcChannel = EFI_ADC_1;

	/**
	 * IAT D15/W7
	 */
	engineConfiguration->iatAdcChannel = EFI_ADC_11;

	/**
	 * CLT D13/W9
	 */
	engineConfiguration->cltAdcChannel = EFI_ADC_12;


	/**
	 * Outputs
	 */
	// Frankenso low out #:
	// Frankenso low out #:
	// Frankenso low out #:
	// Frankenso low out #:
	// Frankenso low out #5: PE3
	// Frankenso low out #6: PE4
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #:
	// Frankenso low out #9: PB9
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8
	// Frankenso low out #12: PB7

	boardConfiguration->idleValvePin = GPIOE_5;
	boardConfiguration->o2heaterPin = GPIOC_13;

	boardConfiguration->injectionPins[0] = GPIOB_8;
	boardConfiguration->injectionPins[1] = GPIOB_9;
	boardConfiguration->injectionPins[2] = GPIOE_1;
	boardConfiguration->injectionPins[3] = GPIOB_7;

	boardConfiguration->ignitionPins[0] = GPIOE_4;
	boardConfiguration->ignitionPins[1] = GPIO_NONE;
	boardConfiguration->ignitionPins[2] = GPIO_NONE;
	boardConfiguration->ignitionPins[3] = GPIO_NONE;

	boardConfiguration->fuelPumpPin = GPIOE_3;
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

	boardConfiguration->gps_rx_pin = GPIO_NONE;
	boardConfiguration->gps_tx_pin = GPIO_NONE;

	setFrankenso_01_LCD(boardConfiguration);

	boardConfiguration->logicAnalyzerPins[1] = GPIO_NONE;


	boardConfiguration->idleSolenoidFrequency = 500;
}

void setHondaAccordConfigurationTwoWires(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->engineType = HONDA_ACCORD_CD_TWO_WIRES;
	engineConfiguration->triggerConfig.triggerType = TT_HONDA_ACCORD_CD_TWO_WIRES;
	setHondaAccordConfigurationCommon(engineConfiguration, boardConfiguration);
}

void setHondaAccordConfigurationThreeWires(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->engineType = HONDA_ACCORD_CD;
	engineConfiguration->triggerConfig.triggerType = TT_HONDA_ACCORD_CD;
	setHondaAccordConfigurationCommon(engineConfiguration, boardConfiguration);
}

void setHondaAccordConfigurationDip(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->engineType = HONDA_ACCORD_CD_DIP;
	engineConfiguration->triggerConfig.triggerType = TT_HONDA_ACCORD_CD_DIP;
	setHondaAccordConfigurationCommon(engineConfiguration, boardConfiguration);
}
