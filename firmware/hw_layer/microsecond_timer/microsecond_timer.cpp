/**
 * @file	microsecond_timer.cpp
 *
 * Here we have a 1MHz timer dedicated to event scheduling. We are using one of the 32-bit timers here,
 * so this timer can schedule events up to 4B/100M ~ 4000 seconds ~ 1 hour from current time.
 *
 * GPT5 timer clock: 84000000Hz
 * If only it was a better multiplier of 2 (84000000 = 328125 * 256)
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "microsecond_timer.h"
#include "port_microsecond_timer.h"

#if EFI_PROD_CODE

#include "periodic_task.h"
// Just in case we have a mechanism to validate that hardware timer is clocked right and all the
// conversions between wall clock and hardware frequencies are done right
// delay in milliseconds
#define TEST_CALLBACK_DELAY 10
// if hardware timer is 20% off we throw a critical error and call it a day
// maybe this threshold should be 5%? 10%?
#define TIMER_PRECISION_THRESHOLD 0.2

/**
 * Maximum duration of complete timer callback, all pending events together
 * See also 'maxEventCallbackDuration' for maximum duration of one event
 */
uint32_t maxPrecisionCallbackDuration = 0;

static efitick_t lastSetTimerTimeNt;
static bool isTimerPending = false;

static int timerCallbackCounter = 0;
static int timerRestartCounter = 0;

static const char * msg;

static char buff[32];

static int timerFreezeCounter = 0;
static int setHwTimerCounter = 0;
static bool hwStarted = false;

/**
 * sets the alarm to the specified number of microseconds from now.
 * This function should be invoked under kernel lock which would disable interrupts.
 */
void setHardwareSchedulerTimer(efitick_t nowNt, efitick_t setTimeNt) {
	efiAssertVoid(OBD_PCM_Processor_Fault, hwStarted, "HW.started");

	// How many ticks in the future is this event?
	auto timeDeltaNt = setTimeNt - nowNt;

	setHwTimerCounter++;

	/**
	 * #259 BUG error: not positive deltaTimeNt
	 * Once in a while we night get an interrupt where we do not expect it
	 */
	if (timeDeltaNt <= 0) {
		timerFreezeCounter++;
		warning(CUSTOM_OBD_LOCAL_FREEZE, "local freeze cnt=%d", timerFreezeCounter);
	}

	// We need the timer to fire after we return - 1 doesn't work as it may actually schedule in the past
	if (timeDeltaNt < US2NT(2)) {
		timeDeltaNt = US2NT(2);
	}

	if (timeDeltaNt >= TOO_FAR_INTO_FUTURE_NT) {
		// we are trying to set callback for too far into the future. This does not look right at all
		firmwareError(CUSTOM_ERR_TIMER_OVERFLOW, "setHardwareSchedulerTimer() too far: %d", timeDeltaNt);
		return;
	}

	// Skip scheduling if there's a firmware error active
	if (hasFirmwareError()) {
		return;
	}

	// Do the actual hardware-specific timer set operation
	portSetHardwareSchedulerTimer(nowNt, setTimeNt);

	lastSetTimerTimeNt = getTimeNowNt();
	isTimerPending = true;
	timerRestartCounter++;
}

void globalTimerCallback();

void portMicrosecondTimerCallback() {
	timerCallbackCounter++;
	isTimerPending = false;

	uint32_t before = getTimeNowLowerNt();
	globalTimerCallback();
	uint32_t precisionCallbackDuration = getTimeNowLowerNt() - before;
	if (precisionCallbackDuration > maxPrecisionCallbackDuration) {
		maxPrecisionCallbackDuration = precisionCallbackDuration;
	}
}

class MicrosecondTimerWatchdogController : public PeriodicTimerController {
	void PeriodicTask() override {
		efitick_t nowNt = getTimeNowNt();
		if (nowNt >= lastSetTimerTimeNt + 2 * CORE_CLOCK) {
			strcpy(buff, "no_event");
			itoa10(&buff[8], lastSetTimerTimeNt);
			firmwareError(CUSTOM_ERR_SCHEDULING_ERROR, buff);
			return;
		}

		msg = isTimerPending ? "No_cb too long" : "Timer not awhile";
		// 2 seconds of inactivity would not look right
		efiAssertVoid(CUSTOM_ERR_6682, nowNt < lastSetTimerTimeNt + 2 * CORE_CLOCK, msg);
	}

	int getPeriodMs() override {
		return 500;
	}
};

static MicrosecondTimerWatchdogController watchdogControllerInstance;

static scheduling_s watchDogBuddy;

static void watchDogBuddyCallback(void*) {
	/**
	 * the purpose of this periodic activity is to make watchdogControllerInstance
	 * watchdog happy by ensuring that we have scheduler activity even in case of very broken configuration
	 * without any PWM or input pins
	 */
	engine->executor.scheduleForLater(&watchDogBuddy, MS2US(1000), watchDogBuddyCallback);
}

static volatile bool testSchedulingHappened = false;
static efitimems_t testSchedulingStart;

static void timerValidationCallback(void*) {
	testSchedulingHappened = true;
	efitimems_t actualTimeSinceScheduling = (currentTimeMillis() - testSchedulingStart);
	
	if (absI(actualTimeSinceScheduling - TEST_CALLBACK_DELAY) > TEST_CALLBACK_DELAY * TIMER_PRECISION_THRESHOLD) {
		firmwareError(CUSTOM_ERR_TIMER_TEST_CALLBACK_WRONG_TIME, "hwTimer broken precision: %ld ms", actualTimeSinceScheduling);
	}
}

/**
 * This method would validate that hardware timer callbacks happen with some reasonable precision
 * helps to make sure our GPT hardware settings are somewhat right
 */
static void validateHardwareTimer() {
	if (hasFirmwareError()) {
		return;
	}
	testSchedulingStart = currentTimeMillis();

	// to save RAM let's use 'watchDogBuddy' here once before we enable watchdog
	engine->executor.scheduleForLater(&watchDogBuddy, MS2US(TEST_CALLBACK_DELAY), timerValidationCallback);

	chThdSleepMilliseconds(TEST_CALLBACK_DELAY + 2);
	if (!testSchedulingHappened) {
		firmwareError(CUSTOM_ERR_TIMER_TEST_CALLBACK_NOT_HAPPENED, "hwTimer not alive");
	}
}

void initMicrosecondTimer() {
	portInitMicrosecondTimer();

	hwStarted = true;

	lastSetTimerTimeNt = getTimeNowNt();

	validateHardwareTimer();

	watchDogBuddyCallback(NULL);
#if EFI_EMULATE_POSITION_SENSORS
	watchdogControllerInstance.Start();
#endif /* EFI_EMULATE_POSITION_SENSORS */
}

#endif /* EFI_PROD_CODE */
