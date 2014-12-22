/**
 * @file	idle_controller.cpp
 * @brief	Simple Idle Air Valve control algorithm
 *
 *			This algorithm is trying to get current RPM to the desired 'target' value
 *			by changing Idle Air Valve solenoid duty cycle. Depending on how far current RPM
 *			is from the target RPM, the incremental change would be smaller or bigger.
 *
 *
 * todo: DEFAULT_IDLE_DUTY should be a field on the IdleValveState, not a constant
 *
 * @date May 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "idle_controller.h"
#include "efilib.h"
#include "rpm_calculator.h"
#include "tps.h"

static int lastGoodValue = DEFAULT_IDLE_DUTY;

EXTERN_ENGINE
;

void idleInit(IdleValveState *idle DECLARE_ENGINE_PARAMETER_S) {
	idle->value = DEFAULT_IDLE_DUTY;
	setIdleRpm(idle, engineConfiguration->targetIdleRpm);
	idle->timeOfLastIdleChange = 0;
}

void setIdleRpm(IdleValveState *idle, int targetRpm) {
	idle->targetRpmRangeLeft = (int) (targetRpm * 0.93);
	idle->targetRpmRangeRight = (int) (targetRpm * 1.07);
}

/**
 * @brief	sets new idle valve duty cycle: checks the bounds and reports new value
 */
static int setNewValue(IdleValveState *idle, int currentRpm, int now, const char * msg, int newValue) {
	newValue = maxI(newValue, MIN_IDLE);
	newValue = minI(newValue, MAX_IDLE);

	if (idle->value != newValue) {
		idleDebug(msg, currentRpm);
		idle->timeOfLastIdleChange = now;
	}

	idle->value = newValue;
	return newValue;
}

static int changeValue(IdleValveState *idle, int currentRpm, int now, const char * msg, int delta DECLARE_ENGINE_PARAMETER_S) {
	if (getTPS(PASS_ENGINE_PARAMETER_F) > 5.0) {
		// We are not supposed to be in idle mode. Don't touch anything
		idleDebug("TPS Lockout, TPS=", getTPS(PASS_ENGINE_PARAMETER_F));
		return idle->value;
	}
	int newValue = idle->value + delta;
	return setNewValue(idle, currentRpm, now, msg, newValue);
}

/**
 * now - current time in seconds
 */
int getIdle(IdleValveState *idle, int currentRpm, int now DECLARE_ENGINE_PARAMETER_S) {
	if (currentRpm == 0 || isCranking()) {
		return setNewValue(idle, currentRpm, now, "cranking value: ", DEFAULT_IDLE_DUTY);
	}

	if (currentRpm < 0.7 * idle->targetRpmRangeLeft) {
		return setNewValue(idle, currentRpm, now, "RPMs are seriously low: ", lastGoodValue);
	}

	if (now - idle->timeOfLastIdleChange < IDLE_PERIOD) {
		// too soon to adjust anything - exiting
		return idle->value;
	}

	if (currentRpm > idle->targetRpmRangeLeft && currentRpm < idle->targetRpmRangeRight) {
		// current RPM is good enough
		// todo: need idle signal input
		//lastGoodValue = idle->value;
		return idle->value;
	}

	if (currentRpm >= idle->targetRpmRangeRight + 100)
		return changeValue(idle, currentRpm, now, "idle control: rpm is too high: ", -IDLE_DECREASE_STEP PASS_ENGINE_PARAMETER);

	if (currentRpm >= idle->targetRpmRangeRight)
		return changeValue(idle, currentRpm, now, "idle control: rpm is a bit too high: ", -1 PASS_ENGINE_PARAMETER);

	// we are here if RPM is low, let's see how low
//	if (currentRpm < 0.7 * idle->targetRpmRangeLeft) {
//		// todo: act faster in case of really low RPM?
//		return setNewValue(idle, currentRpm, now, "RPMs are seriously low: ", 15 * IDLE_INCREASE_STEP);
//	} else
	if (currentRpm < idle->targetRpmRangeLeft - 100) {
		return changeValue(idle, currentRpm, now, "idle control: RPMs are low: ", IDLE_INCREASE_STEP PASS_ENGINE_PARAMETER);
	}
	return changeValue(idle, currentRpm, now, "idle control: RPMs are a bit low: ", 1 PASS_ENGINE_PARAMETER);
}
