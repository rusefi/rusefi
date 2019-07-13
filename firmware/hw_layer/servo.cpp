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
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine.h"

#if EFI_SERVO || EFI_SIMULATOR
#include "servo.h"
#include "pin_repository.h"

EXTERN_ENGINE;

THD_WORKING_AREA(servoThreadStack, UTILITY_THREAD_STACK_SIZE);

static OutputPin pins[SERVO_COUNT];

static int countServos() {
	int result = 0;

	for (int i = 0; i < SERVO_COUNT; i++) {
		if (engineConfiguration->servoOutputPins[i] != GPIO_UNASSIGNED) {
			result++;
		}
	}
	return result;

}

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

		engine->executor.scheduleForLater(&servoTurnSignalOff, (int)MS2US(durationMs), (schfunc_t) &servoTachPinLow, pin);


		chThdSleepMilliseconds(19);
	}
	return 0;
}

void initServo(void) {
	for (int i = 0; i < SERVO_COUNT; i ++) {
		brain_pin_e p = engineConfiguration->servoOutputPins[i];
		pins[i].initPin("servo", p);
	}

	chThdCreateStatic(servoThreadStack, sizeof(servoThreadStack), NORMALPRIO, (tfunc_t)(void*) seThread, NULL);
}
#endif /* EFI_SERVO */

