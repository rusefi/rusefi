/**
 * low pressure fuel pump control
 * for high-pressure see HpfpController@high_pressure_fuel_pump
 *
*/
#include "pch.h"

#include "fuel_pump.h"
#include "bench_test.h"

// Software PWM lets bench tests retain ownership of the output for their whole
// duration, including timer callbacks between fast-controller ticks.
static void applyFuelPumpPwm(int stateIndex, PwmConfig* pwm) {
	if (hasFirmwareError()) {
		enginePins.fuelPumpRelay.setValue(false);
	} else if (!isRunningBenchTest()) {
		applyPinState(stateIndex, pwm);
	}
}

void FuelPumpController::setDefaultConfiguration() {
	engineConfiguration->fuelPumpMode = fuel_pump_mode_e::Single;
	engineConfiguration->fuelPump2Pin = Gpio::Unassigned;
	engineConfiguration->fuelPump2PinMode = OM_DEFAULT;
	engineConfiguration->secondaryFpActivationRpm = 3000;
	engineConfiguration->secondaryFpActivationLoad = 100;
	engineConfiguration->secondaryFpActivationTps = 50;
	engineConfiguration->secondaryFpRpmHysteresis = 500;
	engineConfiguration->secondaryFpLoadHysteresis = 10;
	engineConfiguration->secondaryFpTpsHysteresis = 10;
	engineConfiguration->fuelPumpPwmFrequency = 100;
	engineConfiguration->fuelPumpMinDuty = 0;
	engineConfiguration->fuelPumpMaxDuty = 100;
	engineConfiguration->fuelPumpControl = {};
	engineConfiguration->fuelPumpControl.pFactor = 0.1f;
	engineConfiguration->fuelPumpControl.minValue = -50;
	engineConfiguration->fuelPumpControl.maxValue = 50;
	engineConfiguration->fuelPumpControl.periodMs = FAST_CALLBACK_PERIOD_MS;
	engineConfiguration->fuelPump_iTermMin = -30;
	engineConfiguration->fuelPump_iTermMax = 30;
	engineConfiguration->fuelPumpAggressiveRelief = false;
	engineConfiguration->fuelPumpReliefMaxInjectedMass = 10;
	engineConfiguration->fuelPumpReliefEngageOverpressure = 50;
	engineConfiguration->fuelPumpReliefRecoverOverpressure = 20;
	setLinearCurve(config->fuelPressureTargetRpmBins, 0, 8000);
	setLinearCurve(config->fuelPressureTargetLoadBins, 0, 300);
	setTable(config->fuelPressureTargetTable, 350.0f);
	setLinearCurve(config->fuelPumpBaseDutyRpmBins, 0, 8000);
	setLinearCurve(config->fuelPumpBaseDutyFpBins, 0, 700);
	setTable(config->fuelPumpBaseDutyTable, 50.0f);
}

void FuelPumpController::resetFeedback() {
	m_pid.reset();
	isFpPidActive = false;
	isFpReliefActive = false;
}

bool FuelPumpController::isHardwareConfigurationChanged(engine_configuration_s const* previous) const {
	return previous->fuelPumpMode != engineConfiguration->fuelPumpMode
		|| previous->fuelPumpPin != engineConfiguration->fuelPumpPin
		|| previous->fuelPumpPinMode != engineConfiguration->fuelPumpPinMode
		|| previous->fuelPump2Pin != engineConfiguration->fuelPump2Pin
		|| previous->fuelPump2PinMode != engineConfiguration->fuelPump2PinMode
		|| previous->fuelPumpPwmFrequency != engineConfiguration->fuelPumpPwmFrequency;
}

void FuelPumpController::stopHardware() {
	chibios_rt::CriticalSectionLocker lock;
	if (!m_hardwareInitialized) {
		return;
	}
	if (m_pwm) {
		m_pwm->stop();
		engine->scheduler.cancel(&m_pwm->scheduling);
		m_pwm.reset();
	}
	enginePins.fuelPumpRelay.setValue(false);
	enginePins.fuelPumpRelay2.setValue(false);
	m_hardwareInitialized = false;
	isSecondaryPumpOn = false;
	fuelPumpDuty = 0;
	fuelPressureTarget = 0;
	resetFeedback();
}

void FuelPumpController::initHardware() {
	stopHardware();
	chibios_rt::CriticalSectionLocker lock;
	m_pid.initPidClass(&engineConfiguration->fuelPumpControl);
	resetFeedback();
	m_mode = engineConfiguration->fuelPumpMode;
	m_hardwareInitialized = true;
	isSecondaryPumpOn = false;
	if (!isRunningBenchTest()) {
		enginePins.fuelPumpRelay.setValue(false);
		enginePins.fuelPumpRelay2.setValue(false);
	}
	if (m_mode == fuel_pump_mode_e::Pwm && isBrainPinValid(engineConfiguration->fuelPumpPin)
		&& engineConfiguration->fuelPumpPwmFrequency > 0) {
		m_pwm.emplace("Fuel pump PWM");
		startSimplePwm(&*m_pwm, "Fuel pump PWM", &engine->scheduler,
			&enginePins.fuelPumpRelay, engineConfiguration->fuelPumpPwmFrequency, 0, applyFuelPumpPwm);
	}
}

void FuelPumpController::onConfigurationChange(engine_configuration_s const* previous) {
	if (!m_hardwareInitialized || isHardwareConfigurationChanged(previous)) {
		initHardware();
	} else {
		resetFeedback();
	}
}

void initFuelPump() {
	engine->module<FuelPumpController>()->initHardware();
}

void stopFuelPump() {
	if (engine->module<FuelPumpController>()->isHardwareConfigurationChanged(&activeConfiguration)) {
		engine->module<FuelPumpController>()->stopHardware();
	}
}

void FuelPumpController::onSlowCallback() {
	auto timeSinceIgn = m_ignOnTimer.getElapsedSeconds();

	// If the ignition just turned on, turn on the fuel pump to prime
	isPrime = timeSinceIgn >= 0 && timeSinceIgn < engineConfiguration->startUpFuelPumpDuration;

#if EFI_SHAFT_POSITION_INPUT
	// If there was a trigger event recently, turn on the pump, the engine is running!
	engineTurnedRecently = engine->triggerCentral.engineMovedRecently();
#endif //EFI_SHAFT_POSITION_INPUT

	isFuelPumpOn = (isPrime || engineTurnedRecently) && !hasFirmwareError();

	if (!m_hardwareInitialized || isRunningBenchTest()) {
		return;
	}
	if (m_mode == fuel_pump_mode_e::Single || m_mode == fuel_pump_mode_e::Dual) {
		enginePins.fuelPumpRelay.setValue("FP", isFuelPumpOn);
	}
	updateDualRelay();
}

void FuelPumpController::updateDualRelay() {
	if (m_mode != fuel_pump_mode_e::Dual || !isFuelPumpOn) {
		isSecondaryPumpOn = false;
		enginePins.fuelPumpRelay2.setValue(false);
		return;
	}
	auto rpm = Sensor::get(SensorType::Rpm);
	auto tps = Sensor::get(SensorType::Tps1);
	float load = getEngineState()->fuelingLoad;
	if (!rpm || !tps
		|| !std::isfinite(rpm.Value) || !std::isfinite(tps.Value) || !std::isfinite(load)) {
		isSecondaryPumpOn = false;
	} else if (!isSecondaryPumpOn) {
		isSecondaryPumpOn = rpm.Value > engineConfiguration->secondaryFpActivationRpm
			&& load > engineConfiguration->secondaryFpActivationLoad
			&& tps.Value > engineConfiguration->secondaryFpActivationTps;
	} else if (rpm.Value < engineConfiguration->secondaryFpActivationRpm - engineConfiguration->secondaryFpRpmHysteresis
		|| load < engineConfiguration->secondaryFpActivationLoad - engineConfiguration->secondaryFpLoadHysteresis
		|| tps.Value < engineConfiguration->secondaryFpActivationTps - engineConfiguration->secondaryFpTpsHysteresis) {
		isSecondaryPumpOn = false;
	}
	enginePins.fuelPumpRelay2.setValue("FP2", isSecondaryPumpOn);
}

void FuelPumpController::onFastCallback() {
	if (!m_hardwareInitialized || m_mode != fuel_pump_mode_e::Pwm) {
		return;
	}
	if (isRunningBenchTest()) {
		resetFeedback();
		return;
	}
	isFpPidActive = false;
	if (!observePlant()) {
		resetFeedback();
		auto target = getSetpoint();
		setOutput(target ? getOpenLoop(target.Value) : expected<percent_t>{unexpected});
	} else {
		update();
	}
}

expected<float> FuelPumpController::getSetpoint() {
	fuelPressureTarget = 0;
	if (!isFuelPumpOn || hasFirmwareError()) {
		resetFeedback();
		return unexpected;
	}
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	float load = getEngineState()->fuelingLoad;
	if (!std::isfinite(rpm) || !std::isfinite(load)) {
		resetFeedback();
		return unexpected;
	}
	float target = interpolate3d(config->fuelPressureTargetTable,
		config->fuelPressureTargetLoadBins, load, config->fuelPressureTargetRpmBins, rpm);
	if (!std::isfinite(target)) {
		resetFeedback();
		return unexpected;
	}
	target = clampF(0, target, 1275);
	fuelPressureTarget = target;
	return target;
}

expected<float> FuelPumpController::observePlant() {
	auto pressure = Sensor::get(SensorType::FuelPressureLow);
	return pressure && std::isfinite(pressure.Value) ? pressure : expected<float>{unexpected};
}

expected<percent_t> FuelPumpController::getOpenLoop(float target) {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (!std::isfinite(rpm) || !std::isfinite(target)) {
		return unexpected;
	}
	return interpolate3d(config->fuelPumpBaseDutyTable,
		config->fuelPumpBaseDutyFpBins, target, config->fuelPumpBaseDutyRpmBins, rpm);
}

expected<percent_t> FuelPumpController::getClosedLoop(float target, float pressure) {
	bool reliefEligible = false;
#if EFI_ENGINE_CONTROL
	reliefEligible = engineConfiguration->fuelPumpAggressiveRelief
		&& engine->fuelComputer.running.fuel < engineConfiguration->fuelPumpReliefMaxInjectedMass;
#endif
	if (!reliefEligible) {
		isFpReliefActive = false;
	} else if (pressure > target + engineConfiguration->fuelPumpReliefEngageOverpressure) {
		isFpReliefActive = true;
	} else if (pressure <= target + engineConfiguration->fuelPumpReliefRecoverOverpressure) {
		isFpReliefActive = false;
	}
	if (isFpReliefActive) {
		return unexpected;
	}
	isFpPidActive = true;
	m_pid.iTermMin = engineConfiguration->fuelPump_iTermMin;
	m_pid.iTermMax = engineConfiguration->fuelPump_iTermMax;
	return m_pid.getOutput(target, pressure, FAST_CALLBACK_PERIOD_MS / 1000.0f);
}

bool FuelPumpController::hasValidDutyLimits() const {
	return engineConfiguration->fuelPumpMinDuty <= engineConfiguration->fuelPumpMaxDuty
		&& engineConfiguration->fuelPumpMaxDuty <= 100;
}

void FuelPumpController::setOutput(expected<percent_t> output) {
	percent_t duty = 0;
	if (!isFuelPumpOn || hasFirmwareError() || !hasValidDutyLimits()) {
		resetFeedback();
	} else if (isPrime) {
		duty = engineConfiguration->fuelPumpMaxDuty;
		resetFeedback();
	} else if (output && std::isfinite(output.Value)) {
		duty = clampF(engineConfiguration->fuelPumpMinDuty, output.Value, engineConfiguration->fuelPumpMaxDuty);
	} else {
		duty = engineConfiguration->fuelPumpMinDuty;
		m_pid.reset();
		isFpPidActive = false;
	}
	fuelPumpDuty = duty;
	chibios_rt::CriticalSectionLocker lock;
	if (m_pwm && !isRunningBenchTest()) {
		m_pwm->setSimplePwmDutyCycle(PERCENT_TO_DUTY(duty));
	}
}

void FuelPumpController::onIgnitionStateChanged(bool ignitionOnParam) {
	// live data parser convention is asking for a field
	ignitionOn = ignitionOnParam;
	if (ignitionOn) {
		m_ignOnTimer.reset();
	}
}
