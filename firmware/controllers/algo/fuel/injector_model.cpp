#include "injector_model.h"

EXTERN_ENGINE;

void InjectorModelBase::prepare() {
	m_massFlowRate = getInjectorMassFlowRate();
	m_deadtime = getDeadtime();
}

constexpr float convertToGramsPerSecond(float ccPerMinute) {
	float ccPerSecond = ccPerMinute / 60;
	return ccPerSecond * 0.72f;	// 0.72g/cc fuel density
}

float InjectorModel::getInjectorMassFlowRate() const {
	auto injectorVolumeFlow = CONFIG(injector.flow);

	return convertToGramsPerSecond(injectorVolumeFlow);
}

float InjectorModel::getDeadtime() const {
	float vbatt = ENGINE(sensors.vbatt);

	return interpolate2d("lag", vbatt, engineConfiguration->injector.battLagCorrBins, engineConfiguration->injector.battLagCorr);
}

float InjectorModelBase::getInjectionDuration(float fuelMassGram) const {
	floatms_t baseDuration = fuelMassGram / m_massFlowRate;

	// TODO: support injector nonlinearity correction

	return baseDuration + m_deadtime;
}



