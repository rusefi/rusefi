#include "airmass.h"
#include "sensor.h"
#include "map.h"

EXTERN_ENGINE;

AirmassModelBase::AirmassModelBase(const ValueProvider3D& veTable) : m_veTable(&veTable) {}

float AirmassModelBase::getVeLoadAxis(float passedLoad) const {
	switch(CONFIG(veOverrideMode)) {
		case VE_None: return passedLoad;
		case VE_MAP: return getMap(PASS_ENGINE_PARAMETER_SIGNATURE);
		case VE_TPS: return Sensor::get(SensorType::Tps1).value_or(0);
		default: return 0;
	}
}

float AirmassModelBase::getVe(int rpm, float load) const {
	efiAssert(OBD_PCM_Processor_Fault, m_veTable != nullptr, "VE table null", 0);

	// Override the load value if necessary
	load = getVeLoadAxis(load);

	float ve = m_veTable->getValue(rpm, load);

	auto tps = Sensor::get(SensorType::Tps1);
	// get VE from the separate table for Idle
	if (tps.Valid && CONFIG(useSeparateVeForIdle)) {
		float idleVe = interpolate2d("idleVe", rpm, config->idleVeBins, config->idleVe);
		// interpolate between idle table and normal (running) table using TPS threshold
		ve = interpolateClamped(0.0f, idleVe, CONFIG(idlePidDeactivationTpsThreshold), ve, tps.Value);
	}

	ENGINE(engineState.currentVe) = ve;
	ENGINE(engineState.currentVeLoad) = load;
	return ve * 0.01f;
}
