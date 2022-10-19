
#include "pch.h"

#include "gppwm_channel.h"

#include "table_helper.h"
#include "expected.h"

expected<float> readGppwmChannel(gppwm_channel_e channel) {
	switch (channel) {
	case GPPWM_Zero:
		return 0;
	case GPPWM_Tps:
		return Sensor::get(SensorType::Tps1);
	case GPPWM_Map:
		return Sensor::get(SensorType::Map);
	case GPPWM_Clt:
		return Sensor::get(SensorType::Clt);
	case GPPWM_Iat:
		return Sensor::get(SensorType::Iat);
	case GPPWM_LuaGauge1:
		return Sensor::get(SensorType::LuaGauge1);
	case GPPWM_LuaGauge2:
		return Sensor::get(SensorType::LuaGauge2);
	case GPPWM_FuelLoad:
		return getFuelingLoad();
	case GPPWM_IgnLoad:
		return getIgnitionLoad();
	case GPPWM_AuxTemp1:
		return Sensor::get(SensorType::AuxTemp1);
	case GPPWM_AuxTemp2:
		return Sensor::get(SensorType::AuxTemp2);
	case GPPWM_AccelPedal:
		return Sensor::get(SensorType::AcceleratorPedal);
	case GPPWM_Vbatt:
		return Sensor::get(SensorType::BatteryVoltage);
	case GPPWM_VVT_1I:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
	case GPPWM_VVT_1E:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/1);
	case GPPWM_VVT_2I:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/0);
	case GPPWM_VVT_2E:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/1);
	case GPPWM_EthanolPercent:
		return Sensor::get(SensorType::FuelEthanolPercent);
	case GPPWM_AuxLinear1:
		return Sensor::get(SensorType::AuxLinear1);
	case GPPWM_AuxLinear2:
		return Sensor::get(SensorType::AuxLinear2);
	case GPPWM_GppwmOutput1:
		return (float)engine->outputChannels.gppwmOutput[0];
	case GPPWM_GppwmOutput2:
		return (float)engine->outputChannels.gppwmOutput[1];
	case GPPWM_GppwmOutput3:
		return (float)engine->outputChannels.gppwmOutput[2];
	case GPPWM_GppwmOutput4:
		return (float)engine->outputChannels.gppwmOutput[3];
	}

	return unexpected;
}

float GppwmChannel::setOutput(float result) {
	// Not init yet, nothing to do.
	if (!m_config) {
		return result;
	}

	if (m_usePwm) {
		efiAssert(OBD_PCM_Processor_Fault, m_usePwm, "m_usePwm null", 0);
		m_pwm->setSimplePwmDutyCycle(clampF(0, result / 100.0f, 1));

		return result;
	} else {
		efiAssert(OBD_PCM_Processor_Fault, m_output, "m_output null", 0);
		if (m_config->offBelowDuty > m_config->onAboveDuty) {
			firmwareError(CUSTOM_ERR_6122, "You can't have off below %d greater than on above %d",
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

void GppwmChannel::init(bool usePwm, IPwm* pwm, OutputPin* outputPin, const ValueProvider3D* table, const gppwm_channel* config) {
	m_usePwm = usePwm;
	m_pwm = pwm;
	m_output = outputPin;
	m_table = table;
	m_config = config;
}

percent_t GppwmChannel::getOutput() const {
	expected<float> loadAxisValue = readGppwmChannel(m_config->loadAxis);

	// If we couldn't get load axis value, fall back on error value
	if (!loadAxisValue) {
		return m_config->dutyIfError;
	}

	float rpm = Sensor::getOrZero(SensorType::Rpm);

	float result = m_table->getValue(rpm, loadAxisValue.Value);

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
	return setOutput(output);
}
