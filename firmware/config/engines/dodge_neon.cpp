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
	engineConfiguration->trigger.type = trigger_type_e::TT_DODGE_NEON_1995;

	engineConfiguration->fuelAlgorithm = LM_ALPHA_N;

//	engineConfiguration->spi2SckMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MosiMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MisoMode = PAL_STM32_PUDR_PULLUP; // 32

	//	engineConfiguration->spi2mosiPin = Gpio::B15;
	//	engineConfiguration->spi2misoPin = Gpio::B14;
	//	engineConfiguration->spi2sckPin = Gpio::B13;
	engineConfiguration->is_enabled_spi_2 = true;


	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now

	/**
	 * that's 1995 config
	 */

	setWholeTimingTable_d(12);

	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// this is needed for injector lag auto-tune research if switching to batch
	// enable two_wire_batch_injection
	engineConfiguration->twoWireBatchInjection = true;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// set_firing_order 2
	engineConfiguration->firingOrder = FO_1_3_4_2;

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

	engineConfiguration->injectionPins[0] = Gpio::B9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = Gpio::B8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = Gpio::E3; // Frankenstein: low side - out #8
	engineConfiguration->injectionPins[3] = Gpio::E5; // Frankenstein: low side - out #6

	engineConfiguration->fuelPumpPin = Gpio::C13; // Frankenstein: low side - out #4
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	engineConfiguration->mapErrorDetectionTooHigh = 120;

	engineConfiguration->injectionPinMode = OM_DEFAULT;

	// Frankenstein: high side #1: PE8
	// Frankenstein: high side #2: PE10

	engineConfiguration->ignitionPins[0] = Gpio::E8; // Frankenstein: high side #1
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::E10; // // Frankenstein: high side #2

	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->clt.config = {0, 30, 100, 32500, 7550, 700, 2700};
}

void setDodgeNeonNGCEngineConfiguration() {
	engineConfiguration->trigger.type = trigger_type_e::TT_DODGE_NEON_2003_CAM;

	// set global_trigger_offset_angle 38
	engineConfiguration->globalTriggerAngleOffset = 38;

	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->displacement = 1.996;
	engineConfiguration->cylindersCount = 4;

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

	engineConfiguration->malfunctionIndicatorPin = Gpio::Unassigned;

	/**
	 * PA4 Wideband O2 Sensor
	 */
	engineConfiguration->afr.hwChannel = EFI_ADC_4;

	commonFrankensoAnalogInputs();
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

	engineConfiguration->fanPin = Gpio::D7;

	engineConfiguration->injectionPins[0] = Gpio::B8;
	engineConfiguration->injectionPins[1] = Gpio::B9;
	engineConfiguration->injectionPins[2] = Gpio::E2;
	engineConfiguration->injectionPins[3] = Gpio::B7;

	engineConfiguration->ignitionPins[0] = Gpio::C9;
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::E8;
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;

	engineConfiguration->mainRelayPin = Gpio::E6;

	engineConfiguration->idle.solenoidPin = Gpio::C13;
	engineConfiguration->idle.solenoidFrequency = 300;
	engineConfiguration->manIdlePosition = 36;

	engineConfiguration->fuelPumpPin = Gpio::E3;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	engineConfiguration->triggerInputPins[0] = Gpio::A5;
	engineConfiguration->triggerInputPins[1] = Gpio::C6;

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

	engineConfiguration->cylinderBore = 87.5;

	engineConfiguration->clutchDownPin = Gpio::C12;
	engineConfiguration->clutchDownPinMode = PI_PULLUP;
//	engineConfiguration->clutchUpPin = Gpio::A14; // note SWCLK - conflict with SWD
	engineConfiguration->clutchUpPinMode = PI_PULLUP;

//	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::A8;

	engineConfiguration->fanOnTemperature = 92;
	engineConfiguration->fanOffTemperature = 89;
//	engineConfiguration->fanOnTemperature = 115; // knock testing - value is a bit high
//	engineConfiguration->fanOffTemperature = 100;

//	engineConfiguration->tunerStudioSerialSpeed = 9600;
	engineConfiguration->tunerStudioSerialSpeed = 19200;
	setAlgorithm(LM_SPEED_DENSITY);

//temp	engineConfiguration->alternatorControlPin = Gpio::D5;
	engineConfiguration->targetVBatt = 14.0;
	engineConfiguration->alternatorControl.offset = 20;
	engineConfiguration->alternatorControl.pFactor = 20;
	engineConfiguration->alternatorControl.iFactor = 0.2;
	engineConfiguration->alternatorControl.dFactor = 0.1;
	engineConfiguration->alternatorControl.periodMs = 10;

	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canNbcType = CAN_BUS_NBC_BMW;

	engineConfiguration->tpsAccelEnrichmentThreshold = 10;

	engineConfiguration->wwaeTau = 1.0f;
	engineConfiguration->wwaeBeta = 0.40f;

	engineConfiguration->wwaeTau = 0;
	engineConfiguration->wwaeBeta = 0;

	engineConfiguration->isSdCardEnabled = false;
	engineConfiguration->manIdlePosition = 36; // set_idle_pwm 40

	// end of setDodgeNeonNGCEngineConfiguration
}
