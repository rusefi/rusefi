/**
 * @file	dodge_neon.cpp
 *
 * DODGE_NEON_1995 = 2
 * set engine_type 2
 *
 * DODGE_NEON_2003 = 23
 * set engine_type 23
 * http://rusefi.com/wiki/index.php?title=Vehicle:Dodge_Neon_2003
 *
 * @date Dec 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "dodge_neon.h"

#include "custom_engine.h"

void setDodgeNeon1995EngineConfiguration() {
	setDefaultFrankensoConfiguration();

	engineConfiguration->trigger.type = TT_DODGE_NEON_1995;

	engineConfiguration->fuelAlgorithm = LM_ALPHA_N;

//	engineConfiguration->spi2SckMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MosiMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MisoMode = PAL_STM32_PUDR_PULLUP; // 32

	//	engineConfiguration->spi2mosiPin = GPIOB_15;
	//	engineConfiguration->spi2misoPin = GPIOB_14;
	//	engineConfiguration->spi2sckPin = GPIOB_13;
	engineConfiguration->cj125CsPin = GPIOB_0; // rev 0.4
	engineConfiguration->isCJ125Enabled = true;
	engineConfiguration->is_enabled_spi_2 = true;


	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 550
	engineConfiguration->cranking.rpm = 550;


//	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	/**
	 * that's 1995 config
	 */

	setWholeTimingTable_d(12);

	// set cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// this is needed for injector lag auto-tune research if switching to batch
	// enable two_wire_batch_injection
	engineConfiguration->twoWireBatchInjection = true;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// set_firing_order 2
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	// set global_trigger_offset_angle 497
	engineConfiguration->globalTriggerAngleOffset = 497;

	/**
	 * that's 1995 config
	 */

	// set cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 0;

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

	engineConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
	engineConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6

	engineConfiguration->fuelPumpPin = GPIOC_13; // Frankenstein: low side - out #4
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	engineConfiguration->mapErrorDetectionTooHigh = 120;

	// set injection_pin_mode 0
	engineConfiguration->injectionPinMode = OM_DEFAULT;

	// Frankenstein: high side #1: PE8
	// Frankenstein: high side #2: PE10

	engineConfiguration->ignitionPins[0] = GPIOE_8; // Frankenstein: high side #1
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOE_10; // // Frankenstein: high side #2

	// set ignition_pin_mode 0
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->clt.config = {0, 30, 100, 32500, 7550, 700, 2700};
}

void setDodgeNeonNGCEngineConfiguration() {
	setDefaultFrankensoConfiguration();
	engineConfiguration->trigger.type = TT_DODGE_NEON_2003_CAM;
	setFrankenso_01_LCD(engineConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

	// set global_trigger_offset_angle 38
	engineConfiguration->globalTriggerAngleOffset = 38;

	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->specs.displacement = 1.996;
	engineConfiguration->specs.cylindersCount = 4;

	/**
	 * 77C
	 * 1200 rpm
	 * fuel 3
	 *
	 * 88C
	 * fuel 2.8
	 *
	 */
	//setWholeTimingTable_d(12);

	//setMap(config->veTable, 50);

	// set cranking_timing_angle 710
	engineConfiguration->crankingTimingAngle = -710;

	/**
	 * bosch 4G1139
	 * http://forum.2gn.org/viewtopic.php?t=21657
	 * or is it 225 as mentioned at http://turbobazar.ru/showpost.php?p=750815&postcount=796 ?
	 */
	engineConfiguration->injector.flow = 199;

	setLinearCurve(config->ignitionLoadBins, 20, 120, 1);

	setAlgorithm(LM_SPEED_DENSITY);

	setFuelTablesLoadBin(20, 120);

	engineConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;

	/**
	 * PA4 Wideband O2 Sensor
	 */
	engineConfiguration->afr.hwChannel = EFI_ADC_4;

	commonFrankensoAnalogInputs(engineConfiguration);
	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;

	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Hardware_Frankenso_board
	 */
	// Frankenso low out #1: PE6 main relay
	// Frankenso low out #2: PE5
	// Frankenso low out #3: PD7 coolant fan relay
	// Frankenso low out #4: PC13 idle valve solenoid
	// Frankenso low out #5: PE3 fuel pump relay
	// Frankenso low out #6: PE4
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #8: PE2 injector #3
	// Frankenso low out #9: PB9 injector #2
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8 injector #1
	// Frankenso low out #12: PB7 injector #4

	engineConfiguration->fanPin = GPIOD_7;

	engineConfiguration->injectionPins[0] = GPIOB_8;
	engineConfiguration->injectionPins[1] = GPIOB_9;
	engineConfiguration->injectionPins[2] = GPIOE_2;
	engineConfiguration->injectionPins[3] = GPIOB_7;

	engineConfiguration->ignitionPins[0] = GPIOC_9;
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOE_8;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->mainRelayPin = GPIOE_6;

	engineConfiguration->idle.solenoidPin = GPIOC_13;
	engineConfiguration->idle.solenoidFrequency = 300;
	engineConfiguration->manIdlePosition = 36;

	engineConfiguration->fuelPumpPin = GPIOE_3;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIOC_6;

	/**
	 * Frankenso analog #1 PC2 ADC12 CLT
	 * Frankenso analog #2 PC1 ADC11 IAT
	 * Frankenso analog #3 PA0 ADC0 MAP
	 * Frankenso analog #4 PC3 ADC13
	 * Frankenso analog #5 PA2 ADC2 TPS
	 * Frankenso analog #6 PA1 ADC1
	 * Frankenso analog #7 PA4 ADC4 WBO AFR
	 * Frankenso analog #8 PA3 ADC3
	 * Frankenso analog #9 PA7 ADC7
	 * Frankenso analog #10 PA6 ADC6
	 * Frankenso analog #11 PC5 ADC15
	 * Frankenso analog #12 PC4 ADC14 VBatt
	 */


	setDodgeSensor(&engineConfiguration->clt, 10000);
	setDodgeSensor(&engineConfiguration->iat, 10000);

	/**
	 * MAP PA0
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0; // PA0

//	rev 0.1 green board
//	engineConfiguration->map.sensor.hwChannel = EFI_ADC_6; // PA6
//	engineConfiguration->tps1_1AdcChannel = EFI_ADC_15; // PC5


	/**
	 * TPS
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;
	engineConfiguration->tpsMax = 625; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMin = 125; // convert 12to10 bit (ADC/4)

	/**
	 * IAT D15/W7
	 */
	engineConfiguration->iat.adcChannel = EFI_ADC_11;

	/**
	 * CLT D13/W9
	 */
	engineConfiguration->clt.adcChannel = EFI_ADC_12;

	engineConfiguration->map.sensor.type = MT_DODGE_NEON_2003;

	engineConfiguration->hip9011Gain = 0.3;

	float t = 0.5;

	engineConfiguration->knockNoise[0] = 2.1 + t; // 800
	engineConfiguration->knockNoise[1] = 2.1 + t; // 1700
	engineConfiguration->knockNoise[2] = 2.2 + t; // 2600
	engineConfiguration->knockNoise[3] = 2.2 + t; // 3400
	engineConfiguration->knockNoise[4] = 2.3 + t; // 4300
	engineConfiguration->knockNoise[5] = 2.7 + t; // 5200
	engineConfiguration->knockNoise[6] = 3.1 + t; // 6100
	engineConfiguration->knockNoise[7] = 3.3 + t; // 7000


	engineConfiguration->cylinderBore = 87.5;

	engineConfiguration->clutchDownPin = GPIOC_12;
	engineConfiguration->clutchDownPinMode = PI_PULLUP;
//	engineConfiguration->clutchUpPin = GPIOA_14; // note SWCLK - conflict with SWD
	engineConfiguration->clutchUpPinMode = PI_PULLUP;

//	engineConfiguration->vehicleSpeedSensorInputPin = GPIOA_8;

	engineConfiguration->fanOnTemperature = 92;
	engineConfiguration->fanOffTemperature = 89;
//	engineConfiguration->fanOnTemperature = 115; // knock testing - value is a bit high
//	engineConfiguration->fanOffTemperature = 100;

//	engineConfiguration->tunerStudioSerialSpeed = 9600;
	engineConfiguration->tunerStudioSerialSpeed = 19200;
	setAlgorithm(LM_SPEED_DENSITY);

//temp	engineConfiguration->alternatorControlPin = GPIOD_5;
	engineConfiguration->targetVBatt = 14.0;
	engineConfiguration->alternatorControl.offset = 20;
	engineConfiguration->alternatorControl.pFactor = 20;
	engineConfiguration->alternatorControl.iFactor = 0.2;
	engineConfiguration->alternatorControl.dFactor = 0.1;
	engineConfiguration->alternatorControl.periodMs = 10;

//	enableFrankensoCan();
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canNbcType = CAN_BUS_NBC_BMW;
//	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;

	engineConfiguration->tpsAccelEnrichmentThreshold = 10;

	engineConfiguration->wwaeTau = 1.0f;
	engineConfiguration->wwaeBeta = 0.40f;

	engineConfiguration->wwaeTau = 0;
	engineConfiguration->wwaeBeta = 0;

	engineConfiguration->isSdCardEnabled = false;
	engineConfiguration->manIdlePosition = 36; // set_idle_pwm 40

	engineConfiguration->slowAdcAlpha = 0.33333;

	// end of setDodgeNeonNGCEngineConfiguration
}
