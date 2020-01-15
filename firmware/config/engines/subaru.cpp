/**
 * @file	subaru.cpp
 *
 * SUBARU_2003_WRX
 * set engine_type 22
 *
 * @date Sep 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "subaru.h"
#include "custom_engine.h"

EXTERN_CONFIG;

void setSubaru2003Wrx(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankenso_01_LCD(engineConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 5;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	engineConfiguration->sensorChartFrequency = 2;

	engineConfiguration->useStepperIdle = true;

	// See http://rusefi.com/forum/viewtopic.php?f=4&t=1161
	engineConfiguration->idle.stepperDirectionPin = GPIOD_1; // top stepper drive pin, white wire recommended
	engineConfiguration->idle.stepperStepPin = GPIOD_6; // yellow wire recommended
	engineConfiguration->stepperEnablePin = GPIOB_1; // bottom stepper driver board pin, blue wire recommended

	engineConfiguration->triggerSimulatorPins[0] = GPIO_UNASSIGNED; // we want to avoid PD1 conflict
	engineConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
}

/*
 * Subaru WRX <=1996 Turbo.
 */

void setSubaruEJ20GDefaults(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_SUBARU_7_6;
}
