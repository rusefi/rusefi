/*
 * @file zil130.cpp
 *
 * http://rusefi.com/forum/viewtopic.php?f=15&t=644
 *
 * set engine_type 42
 *
 * @date May 4, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_template.h"
#include "custom_engine.h"

EXTERN_CONFIG;

void setZil130(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->specs.cylindersCount = 8;

	engineConfiguration->specs.displacement = 5.9;
	engineConfiguration->specs.firingOrder = FO_1_5_4_2_6_3_7_8;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 30;
	engineConfiguration->trigger.customSkippedToothCount = 1;



	engineConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;
//	engineConfiguration->twoWireBatchInjection = true;
	engineConfiguration->injectionPinMode = OM_INVERTED;
	engineConfiguration->injectionPins[0] = GPIOB_8; // #1
	engineConfiguration->injectionPins[1] = GPIOE_2; // #2
	engineConfiguration->injectionPins[2] = GPIOE_3; // #3
	engineConfiguration->injectionPins[3] = GPIOE_4; // #4
	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED; // #5
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED; // #6
	engineConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;

	engineConfiguration->ignitionPins[0] = GPIOB_5; // #1
	engineConfiguration->ignitionPins[1] = GPIOB_6; // #2
	engineConfiguration->ignitionPins[2] = GPIO_UNASSIGNED; // #3
	engineConfiguration->ignitionPins[3] = GPIOB_7; // #4
	engineConfiguration->ignitionPins[4] = GPIOC_7; // #5

	// fuel pump
	engineConfiguration->fuelPumpPin = GPIOD_4;

	// idle
	engineConfiguration->idle.solenoidPin = GPIOC_14;
	engineConfiguration->idle.solenoidPinMode = OM_INVERTED;
	engineConfiguration->idle.solenoidFrequency = 300;
	engineConfiguration->manIdlePosition = 50; // set_idle_pwm 50

	// disable sd_card
	engineConfiguration->sdCardCsPin = GPIO_UNASSIGNED;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = false;
	engineConfiguration->max31855spiDevice = SPI_NONE;

	engineConfiguration->fanPin = GPIO_UNASSIGNED;

}
