#include "injector_model.h"
#include "tunerstudio_outputs.h"
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

expected<float> InjectorModel::getAbsoluteRailPressure() const {
	switch (CONFIG(injectorCompensationMode)) {
		case ICM_FixedRailPressure:
			// TODO: should this add baro pressure instead of 1atm?
			return (CONFIG(fuelReferencePressure) + 101.325f);
		case ICM_SensedRailPressure:
			// TODO: what happens when the sensor fails?
			return Sensor::get(SensorType::FuelPressureInjector);
		default: return unexpected;
	}
}

float InjectorModel::getInjectorFlowRatio() const {
	// Compensation disabled, use reference flow.
	if (CONFIG(injectorCompensationMode) == ICM_None) {
		return 1.0f;
	}

	float referencePressure = CONFIG(fuelReferencePressure);
	expected<float> absRailPressure = getAbsoluteRailPressure();

	// If sensor failed, best we can do is disable correction
	if (!absRailPressure) {
		return 1.0f;
	}

	float map = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);

	// TODO: what to do when pressureDelta is less than 0?
	float pressureDelta = absRailPressure.Value - map;
	float pressureRatio = pressureDelta / referencePressure;
	float flowRatio = sqrtf(pressureRatio);

#if EFI_TUNER_STUDIO
	if (CONFIG(debugMode) == DBG_INJECTOR_COMPENSATION) {
		tsOutputChannels.debugFloatField1 = pressureDelta;
		tsOutputChannels.debugFloatField2 = pressureRatio;
		tsOutputChannels.debugFloatField3 = flowRatio;
	}
#endif // EFI_TUNER_STUDIO

	// TODO: should the flow ratio be clamped?
	return flowRatio;
}

float InjectorModel::getInjectorMassFlowRate() const {
	// TODO: injector flow dependent upon temperature/ethanol content?
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
