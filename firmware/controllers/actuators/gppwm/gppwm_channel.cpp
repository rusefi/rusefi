
#include "pch.h"

#include "gppwm_channel.h"

#include "table_helper.h"
#include "expected.h"

expected<float> readGppwmChannel(gppwm_channel_e channel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (channel) {
	case GPPWM_Tps:
		return Sensor::get(SensorType::Tps1);
	case GPPWM_Map:
		return Sensor::get(SensorType::Map);
	case GPPWM_Clt:
		return Sensor::get(SensorType::Clt);
	case GPPWM_Iat:
		return Sensor::get(SensorType::Iat);
	case GPPWM_FuelLoad:
		return getFuelingLoad(PASS_ENGINE_PARAMETER_SIGNATURE);
	case GPPWM_IgnLoad:
		return getIgnitionLoad(PASS_ENGINE_PARAMETER_SIGNATURE);
	case GPPWM_AuxTemp1:
		return Sensor::get(SensorType::AuxTemp1);
	case GPPWM_AuxTemp2:
		return Sensor::get(SensorType::AuxTemp2);
	case GPPWM_Zero:
		return 0;
	case GPPWM_AccelPedal:
		return Sensor::get(SensorType::AcceleratorPedal);
	}

	return unexpected;
}

void GppwmChannel::setOutput(float result) {
	// Not init yet, nothing to do.
	if (!m_config) {
		return;
	}

	if (m_usePwm) {
		efiAssertVoid(OBD_PCM_Processor_Fault, m_usePwm, "m_usePwm null");
		m_pwm->setSimplePwmDutyCycle(clampF(0, result / 100.0f, 1));
	} else {
		efiAssertVoid(OBD_PCM_Processor_Fault, m_output, "m_output null");
		// Apply hysteresis with provided values
		if (m_state && result < m_config->offBelowDuty) {
			m_state = false;
		} else if (!m_state && result > m_config->onAboveDuty) {
			m_state = true;
		}

		m_output->setValue(m_state);
	}
}

void GppwmChannel::init(bool usePwm, IPwm* pwm, OutputPin* outputPin, const ValueProvider3D* table, const gppwm_channel* config) {
	m_usePwm = usePwm;
	m_pwm = pwm;
	m_output = outputPin;
	m_table = table;
	m_config = config;
}

percent_t GppwmChannel::getOutput() const {
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

float GppwmChannel::update() {
	// Without a config, nothing to do.
	if (!m_config) {
		return 0;
	}

	float output = getOutput();
	setOutput(output);
	return output;
}
