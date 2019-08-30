/**
 * @file    alternator_controller.cpp
 * @brief   alternator controller - some newer vehicles control alternator with ECU
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#if EFI_ALTERNATOR_CONTROL
#include "engine.h"
#include "rpm_calculator.h"
#include "alternator_controller.h"
#include "voltage.h"
#include "pid.h"
#include "local_version_holder.h"
#include "periodic_task.h"

#include "pwm_generator.h"
#include "pin_repository.h"
#include "tunerstudio_configuration.h"

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

EXTERN_ENGINE
;

static Logging *logger;

static SimplePwm alternatorControl("alt");
static pid_s *altPidS = &persistentState.persistentConfiguration.engineConfiguration.alternatorControl;
static Pid altPid(altPidS);

static percent_t currentAltDuty;

#if EFI_TUNER_STUDIO
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

static bool currentPlainOnOffState = false;
static bool shouldResetPid = false;

static void pidReset(void) {
	altPid.reset();
}

class AlternatorController : public PeriodicTimerController {
	int getPeriodMs() override {
		return GET_PERIOD_LIMITED(&engineConfiguration->alternatorControl);
	}

	void PeriodicTask() override {
#if ! EFI_UNIT_TEST
		if (shouldResetPid) {
			pidReset();
			shouldResetPid = false;
		}
#endif

		if (engineConfiguration->debugMode == DBG_ALTERNATOR_PID) {
			// this block could be executed even in on/off alternator control mode
			// but at least we would reflect latest state
#if EFI_TUNER_STUDIO
			altPid.postState(&tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */
		}

		// todo: migrate this to FSIO
		bool alternatorShouldBeEnabledAtCurrentRpm = GET_RPM_VALUE > engineConfiguration->cranking.rpm;
		engine->isAlternatorControlEnabled = CONFIG(isAlternatorControlEnabled) && alternatorShouldBeEnabledAtCurrentRpm;

		if (!engine->isAlternatorControlEnabled) {
			// we need to avoid accumulating iTerm while engine is not running
			pidReset();
			return;
		}

		float vBatt = getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE);
		float targetVoltage = engineConfiguration->targetVBatt;

		if (CONFIGB(onOffAlternatorLogic)) {
			float h = 0.1;
			bool newState = (vBatt < targetVoltage - h) || (currentPlainOnOffState && vBatt < targetVoltage);
			enginePins.alternatorPin.setValue(newState);
			currentPlainOnOffState = newState;
			if (engineConfiguration->debugMode == DBG_ALTERNATOR_PID) {
#if EFI_TUNER_STUDIO
				tsOutputChannels.debugIntField1 = newState;
#endif /* EFI_TUNER_STUDIO */
			}

			return;
		}


		currentAltDuty = altPid.getOutput(targetVoltage, vBatt);
		if (CONFIGB(isVerboseAlternator)) {
			scheduleMsg(logger, "alt duty: %.2f/vbatt=%.2f/p=%.2f/i=%.2f/d=%.2f int=%.2f", currentAltDuty, vBatt,
					altPid.getP(), altPid.getI(), altPid.getD(), altPid.getIntegration());
		}


		alternatorControl.setSimplePwmDutyCycle(PERCENT_TO_DUTY(currentAltDuty));
	}
};

static AlternatorController instance;

void showAltInfo(void) {
	scheduleMsg(logger, "alt=%s @%s t=%dms", boolToString(engineConfiguration->isAlternatorControlEnabled),
			hwPortname(CONFIGB(alternatorControlPin)),
			engineConfiguration->alternatorControl.periodMs);
	scheduleMsg(logger, "p=%.2f/i=%.2f/d=%.2f offset=%.2f", engineConfiguration->alternatorControl.pFactor,
			0, 0, engineConfiguration->alternatorControl.offset); // todo: i & d
	scheduleMsg(logger, "vbatt=%.2f/duty=%.2f/target=%.2f", getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE), currentAltDuty,
			engineConfiguration->targetVBatt);
}

void setAltPFactor(float p) {
	engineConfiguration->alternatorControl.pFactor = p;
	scheduleMsg(logger, "setAltPid: %.2f", p);
	pidReset();
	showAltInfo();
}

static void applyAlternatorPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
	efiAssertVoid(CUSTOM_ERR_6643, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(CUSTOM_IDLE_WAVE_CNT, state->multiWave.waveCount == 1, "invalid idle waveCount");
	OutputPin *output = state->outputPins[0];
	int value = state->multiWave.getChannelState(/*channelIndex*/0, stateIndex);
	/**
	 * 'engine->isAlternatorControlEnabled' would be false is RPM is too low
	 */
	if (!value || engine->isAlternatorControlEnabled)
		output->setValue(value);
}

void setDefaultAlternatorParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->alternatorOffAboveTps = 120;

	engineConfiguration->targetVBatt = 14;

	engineConfiguration->alternatorControl.offset = 0;
	engineConfiguration->alternatorControl.pFactor = 30;
	engineConfiguration->alternatorControl.periodMs = 100;
}

void onConfigurationChangeAlternatorCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !altPid.isSame(&previousConfiguration->alternatorControl);
}

void initAlternatorCtrl(Logging *sharedLogger) {
	logger = sharedLogger;
	addConsoleAction("altinfo", showAltInfo);
	if (CONFIGB(alternatorControlPin) == GPIO_UNASSIGNED)
		return;

	if (CONFIGB(onOffAlternatorLogic)) {
		enginePins.alternatorPin.initPin("on/off alternator", CONFIGB(alternatorControlPin));

	} else {
		startSimplePwmExt(&alternatorControl,
				"Alternator control",
				&engine->executor,
				CONFIGB(alternatorControlPin),
				&enginePins.alternatorPin,
				engineConfiguration->alternatorPwmFrequency, 0.1, (pwm_gen_callback*)applyAlternatorPinState);
	}
	instance.Start();
}

#endif /* EFI_ALTERNATOR_CONTROL */
