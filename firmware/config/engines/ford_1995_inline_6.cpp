/**
 * @file ford_1995_inline_6.cpp
 * @brief Default engine configuration for a 1995 Ford inline 6 engine
 *
 * todo: adjust/remove/modernize "unit" tests which still depend on this config?
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

	/**
	 * 0.5ms dwell time just to be sure it would fit within camshaft revolution, dwell is not controlled by us anyway
	 */
	setConstantDwell(FORD_INLINE_DWELL);

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

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_15;

	engineConfiguration->injectionPins[0] = Gpio::B9;
	engineConfiguration->injectionPins[1] = Gpio::E3;
	engineConfiguration->ignitionPins[0] = Gpio::C15;

	engineConfiguration->injectionPins[2] = Gpio::Unassigned;
	engineConfiguration->fanPin = Gpio::Unassigned;

	//	engineConfiguration->vbattAdcChannel = 0; //
	engineConfiguration->mafAdcChannel = EFI_ADC_1;

	engineConfiguration->triggerInputPins[0] = Gpio::A8;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
}
