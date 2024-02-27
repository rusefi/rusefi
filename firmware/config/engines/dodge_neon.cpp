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

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now

	setWholeTimingTable_d(12);

	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	// set global_trigger_offset_angle 497
	engineConfiguration->globalTriggerAngleOffset = 497;

	// set cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 0;

	engineConfiguration->mapErrorDetectionTooHigh = 120;

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

	engineConfiguration->tpsAccelEnrichmentThreshold = 10;

	engineConfiguration->wwaeTau = 1.0f;
	engineConfiguration->wwaeBeta = 0.40f;

	engineConfiguration->wwaeTau = 0;
	engineConfiguration->wwaeBeta = 0;

	engineConfiguration->isSdCardEnabled = false;
	engineConfiguration->manIdlePosition = 36; // set_idle_pwm 40

	// end of setDodgeNeonNGCEngineConfiguration
}
