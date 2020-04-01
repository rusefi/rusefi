/*
 * boost_control.cpp
 *
 *  Created on: 13. des. 2019
 *      Author: Ola Ruud
 */
#include "global.h"

#if EFI_BOOST_CONTROL

#if EFI_TUNER_STUDIO
#include "tunerstudio_configuration.h"
#endif /* EFI_TUNER_STUDIO */
#include "engine.h"
#include "boost_control.h"
#include "tps.h"
#include "map.h"
#include "io_pins.h"
#include "engine_configuration.h"
#include "pwm_generator_logic.h"
#include "pid.h"
#include "engine_controller.h"
#include "periodic_task.h"
#include "pin_repository.h"
#include "pwm_generator.h"
#include "pid_auto_tune.h"
#include "local_version_holder.h"
#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

EXTERN_ENGINE;

static Logging *logger;
static boostOpenLoop_Map3D_t boostMapOpen("boostmapopen", 1);
static boostOpenLoop_Map3D_t boostMapClosed("boostmapclosed", 1);
static SimplePwm boostPwmControl("boost");
static Pid boostControlPid;

static bool shouldResetPid = false;

#if EFI_TUNER_STUDIO
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

static void pidReset(void) {
	boostControlPid.reset();
}

class BoostControl: public PeriodicTimerController {
	DECLARE_ENGINE_PTR;

	int getPeriodMs() override {
		return GET_PERIOD_LIMITED(&engineConfiguration->boostPid);
	}

	void PeriodicTask() override {
		if (shouldResetPid) {
			pidReset();
			shouldResetPid = false;
		}

		float rpm = GET_RPM_VALUE;
		float mapValue = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);

		if (!engineConfiguration->isBoostControlEnabled)
			return;

		bool engineRunning = rpm > engineConfiguration->cranking.rpm;
		if (!engineRunning) {
			boostControlPid.reset();
			return;
		}

		percent_t openLoopDuty = boostMapOpen.getValue(rpm / RPM_1_BYTE_PACKING_MULT, mapValue / LOAD_1_BYTE_PACKING_MULT) * LOAD_1_BYTE_PACKING_MULT;
		percent_t closedLoopDuty = 0;
		percent_t duty = openLoopDuty;

		if (engineConfiguration->boostType == CLOSED_LOOP) {
			float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
			float targetBoost = boostMapClosed.getValue(rpm / RPM_1_BYTE_PACKING_MULT, tps / TPS_1_BYTE_PACKING_MULT) * LOAD_1_BYTE_PACKING_MULT;
			closedLoopDuty = openLoopDuty + boostControlPid.getOutput(targetBoost, mapValue);
			duty += closedLoopDuty;
		}

		boostControlPid.iTermMin = -50;
		boostControlPid.iTermMax = 50;

		if (engineConfiguration->debugMode == DBG_BOOST) {
#if EFI_TUNER_STUDIO
			boostControlPid.postState(&tsOutputChannels);
			tsOutputChannels.debugFloatField1 = openLoopDuty;
			tsOutputChannels.debugFloatField7 = closedLoopDuty;
#endif /* EFI_TUNER_STUDIO */
		}

#if EFI_LAUNCH_CONTROL
	if (engine->setLaunchBoostDuty) {
		duty = engineConfiguration->launchBoostDuty;
	}
#endif /* EFI_LAUNCH_CONTROL */

		boostPwmControl.setSimplePwmDutyCycle(PERCENT_TO_DUTY(duty));
	}
};

static BoostControl BoostController;

#if !EFI_UNIT_TEST
void setBoostPFactor(float value) {
	engineConfiguration->boostPid.pFactor = value;
	boostControlPid.reset();
}

void setBoostIFactor(float value) {
	engineConfiguration->boostPid.iFactor = value;
	boostControlPid.reset();
}

void setBoostDFactor(float value) {
	engineConfiguration->boostPid.dFactor = value;
	boostControlPid.reset();
}
#endif /* EFI_UNIT_TEST */

void setDefaultBoostParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->isBoostControlEnabled = true;
	engineConfiguration->boostPwmFrequency = 55;
	engineConfiguration->boostPid.offset = 0;
	engineConfiguration->boostPid.pFactor = 0.5;
	engineConfiguration->boostPid.iFactor = 0.3;
	engineConfiguration->boostPid.periodMs = 100;
	engineConfiguration->boostPid.maxValue = 99;
	engineConfiguration->boostPid.minValue = -99;
	engineConfiguration->boostControlPin = GPIO_UNASSIGNED;
	engineConfiguration->boostControlPinMode = OM_DEFAULT;

	setLinearCurve(config->boostRpmBins, 0, 8000 / RPM_1_BYTE_PACKING_MULT, 1);
	setLinearCurve(config->boostMapBins, 0, 300 / LOAD_1_BYTE_PACKING_MULT, 1);
	for (int loadIndex = 0;loadIndex<BOOST_LOAD_COUNT;loadIndex++) {
		for (int rpmIndex = 0;rpmIndex<BOOST_RPM_COUNT;rpmIndex++) {
			config->boostTableOpenLoop[loadIndex][rpmIndex] = config->boostMapBins[loadIndex];
		}
	}

	setLinearCurve(config->boostTpsBins, 0, 100 / TPS_1_BYTE_PACKING_MULT, 1);
	for (int loadIndex = 0;loadIndex<BOOST_LOAD_COUNT;loadIndex++) {
		for (int rpmIndex = 0;rpmIndex<BOOST_RPM_COUNT;rpmIndex++) {
			config->boostTableClosedLoop[loadIndex][rpmIndex] = config->boostTpsBins[loadIndex];
		}
	}
}

static void turnBoostPidOn() {
#if !EFI_UNIT_TEST
	if (CONFIG(boostControlPin) == GPIO_UNASSIGNED){
		return;
	}

	startSimplePwmExt(
		&boostPwmControl,
		"Boost",
		&engine->executor,
		CONFIG(boostControlPin),
		&enginePins.boostPin,
		engineConfiguration->boostPwmFrequency,
		0.5f
	);
#endif /* EFI_UNIT_TEST */
}

void startBoostPin(void) {
	turnBoostPidOn();
}

void stopBoostPin(void) {
#if !EFI_UNIT_TEST
	brain_pin_markUnused(activeConfiguration.boostControlPin);
#endif /* EFI_UNIT_TEST */
}

void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !boostControlPid.isSame(&previousConfiguration->boostPid);
}

void initBoostCtrl(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if !EFI_UNIT_TEST
	if (CONFIG(boostControlPin) == GPIO_UNASSIGNED){
		return;
	}
#endif

	boostControlPid.initPidClass(&engineConfiguration->boostPid);

	logger = sharedLogger;
	boostMapOpen.init(config->boostTableOpenLoop, config->boostMapBins, config->boostRpmBins);
	boostMapClosed.init(config->boostTableClosedLoop, config->boostTpsBins, config->boostRpmBins);
	boostControlPid.reset();
#if !EFI_UNIT_TEST
	startBoostPin();
	BoostController.Start();
#endif
}

#endif
