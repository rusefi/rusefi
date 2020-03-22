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
#define TEST_BENCH
#ifdef TEST_BENCH
static uint8_t loop = 0;
#define setDbgHigh   enginePins.dizzyOutput.setValue(true)
#define setDbgLow    enginePins.dizzyOutput.setValue(false)
#endif


struct tach_ctx {
	OutputPin *Pin;
	bool State;
	int action;
};

static scheduling_s schHigh;
static scheduling_s schLow;
static tach_ctx ctx_high;
static tach_ctx ctx_low;
static angle_t angleHigh;
static angle_t angleLow;
static int maxActions;

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

static void setTach(void *param) {
	auto ctx = reinterpret_cast<tach_ctx *>(param);
	uint32_t timeStamp = getTimeNowUs();
	ctx->Pin->setValue(ctx->State);
	ctx->action++;
	if (ctx->action < maxActions){
		if(ctx->State == true)
		{
			scheduleByAngle(&schHigh, timeStamp, angleHigh, {&setTach, &ctx} PASS_ENGINE_PARAMETER_SUFFIX);	
		}else{
			scheduleByAngle(&schLow, timeStamp, angleLow, {&setTach, &ctx} PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
}

static void tachSignalCallback(trigger_event_e ckpSignalType,
		uint32_t index, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// only process at index configured to avoid too much cpu time for index 0?
	if (index != (uint32_t)engineConfiguration->tachPulseTriggerIndex) {
		return;
	}

#ifdef TEST_BENCH
	if (loop == 0)
	{
		setDbgHigh;
		loop++;
	}
	else
	{
		setDbgLow;
		loop = 0;
	}
#endif	

#if EFI_UNIT_TEST
	printf("tachSignalCallback(%d %d)\n", ckpSignalType, index);
#else
	UNUSED(ckpSignalType);
#endif

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
	angleHigh = period * duty;
	angleLow = period - angleHigh;

	// cam trigger vs. crank trigger vs. 2 stroke trigger shapes fire this callback either once
	// per rev, or once every other rev.  Adjust appropriately.
	maxActions = periods * multiplierFromEngineType(PASS_ENGINE_PARAMETER_SIGNATURE);

	//schedule the first two events
	ctx_high.action = 0;
	scheduleByAngle(&schHigh, edgeTimestamp, 0.0, {&setTach, &ctx_high} PASS_ENGINE_PARAMETER_SUFFIX);
	ctx_low.action = 0;
	scheduleByAngle(&schLow, edgeTimestamp, angleLow, {&setTach, &ctx_low} PASS_ENGINE_PARAMETER_SUFFIX);
}

void initTachometer(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	if (CONFIG(tachOutputPin) == GPIO_UNASSIGNED) {
		return;
	}

	enginePins.tachOut.initPin("analog tach output", CONFIG(tachOutputPin), &CONFIG(tachOutputPinMode));
#ifdef TEST_BENCH
    enginePins.dizzyOutput.initPin("dizzy tach",  GPIOE_8,&engineConfiguration->dizzySparkOutputPinMode);
#endif
	ctx_high.Pin = &enginePins.tachOut;
	ctx_high.State = true;

	ctx_low.Pin = &enginePins.tachOut;
	ctx_low.State = false;

#if EFI_SHAFT_POSITION_INPUT
	#if EFI_UNIT_TEST
		printf("registerCkpListener: tach\n");
	#endif
	addTriggerEventListener(tachSignalCallback, "tach", engine);
#endif /* EFI_SHAFT_POSITION_INPUT */
}



