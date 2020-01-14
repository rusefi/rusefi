/*
 * @file lada_kalina.cpp
 *
 * set engine_type 39
 *
 * @date  Jan 21, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "lada_kalina.h"
#include "custom_engine.h"
#include "fsio_impl.h"

EXTERN_CONFIG;

void setLadaKalina(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
	disableLCD(engineConfiguration);

	engineConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_e = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db7 = GPIO_UNASSIGNED;


	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;

	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;

	engineConfiguration->globalTriggerAngleOffset = 114;

	engineConfiguration->ignitionPins[0] = GPIOE_14;
	engineConfiguration->ignitionPins[1] = GPIOC_7;
	engineConfiguration->ignitionPins[2] = GPIOC_9;
	engineConfiguration->ignitionPins[3] = GPIOE_12;

	engineConfiguration->useStepperIdle = true;
	engineConfiguration->fuelPumpPin = GPIOC_13;
	engineConfiguration->mainRelayPin = GPIOD_7;

	engineConfiguration->idle.stepperDirectionPin = GPIOE_15;
	engineConfiguration->idle.stepperStepPin = GPIOE_13;
	engineConfiguration->stepperEnablePin = GPIOE_11;



	// starter relay solenoid
#if EFI_FSIO
	/**
	 * to test
	 * set_fsio_setting 1 5000
	 */
	engineConfiguration->fsio_setting[0] = 500;
	// set_rpn_expression 1 "rpm 0 fsio_setting <"
	setFsioExt(0, GPIOE_3, RPM_BELOW_USER_SETTING_1, 0 PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */
}
