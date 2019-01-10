/*
 * @file lada_kalina.cpp
 *
 * set engine_type 39
 *
 * @date  Jan 21, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "lada_kalina.h"
#include "custom_engine.h"
#include "fsio_impl.h"


EXTERN_ENGINE;

void setLadaKalina(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
	disableLCD(boardConfiguration);

	boardConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_e = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db7 = GPIO_UNASSIGNED;


	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;

	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;

	engineConfiguration->globalTriggerAngleOffset = 114;

	boardConfiguration->ignitionPins[0] = GPIOE_14;
	boardConfiguration->ignitionPins[1] = GPIOC_7;
	boardConfiguration->ignitionPins[2] = GPIOC_9;
	boardConfiguration->ignitionPins[3] = GPIOE_12;

	boardConfiguration->useStepperIdle = true;
	boardConfiguration->fuelPumpPin = GPIOC_13;
	boardConfiguration->mainRelayPin = GPIOD_7;

	boardConfiguration->idle.stepperDirectionPin = GPIOE_15;
	boardConfiguration->idle.stepperStepPin = GPIOE_13;
	engineConfiguration->stepperEnablePin = GPIOE_11;



	// starter relay solenoid
	/**
	 * to test
	 * set_fsio_setting 1 5000
	 */
	engineConfiguration->bc.fsio_setting[0] = 500;
	// set_rpn_expression 1 "rpm 0 fsio_setting <"
	setFsioExt(0, GPIOE_3, RPM_BELOW_USER_SETTING_1, 0 PASS_ENGINE_PARAMETER_SUFFIX);

}
