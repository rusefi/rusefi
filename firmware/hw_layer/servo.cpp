/**
 * @file	servo.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Servo_motor
 *
 * SG90 pinout:
 * brown  GND
 * red    VCC
 * orange PWM signal
 *
 * @date Jan 3, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_SERVO || EFI_SIMULATOR
#include "servo.h"
#include "thread_priority.h"

// This thread calls scheduleForLater which eventually could trip the main trigger callback
// if self stimulation (heh) is enabled, which uses a TON of stack space.
// So this stack has to be pretty big, unfortunately.
THD_WORKING_AREA(servoThreadStack, UTILITY_THREAD_STACK_SIZE * 3);

static OutputPin pins[SERVO_COUNT];

static scheduling_s servoTurnSignalOff;

// todo: extract common 'pin off' callback?
static void servoTachPinLow(OutputPin *pin) {
	pin->setValue(false);
}

static msg_t seThread(void *arg) {
	(void)arg;
	chRegSetThreadName("servo");
	while (true) {

		OutputPin *pin = &pins[0];
		pin->setValue(1);


		percent_t position = (currentTimeMillis() / 5) % 200;
		if (position > 100)
			 position = 200 - position;

		float durationMs = 0 + position * 0.02f;

		engine->executor.scheduleForLater(&servoTurnSignalOff, (int)MS2US(durationMs), { &servoTachPinLow, pin });


		chThdSleepMilliseconds(19);
	}
	return 0;
}

void initServo(void) {
	for (int i = 0; i < SERVO_COUNT; i ++) {
		brain_pin_e p = engineConfiguration->servoOutputPins[i];
		pins[i].initPin("servo", p);
	}

	chThdCreateStatic(servoThreadStack, sizeof(servoThreadStack), PRIO_SERVO, (tfunc_t)(void*) seThread, NULL);
}
#endif /* EFI_SERVO */

