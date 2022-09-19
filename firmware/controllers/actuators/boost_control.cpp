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
	resetLua();
}

void BoostController::resetLua() {
	luaTargetAdd = 0;
	luaTargetMult = 1;
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
		boostControllerClosedLoopPart = 0;
		return (float)boostControllerClosedLoopPart;
	}

	float rpm = Sensor::getOrZero(SensorType::Rpm);

	auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	isTpsInvalid = !tps.Valid;

	if (isTpsInvalid) {
		return unexpected;
	}

	efiAssert(OBD_PCM_Processor_Fault, m_closedLoopTargetMap != nullptr, "boost closed loop target", unexpected);

    return m_closedLoopTargetMap->getValue(rpm, tps.Value) * luaTargetMult + luaTargetAdd;
}

expected<percent_t> BoostController::getOpenLoop(float target) {
	// Boost control open loop doesn't care about target - only TPS/RPM
	UNUSED(target);

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	auto tps = Sensor::get(SensorType::DriverThrottleIntent);

	isTpsInvalid = !tps.Valid;

	if (isTpsInvalid) {
		return unexpected;
	}

	efiAssert(OBD_PCM_Processor_Fault, m_openLoopMap != nullptr, "boost open loop", unexpected);

	openLoopPart = m_openLoopMap->getValue(rpm, tps.Value);

#if EFI_TUNER_STUDIO
	engine->outputChannels.boostControllerOpenLoopPart = openLoopPart;
#endif

	return openLoopPart;
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
	isZeroRpm = Sensor::getOrZero(SensorType::Rpm) == 0;
	if (isZeroRpm) {
		m_pid.reset();
		return 0;
	}

	isBelowClosedLoopThreshold = manifoldPressure < engineConfiguration->minimumBoostClosedLoopMap;
	if (isBelowClosedLoopThreshold) {
		// We're below the CL threshold, inhibit CL for now
		m_pid.reset();
		return 0;
	}

	return m_pid.getOutput(target, manifoldPressure, SLOW_CALLBACK_PERIOD_MS / 1000.0f);
}

expected<percent_t> BoostController::getClosedLoop(float target, float manifoldPressure) {
	boostControllerClosedLoopPart = getClosedLoopImpl(target, manifoldPressure);

	m_pid.postState(engine->outputChannels.boostStatus);

	boostControlTarget = target;

	return (float)boostControllerClosedLoopPart;
}

void BoostController::setOutput(expected<float> output) {
	percent_t percent = output.value_or(engineConfiguration->boostControlSafeDutyCycle);

	if (!engineConfiguration->isBoostControlEnabled) {
		// If not enabled, force 0% output
		percent = 0;
	}

#if EFI_TUNER_STUDIO
	engine->outputChannels.boostControllerOutput = percent;
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

	bool rpmTooLow = Sensor::getOrZero(SensorType::Rpm) < engineConfiguration->boostControlMinRpm;
	bool tpsTooLow = Sensor::getOrZero(SensorType::Tps1) < engineConfiguration->boostControlMinTps;
	bool mapTooLow = Sensor::getOrZero(SensorType::Map) < engineConfiguration->boostControlMinMap;

	if (rpmTooLow || tpsTooLow || mapTooLow) {
		// Passing unexpected will use the safe duty cycle configured by the user
		setOutput(unexpected);
	} else {
		ClosedLoopController::update();
	}
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
	engineConfiguration->boostControlPinMode = OM_DEFAULT;

	setLinearCurve(config->boostRpmBins, 0, 8000, 1);
	setLinearCurve(config->boostTpsBins, 0, 100, 1);

	for (int loadIndex = 0; loadIndex < BOOST_LOAD_COUNT; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < BOOST_RPM_COUNT; rpmIndex++) {
			config->boostTableOpenLoop[loadIndex][rpmIndex] = (float)config->boostTpsBins[loadIndex];
			config->boostTableClosedLoop[loadIndex][rpmIndex] = (float)config->boostTpsBins[loadIndex];
		}
	}

	// Defaults for ETB-style wastegate actuator
	engineConfiguration->etbWastegatePid.pFactor = 1;
	engineConfiguration->etbWastegatePid.minValue = -60;
	engineConfiguration->etbWastegatePid.maxValue = 60;
}

void startBoostPin() {
#if !EFI_UNIT_TEST
	// Only init if a pin is set, no need to start PWM without a pin
	if (!engineConfiguration->isBoostControlEnabled || !isBrainPinValid(engineConfiguration->boostControlPin)) {
		return;
	}

	startSimplePwm(
		&boostPwmControl,
		"Boost",
		&engine->executor,
		&enginePins.boostPin,
		engineConfiguration->boostPwmFrequency,
		0
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
		hasAnyEtbWastegate |= engineConfiguration->etbFunctions[i] == ETB_Wastegate;
	}

	// If we have neither a boost PWM pin nor ETB wastegate, nothing more to do
	if (!isBrainPinValid(engineConfiguration->boostControlPin) && !hasAnyEtbWastegate) {
		return;
	}

	// Set up open & closed loop tables
	boostMapOpen.init(config->boostTableOpenLoop, config->boostTpsBins, config->boostRpmBins);
	boostMapClosed.init(config->boostTableClosedLoop, config->boostTpsBins, config->boostRpmBins);

	// Set up boost controller instance
	engine->boostController.init(&boostPwmControl, &boostMapOpen, &boostMapClosed, &engineConfiguration->boostPid);

#if !EFI_UNIT_TEST
	startBoostPin();
	hasInitBoost = true;
#endif
}

#endif
