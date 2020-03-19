/*
 * @file tachometer.cpp
 * @brief This is about driving external analog tachometers
 *
 * This implementation produces one pulse per engine cycle
 *
 * todo: these is a bit of duplication with dizzySparkOutputPin
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "tachometer.h"
#include "trigger_central.h"


EXTERN_ENGINE;


// [4 pulse/rev] * [2 edges per pulse] * [2 revs per cycle] = 16
#define MAX_EVENTS	16
static scheduling_s events[MAX_EVENTS];

struct tach_ctx {
	OutputPin *Pin;
	bool State;
};

static tach_ctx ctx_high;
static tach_ctx ctx_low;

static void setTach(void *param) {
	auto ctx = reinterpret_cast<tach_ctx *>(param);
	ctx->Pin->setValue(ctx->State);
}

static int multiplierFromEngineType(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	switch (engineConfiguration->ambiguousOperationMode) {
	// These modes have index 0 once per rev
	case TWO_STROKE:
	case FOUR_STROKE_CRANK_SENSOR:
		return 1;
	// This mode has index 0 once every other rev (once per cam rev)
	case FOUR_STROKE_CAM_SENSOR:
		return 2;
	case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
		// is this correct? or should it be 0.5?
		return 1;
	default:
		//warning(0, "Unknown ");
		return 1;
	}
}

static void tachSignalCallback(trigger_event_e ckpSignalType,
		uint32_t index, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// only process at index 0 - we schedule the full cycle all at once
	if (index != (uint32_t)engineConfiguration->tachPulseTriggerIndex) {
		return;
	}

#if EFI_UNIT_TEST
	printf("tachSignalCallback(%d %d)\n", ckpSignalType, index);
#else
	UNUSED(ckpSignalType);
#endif

	// TODO: warning if periods is set too high

	// How many tach pulse periods do we have?
	int periods = engineConfiguration->tachPulsePerRev;

	if(periods == 0){
		warning(CUSTOM_ERR_6709,"Check Tachometer Pulse per Rev!");
		return;
	}
	// What is the angle per tach output period?
	const angle_t period = 360.0 / periods;

	float duty;

	if (engineConfiguration->tachPulseDurationAsDutyCycle) {
		// Simple case - duty explicitly set
		duty = engineConfiguration->tachPulseDuractionMs;
	} else {
		// Constant high-time mode - compute the correct duty cycle
		float cycleTimeMs = 60000.0 / GET_RPM();
		float periodTimeMs = cycleTimeMs / periods;

		duty = engineConfiguration->tachPulseDuractionMs / periodTimeMs;
	}

	// limit to 10..90% duty
	duty = maxF(0.1f, minF(duty, 0.9f));

	// Use duty to compute the angle widths of high/low periods
	angle_t angleHigh = period * duty;
	angle_t angleLow = period - angleHigh;

	angle_t angle = 0;

	// cam trigger vs. crank trigger vs. 2 stroke trigger shapes fire this callback either once
	// per rev, or once every other rev.  Adjust appropriately.
	periods *= multiplierFromEngineType(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (periods >= MAX_EVENTS){
		warning(CUSTOM_DUTY_INVALID,"Check Tachometer Pulse per Rev!");
		return;
	}

	for (int i = 0; i < periods; i++) {
		// Rising edge
		scheduleByAngle(&events[2 * i], edgeTimestamp, angle, {&setTach, &ctx_high} PASS_ENGINE_PARAMETER_SUFFIX);
		angle += angleHigh;

		// Followed by falling edge
		scheduleByAngle(&events[2 * i + 1], edgeTimestamp, angle, {&setTach, &ctx_low} PASS_ENGINE_PARAMETER_SUFFIX);
		angle += angleLow;
	}
}

void initTachometer(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
//#if !EFI_UNIT_TEST
#if EFI_UNIT_TEST
	printf("initTachometer\n");
#endif
	if (CONFIG(tachOutputPin) == GPIO_UNASSIGNED) {
		return;
	}

	enginePins.tachOut.initPin("analog tach output", CONFIG(tachOutputPin), &CONFIG(tachOutputPinMode));


	ctx_high.Pin = &enginePins.tachOut;
	ctx_high.State = true;

	ctx_low.Pin = &enginePins.tachOut;
	ctx_low.State = false;

#if EFI_SHAFT_POSITION_INPUT
	addTriggerEventListener(tachSignalCallback, "tach", engine);
#endif /* EFI_SHAFT_POSITION_INPUT */
}



