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
#include "gppwm_channel_reader.h"

#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

namespace {
    Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t> boostMapOpen{"bo"};
    Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t> boostMapClosed{"bc"};
    Map2D<BOOST_CURVE_SIZE, float, float> boostCltCorr { "clt" };
    Map2D<BOOST_CURVE_SIZE, float, float> boostIatCorr { "iat" };
    Map2D<BOOST_CURVE_SIZE, float, float> boostCltAdder { "clt (adder)" };
    Map2D<BOOST_CURVE_SIZE, float, float> boostIatAdder { "iat (adder)" };
    SimplePwm boostPwmControl("boost");
}

void BoostController::init(
    IPwm* const pwm,
    const ValueProvider3D* const openLoopMap,
    const ValueProvider3D* const closedLoopTargetMap,
    const ValueProvider2D& cltMultiplierProvider,
    const ValueProvider2D& iatMultiplierProvider,
    const ValueProvider2D& cltAdderProvider,
    const ValueProvider2D& iatAdderProvider,
    pid_s* const pidParams
) {
	m_pwm = pwm;
	m_openLoopMap = openLoopMap;
	m_closedLoopTargetMap = closedLoopTargetMap;
    m_cltBoostCorrMap = &cltMultiplierProvider;
    m_iatBoostCorrMap = &iatMultiplierProvider;
    m_cltBoostAdderMap = &cltAdderProvider;
    m_iatBoostAdderMap = &iatAdderProvider;

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
#if EFI_PROD_CODE
  initBoostCtrl();
#endif

	if (!previousConfig || !m_pid.isSame(&previousConfig->boostPid)) {
		m_shouldResetPid = true;
	}
}

expected<float> BoostController::observePlant() {
    expected<float> map = Sensor::get(SensorType::Map);
    if (!map.Valid && engineConfiguration->boostType != CLOSED_LOOP) {
        // if we're in open loop only let's somewhat operate even without valid Map sensor
        map = 0;
    }
    isPlantValid = map.Valid;
    return map;
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
#if EFI_ENGINE_CONTROL
	// Add any blends if configured
	for (size_t i = 0; i < efi::size(config->boostClosedLoopBlends); i++) {
		auto result = calculateBlend(config->boostClosedLoopBlends[i], rpm, driverIntent.Value);

		engine->outputChannels.boostClosedLoopBlendParameter[i] = result.BlendParameter;
		engine->outputChannels.boostClosedLoopBlendBias[i] = result.Bias;
		engine->outputChannels.boostClosedLoopBlendOutput[i] = result.Value;

		target += result.Value;
	}
#endif //EFI_ENGINE_CONTROL

	target *= luaTargetMult;
	target += luaTargetAdd;
	const std::optional<float> temperatureAdder = getBoostControlTargetTemperatureAdder();
	if (temperatureAdder.has_value()) {
		target += temperatureAdder.value();
	}
	return target;
}

expected<percent_t> BoostController::getOpenLoop(float target) {
	// Boost control open loop doesn't care about target - only TPS/RPM
	UNUSED(target);

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	auto driverIntent = readGppwmChannel(engineConfiguration->boostOpenLoopYAxis);

	isTpsInvalid = !driverIntent.Valid;

	if (isTpsInvalid) {
		return unexpected;
	}

	efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_openLoopMap != nullptr, "boost open loop", unexpected);
    efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_cltBoostCorrMap != nullptr, "boost CLT multiplier", unexpected);
    efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_iatBoostCorrMap != nullptr, "boost IAT multiplier", unexpected);

	percent_t openLoop = luaOpenLoopAdd + getBoostControlDutyCycleWithTemperatureCorrections(rpm, driverIntent.Value);

#if EFI_ENGINE_CONTROL
	// Add any blends if configured
	for (size_t i = 0; i < efi::size(config->boostOpenLoopBlends); i++) {
		auto result = calculateBlend(config->boostOpenLoopBlends[i], rpm, driverIntent.Value);

		engine->outputChannels.boostOpenLoopBlendParameter[i] = result.BlendParameter;
		engine->outputChannels.boostOpenLoopBlendBias[i] = result.Bias;
		engine->outputChannels.boostOpenLoopBlendOutput[i] = result.Value;

		openLoop += result.Value;
	}
#endif // EFI_ENGINE_CONTROL

	// Add gear-based adder
	auto gear = Sensor::getOrZero(SensorType::DetectedGear);
	float gearAdder = engineConfiguration->gearBasedOpenLoopBoostAdder[static_cast<int>(gear) + 1];
	openLoop += gearAdder;

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

float BoostController::getBoostControlDutyCycleWithTemperatureCorrections(
    const float rpm,
    const float driverIntent
) const {
    float result = m_openLoopMap->getValue(rpm, driverIntent);
    std::optional<float> cltBoostMultiplier = getBoostTemperatureCorrection(SensorType::Clt, *m_cltBoostCorrMap);
    if (cltBoostMultiplier.has_value()) {
        result *= cltBoostMultiplier.value();
    }
    std::optional<float> iatBoostMultiplier = getBoostTemperatureCorrection(SensorType::Iat, *m_iatBoostCorrMap);
    if (iatBoostMultiplier.has_value()) {
        result *= iatBoostMultiplier.value();
    }
    return result;
}

std::optional<float> BoostController::getBoostControlTargetTemperatureAdder() const {
    std::optional<float> result = getBoostTemperatureCorrection(SensorType::Clt, *m_cltBoostAdderMap);
    const std::optional<float> iatBoostAdder = getBoostTemperatureCorrection(SensorType::Iat, *m_iatBoostAdderMap);
    if (iatBoostAdder.has_value()) {
        if (result.has_value()) {
            result.value() += iatBoostAdder.value();
        } else {
            result = iatBoostAdder;
        }
    }
    return result;
}

std::optional<float> BoostController::getBoostTemperatureCorrection(
    const SensorType sensorType,
    const ValueProvider2D& correctionCurve
) const {
    const SensorResult temperature = Sensor::get(sensorType);
    if (temperature.Valid) {
        const std::optional<float> boostCorrection = correctionCurve.getValue(temperature.Value);
        if (boostCorrection.has_value()) {
            return std::make_optional<float>(boostCorrection.value());
        }
    }
    return {};
}


expected<percent_t> BoostController::getClosedLoop(float target, float manifoldPressure) {
	boostControllerClosedLoopPart = getClosedLoopImpl(target, manifoldPressure);

	m_pid.postState(engine->outputChannels.boostStatus);

	boostControlTarget = target;

	return (float)boostControllerClosedLoopPart;
}

void BoostController::setOutput(expected<float> output) {
	// this clamping is just for happier gauge #6339
	boostOutput = clampPercentValue(output.value_or(engineConfiguration->boostControlSafeDutyCycle));

	if (!engineConfiguration->isBoostControlEnabled) {
		// If not enabled, force 0% output
		boostOutput = 0;
	}

	float duty = PERCENT_TO_DUTY(boostOutput);

	if (m_pwm) {
		m_pwm->setSimplePwmDutyCycle(duty);
	}

#if EFI_ELECTRONIC_THROTTLE_BODY
	// inject wastegate position into DC controllers, pretty weird workflow to be honest
	// todo: should it be DC controller pulling?
	setEtbWastegatePosition(boostOutput);
#endif // EFI_ELECTRONIC_THROTTLE_BODY
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

  isBoostControlled = !(rpmTooLow || tpsTooLow || mapTooLow);

	if (!isBoostControlled) {
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
	engineConfiguration->boostControlPinMode = OM_DEFAULT;

	setRpmTableBin(config->boostRpmBins);
	setLinearCurve(config->boostLoadBins, 0, 100, 1);

	for (int loadIndex = 0; loadIndex < BOOST_LOAD_COUNT; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < BOOST_RPM_COUNT; rpmIndex++) {
			config->boostTableClosedLoop[loadIndex][rpmIndex] = (float)config->boostLoadBins[loadIndex];
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
		/*dutyCycle*/0
	);
#endif /* EFI_UNIT_TEST */
}

void initBoostCtrl() {
#if EFI_PROD_CODE
	if (engine->module<BoostController>().unmock().hasInitBoost) {
    // already initialized - nothing to do here
	  return;
	}
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
#endif

	// Set up open & closed loop tables
	boostMapOpen.initTable(config->boostTableOpenLoop, config->boostRpmBins, config->boostLoadBins);
	boostMapClosed.initTable(config->boostTableClosedLoop, config->boostRpmBins, config->boostLoadBins);
	boostCltCorr.initTable(config->cltBoostCorr, config->cltBoostCorrBins);
	boostIatCorr.initTable(config->iatBoostCorr, config->iatBoostCorrBins);
	boostCltAdder.initTable(config->cltBoostAdder, config->cltBoostAdderBins);
	boostIatAdder.initTable(config->iatBoostAdder, config->iatBoostAdderBins);

	// Set up boost controller instance
	engine->module<BoostController>().unmock().init(
        &boostPwmControl,
        &boostMapOpen,
        &boostMapClosed,
        boostCltCorr,
        boostIatCorr,
        boostCltAdder,
        boostIatAdder,
        &engineConfiguration->boostPid
    );

#if !EFI_UNIT_TEST
	startBoostPin();
#endif
}

#endif // EFI_BOOST_CONTROL
