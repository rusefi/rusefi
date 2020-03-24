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
#include "pwm_generator.h"

EXTERN_ENGINE;


static SimplePwm tachControl("tach");
static bool isTachEnabled;

static void applyTachoPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
	efiAssertVoid(CUSTOM_ERR_6643, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(CUSTOM_ERR_PWM_CALLBACK_ASSERT, state->multiChannelStateSequence.waveCount == 1, "invalid idle waveCount");
	OutputPin *output = state->outputPins[0];
	int value = state->multiChannelStateSequence.getChannelState(/*channelIndex*/0, stateIndex);

	if (!value || isTachEnabled)
		output->setValue(value);
}        

static void tachSignalCallback(trigger_event_e ckpSignalType,
		uint32_t index, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// only process at index configured to avoid too much cpu time for index 0?
	if (index != (uint32_t)engineConfiguration->tachPulseTriggerIndex) {
		return;
	}

#if EFI_UNIT_TEST
	printf("tachSignalCallback(%d %d)\n", ckpSignalType, index);
#else
	UNUSED(ckpSignalType);
	UNUSED(edgeTimestamp);
#endif

	// How many tach pulse periods do we have?
	int periods = engineConfiguration->tachPulsePerRev;

	if(periods == 0){
		warning(CUSTOM_ERR_6709,"Check Tachometer Pulse per Rev!");
		return;
	}

	// What is the angle per tach output period?
	float duty;
	float cycleTimeMs = 60000.0 / GET_RPM();
	float periodTimeMs = cycleTimeMs / periods;
	float tachFreq = 1000.0 / periodTimeMs;
	
	if (engineConfiguration->tachPulseDurationAsDutyCycle) {
		// Simple case - duty explicitly set
		duty = engineConfiguration->tachPulseDuractionMs;
	} else {
		// Constant high-time mode - compute the correct duty cycle
		duty = engineConfiguration->tachPulseDuractionMs / periodTimeMs;
	}

	// limit to 10..90% duty
	duty = maxF(0.1f, minF(duty, 0.9f));

	tachControl.setSimplePwmDutyCycle(duty);
	if (tachFreq >= 1.0){
		tachControl.setFrequency(tachFreq);
		isTachEnabled = true;
	}else{
		isTachEnabled = false;
	}
	

}

void initTachometer(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	if (CONFIG(tachOutputPin) == GPIO_UNASSIGNED) {
		return;
	}

	isTachEnabled = false;
#ifdef EFI_UNIT_TEST
	printf("initTachometer\n");
#endif

#if EFI_SHAFT_POSITION_INPUT
	addTriggerEventListener(tachSignalCallback, "tach", engine);
#endif /* EFI_SHAFT_POSITION_INPUT */

startSimplePwmExt(&tachControl,
				"analog tach output",
				&engine->executor,
				CONFIG(tachOutputPin),
				&enginePins.tachOut,
				1.0, 0.1, (pwm_gen_callback*)applyTachoPinState);

}



