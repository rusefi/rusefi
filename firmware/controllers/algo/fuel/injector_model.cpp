#include "injector_model.h"
#include "map.h"

EXTERN_ENGINE;

void InjectorModelBase::prepare() {
	m_massFlowRate = getInjectorMassFlowRate();
	float deadtime = getDeadtime();
	m_deadtime = deadtime;

	postState(deadtime);
}

constexpr float convertToGramsPerSecond(float ccPerMinute) {
	float ccPerSecond = ccPerMinute / 60;
	return ccPerSecond * 0.72f;	// 0.72g/cc fuel density
}

float InjectorModel::getAbsoluteRailPressure() const {
	switch (CONFIG(injectorCompensationMode)) {
		case ICM_FixedRailPressure:
			return (CONFIG(fuelReferencePressure) + 101.325f);
		case ICM_SensedRailPressure:
			// TODO: what happens when the sensor fails?
			return Sensor::get(SensorType::FuelPressureInjector).value_or(0);
		default: return 0;
	}
}

float InjectorModel::getInjectorFlowRatio() const {
	// Compensation disabled, use reference flow.
	if (CONFIG(injectorCompensationMode) == ICM_None) {
		return 1.0f;
	}

	float referencePressure = CONFIG(fuelReferencePressure);
	float absRailPressure = getAbsoluteRailPressure();

	float map = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);

	float pressureDelta = absRailPressure - map;

	return sqrtf(pressureDelta / referencePressure);
}

float InjectorModel::getInjectorMassFlowRate() const {
	// TODO: injector flow dependent upon rail pressure (and temperature/ethanol content?)
	auto injectorVolumeFlow = CONFIG(injector.flow);

	float flowRatio = getInjectorFlowRatio();

	return flowRatio * convertToGramsPerSecond(injectorVolumeFlow);
}

float InjectorModel::getDeadtime() const {
	return interpolate2d(
		"lag",
		ENGINE(sensors.vBatt),
		engineConfiguration->injector.battLagCorrBins,
		engineConfiguration->injector.battLagCorr
	);
}

void InjectorModel::postState(float deadtime) const {
	engine->engineState.running.injectorLag = deadtime;
}

float InjectorModelBase::getInjectionDuration(float fuelMassGram) const {
	// TODO: support injector nonlinearity correction

	floatms_t baseDuration = fuelMassGram / m_massFlowRate * 1000;

	if (baseDuration <= 0) {
		// If 0 duration, don't add deadtime, just skip the injection.
		return 0.0f;
	} else {
		return baseDuration + m_deadtime;
	}
}
