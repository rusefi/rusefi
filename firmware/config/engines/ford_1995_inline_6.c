/**
 * @file ford_1995_inline_6.c
 * @brief Default engine configuration for a 1995 Ford inline 6 engine
 *
 * http://rusefi.com/forum/viewtopic.php?f=3&t=469
 *
 * FORD_INLINE_6_1995 = 7
 *
 * @date Feb 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "ford_1995_inline_6.h"
#include "engine_math.h"
#include "thermistors.h"

#if EFI_SUPPORT_1995_FORD_INLINE_6 || defined(__DOXYGEN__)

/**
 * @brief Default values for persistent properties
 */
void setFordInline6(engine_configuration_s *engineConfiguration) {
	engineConfiguration->cylindersCount = 6;

	/**
	 * 0.5 means primary position sensor is on a camshaft
	 */
	engineConfiguration->rpmMultiplier = 0.5;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->firingOrder = FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4;

	/**
	 * 0.5ms dweel time just to be sure it would fit within camshaft revolution, dwell is not controlled by us anyway
	 */
	setConstantDwell(engineConfiguration, 0.5);

	/**
	 * We treat the trigger as 6/0 toothed wheel
	 */
	engineConfiguration->triggerConfig.totalToothCount = 6;
	engineConfiguration->triggerConfig.skippedToothCount = 0;
	engineConfiguration->triggerConfig.isSynchronizationNeeded = FALSE;
	engineConfiguration->triggerConfig.useRiseEdge = TRUE;
	engineConfiguration->needSecondTriggerInput = FALSE;


	setThermistorConfiguration(&engineConfiguration->cltThermistorConf, -10, 160310, 60, 7700, 120.00, 1180);
	engineConfiguration->cltThermistorConf.bias_resistor = 2700;

	setThermistorConfiguration(&engineConfiguration->iatThermistorConf, -10, 160310, 60, 7700, 120.00, 1180);
	engineConfiguration->iatThermistorConf.bias_resistor = 2700;

	engineConfiguration->tpsAdcChannel = 7; // input channel 3 is PA7, that's ADC7
	engineConfiguration->cltAdcChannel = 0; // input channel 9 is PA0, that's ADC0
	engineConfiguration->iatAdcChannel = 11; // input channel 12 is PC1, that's ADC11

	// divided by 2 because of voltage divider, then converted into 10bit ADC value (TunerStudio format)
	engineConfiguration->tpsMin = (1.250 / 2) * 1024 / 3.3;
	engineConfiguration->tpsMax = (4.538 / 2) * 1024 / 3.3;

	engineConfiguration->map.config.mapType = MT_MPX4250;
	engineConfiguration->map.channel = 2; // input channel 8 is ADC2

	//	engineConfiguration->vBattAdcChannel = 0; //
//	engineConfiguration->mafAdcChannel = 1;

}

/**
 * @brief These settings are not persistent yet
 */
void setFordInline6_2(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {
	initializeIgnitionActions(engineConfiguration, engineConfiguration2);
}

#endif /* EFI_SUPPORT_1995_FORD_INLINE_6 */
