/**
 * @file trigger_emulator_algo.cpp
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#include "main.h"
#include "trigger_emulator_algo.h"
#include "engine_configuration.h"
#include "wave_math.h"
#include "LocalVersionHolder.h"
#include "ec2.h"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

/*
 * todo: should we simply re-use instances used by trigger_decoder?
 * todo: since we are emulating same shape we are decoding
 */
static int pinStates1[PWM_PHASE_MAX_COUNT];
static int pinStates2[PWM_PHASE_MAX_COUNT];
static single_wave_s waves[2] = {single_wave_s(pinStates1), single_wave_s(pinStates2)};
static single_wave_s sr[2] = {waves[0], waves[1]};

static float swtchTms[PWM_PHASE_MAX_COUNT];

PwmConfig triggerSignal(swtchTms, sr);

static Logging logger;
static LocalVersionHolder localVersion;

void setTriggerEmulatorRPM(int rpm) {
	/**
	 * All we need to do here is to change the periodMs
	 * togglePwmState() would see that the periodMs has changed and act accordingly
	 */
	if (rpm == 0) {
		triggerSignal.periodMs = NAN;
	} else {
		float gRpm = rpm * engineConfiguration->rpmMultiplier / 60.0; // per minute converted to per second
		triggerSignal.periodMs = frequency2period(gRpm);
	}
	scheduleMsg(&logger, "Emulating position sensor(s). RPM=%d", rpm);
}

static void updateTriggerShapeIfNeeded(PwmConfig *state) {
	if(localVersion.isOld()) {
		scheduleMsg(&logger, "Stimulator: updating trigger shape: %d/%d %d", localVersion.getVersion(), getGlobalConfigurationVersion(), currentTimeMillis());

		trigger_shape_s *s = &engineConfiguration2->triggerShape;
		int *pinStates[2] = {s->wave.waves[0].pinStates, s->wave.waves[1].pinStates};
		copyPwmParameters(state, s->getSize(), s->wave.switchTimes, 2, pinStates);
		state->safe.periodMs = -1; // this would cause loop re-initialization
	}
}

void initTriggerEmulatorLogic(pwm_gen_callback *stateChangeCallback) {
	initLogging(&logger, "position sensor(s) emulator");


	trigger_shape_s *s = &engineConfiguration2->triggerShape;
	setTriggerEmulatorRPM(DEFAULT_EMULATION_RPM);
	int *pinStates[2] = { s->wave.waves[0].pinStates, s->wave.waves[1].pinStates};
	weComplexInit("position sensor", &triggerSignal, s->getSize(), s->wave.switchTimes, 2, pinStates,
			updateTriggerShapeIfNeeded,
			stateChangeCallback);

	addConsoleActionI("rpm", &setTriggerEmulatorRPM);

}
