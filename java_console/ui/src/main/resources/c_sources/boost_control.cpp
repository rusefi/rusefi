/*
 * boost_control.cpp
 *
 *  Created on: 13. des. 2019
 *      Author: Ola Ruud
 */
#include "pch.h"

#if EFI_BOOST_CONTROL

#include "boost_control.h"
#include "pid_auto_tune.h"
#include "electronic_throttle.h"

#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

static boostOpenLoop_Map3D_t boostMapOpen;
static boostOpenLoop_Map3D_t boostMapClosed;
static SimplePwm boostPwmControl("boost");

void BoostController::init(IPwm* pwm, const ValueProvider3D* openLoopMap, const ValueProvider3D* closedLoopTargetMap, pid_s* pidParams) {
	m_pwm = pwm;
	m_openLoopMap = openLoopMap;
	m_closedLoopTargetMap = closedLoopTargetMap;

	m_pid.initPidClass(pidParams);
}

void BoostController::onConfigurationChange(pid_s* previousConfiguration) {
	if (!m_pid.isSame(previousConfiguration)) {
		m_shouldResetPid = true;
	}
}

expected<float> BoostController::observePlant() const {
	return Sensor::get(SensorType::Map);
}

expected<float> BoostController::getSetpoint() {
	// If we're in open loop only mode, disregard any target computation.
	// Open loop needs to work even in case of invalid closed loop config
	isNotClosedLoop = engineConfiguration->boostType != CLOSED_LOOP;
	if (isNotClosedLoop) {
		closedLoopPart = 0;
		return closedLoopPart;
	}

	float rpm = GET_RPM();

	auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	isTpsValid = tps.Valid;

	if (!isTpsValid) {
		return unexpected;
	}

	if (!m_closedLoopTargetMap) {
		return unexpected;
	}

	return m_closedLoopTargetMap->getValue(rpm / RPM_1_BYTE_PACKING_MULT, tps.Value / TPS_1_BYTE_PACKING_MULT);
}

expected<percent_t> BoostController::getOpenLoop(float target) {
	// Boost control open loop doesn't care about target - only TPS/RPM
	UNUSED(target);

	float rpm = GET_RPM();
	auto tps = Sensor::get(SensorType::DriverThrottleIntent);

	isTpsValid = tps.Valid;

	if (!isTpsValid) {
		return unexpected;
	}

	if (!m_openLoopMap) {
		return unexpected;
	}

	percent_t openLoop = m_openLoopMap->getValue(rpm / RPM_1_BYTE_PACKING_MULT, tps.Value / TPS_1_BYTE_PACKING_MULT);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_BOOST) {
		engine->outputChannels.debugFloatField1 = openLoop;
	}
#endif

	return openLoop;
}

percent_t BoostController::getClosedLoopImpl(float target, float manifoldPressure) {
	// If we're in open loop only mode, make no closed loop correction.
	isNotClosedLoop = engineConfiguration->boostType != CLOSED_LOOP;
	if (isNotClosedLoop) {
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

	isBelowClosedLoopThreshold = manifoldPressure < engineConfiguration->minimumBoostClosedLoopMap;
	if (isBelowClosedLoopThreshold) {
		// We're below the CL threshold, inhibit CL for now
		m_pid.reset();
		closedLoopPart = 0;
		return closedLoopPart;
	}

	closedLoopPart = m_pid.getOutput(target, manifoldPressure, SLOW_CALLBACK_PERIOD_MS / 1000.0f);
	return closedLoopPart;
}

expected<percent_t> BoostController::getClosedLoop(float target, float manifoldPressure) {
	auto closedLoop = getClosedLoopImpl(target, manifoldPressure);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_BOOST) {
		engine->outputChannels.debugFloatField2 = closedLoop;
		engine->outputChannels.debugFloatField3 = target;
	}
#endif /* EFI_TUNER_STUDIO */

	return closedLoop;
}

void BoostController::setOutput(expected<float> output) {
	percent_t percent = output.value_or(engineConfiguration->boostControlSafeDutyCycle);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_BOOST) {
		engine->outputChannels.debugFloatField3 = percent;
	}
#endif /* EFI_TUNER_STUDIO */

	float duty = PERCENT_TO_DUTY(percent);

	if (m_pwm) {
		m_pwm->setSimplePwmDutyCycle(duty);
	}

	setEtbWastegatePosition(percent);
}

void BoostController::update() {
	m_pid.iTermMin = -50;
	m_pid.iTermMax = 50;

	ClosedLoopController::update();
}

static bool hasInitBoost = false;

void updateBoostControl() {
	if (hasInitBoost) {
		engine->boostController.update();
	}
}

void setDefaultBoostParameters() {
	engineConfiguration->boostPwmFrequency = 33;
	engineConfiguration->boostPid.offset = 0;
	engineConfiguration->boostPid.pFactor = 0.5;
	engineConfiguration->boostPid.iFactor = 0.3;
	engineConfiguration->boostPid.maxValue = 20;
	engineConfiguration->boostPid.minValue = -20;
	engineConfiguration->boostControlPin = GPIO_UNASSIGNED;
	engineConfiguration->boostControlPinMode = OM_DEFAULT;

	setLinearCurve(config->boostRpmBins, 0, 8000 / RPM_1_BYTE_PACKING_MULT, 1);
	setLinearCurve(config->boostLoadBins, 0, 100 / TPS_1_BYTE_PACKING_MULT, 1);

	for (int loadIndex = 0; loadIndex < BOOST_LOAD_COUNT; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < BOOST_RPM_COUNT; rpmIndex++) {
			config->boostTableOpenLoop[loadIndex][rpmIndex] = config->boostLoadBins[loadIndex];
			config->boostTableClosedLoop[loadIndex][rpmIndex] = config->boostLoadBins[loadIndex];
		}
	}

	// Defaults for ETB-style wastegate actuator
	engineConfiguration->etbWastegatePid.pFactor = 4.5;
  engineConfiguration->etbWastegatePid.iFactor = 100;
  engineConfiguration->etbWastegatePid.dFactor = 0.05;
	engineConfiguration->etbWastegatePid.minValue = -60;
	engineConfiguration->etbWastegatePid.maxValue = 60;
}

void startBoostPin() {
#if !EFI_UNIT_TEST
	// Only init if a pin is set, no need to start PWM without a pin
	if (!isBrainPinValid(engineConfiguration->boostControlPin)) {
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

void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration) {
	engine->boostController.onConfigurationChange(&previousConfiguration->boostPid);
}

void initBoostCtrl() {
	// todo: why do we have 'isBoostControlEnabled' setting exactly?
	// 'initAuxPid' is an example of a subsystem without explicit enable
	if (!engineConfiguration->isBoostControlEnabled) {
		return;
	}

	bool hasAnyEtbWastegate = false;

	for (size_t i = 0; i < efi::size(engineConfiguration->etbFunctions); i++) {
		hasAnyEtbWastegate |= engineConfiguration->etbFunctions[i] == DC_Wastegate;
	}

	// If we have neither a boost PWM pin nor ETB wastegate, nothing more to do
	if (!isBrainPinValid(engineConfiguration->boostControlPin) && !hasAnyEtbWastegate) {
		return;
	}

	// Set up open & closed loop tables
	boostMapOpen.init(config->boostTableOpenLoop, config->boostLoadBins, config->boostRpmBins);
	boostMapClosed.init(config->boostTableClosedLoop, config->boostLoadBins, config->boostRpmBins);

	// Set up boost controller instance
	engine->boostController.init(&boostPwmControl, &boostMapOpen, &boostMapClosed, &engineConfiguration->boostPid);

#if !EFI_UNIT_TEST
	startBoostPin();
	hasInitBoost = true;
#endif
}

#endif
