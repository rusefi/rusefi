
#include "pch.h"

#include "gppwm_channel.h"

#include "table_helper.h"
#include "gppwm_channel_reader.h"

float GppwmChannel::setOutput(float result) {
	// Not init yet, nothing to do.
	if (!m_config) {
		return result;
	}

	if (m_usePwm) {
		efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_usePwm, "m_usePwm null", 0);
		m_pwm->setSimplePwmDutyCycle(clampF(0, result / 100.0f, 1));

		return result;
	} else {
		efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_output, "m_output null", 0);
		if (m_config->offBelowDuty > m_config->onAboveDuty) {
			firmwareError(ObdCode::CUSTOM_ERR_6122, "You can't have off below %d greater than on above %d",
					m_config->offBelowDuty,
					m_config->onAboveDuty);
		}
		// Apply hysteresis with provided values
		if (m_state && result < m_config->offBelowDuty) {
			m_state = false;
		} else if (!m_state && result > m_config->onAboveDuty) {
			m_state = true;
		}

		m_output->setValue(m_state);

		// Return the actual output value with hysteresis
		return m_state ? 100 : 0;
	}
}

void GppwmChannel::init(bool usePwm, IPwm* pwm, OutputPin* outputPin, const ValueProvider3D* table, const gppwm_channel* p_config) {
	m_usePwm = usePwm;
	m_pwm = pwm;
	m_output = outputPin;
	m_table = table;
	m_config = p_config;
}

GppwmResult GppwmChannel::getOutput() const {
	expected<float> xAxisValue = readGppwmChannel(m_config->rpmAxis);
	expected<float> yAxisValue = readGppwmChannel(m_config->loadAxis);

	GppwmResult result	{ (float)m_config->dutyIfError, xAxisValue.value_or(0), yAxisValue.value_or(0) };

	// If we couldn't get load axis value, fall back on error value
	if (!xAxisValue || !yAxisValue) {
		return result;
	}

	float resultVal = m_table->getValue(xAxisValue.Value, yAxisValue.Value);

	if (std::isnan(result.Result)) {
		return result;
	}

	result.Result = resultVal;
	return result;
}

GppwmResult GppwmChannel::update() {
	// Without a config, nothing to do.
	if (!m_config) {
		return {};
	}

	auto output = getOutput();
	output.Result = setOutput(output.Result);

	return output;
}
