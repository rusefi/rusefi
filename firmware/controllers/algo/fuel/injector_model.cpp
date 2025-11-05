#include "pch.h"

#include "injector_model.h"
#include "fuel_computer.h"

void InjectorModelBase::prepare() {
	float flowRatio = getInjectorFlowRatio();

	// "large pulse" flow rate
	m_massFlowRate = flowRatio * getBaseFlowRate();
	m_deadtime = getDeadtime();

	if (getNonlinearMode() == INJ_FordModel) {
		m_smallPulseFlowRate = flowRatio * getSmallPulseFlowRate();
		m_smallPulseBreakPoint = getSmallPulseBreakPoint();

		// amount added to small pulses to correct for the "kink" from low flow region
		m_smallPulseOffset = 1000 * ((m_smallPulseBreakPoint / m_massFlowRate) - (m_smallPulseBreakPoint / m_smallPulseFlowRate));
	}
}

constexpr float convertToGramsPerSecond(float ccPerMinute) {
	return ccPerMinute * (fuelDensity / 60.f);
}

// returns: grams per second flow
float InjectorModelWithConfig::getBaseFlowRate() const {
	if (engineConfiguration->injectorFlowAsMassFlow) {
		return m_cfg->flow;
	} else {
		return convertToGramsPerSecond(m_cfg->flow);
	}
}

float InjectorModelPrimary::getSmallPulseFlowRate() const {
	return engineConfiguration->fordInjectorSmallPulseSlope;
}

float InjectorModelPrimary::getSmallPulseBreakPoint() const {
	// convert milligrams -> grams
	// todo: make UI deal with scaling?!
	return 0.001f * engineConfiguration->fordInjectorSmallPulseBreakPoint;
}

InjectorNonlinearMode InjectorModelPrimary::getNonlinearMode() const {
	return engineConfiguration->injectorNonlinearMode;
}

injector_compensation_mode_e InjectorModelPrimary::getInjectorCompensationMode() const {
	return engineConfiguration->injectorCompensationMode;
}

float InjectorModelPrimary::getFuelReferencePressure() const {
	return engineConfiguration->fuelReferencePressure;
}

float InjectorModelSecondary::getSmallPulseFlowRate() const {
	// not supported on second bank
	return 0;
}

float InjectorModelSecondary::getSmallPulseBreakPoint() const {
	// not supported on second bank
	return 0;
}

injector_compensation_mode_e InjectorModelSecondary::getInjectorCompensationMode() const {
	return engineConfiguration->secondaryInjectorCompensationMode;
}

float InjectorModelSecondary::getFuelReferencePressure() const {
	return engineConfiguration->secondaryInjectorFuelReferencePressure;
}

InjectorNonlinearMode InjectorModelSecondary::getNonlinearMode() const {
	// nonlinear not supported on second bank
	return InjectorNonlinearMode::INJ_None;
}

void InjectorModelWithConfig::updateState() {
	// TODO: remove at the end of 2025
	// hack not to break tunes before 2779925f54f5c2d23499cbb2797f71508c652f54 "injector lag lookup should be done based on differential pressure"
	if (engineConfiguration->useAbsolutePressureForLagTime) {
		pressureCorrectionReference = getFuelPressure().Value;
	} else {
		pressureCorrectionReference = getFuelDifferentialPressure().Value;
	}
}

expected<float> InjectorModelWithConfig::getFuelPressure() const {
  return getFuelDifferentialPressure().Value + Sensor::get(SensorType::Map).value_or(STD_ATMOSPHERE);
}

expected<float> InjectorModelWithConfig::getFuelDifferentialPressure() const {
	auto map = Sensor::get(SensorType::Map);
	auto baro = Sensor::get(SensorType::BarometricPressure);

	float baroKpa = baro.Value;
	// todo: extract baro sensor validation logic
	if (!baro || baro.Value > 120 || baro.Value < 50) {
		baroKpa = STD_ATMOSPHERE;
	}

	switch (getInjectorCompensationMode()) {
		case ICM_FixedRailPressure:
			// Add barometric pressure, as "fixed" really means "fixed pressure above atmosphere"
			return getFuelReferencePressure()
				+ baroKpa
				- map.value_or(STD_ATMOSPHERE);
		case ICM_SensedRailPressure: {
			if (!Sensor::hasSensor(SensorType::FuelPressureInjector)) {
				warning(ObdCode::OBD_Fuel_Pressure_Sensor_Missing, "Fuel pressure compensation is set to use a pressure sensor, but none is configured.");
				return unexpected;
			}

			auto fps = Sensor::get(SensorType::FuelPressureInjector);

			// TODO: what happens when the sensor fails?
			if (!fps) {
				return unexpected;
			}

			switch (engineConfiguration->fuelPressureSensorMode) {
				case FPM_Differential:
					// This sensor directly measures delta-P, no math needed!
					return fps.Value;
				case FPM_Gauge:
					if (!map) {
						return unexpected;
					}

					return fps.Value + baroKpa - map.Value;
				case FPM_Absolute:
				default:
					if (!map) {
						return unexpected;
					}

					return fps.Value - map.Value;
			}
		} default: return unexpected;
	}
}

float InjectorModelWithConfig::getInjectorFlowRatio() {
	// Compensation disabled, use reference flow.
	auto compensationMode = getInjectorCompensationMode();
	if (compensationMode == ICM_None || compensationMode == ICM_HPFP_Manual_Compensation) {
		return 1.0f;
	}

	const float referencePressure = getFuelReferencePressure();

	if (referencePressure < 50) {
		// impossibly low fuel ref pressure
		criticalError("Impossible fuel reference pressure: %f", referencePressure);

		return 1.0f;
	}

	expected<float> diffPressure = getFuelDifferentialPressure();

	// If sensor failed, best we can do is disable correction
	if (!diffPressure) {
		return 1.0f;
	}

	pressureDelta = diffPressure.Value;

	// Somehow pressure delta is less than 0, assume failed sensor and return default flow
	if (pressureDelta <= 0) {
		return 1.0f;
	}

	pressureRatio = pressureDelta / referencePressure;
	// todo: live data model?
	float flowRatio = sqrtf(pressureRatio);

	// TODO: should the flow ratio be clamped?
	return flowRatio;
}

float InjectorModelWithConfig::getDeadtime() const {
	return interpolate3d(
		m_cfg->battLagCorrTable,
      	m_cfg->battLagCorrPressBins, pressureCorrectionReference,
      	m_cfg->battLagCorrBattBins, Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE)
	);
}

//TODO: only used in the tests, refactor pending to InjectorModelWithConfig
floatms_t InjectorModelBase::getInjectionDuration(float fuelMassGram) const {
	if (fuelMassGram <= 0) {
		// If 0 mass, don't do any math, just skip the injection.
		return 0.0f;
	}

	// Get the no-offset duration
	floatms_t baseDuration = getBaseDurationImpl(fuelMassGram);

	return baseDuration + m_deadtime;
}

floatms_t InjectorModelWithConfig::getInjectionDuration(float fuelMassGram) const {
	if (fuelMassGram <= 0) {
		// If 0 mass, don't do any math, just skip the injection.
		return 0.0f;
	}

  // hopefully one day we pick between useInjectorFlowLinearizationTable and ICM_HPFP_Manual_Compensation approaches
  // and not more than one of these would stay
	if (engineConfiguration->useInjectorFlowLinearizationTable) {
	  auto fps = Sensor::get(SensorType::FuelPressureInjector);
	// todo: KPA vs BAR mess?!
    return interpolate3d(config->injectorFlowLinearization,
			config->injectorFlowLinearizationPressureBins, KPA2BAR(fps.Value),// array values are on bar
			config->injectorFlowLinearizationFuelMassBins, fuelMassGram * 1000);  // array values are on mg
	}

	// Get the no-offset duration
	floatms_t baseDuration = getBaseDurationImpl(fuelMassGram);

	// default non GDI case
	if (getInjectorCompensationMode() != ICM_HPFP_Manual_Compensation) {
		// Add deadtime offset
		return baseDuration + m_deadtime;
	}

	if (!Sensor::hasSensor(SensorType::FuelPressureHigh)) {
		return baseDuration + m_deadtime;
	}

	auto fps = Sensor::get(SensorType::FuelPressureHigh);
	// todo: KPA vs BAR mess in code and UI?!
	float fuelMassCompensation = interpolate3d(config->hpfpFuelMassCompensation,
			config->hpfpFuelMassCompensationFuelPressure, KPA2BAR(fps.Value),// array values are on bar
			config->hpfpFuelMassCompensationFuelMass, fuelMassGram * 1000);  // array values are on mg

	// recalculate base duration with fuel mass compensation
	baseDuration =  getBaseDurationImpl(fuelMassGram * fuelMassCompensation);
	return baseDuration + m_deadtime;
}

float InjectorModelBase::getFuelMassForDuration(floatms_t duration) const {
	// Convert from ms -> grams
	return duration * m_massFlowRate * 0.001f;
}

// todo: all that *1000 and *0.001f is pretty annoying, we need a cleaner approach for units!
floatms_t InjectorModelBase::getBaseDurationImpl(float fuelMassGram) const {
	floatms_t baseDuration = fuelMassGram / m_massFlowRate * 1000;

	switch (getNonlinearMode()) {
	case INJ_FordModel:
		if (fuelMassGram < m_smallPulseBreakPoint) {
			// Small pulse uses a different slope, and adds the "zero fuel pulse" offset
			return (fuelMassGram / m_smallPulseFlowRate * 1000) + m_smallPulseOffset;
		} else {
			// Large pulse
			return baseDuration;
		}
	case INJ_PolynomialAdder:
		return correctInjectionPolynomial(baseDuration);
	case INJ_None:
	default:
		return baseDuration;
	}
}

floatms_t InjectorModelBase::correctInjectionPolynomial(floatms_t baseDuration) const {
	if (baseDuration > engineConfiguration->applyNonlinearBelowPulse) {
		// Large pulse, skip correction.
		return baseDuration;
	}

	auto& is = engineConfiguration->injectorCorrectionPolynomial;
	float xi = 1;

	float adder = 0;

	// Add polynomial terms, starting with x^0
	for (size_t i = 0; i < efi::size(is); i++) {
		adder += is[i] * xi;
		xi *= baseDuration;
	}

	return baseDuration + adder;
}

InjectorModelWithConfig::InjectorModelWithConfig(const injector_s* const cfg)
	: m_cfg(cfg)
{
}

InjectorModelPrimary::InjectorModelPrimary()
	: InjectorModelWithConfig(&engineConfiguration->injector)
{
}

// TODO: actual separate config for second bank!
InjectorModelSecondary::InjectorModelSecondary()
	: InjectorModelWithConfig(&engineConfiguration->injectorSecondary)
{
}
