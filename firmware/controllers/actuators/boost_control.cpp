/*
 * boost_control.cpp
 *
 *  Created on: 13. des. 2019
 *      Author: Ola Ruud
 */
#include "pch.h"

#if EFI_BOOST_CONTROL

#include "boost_control.h"
#include "electronic_throttle.h"

#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

static Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t> boostMapOpen;
static Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t> boostMapClosed;
static SimplePwm boostPwmControl("boost");

void BoostController::init(IPwm* pwm, const ValueProvider3D* openLoopMap, const ValueProvider3D* closedLoopTargetMap, pid_s* pidParams) {
	m_pwm = pwm;
	m_openLoopMap = openLoopMap;
	m_closedLoopTargetMap = closedLoopTargetMap;

	m_pid.initPidClass(pidParams);
	resetLua();

	hasInitBoost = true;
}

void BoostController::resetLua() {
	luaTargetAdd = 0;
	luaTargetMult = 1;
	luaOpenLoopAdd = 0;
}

void BoostController::onConfigurationChange(engine_configuration_s const * previousConfig) {
	if (!previousConfig || !m_pid.isSame(&previousConfig->boostPid)) {
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

	auto driverIntent = Sensor::get(SensorType::DriverThrottleIntent);
	isTpsInvalid = !driverIntent.Valid;

	if (isTpsInvalid) {
		return unexpected;
	}

	efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_closedLoopTargetMap != nullptr, "boost closed loop target", unexpected);

	float target = m_closedLoopTargetMap->getValue(rpm, driverIntent.Value);

	// Add any blends if configured
	for (size_t i = 0; i < efi::size(config->boostClosedLoopBlends); i++) {
		auto result = calculateBlend(config->boostClosedLoopBlends[i], rpm, driverIntent.Value);

		engine->outputChannels.boostClosedLoopBlendParameter[i] = result.BlendParameter;
		engine->outputChannels.boostClosedLoopBlendBias[i] = result.Bias;
		engine->outputChannels.boostClosedLoopBlendOutput[i] = result.Value;

		target += result.Value;
	}

	return target * luaTargetMult + luaTargetAdd;
}

expected<percent_t> BoostController::getOpenLoop(float target) {
	// Boost control open loop doesn't care about target - only TPS/RPM
	UNUSED(target);

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	auto driverIntent = Sensor::get(SensorType::DriverThrottleIntent);

	isTpsInvalid = !driverIntent.Valid;

	if (isTpsInvalid) {
		return unexpected;
	}

	efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_openLoopMap != nullptr, "boost open loop", unexpected);

	float openLoop = luaOpenLoopAdd + m_openLoopMap->getValue(rpm, driverIntent.Value);

	// Add any blends if configured
	for (size_t i = 0; i < efi::size(config->boostOpenLoopBlends); i++) {
		auto result = calculateBlend(config->boostOpenLoopBlends[i], rpm, driverIntent.Value);

		engine->outputChannels.boostOpenLoopBlendParameter[i] = result.BlendParameter;
		engine->outputChannels.boostOpenLoopBlendBias[i] = result.Bias;
		engine->outputChannels.boostOpenLoopBlendOutput[i] = result.Value;

		openLoop += result.Value;
	}

	openLoopPart = openLoop;
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

	return m_pid.getOutput(target, manifoldPressure, FAST_CALLBACK_PERIOD_MS / 1000.0f);
}

expected<percent_t> BoostController::getClosedLoop(float target, float manifoldPressure) {
	boostControllerClosedLoopPart = getClosedLoopImpl(target, manifoldPressure);

	m_pid.postState(engine->outputChannels.boostStatus);

	boostControlTarget = target;

	return (float)boostControllerClosedLoopPart;
}

void BoostController::setOutput(expected<float> output) {
	boostOutput = output.value_or(engineConfiguration->boostControlSafeDutyCycle);

	if (!engineConfiguration->isBoostControlEnabled) {
		// If not enabled, force 0% output
		boostOutput = 0;
	}

	float duty = PERCENT_TO_DUTY(boostOutput);

	if (m_pwm) {
		m_pwm->setSimplePwmDutyCycle(duty);
	}

	// inject wastegate position into DC controllers, pretty weird workflow to be honest
	// todo: should it be DC controller pulling?
	setEtbWastegatePosition(boostOutput);
}

void BoostController::onFastCallback() {
	if (!hasInitBoost) {
		return;
	}

	m_pid.iTermMin = -20;
	m_pid.iTermMax = 20;

	rpmTooLow = Sensor::getOrZero(SensorType::Rpm) < engineConfiguration->boostControlMinRpm;
	tpsTooLow = Sensor::getOrZero(SensorType::Tps1) < engineConfiguration->boostControlMinTps;
	mapTooLow = Sensor::getOrZero(SensorType::Map) < engineConfiguration->boostControlMinMap;

	if (rpmTooLow || tpsTooLow || mapTooLow) {
		// Passing unexpected will use the safe duty cycle configured by the user
		setOutput(unexpected);
	} else {
		ClosedLoopController::update();
	}
}

void setDefaultBoostParameters() {
	engineConfiguration->boostPwmFrequency = 33;
	engineConfiguration->boostPid.offset = 0;
	engineConfiguration->boostPid.pFactor = 0.5;
	engineConfiguration->boostPid.iFactor = 0.3;
	engineConfiguration->boostPid.maxValue = 20;
	engineConfiguration->boostPid.minValue = -20;

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
		&engine->scheduler,
		&enginePins.boostPin,
		engineConfiguration->boostPwmFrequency,
		0
	);
#endif /* EFI_UNIT_TEST */
}

void initBoostCtrl() {
	// todo: why do we have 'isBoostControlEnabled' setting exactly?
	// 'initVvtActuators' is an example of a subsystem without explicit enable
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
	boostMapOpen.init(config->boostTableOpenLoop, config->boostTpsBins, config->boostRpmBins);
	boostMapClosed.init(config->boostTableClosedLoop, config->boostTpsBins, config->boostRpmBins);

	// Set up boost controller instance
	engine->module<BoostController>().unmock().init(&boostPwmControl, &boostMapOpen, &boostMapClosed, &engineConfiguration->boostPid);

#if !EFI_UNIT_TEST
	startBoostPin();
#endif
}

#endif
