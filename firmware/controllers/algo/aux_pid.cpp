/*
 * @file aux_pid.cpp
 *
 * This class is a copy-paste of alternatorController.cpp TODO: do something about it? extract more common logic?
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "aux_pid.h"
#include "LocalVersionHolder.h"
#include "allsensors.h"

#if EFI_AUX_PID || defined(__DOXYGEN__)
#include "pwm_generator.h"
#include "tunerstudio_configuration.h"
#include "fsio_impl.h"
#include "engine_math.h"
#include "pin_repository.h"

EXTERN_ENGINE
;

extern fsio8_Map3D_f32t fsioTable1;

// todo: this is to some extent a copy-paste of alternatorController. maybe same loop
// for all PIDs?

extern TunerStudioOutputChannels tsOutputChannels;

static THD_WORKING_AREA(auxPidThreadStack, UTILITY_THREAD_STACK_SIZE);

static LocalVersionHolder parametersVersion;
static SimplePwm auxPidPwm[AUX_PID_COUNT];
static OutputPin auxPidPin[AUX_PID_COUNT];

static pid_s *auxPidS = &persistentState.persistentConfiguration.engineConfiguration.auxPid[0];
static Pid auxPid(auxPidS);
static Logging *logger;

static bool isEnabled(int index) {
	// todo: implement bit arrays for configuration
	switch(index) {
	case 0:
		return engineConfiguration->activateAuxPid1;
	case 1:
		return engineConfiguration->activateAuxPid2;
	case 2:
		return engineConfiguration->activateAuxPid3;
	default:
		return engineConfiguration->activateAuxPid4;
	}
}

static void pidReset(void) {
	auxPid.reset();
}

static msg_t auxPidThread(int param) {
	UNUSED(param);
		chRegSetThreadName("AuxPidController");
		while (true) {
			auxPid.sleep();

			if (parametersVersion.isOld()) {
				pidReset();
			}

			float rpm = GET_RPM();

			// todo: make this configurable?
			bool enabledAtCurrentRpm = rpm > engineConfiguration->cranking.rpm;

			if (!enabledAtCurrentRpm) {
				// we need to avoid accumulating iTerm while engine is not running
				pidReset();
				continue;
			}


			float value = engine->triggerCentral.vvtPosition; // getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE); // that's temporary
			float targetValue = fsioTable1.getValue(rpm, getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE));

			percent_t pwm = auxPid.getValue(targetValue, value);
			if (engineConfiguration->isVerboseAuxPid1) {
				scheduleMsg(logger, "aux duty: %.2f/value=%.2f/p=%.2f/i=%.2f/d=%.2f int=%.2f", pwm, value,
						auxPid.getP(), auxPid.getI(), auxPid.getD(), auxPid.getIntegration());
			}


			if (engineConfiguration->debugMode == DBG_AUX_PID_1) {
				auxPid.postState(&tsOutputChannels);
				tsOutputChannels.debugIntField3 = (int)(10 * targetValue);
			}

			auxPidPwm[0].setSimplePwmDutyCycle(pwm / 100);


		}
#if defined __GNUC__
	return -1;
#endif
}

static void turnAuxPidOn(int index) {
	if (!isEnabled(index)) {
		return;
	}

	if (engineConfiguration->auxPidPins[index] == GPIO_UNASSIGNED) {
		return;
	}

	startSimplePwmExt(&auxPidPwm[index], "Aux PID", engineConfiguration->auxPidPins[index],
			&auxPidPin[0],
			engineConfiguration->auxPidFrequency[index], 0.1, applyPinState);
}

void startAuxPins(void) {
	for (int i = 0;i <AUX_PID_COUNT;i++) {
		turnAuxPidOn(i);
	}
}

void stopAuxPins(void) {
	for (int i = 0;i < AUX_PID_COUNT;i++) {
		unmarkPin(activeConfiguration.auxPidPins[i]);
	}
}

void initAuxPid(Logging *sharedLogger) {
	chThdCreateStatic(auxPidThreadStack, sizeof(auxPidThreadStack), LOWPRIO,
			(tfunc_t) auxPidThread, NULL);

	logger = sharedLogger;

	startAuxPins();
}

#endif
