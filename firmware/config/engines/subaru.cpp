/**
 * @file	subaru.cpp
 *
 * SUBARU_2003_WRX
 * set engine_type 22
 *
 * @date Sep 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "subaru.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setSubaru2003Wrx(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setFrankenso_01_LCD(PASS_ENGINE_PARAMETER_SIGNATURE);
	setFrankenso0_1_joystick(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 5;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	engineConfiguration->sensorChartFrequency = 2;

	CONFIGB(useStepperIdle) = true;

	// See http://rusefi.com/forum/viewtopic.php?f=4&t=1161
	CONFIGB(idle).stepperDirectionPin = GPIOD_1; // top stepper drive pin, white wire recommended
	CONFIGB(idle).stepperStepPin = GPIOD_6; // yellow wire recommended
	engineConfiguration->stepperEnablePin = GPIOB_1; // bottom stepper driver board pin, blue wire recommended

	CONFIGB(triggerSimulatorPins)[0] = GPIO_UNASSIGNED; // we want to avoid PD1 conflict
	CONFIGB(triggerSimulatorPins)[1] = GPIO_UNASSIGNED;
	CONFIGB(triggerSimulatorPins)[2] = GPIO_UNASSIGNED;

}
