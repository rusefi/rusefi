/**
 * @file trigger_emulator_algo.c
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#include "main.h"
#include "trigger_emulator_algo.h"
#include "engine_configuration.h"
#include "wave_math.h"
#include "LocalVersionHolder.h"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

#if EFI_PROD_CODE && defined __GNUC__
PwmConfig configuration __attribute__((section(".ccm")));
#else
PwmConfig configuration;
#endif

static Logging logger;
static LocalVersionHolder localVersion;

void setTriggerEmulatorRPM(int rpm) {
	if (rpm == 0) {
		configuration.periodMs = NAN;
	} else {
		float gRpm = rpm * engineConfiguration->rpmMultiplier / 60.0; // per minute converted to per second
		configuration.periodMs = frequency2period(gRpm);
	}
	scheduleMsg(&logger, "Emulating position sensor(s). RPM=%d", rpm);
}

static void updateTriggerShapeIfNeeded(PwmConfig *state) {
	if(localVersion.isOld()) {
		scheduleMsg(&logger, "Stimulator: updating trigger shape: %d/%d %d", localVersion.getVersion(), getGlobalConfigurationVersion(), currentTimeMillis());

		trigger_shape_s *s = &engineConfiguration2->triggerShape;
		int *pinStates[2] = {s->wave.waves[0].pinStates, s->wave.waves[1].pinStates};
		copyPwmParameters(state, s->size, s->wave.switchTimes, 2, pinStates);
		state->safe.periodMs = -1; // this would cause loop re-initialization
	}
}

void initTriggerEmulatorLogic(pwm_gen_callback *stateChangeCallback) {
	initLogging(&logger, "position sensor(s) emulator");


	trigger_shape_s *s = &engineConfiguration2->triggerShape;
	setTriggerEmulatorRPM(DEFAULT_EMULATION_RPM);
	int *pinStates[2] = {s->wave.waves[0].pinStates, s->wave.waves[1].pinStates};
	weComplexInit("position sensor", &configuration, s->size, s->wave.switchTimes, 2, pinStates,
			updateTriggerShapeIfNeeded,
			stateChangeCallback);

	addConsoleActionI("rpm", &setTriggerEmulatorRPM);

}
