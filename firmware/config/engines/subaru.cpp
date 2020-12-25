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

/**
 * MRE_SUBARU_EJ18
 * set engine_type 37
 */
void setSubaruEJ18_MRE(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_SUBARU_7_WITHOUT_6;


//	CONFIG(isDoubleSolenoidIdle) = true;

	engineConfiguration->specs.displacement = 1.8;
	strcpy(CONFIG(engineMake), ENGINE_MAKE_SUBARU);
	strcpy(CONFIG(engineCode), "EJ18");

	engineConfiguration->specs.firingOrder = FO_1_3_2_4;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

#if (BOARD_TLE8888_COUNT > 0)
	// TLE8888_PIN_23: "33 - GP Out 3"
	engineConfiguration->malfunctionIndicatorPin = TLE8888_PIN_23;
#endif /* BOARD_TLE8888_COUNT */

	// this car has high-side main relay WOW so we have to hard wire it to ingition switch


}
