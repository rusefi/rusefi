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



	CONFIGB(malfunctionIndicatorPin) = GPIO_UNASSIGNED;
//	engineConfiguration->twoWireBatchInjection = true;
	CONFIGB(injectionPinMode) = OM_INVERTED;
	CONFIGB(injectionPins)[0] = GPIOB_8; // #1
	CONFIGB(injectionPins)[1] = GPIOE_2; // #2
	CONFIGB(injectionPins)[2] = GPIOE_3; // #3
	CONFIGB(injectionPins)[3] = GPIOE_4; // #4
	CONFIGB(injectionPins)[4] = GPIO_UNASSIGNED; // #5
	CONFIGB(injectionPins)[5] = GPIO_UNASSIGNED; // #6
	CONFIGB(injectionPins)[6] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[5] = GPIO_UNASSIGNED;

	CONFIGB(ignitionPins)[0] = GPIOB_5; // #1
	CONFIGB(ignitionPins)[1] = GPIOB_6; // #2
	CONFIGB(ignitionPins)[2] = GPIO_UNASSIGNED; // #3
	CONFIGB(ignitionPins)[3] = GPIOB_7; // #4
	CONFIGB(ignitionPins)[4] = GPIOC_7; // #5

	// fuel pump
	CONFIGB(fuelPumpPin) = GPIOD_4;

	// idle
	CONFIGB(idle).solenoidPin = GPIOC_14;
	CONFIGB(idle).solenoidPinMode = OM_INVERTED;
	CONFIGB(idle).solenoidFrequency = 300;
	CONFIGB(manIdlePosition) = 50; // set_idle_pwm 50

	// disable sd_card
	CONFIGB(sdCardCsPin) = GPIO_UNASSIGNED;
	CONFIGB(is_enabled_spi_2) = false;
	CONFIGB(is_enabled_spi_3) = false;
	CONFIGB(max31855spiDevice) = SPI_NONE;

	CONFIGB(fanPin) = GPIO_UNASSIGNED;

}
