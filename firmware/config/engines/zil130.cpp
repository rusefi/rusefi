/*
 * @file zil130.cpp
 *
 * http://rusefi.com/forum/viewtopic.php?f=15&t=644
 *
 * set engine_type 42
 *
 * @date May 4, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine_template.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setZil130(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->specs.cylindersCount = 8;

	engineConfiguration->specs.displacement = 5.9;
	engineConfiguration->specs.firingOrder = FO_1_5_4_2_6_3_7_8;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 30;
	engineConfiguration->trigger.customSkippedToothCount = 1;



	boardConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;
//	engineConfiguration->twoWireBatchInjection = true;
	boardConfiguration->injectionPinMode = OM_INVERTED;
	boardConfiguration->injectionPins[0] = GPIOB_8; // #1
	boardConfiguration->injectionPins[1] = GPIOE_2; // #2
	boardConfiguration->injectionPins[2] = GPIOE_3; // #3
	boardConfiguration->injectionPins[3] = GPIOE_4; // #4
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED; // #5
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED; // #6
	boardConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;

	boardConfiguration->ignitionPins[0] = GPIOB_5; // #1
	boardConfiguration->ignitionPins[1] = GPIOB_6; // #2
	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED; // #3
	boardConfiguration->ignitionPins[3] = GPIOB_7; // #4
	boardConfiguration->ignitionPins[4] = GPIOC_7; // #5

	// fuel pump
	boardConfiguration->fuelPumpPin = GPIOD_4;

	// idle
	boardConfiguration->idle.solenoidPin = GPIOC_14;
	boardConfiguration->idle.solenoidPinMode = OM_INVERTED;
	boardConfiguration->idle.solenoidFrequency = 300;
	boardConfiguration->manIdlePosition = 50; // set_idle_pwm 50

	// disable sd_card
	boardConfiguration->sdCardCsPin = GPIO_UNASSIGNED;
	boardConfiguration->is_enabled_spi_2 = false;
	boardConfiguration->is_enabled_spi_3 = false;
	boardConfiguration->max31855spiDevice = SPI_NONE;

	boardConfiguration->fanPin = GPIO_UNASSIGNED;

}
