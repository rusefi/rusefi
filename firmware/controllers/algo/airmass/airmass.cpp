#include "airmass.h"
#include "sensor.h"

EXTERN_ENGINE;

AirmassModelBase::AirmassModelBase(const ValueProvider3D& veTable) : m_veTable(&veTable) {}

float AirmassModelBase::getVe(int rpm, float load) const {
	efiAssert(OBD_PCM_Processor_Fault, m_veTable != nullptr, "VE table null", 0);

	// TODO: allow override of the Y axis value based on a config field
	float ve = m_veTable->getValue(rpm, load);

	auto tps = Sensor::get(SensorType::Tps1);
	// get VE from the separate table for Idle
	if (tps.Valid && CONFIG(useSeparateVeForIdle)) {
		float idleVe = interpolate2d("idleVe", rpm, config->idleVeBins, config->idleVe);
		// interpolate between idle table and normal (running) table using TPS threshold
		ve = interpolateClamped(0.0f, idleVe, CONFIG(idlePidDeactivationTpsThreshold), ve, tps.Value);
	}

	ENGINE(engineState.currentVe) = ve;
	return ve;
}
