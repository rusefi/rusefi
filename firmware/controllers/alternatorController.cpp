/**
 * @file    alternatorController.cpp
 * @brief   alternator controller - turn alternator off if you do not really need it
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy (c) 2012-2014
 */

#include "main.h"
#include "rpm_calculator.h"
#include "pwm_generator.h"
#include "wave_math.h"
#include "alternatorController.h"
#include "pin_repository.h"
#include "engine_configuration.h"
#include "voltage.h"

#if 0

extern board_configuration_s *boardConfiguration;

#define ALTERNATOR_VALVE_PWM_FREQUENCY 30000

static PwmConfig alternatorControl;

static THD_WORKING_AREA(ivThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t AltCtrlThread(int param) {
	chRegSetThreadName("AlternatorController");
	int alternatorDutyCycle = 500;
	while (TRUE) {
		chThdSleepMilliseconds(10);

		if ( getVBatt() > 14.2 )
			alternatorDutyCycle = alternatorDutyCycle + 1 ;
			else
			alternatorDutyCycle = alternatorDutyCycle - 1;


		if (alternatorDutyCycle < 150 )
			alternatorDutyCycle = 150;
		if (alternatorDutyCycle > 950)
			alternatorDutyCycle = 950;
		setSimplePwmDutyCycle(&alternatorControl, 0.001 * alternatorDutyCycle);
	}
#if defined __GNUC__
	return -1;
#endif
}

void initAlternatorCtrl() {
	startSimplePwm(&alternatorControl, "Alternator control",
				boardConfiguration->alternatorControlPin,
				0.5,
				ALTERNATOR_VALVE_PWM_FREQUENCY,
				ALTERNATOR_SWITCH
				);
	chThdCreateStatic(ivThreadStack, sizeof(ivThreadStack), LOWPRIO, (tfunc_t)AltCtrlThread, NULL);
}

#endif
