
// here am flirting with not using pch.h and not including at least Engine
#include <rusefi/interpolation.h>
#include <rusefi/arrays.h>
#include "engine_configuration.h"
#include "sensor.h"
#include "error_handling.h"

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

float InjectorModel::getBaseFlowRate() const {
	if (engineConfiguration->injectorFlowAsMassFlow) {
		return engineConfiguration->injector.flow;
	} else {
		return convertToGramsPerSecond(engineConfiguration->injector.flow);
	}
}

float InjectorModel::getSmallPulseFlowRate() const {
	return engineConfiguration->fordInjectorSmallPulseSlope;
}

float InjectorModel::getSmallPulseBreakPoint() const {
	// convert milligrams -> grams
	return 0.001f * engineConfiguration->fordInjectorSmallPulseBreakPoint;
}

InjectorNonlinearMode InjectorModel::getNonlinearMode() const {
	return engineConfiguration->injectorNonlinearMode;
}

expected<float> InjectorModel::getAbsoluteRailPressure() const {
	switch (engineConfiguration->injectorCompensationMode) {
		case ICM_FixedRailPressure:
			// Add barometric pressure, as "fixed" really means "fixed pressure above atmosphere"
			return engineConfiguration->fuelReferencePressure + Sensor::get(SensorType::BarometricPressure).value_or(101.325f);
		case ICM_SensedRailPressure:
			if (!Sensor::hasSensor(SensorType::FuelPressureInjector)) {
				firmwareError(OBD_PCM_Processor_Fault, "Fuel pressure compensation is set to use a pressure sensor, but none is configured.");
				return unexpected;
			}

			// TODO: what happens when the sensor fails?
			return Sensor::get(SensorType::FuelPressureInjector);
		default: return unexpected;
	}
}

float InjectorModel::getInjectorFlowRatio() {
	// Compensation disabled, use reference flow.
	if (engineConfiguration->injectorCompensationMode == ICM_None) {
		return 1.0f;
	}

	float referencePressure = engineConfiguration->fuelReferencePressure;

	if (referencePressure < 50) {
		// impossibly low fuel ref pressure
		firmwareError(OBD_PCM_Processor_Fault, "Impossible fuel reference pressure: %f", referencePressure);

		return 1.0f;
	}

	expected<float> absRailPressure = getAbsoluteRailPressure();

	// If sensor failed, best we can do is disable correction
	if (!absRailPressure) {
		return 1.0f;
	}

	auto map = Sensor::get(SensorType::Map);

	// Map has failed, assume nominal pressure
	if (!map) {
		return 1.0f;
	}

	pressureDelta = absRailPressure.Value - map.Value;

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

float InjectorModel::getDeadtime() const {
	return interpolate2d(
		Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE),
		engineConfiguration->injector.battLagCorrBins,
		engineConfiguration->injector.battLagCorr
	);
}

float InjectorModelBase::getInjectionDuration(float fuelMassGram) const {
	if (fuelMassGram <= 0) {
		// If 0 mass, don't do any math, just skip the injection.
		return 0.0f;
	}

	// Get the no-offset duration
	float baseDuration = getBaseDurationImpl(fuelMassGram);

	// Add deadtime offset
	return baseDuration + m_deadtime;
}

float InjectorModelBase::getFuelMassForDuration(floatms_t duration) const {
	// Convert from ms -> grams
	return duration * m_massFlowRate * 0.001f;
}

float InjectorModelBase::getBaseDurationImpl(float fuelMassGram) const {
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

float InjectorModelBase::correctInjectionPolynomial(float baseDuration) const {
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
