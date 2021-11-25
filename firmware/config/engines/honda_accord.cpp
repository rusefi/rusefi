/**
 * @file	honda_accord.cpp
 *
 * 1993 Honda Prelude 1993
 * http://rusefi.com/wiki/index.php?title=Vehicle:Honda_Prelude_1993
 * http://rusefi.com/forum/viewtopic.php?f=3&t=887
 *
 * 1995 Honda Accord EX
 * http://rusefi.com/wiki/index.php?title=Vehicle:Honda_Accord_1995
 * http://rusefi.com/forum/viewtopic.php?f=3&t=621
 *
 * set engine_type 6
 *
 * set engine_type 17
 *   1+24 signal, JM Predule
 *
 *
 * set engine_type 48
 *   debug configuration to reproduce https://sourceforge.net/p/rusefi/tickets/319/
 *
 *
 * set engine_type 50
 * set engine_type 51
 *   debug configurations to reproduce https://github.com/rusefi/rusefi/issues/363
 *
 * @date Jan 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "honda_accord.h"
#include "advance_map.h"
#include "custom_engine.h"

static void setHondaAccordConfigurationCommon() {
	engineConfiguration->map.sensor.type = MT_DENSO183;

	// set ignition_mode 0
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	// set injection_mode
	engineConfiguration->injectionMode = IM_BATCH;

	engineConfiguration->idleMode = IM_MANUAL;

	engineConfiguration->HD44780height = 4;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2.156;

	// Keihin 06164-P0A-A00
	engineConfiguration->injector.flow = 248;

	setAlgorithm(LM_SPEED_DENSITY);

	buildTimingMap(35);

	/**
	 * 18K Ohm @ -20C
	 * 2.1K Ohm @ 24C
	 * 100 Ohm @ 120C
	 * 1500 = same pullup as OEM ECU
	 */
	setCommonNTCSensor(&engineConfiguration->clt, 1500);
	setCommonNTCSensor(&engineConfiguration->iat, 1500);

	// set cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = -45;

	// set global_trigger_offset_angle 34
	engineConfiguration->globalTriggerAngleOffset = 34;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 2000
	engineConfiguration->cranking.rpm = 500;


	/**
	 * ADC inputs:
	 *
	 * Inp1/ADC12 PC2: CLT
	 * Inp2/ADC11 PC1: AIT/IAT
	 * Inp3/ADC0 PA0: MAP
	 * Inp4/ADC13 PC3: AFR green wire
	 * Inp6/ADC1 PA1: TPS
	 * Inp12/ADC14 PC4: VBatt
	 */

	/**
	 * wideband O2 Sensor
	 */
	engineConfiguration->afr.hwChannel = EFI_ADC_13;

	/**
	 * VBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 39)) / 10 * 2;

	//	todo engineConfiguration->afr.hwChannel = 14;


	/**
	 * MAP D17/W5 blue wire
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;
	/**
	 * IAT D15/W7 green wire
	 */
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
	/**
	 * CLT D13/W9 yellow wire
	 */
	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	/**
	 * TPS D11/W11 blue wire
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_1;


	/**
	 * Outputs
	 */
	// Frankenso low out #1: PE6
	// Frankenso low out #2: PE5
	// Frankenso low out #3: PD7
	// Frankenso low out #4: PC13
	// Frankenso low out #5: PE3 Fuel Relay
	// Frankenso low out #6: PE4 radiator fan - blue wire
	// Frankenso low out #7: PD3 idle air valve solenoid - green wire
	// Frankenso low out #8: PE2 MIL - white wire
	// Frankenso low out #9: PB9 Injector #2
	// Frankenso low out #10: PD5 Injector #3
	// Frankenso low out #11: PB8 injector #1
	// Frankenso low out #12: PB7 injector #4
	engineConfiguration->fuelPumpPin = GPIOE_3;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;
	engineConfiguration->malfunctionIndicatorPin = GPIOE_2;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;
	engineConfiguration->fanPin = GPIOE_4; // blue wire

	engineConfiguration->idle.solenoidPin = GPIOD_3; // green wire

	engineConfiguration->injectionPins[0] = GPIOB_8;
	engineConfiguration->injectionPins[1] = GPIOB_9;
	engineConfiguration->injectionPins[2] = GPIOD_5;
	engineConfiguration->injectionPins[3] = GPIOB_7;

	engineConfiguration->ignitionPins[0] = GPIOE_12; // white wire
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;


	setFrankenso_01_LCD(engineConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

	engineConfiguration->idle.solenoidFrequency = 500;
}

void setHondaAccordConfiguration1_24() {
	engineConfiguration->trigger.type = TT_HONDA_1_24;
	setHondaAccordConfigurationCommon();
}
