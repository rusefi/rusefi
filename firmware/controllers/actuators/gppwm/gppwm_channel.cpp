
#include "gppwm_channel.h"

#include "engine.h"
#include "pwm_generator_logic.h"
#include "table_helper.h"
#include "expected.h"
#include "sensor.h"
#include "map.h"

EXTERN_ENGINE;

expected<float> readGppwmChannel(gppwm_channel_e channel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (channel) {
	case GPPWM_Tps:
		return Sensor::get(SensorType::Tps1);
	case GPPWM_Map: {
		float map = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);

		if (cisnan(map)) {
			return unexpected;
		}

		return map;
	}
	case GPPWM_Clt:
		return Sensor::get(SensorType::Clt);
	case GPPWM_Iat:
		return Sensor::get(SensorType::Iat);
	default:
		return unexpected;
	}
}

void GppwmChannel::setOutput(float result) {
	// Not init yet, nothing to do.
	if (!m_pwm || !m_config) {
		return;
	}
	
	if (!m_usePwm) {
		// Apply hysteresis with provided values
		if (m_state && result < m_config->offBelowDuty) {
			m_state = false;
		} else if (!m_state && result > m_config->onAboveDuty) {
			m_state = true;
		}

		result = m_state ? 100 : 0;
	}

	m_pwm->setSimplePwmDutyCycle(clampF(0, result / 100.0f, 1));
}

void GppwmChannel::init(bool usePwm, SimplePwm* pwm, const ValueProvider3D* table, const gppwm_channel* config) {
	m_usePwm = usePwm;
	m_pwm = pwm;
	m_table = table;
	m_config = config;
}

float GppwmChannel::getOutput() const {
	expected<float> loadAxisValue = readGppwmChannel(m_config->loadAxis PASS_ENGINE_PARAMETER_SUFFIX);

	// If we couldn't get load axis value, fall back on error value
	if (!loadAxisValue) {
		return m_config->dutyIfError;
	}

	float rpm = GET_RPM();

	float result = m_table->getValue(rpm / RPM_1_BYTE_PACKING_MULT, loadAxisValue.Value);

	if (cisnan(result)) {
		return m_config->dutyIfError;
	}

	return result;
}

void GppwmChannel::update() {
	// Without a config, nothing to do.
	if (!m_config) {
		return;
	}

	float output = getOutput();
	setOutput(output);
}
