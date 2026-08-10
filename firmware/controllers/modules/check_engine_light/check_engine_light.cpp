#include "pch.h"

#include "check_engine_light.h"
#include "malfunction_central.h"

static constexpr float CelDebounceSeconds = 1.0f;

void CheckEngineLight::setDefaultConfiguration() {
	config->cel_battery_min_v = 6.0f;
	config->cel_battery_max_v = 18.0f;

	config->cel_map_min_v = 0.2f;
	config->cel_map_max_v = 4.8f;
	config->cel_iat_min_v = 0.2f;
	config->cel_iat_max_v = 4.8f;
	config->cel_tps_min_v = 0.2f;
	config->cel_tps_max_v = 4.8f;
}

void CheckEngineLight::updateRange(RangeState& state, bool available, float value, float minimum, float maximum,
		ObdCode lowCode, ObdCode highCode) {
	ObdCode candidate = ObdCode::None;

	if (available && minimum < maximum) {
		if (value < minimum) {
			candidate = lowCode;
		} else if (value > maximum) {
			candidate = highCode;
		}
	}

	if (candidate == state.activeCode) {
		state.pendingCode = state.activeCode;
		state.pendingTimer.init();

		if (candidate != ObdCode::None) {
			addError(candidate);
		}
		return;
	}

	if (candidate != state.pendingCode) {
		state.pendingCode = candidate;
		state.pendingTimer.reset();
		return;
	}

	if (!state.pendingTimer.hasElapsedSec(CelDebounceSeconds)) {
		return;
	}

	if (state.activeCode != ObdCode::None) {
		removeError(state.activeCode);
	}

	state.activeCode = candidate;
	if (state.activeCode != ObdCode::None) {
		addError(state.activeCode);
	}
}

void CheckEngineLight::clearCurrentFaults() {
	RangeState* states[] = { &m_battery, &m_map, &m_iat, &m_tps };

	for (auto state : states) {
		if (state->activeCode != ObdCode::None) {
			removeError(state->activeCode);
		}

		*state = {};
	}
}

static bool getRawVoltage(SensorType type, float& voltage) {
	const Sensor* sensor = Sensor::getSensorOfType(type);
	if (!sensor || !sensor->hasSensor() || !sensor->hasRaw()) {
		return false;
	}

	auto result = sensor->get();
	if (!result.Valid && result.Code != UnexpectedCode::Low && result.Code != UnexpectedCode::High) {
		return false;
	}

	voltage = sensor->getRaw();
	return true;
}

void CheckEngineLight::onSlowCallback() {
	if (!engine->module<SensorChecker>()->analogSensorsShouldWork()) {
		clearCurrentFaults();
		return;
	}

	auto battery = Sensor::get(SensorType::BatteryVoltage);
	updateRange(m_battery, battery.Valid, battery.Value,
		config->cel_battery_min_v, config->cel_battery_max_v,
		ObdCode::OBD_System_Voltage_Low, ObdCode::OBD_System_Voltage_Malfunction);

	float voltage = 0;
	bool available = getRawVoltage(SensorType::MapSlow, voltage);
	updateRange(m_map, available, voltage, config->cel_map_min_v, config->cel_map_max_v,
		ObdCode::OBD_Map_Low, ObdCode::OBD_Map_High);

	available = getRawVoltage(SensorType::Iat, voltage);
	updateRange(m_iat, available, voltage, config->cel_iat_min_v, config->cel_iat_max_v,
		ObdCode::OBD_Iat_Low, ObdCode::OBD_Iat_High);

	available = getRawVoltage(SensorType::Tps1Primary, voltage);
	updateRange(m_tps, available, voltage, config->cel_tps_min_v, config->cel_tps_max_v,
		ObdCode::OBD_TPS1_Primary_Low, ObdCode::OBD_TPS1_Primary_High);
}
