/*
 * @file aux_pid.cpp
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "aux_pid.h"

#if EFI_AUX_PID || defined(__DOXYGEN__)

EXTERN_ENGINE
;

// todo: this is to some extent a copy-paste of alternatorController. maybe same loop
// for all PIDs?

static THD_WORKING_AREA(auxPidThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t auxPidThread(int param) {
	UNUSED(param);
		chRegSetThreadName("AuxPidController");
		while (true) {
			int dt = maxI(10, engineConfiguration->auxPid1DT);
			chThdSleepMilliseconds(dt);


		}
#if defined __GNUC__
	return -1;
#endif
}

void initAuxPid(Logging *sharedLogger) {
	chThdCreateStatic(auxPidThreadStack, sizeof(auxPidThreadStack), LOWPRIO,
			(tfunc_t) auxPidThread, NULL);

}

#endif
