/*
 * @file aux_pid.cpp
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "aux_pid.h"
#include "LocalVersionHolder.h"

#if EFI_AUX_PID || defined(__DOXYGEN__)
#include "pwm_generator.h"

EXTERN_ENGINE
;

// todo: this is to some extent a copy-paste of alternatorController. maybe same loop
// for all PIDs?

static THD_WORKING_AREA(auxPidThreadStack, UTILITY_THREAD_STACK_SIZE);

static LocalVersionHolder parametersVersion;
static SimplePwm auxPid1;
static OutputPin auxPid1Pin;
static pid_s *altPidS = &persistentState.persistentConfiguration.engineConfiguration.alternatorControl;
static Pid altPid(altPidS, 1, 90);

static msg_t auxPidThread(int param) {
	UNUSED(param);
		chRegSetThreadName("AuxPidController");
		while (true) {
			int dt = maxI(10, engineConfiguration->auxPid1DT);
			chThdSleepMilliseconds(dt);

			if (parametersVersion.isOld())
				altPid.reset();



		}
#if defined __GNUC__
	return -1;
#endif
}

void initAuxPid(Logging *sharedLogger) {
	chThdCreateStatic(auxPidThreadStack, sizeof(auxPidThreadStack), LOWPRIO,
			(tfunc_t) auxPidThread, NULL);

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
