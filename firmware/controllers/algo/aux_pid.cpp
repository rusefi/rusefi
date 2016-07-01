/*
 * @file aux_pid.cpp
 *
 * This class is a copy-paste of alternatorController.cpp TODO: do something about it? extract more common logic?
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "aux_pid.h"
#include "LocalVersionHolder.h"
#include "allsensors.h"

#if EFI_AUX_PID || defined(__DOXYGEN__)
#include "pwm_generator.h"
#include "tunerstudio_configuration.h"

EXTERN_ENGINE
;

// todo: this is to some extent a copy-paste of alternatorController. maybe same loop
// for all PIDs?

extern TunerStudioOutputChannels tsOutputChannels;

static THD_WORKING_AREA(auxPidThreadStack, UTILITY_THREAD_STACK_SIZE);

static LocalVersionHolder parametersVersion;
static SimplePwm auxPid1;
static OutputPin auxPid1Pin;
static pid_s *auxPidS = &persistentState.persistentConfiguration.engineConfiguration.auxPid1;
static Pid auxPid(auxPidS, 1, 90);
static Logging *logger;

static msg_t auxPidThread(int param) {
	UNUSED(param);
		chRegSetThreadName("AuxPidController");
		while (true) {
			int dt = maxI(10, engineConfiguration->auxPid1DT);
			chThdSleepMilliseconds(dt);

			if (parametersVersion.isOld())
				auxPid.reset();

			float value = getVBatt(PASS_ENGINE_PARAMETER_F); // that's temporary
			float targetValue = engineConfiguration->targetVBatt; // that's temporary

			float pwm = auxPid.getValue(targetValue, value, 1);
			if (engineConfiguration->isVerboseAuxPid) {
				scheduleMsg(logger, "aux duty: %f/value=%f/p=%f/i=%f/d=%f int=%f", pwm, value,
						auxPid.getP(), auxPid.getI(), auxPid.getD(), auxPid.getIntegration());
			}


			if (engineConfiguration->debugMode == AUX_PID_1) {
				tsOutputChannels.debugFloatField1 = pwm;
				auxPid.postState(&tsOutputChannels);
			}

			auxPid1.setSimplePwmDutyCycle(pwm / 100);


		}
#if defined __GNUC__
	return -1;
#endif
}

void initAuxPid(Logging *sharedLogger) {
	chThdCreateStatic(auxPidThreadStack, sizeof(auxPidThreadStack), LOWPRIO,
			(tfunc_t) auxPidThread, NULL);

	logger = sharedLogger;

	if (engineConfiguration->activateAuxPid1) {
		return;
	}

	if (boardConfiguration->fsioPins[0] == GPIO_UNASSIGNED) {
		return;
	}

	startSimplePwmExt(&auxPid1, "Aux PID", boardConfiguration->fsioPins[0],
			&auxPid1Pin,
			boardConfiguration->fsioFrequency[0], 0.1, applyPinState);
}

#endif
