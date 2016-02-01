/**
 * @file    alternatorController.cpp
 * @brief   alternator controller - turn alternator off if you do not really need it
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "main.h"
#include "engine.h"
#include "rpm_calculator.h"
#include "pwm_generator.h"
#include "alternatorController.h"
#include "pin_repository.h"
#include "voltage.h"
#include "pid.h"
#include "LocalVersionHolder.h"
#include "tunerstudio_configuration.h"

#if EFI_ALTERNATOR_CONTROL || defined(__DOXYGEN__)

EXTERN_ENGINE
;

static Logging *logger;

#define ALTERNATOR_VALVE_PWM_FREQUENCY 300

static SimplePwm alternatorControl;
static OutputPin alternatorPin;
static pid_s *altPidS = &persistentState.persistentConfiguration.engineConfiguration.alternatorControl;
static Pid altPid(altPidS, 1, 90);

static THD_WORKING_AREA(alternatorControlThreadStack, UTILITY_THREAD_STACK_SIZE);

static float currentAltDuty;

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
static LocalVersionHolder parametersVersion;
extern TunerStudioOutputChannels tsOutputChannels;
#endif

static msg_t AltCtrlThread(int param) {
	UNUSED(param);
	chRegSetThreadName("AlternatorController");
	while (true) {
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (parametersVersion.isOld())
			altPid.reset();
#endif

		int dt = maxI(10, engineConfiguration->alternatorDT);
		chThdSleepMilliseconds(dt);

		// todo: migrate this to FSIO
		bool alternatorShouldBeEnabledAtCurrentRpm = engine->rpmCalculator.rpmValue > 400;
		engine->isAlternatorControlEnabled = CONFIG(isAlternatorControlEnabled) && alternatorShouldBeEnabledAtCurrentRpm;

		if (!engine->isAlternatorControlEnabled) {
			// we need to avoid accumulating iTerm while engine is not spinning
			altPid.reset();
			continue;
		}

		currentAltDuty = engineConfiguration->alternatorOffset + altPid.getValue(engineConfiguration->targetVBatt, getVBatt(PASS_ENGINE_PARAMETER_F), 1);
		if (boardConfiguration->isVerboseAlternator) {
			scheduleMsg(logger, "alt duty: %f/vbatt=%f/p=%f/i=%f/d=%f int=%f", currentAltDuty, getVBatt(PASS_ENGINE_PARAMETER_F),
					altPid.getP(), altPid.getI(), altPid.getD(), altPid.getIntegration());
		}

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
		tsOutputChannels.debugFloatField1 = currentAltDuty;
		tsOutputChannels.debugFloatField2 = altPid.getIntegration();
		tsOutputChannels.debugFloatField3 = altPid.getPrevError();
#endif

		alternatorControl.setSimplePwmDutyCycle(currentAltDuty / 100);
	}
#if defined __GNUC__
	return -1;
#endif
}


void showAltInfo(void) {
	scheduleMsg(logger, "alt=%s @%s t=%dms", boolToString(engineConfiguration->isAlternatorControlEnabled),
			hwPortname(boardConfiguration->alternatorControlPin),
			engineConfiguration->alternatorDT);
	scheduleMsg(logger, "p=%f/i=%f/d=%f offset=%f", engineConfiguration->alternatorControl.pFactor,
			0, 0, engineConfiguration->alternatorOffset); // todo: i & d
	scheduleMsg(logger, "vbatt=%f/duty=%f/target=%f", getVBatt(PASS_ENGINE_PARAMETER_F), currentAltDuty,
			engineConfiguration->targetVBatt);
}

void setAltPFactor(float p) {
	engineConfiguration->alternatorControl.pFactor = p;
	scheduleMsg(logger, "setAltPid: %f", p);
	altPid.reset();
	showAltInfo();
}

static void applyAlternatorPinState(PwmConfig *state, int stateIndex) {
	efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(state->multiWave.waveCount == 1, "invalid idle waveCount");
	OutputPin *output = state->outputPins[0];
	int value = state->multiWave.waves[0].pinStates[stateIndex];
	if (!value || engine->isAlternatorControlEnabled)
		output->setValue(value);
}

void setDefaultAlternatorParameters(void) {
	engineConfiguration->alternatorOffAboveTps = 120;

	boardConfiguration->alternatorControlPin = GPIO_UNASSIGNED;
	boardConfiguration->alternatorControlPinMode = OM_DEFAULT;
	engineConfiguration->targetVBatt = 14;

	engineConfiguration->alternatorOffset = 0;
	engineConfiguration->alternatorControl.pFactor = 30;
	engineConfiguration->alternatorDT = 100;
}

void initAlternatorCtrl(Logging *sharedLogger) {
	logger = sharedLogger;
	addConsoleAction("altinfo", showAltInfo);
	if (boardConfiguration->alternatorControlPin == GPIO_UNASSIGNED)
		return;

	startSimplePwmExt(&alternatorControl, "Alternator control", boardConfiguration->alternatorControlPin,
			&alternatorPin,
			ALTERNATOR_VALVE_PWM_FREQUENCY, 0.1, applyAlternatorPinState);
	chThdCreateStatic(alternatorControlThreadStack, sizeof(alternatorControlThreadStack), LOWPRIO,
			(tfunc_t) AltCtrlThread, NULL);
}

#endif /* EFI_ALTERNATOR_CONTROL */
