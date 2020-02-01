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
static pid_s *boostPidS = &persistentState.persistentConfiguration.engineConfiguration.boostPid;
static Pid boostControlPid(boostPidS);


static bool shouldResetPid = false;

#if EFI_TUNER_STUDIO
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */


static void pidReset(void) {
	boostControlPid.reset();
}


class BoostControl: public PeriodicTimerController {
	int getPeriodMs()
	 override {
		return GET_PERIOD_LIMITED(&engineConfiguration->boostPid);
	}
	void PeriodicTask() override {
		if (shouldResetPid) {
					pidReset();
					shouldResetPid = false;
				}

#if EFI_TUNER_STUDIO
		boostControlPid.postState(&tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */
	float rpm = GET_RPM_VALUE;
	float mapValue = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);

   if (!engineConfiguration->isBoostControlEnabled)
	   return;
   bool enabledAtEngineRunning = rpm > engineConfiguration->cranking.rpm;
     if (!enabledAtEngineRunning) {
    	 boostControlPid.reset();
   				return;
     }


   	percent_t openLoopDuty = boostMapOpen.getValue(rpm / RPM_1_BYTE_PACKING_MULT, mapValue/ LOAD_1_BYTE_PACKING_MULT) * LOAD_1_BYTE_PACKING_MULT;
   	percent_t duty, closedLoopDuty = 0;

	   if (engineConfiguration->boostType == OPEN_LOOP) {
		   duty = openLoopDuty;
	   }
	  else if (engineConfiguration->boostType == CLOSED_LOOP) {
		  float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
		  float targetBoost = boostMapClosed.getValue(rpm / RPM_1_BYTE_PACKING_MULT, tps / TPS_1_BYTE_PACKING_MULT) * LOAD_1_BYTE_PACKING_MULT;
		  closedLoopDuty = openLoopDuty + boostControlPid.getOutput(targetBoost, mapValue);
			duty = closedLoopDuty;
	  }


	   boostControlPid.iTermMin = -50;
	   boostControlPid.iTermMax = 50;

	   if (engineConfiguration->debugMode == DBG_BOOST) {
	   	#if EFI_TUNER_STUDIO
	   	tsOutputChannels.debugFloatField1 = openLoopDuty;
	   	tsOutputChannels.debugFloatField7 = closedLoopDuty;
	   	tsOutputChannels.boostDuty = duty;
	   	#endif /* EFI_TUNER_STUDIO */
	   				}
	boostPwmControl.setSimplePwmDutyCycle(PERCENT_TO_DUTY(duty));

		}


};

static BoostControl BoostController;


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
	if (CONFIG(boostControlPin) == GPIO_UNASSIGNED){
		return;
	}
	startSimplePwmExt(&boostPwmControl, "Boost", &engine->executor,
			CONFIG(boostControlPin), &enginePins.boostPin,
			engineConfiguration->boostPwmFrequency, 0.5f,
			(pwm_gen_callback*) applyPinState);
}

void startBoostPin(void) {

	turnBoostPidOn();

}
void stopBoostPin(void) {
	brain_pin_markUnused(activeConfiguration.boostControlPin);
}

void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !boostControlPid.isSame(&previousConfiguration->boostPid);
}

void initBoostCtrl(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX){
	logger = sharedLogger;
	boostMapOpen.init(config->boostTableOpenLoop, config->boostMapBins, config->boostRpmBins);
	boostMapClosed.init(config->boostTableClosedLoop, config->boostTpsBins, config->boostRpmBins);
	boostControlPid.reset();
	startBoostPin();
	BoostController.Start();

}

#endif
