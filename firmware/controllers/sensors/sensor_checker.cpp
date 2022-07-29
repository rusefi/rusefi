#include "pch.h"

// Decode what OBD code we should use for a particular [sensor, code] problem
static obd_code_e getCode(SensorType type, UnexpectedCode code) {
	switch (type) {
		case SensorType::Tps1:
		case SensorType::Tps1Primary:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_TPS1_Primary_Timeout;
				case UnexpectedCode::Low:          return OBD_TPS1_Primary_Low;
				case UnexpectedCode::High:         return OBD_TPS1_Primary_High;
				case UnexpectedCode::Inconsistent: return OBD_TPS1_Correlation;
				default: break;
			} break;
		case SensorType::Tps1Secondary:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_TPS1_Secondary_Timeout;
				case UnexpectedCode::Low:          return OBD_TPS1_Secondary_Low;
				case UnexpectedCode::High:         return OBD_TPS1_Secondary_High;
				default: break;
			} break;
		case SensorType::Tps2:
		case SensorType::Tps2Primary:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_TPS2_Primary_Timeout;
				case UnexpectedCode::Low:          return OBD_TPS2_Primary_Low;
				case UnexpectedCode::High:         return OBD_TPS2_Primary_High;
				case UnexpectedCode::Inconsistent: return OBD_TPS2_Correlation;
				default: break;
			} break;
		case SensorType::Tps2Secondary:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_TPS2_Secondary_Timeout;
				case UnexpectedCode::Low:          return OBD_TPS2_Secondary_Low;
				case UnexpectedCode::High:         return OBD_TPS2_Secondary_High;
				default: break;
			} break;

		case SensorType::AcceleratorPedal:
		case SensorType::AcceleratorPedalPrimary:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_PPS_Primary_Timeout;
				case UnexpectedCode::Low:          return OBD_PPS_Primary_Low;
				case UnexpectedCode::High:         return OBD_PPS_Primary_High;
				case UnexpectedCode::Inconsistent: return OBD_PPS_Correlation;
				default: break;
			} break;
		case SensorType::AcceleratorPedalSecondary:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_PPS_Secondary_Timeout;
				case UnexpectedCode::Low:          return OBD_PPS_Secondary_Low;
				case UnexpectedCode::High:         return OBD_PPS_Secondary_High;
				default: break;
			} break;

		case SensorType::Map:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_Map_Timeout;
				case UnexpectedCode::Low:          return OBD_Map_Low;
				case UnexpectedCode::High:         return OBD_Map_High;
				default: break;
			} break;
		case SensorType::Clt:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_Clt_Timeout;
				case UnexpectedCode::Low:          return OBD_Clt_Low;
				case UnexpectedCode::High:         return OBD_Clt_High;
				default: break;
			} break;
		case SensorType::Iat:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_Iat_Timeout;
				case UnexpectedCode::Low:          return OBD_Iat_Low;
				case UnexpectedCode::High:         return OBD_Iat_High;
				default: break;
			} break;
		case SensorType::FuelEthanolPercent:
			switch (code) {
				case UnexpectedCode::Timeout:      return OBD_FlexSensor_Timeout;
				case UnexpectedCode::Low:          return OBD_FlexSensor_Low;
				case UnexpectedCode::High:         return OBD_FlexSensor_High;
				default: break;
			} break;
		default:
			break;
	}

	return OBD_None;
}

inline const char* describeUnexpected(UnexpectedCode code) {
	switch (code) {
		case UnexpectedCode::Timeout: return "has timed out";
		case UnexpectedCode::High: return "input too high";
		case UnexpectedCode::Low: return "input too low";
		case UnexpectedCode::Inconsistent: return "is inconsistent";
		case UnexpectedCode::Configuration: return "is misconfigured";
		case UnexpectedCode::Unknown:
		default:
			return "unknown";
	}
}

static void check(SensorType type) {
	// Don't check sensors we don't have
	if (!Sensor::hasSensor(type)) {
		return;
	}

	auto result = Sensor::get(type);

	// If the sensor is OK, nothing to check.
	if (result) {
		return;
	}

	obd_code_e code = getCode(type, result.Code);

	if (code != OBD_None) {
		warning(code, "Sensor fault: %s %s", Sensor::getSensorName(type), describeUnexpected(result.Code));
	}
}

void SensorChecker::onSlowCallback() {
	// Don't check when the ignition is off, or when it was just turned on (let things stabilize)
	// TODO: also inhibit checking if we just did a flash burn, since that blocks the ECU for a few seconds.
	if (!m_ignitionIsOn || !m_timeSinceIgnOff.hasElapsedSec(5)) {
		return;
	}

	check(SensorType::Tps1Primary);
	check(SensorType::Tps1Secondary);
	check(SensorType::Tps1);
	check(SensorType::Tps2Primary);
	check(SensorType::Tps2Secondary);
	check(SensorType::Tps2);

	check(SensorType::Tps2);
	check(SensorType::Tps2Primary);
	check(SensorType::Tps2Secondary);

	check(SensorType::Map);

	check(SensorType::Clt);
	check(SensorType::Iat);

	check(SensorType::FuelEthanolPercent);
}

void SensorChecker::onIgnitionStateChanged(bool ignitionOn) {
	m_ignitionIsOn = ignitionOn;

	if (!ignitionOn) {
		// timer keeps track of how long since the state was turned to on (ie, how long ago was it last off)
		m_timeSinceIgnOff.reset();
	}
}
