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


struct tach_ctx {
	OutputPin *Pin;
	bool State;
	int action;
#ifdef EFI_UNIT_TEST
	Engine *engine;
	engine_configuration_s* engineConfiguration;
	persistent_config_s* config;
#endif	
};

static scheduling_s schHigh;
static scheduling_s schLow;
static tach_ctx ctx_high;
static tach_ctx ctx_low;
static angle_t period;
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
	uint32_t timeStamp = getTimeNowNt();
	ctx->Pin->setValue(ctx->State);
	ctx->action++;
	if (ctx->action < maxActions){
		if(ctx->State == true)
		{
#ifdef EFI_UNIT_TEST
			scheduleByAngle(&schHigh, timeStamp, period, {&setTach, &ctx_high}, ctx_high.engine, ctx_high.engineConfiguration, ctx_high.config);
#else			
			scheduleByAngle(&schHigh, timeStamp, period, {&setTach, &ctx_high} PASS_ENGINE_PARAMETER_SUFFIX);	
#endif			
		}else{
#ifdef EFI_UNIT_TEST
			scheduleByAngle(&schLow, timeStamp, period, {&setTach, &ctx_low}, ctx_low.engine, ctx_low.engineConfiguration, ctx_low.config);
#else			

			scheduleByAngle(&schLow, timeStamp, period, {&setTach, &ctx_low} PASS_ENGINE_PARAMETER_SUFFIX);
#endif
		}
	}
}

static void tachSignalCallback(trigger_event_e ckpSignalType,
		uint32_t index, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// only process at index configured to avoid too much cpu time for index 0?
	if (index != (uint32_t)engineConfiguration->tachPulseTriggerIndex) {
		return;
	}

	angle_t angleDuty;

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

	ctx_high.action = 0;
	ctx_low.action = 0;
	
	// What is the angle per tach output period?
	period = 360.0 / periods;

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
	angleDuty = period * duty;

	// cam trigger vs. crank trigger vs. 2 stroke trigger shapes fire this callback either once
	// per rev, or once every other rev.  Adjust appropriately.
	maxActions = periods * multiplierFromEngineType(PASS_ENGINE_PARAMETER_SIGNATURE);

	//schedule the first two events
	scheduleByAngle(&schHigh, edgeTimestamp, 0.0, {&setTach, &ctx_high} PASS_ENGINE_PARAMETER_SUFFIX);

	scheduleByAngle(&schLow, edgeTimestamp, angleDuty, {&setTach, &ctx_low} PASS_ENGINE_PARAMETER_SUFFIX);
}

void initTachometer(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	if (CONFIG(tachOutputPin) == GPIO_UNASSIGNED) {
		return;
	}

	enginePins.tachOut.initPin("analog tach output", CONFIG(tachOutputPin), &CONFIG(tachOutputPinMode));

	ctx_high.Pin = &enginePins.tachOut;
	ctx_high.State = true;
#ifdef EFI_UNIT_TEST
	ctx_high.engine = engine;
	ctx_high.engineConfiguration = engineConfiguration;
	ctx_high.config = config;
#endif

	ctx_low.Pin = &enginePins.tachOut;
	ctx_low.State = false;
#ifdef EFI_UNIT_TEST
	ctx_low.engine = engine;
	ctx_low.engineConfiguration = engineConfiguration;
	ctx_low.config = config;
#endif


#ifdef EFI_UNIT_TEST
	printf("initTachometer\n");
#endif
#if EFI_SHAFT_POSITION_INPUT
	addTriggerEventListener(tachSignalCallback, "tach", engine);
#endif /* EFI_SHAFT_POSITION_INPUT */
}



