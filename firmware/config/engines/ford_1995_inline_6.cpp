/**
 * @file ford_1995_inline_6.cpp
 * @brief Default engine configuration for a 1995 Ford inline 6 engine
 *
 * http://rusefi.com/forum/viewtopic.php?f=3&t=469
 *
 * FORD_INLINE_6_1995 = 7
 * set engine_type 7
 *
 * @date Feb 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "ford_1995_inline_6.h"

/**
 * @brief Default values for persistent properties
 */
void setFordInline6() {
	engineConfiguration->cylindersCount = 6;

	setCamOperationMode();

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->twoWireBatchInjection = true;


	/**
	 * 0.5ms dwell time just to be sure it would fit within camshaft revolution, dwell is not controlled by us anyway
	 */
	setConstantDwell(0.5);

	/**
	 * We treat the trigger as 6/0 toothed wheel
	 */
	engineConfiguration->skippedWheelOnCam = true;
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 6;
	engineConfiguration->trigger.customSkippedToothCount = 0;

	engineConfiguration->globalTriggerAngleOffset = 0;

	engineConfiguration->clt.config = {-10, 60, 120, 160310, 7700, 1180, 2700};
	engineConfiguration->iat.config = {-10, 60, 120, 160310, 7700, 1180, 2700};
	
	// 12ch analog board pinout:
	// input channel 3 is PA7, that's ADC7
	// input channel 5 is PA4, that's ADC4
	// input channel 6 is PA3, that's ADC3
	// input channel 7 is PA2, that's ADC2
	// input channel 8 is PA1, that's ADC1
	// input channel 9 is PA0, that's ADC0
	// input channel 10 is PC3, that's ADC13
	// input channel 12 is PC1, that's ADC11

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->iat.adcChannel = EFI_ADC_2;
	engineConfiguration->clt.adcChannel = EFI_ADC_1;
	engineConfiguration->afr.hwChannel = EFI_ADC_11;

	engineConfiguration->map.sensor.type = MT_MPX4250;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_15;
	engineConfiguration->baroSensor.type = MT_MPX4250;
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_7;

	// 6 channel output board
	// output 1 is PB9
	// output 3 is PE3
	// output 5 is PC13
	// output 6 is PC15

	engineConfiguration->fuelPumpPin = Gpio::C13;
	engineConfiguration->injectionPins[0] = Gpio::B9;
	engineConfiguration->injectionPins[1] = Gpio::E3;
	engineConfiguration->ignitionPins[0] = Gpio::C15;

	engineConfiguration->injectionPins[2] = Gpio::Unassigned;
	engineConfiguration->fanPin = Gpio::Unassigned;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);

	//	engineConfiguration->vbattAdcChannel = 0; //
	engineConfiguration->mafAdcChannel = EFI_ADC_1;

	engineConfiguration->triggerInputPins[0] = Gpio::A8;
	engineConfiguration->triggerInputPins[1] = Gpio::A5;
}
