/*
 * @file lada_kalina.cpp
 *
 * set engine_type 39
 *
 * @date  Jan 21, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "lada_kalina.h"
#include "custom_engine.h"
#include "fsio_impl.h"

#if (BOARD_TLE8888_COUNT > 0)
void setLadaKalina(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	/* MRE uses TLE8888_PIN_21 for fuel pump */
	engineConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
	/* PRE uses TLE8888_PIN_22 for fan */
	engineConfiguration->fanPin = GPIO_UNASSIGNED;
	// TLE8888 two bridge drivers for stepper
	engineConfiguration->stepperDcIo[0].directionPin1 = TLE8888_PIN_21;
	engineConfiguration->stepperDcIo[0].directionPin2 = TLE8888_PIN_22;
	engineConfiguration->stepperDcIo[1].directionPin1 = TLE8888_PIN_23;
	engineConfiguration->stepperDcIo[1].directionPin2 = TLE8888_PIN_24;
	/* IDLE configuration */
	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridges = true;
	engineConfiguration->idleMode = IM_AUTO;
	engineConfiguration->useTLE8888_stepper = true;
}

#else

void setLadaKalina(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
	disableLCD(engineConfiguration);

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
#endif
