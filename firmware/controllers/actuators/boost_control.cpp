/*
 * boost_control.cpp
 *
 *  Created on: 13. des. 2019
 *      Author: Ola Ruud
 */
#include "global.h"

#if EFI_BOOST_CONTROL

#if EFI_TUNER_STUDIO
#include "tunerstudio_outputs.h"
#endif /* EFI_TUNER_STUDIO */
#include "engine.h"
#include "boost_control.h"
#include "sensor.h"
#include "map.h"
#include "pin_repository.h"
#include "pwm_generator_logic.h"
#include "pid_auto_tune.h"
#include "electronic_throttle.h"

#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

EXTERN_ENGINE;

static Logging *logger;
static boostOpenLoop_Map3D_t boostMapOpen("boostmapopen");
static boostOpenLoop_Map3D_t boostMapClosed("boostmapclosed");
static SimplePwm boostPwmControl("boost");

void BoostController::init(SimplePwm* pwm, const ValueProvider3D* openLoopMap, const ValueProvider3D* closedLoopTargetMap, pid_s* pidParams) {
	m_pwm = pwm;
	m_openLoopMap = openLoopMap;
	m_closedLoopTargetMap = closedLoopTargetMap;

	m_pid.initPidClass(pidParams);
}


void BoostController::reset() {
	m_shouldResetPid = true;
}

void BoostController::onConfigurationChange(pid_s* previousConfiguration) {
	if (!m_pid.isSame(previousConfiguration)) {
		m_shouldResetPid = true;
	}
}

int BoostController::getPeriodMs() {
	return GET_PERIOD_LIMITED(&engineConfiguration->boostPid);
}

expected<float> BoostController::observePlant() const {
	float map = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (cisnan(map)) {
		return unexpected;
	}

	return map;
}

expected<float> BoostController::getSetpoint() const {
	float rpm = GET_RPM();

	auto tps = Sensor::get(SensorType::DriverThrottleIntent);

	if (!tps) {
		return unexpected;
	}

	if (!m_closedLoopTargetMap) {
		return unexpected;
	}

	return m_closedLoopTargetMap->getValue(rpm / RPM_1_BYTE_PACKING_MULT, tps.Value / TPS_1_BYTE_PACKING_MULT);
}

expected<percent_t> BoostController::getOpenLoop(float target) const {
	// Boost control open loop doesn't care about target - only MAP/RPM
	UNUSED(target);

	float rpm = GET_RPM();
	auto tps = Sensor::get(SensorType::DriverThrottleIntent);

	if (!tps) {
		return unexpected;
	}

	if (!m_openLoopMap) {
		return unexpected;
	}

	percent_t openLoop = m_openLoopMap->getValue(rpm / RPM_1_BYTE_PACKING_MULT, tps.Value / TPS_1_BYTE_PACKING_MULT);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_BOOST) {
		tsOutputChannels.debugFloatField1 = openLoop;
	}
#endif

	return openLoop;
}

expected<percent_t> BoostController::getClosedLoop(float target, float manifoldPressure) {
	// If we're in open loop only mode, make no closed loop correction.
	if (engineConfiguration->boostType != CLOSED_LOOP) {
		return 0;
	}

	// Reset PID if requested
	if (m_shouldResetPid) {
		m_pid.reset();
		m_shouldResetPid = false;
	}

	// If the engine isn't running, don't correct.
	if (GET_RPM() == 0) {
		m_pid.reset();
		return 0;
	}

	float closedLoop = m_pid.getOutput(target, manifoldPressure);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_BOOST) {
		m_pid.postState(&tsOutputChannels);
		tsOutputChannels.debugFloatField2 = closedLoop;
	}
#endif /* EFI_TUNER_STUDIO */

	return closedLoop;
}

void BoostController::setOutput(expected<float> output) {
	// TODO: hook up safe duty cycle
	float duty = PERCENT_TO_DUTY(output.value_or(/*CONFIG(boostControlSafeDutyCycle)*/ 0));
	
	if (m_pwm) {
		m_pwm->setSimplePwmDutyCycle(duty);
	}

	setEtbWastegatePosition(duty PASS_ENGINE_PARAMETER_SUFFIX);
}

void BoostController::PeriodicTask() {
	m_pid.iTermMin = -50;
	m_pid.iTermMax = 50;

	update();
}

BoostController boostController;

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
	setLinearCurve(config->boostTpsBins, 0, 100 / TPS_1_BYTE_PACKING_MULT, 1);

	for (int loadIndex = 0; loadIndex < BOOST_LOAD_COUNT; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < BOOST_RPM_COUNT; rpmIndex++) {
			config->boostTableOpenLoop[loadIndex][rpmIndex] = config->boostTpsBins[loadIndex];
			config->boostTableClosedLoop[loadIndex][rpmIndex] = config->boostTpsBins[loadIndex];
		}
	}
}

void startBoostPin() {
#if !EFI_UNIT_TEST
	if (CONFIG(boostControlPin) == GPIO_UNASSIGNED){
		return;
	}

	startSimplePwm(
		&boostPwmControl,
		"Boost",
		&engine->executor,
		&enginePins.boostPin,
		engineConfiguration->boostPwmFrequency,
		0.5f
	);
#endif /* EFI_UNIT_TEST */
}

void stopBoostPin() {
#if !EFI_UNIT_TEST
	brain_pin_markUnused(activeConfiguration.boostControlPin);
#endif /* EFI_UNIT_TEST */
}

void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration) {
	boostController.onConfigurationChange(&previousConfiguration->boostPid);
}

void initBoostCtrl(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if !EFI_UNIT_TEST
	if (CONFIG(boostControlPin) == GPIO_UNASSIGNED){
		return;
	}
#endif

	logger = sharedLogger;

	// Set up open & closed loop tables
	boostMapOpen.init(config->boostTableOpenLoop, config->boostTpsBins, config->boostRpmBins);
	boostMapClosed.init(config->boostTableClosedLoop, config->boostTpsBins, config->boostRpmBins);

	// Set up boost controller instance
	boostController.init(&boostPwmControl, &boostMapOpen, &boostMapClosed, &engineConfiguration->boostPid);

#if !EFI_UNIT_TEST
	startBoostPin();
	boostController.Start();
#endif
}

#endif
