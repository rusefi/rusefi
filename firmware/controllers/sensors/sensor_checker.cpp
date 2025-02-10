#include "pch.h"

// Decode what OBD code we should use for a particular [sensor, code] problem
static ObdCode getCode(SensorType type, UnexpectedCode code) {
	switch (type) {
		case SensorType::Tps1:
		case SensorType::Tps1Primary:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_TPS1_Primary_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_TPS1_Primary_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_TPS1_Primary_High;
				case UnexpectedCode::Inconsistent: return ObdCode::OBD_TPS1_Correlation;
				default: break;
			} break;
		case SensorType::Tps1Secondary:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_TPS1_Secondary_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_TPS1_Secondary_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_TPS1_Secondary_High;
				default: break;
			} break;
		case SensorType::Tps2:
		case SensorType::Tps2Primary:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_TPS2_Primary_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_TPS2_Primary_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_TPS2_Primary_High;
				case UnexpectedCode::Inconsistent: return ObdCode::OBD_TPS2_Correlation;
				default: break;
			} break;
		case SensorType::Tps2Secondary:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_TPS2_Secondary_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_TPS2_Secondary_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_TPS2_Secondary_High;
				default: break;
			} break;

		case SensorType::AcceleratorPedal:
		case SensorType::AcceleratorPedalPrimary:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_PPS_Primary_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_PPS_Primary_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_PPS_Primary_High;
				case UnexpectedCode::Inconsistent: return ObdCode::OBD_PPS_Correlation;
				default: break;
			} break;
		case SensorType::AcceleratorPedalSecondary:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_PPS_Secondary_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_PPS_Secondary_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_PPS_Secondary_High;
				default: break;
			} break;

		case SensorType::Map:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_Map_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_Map_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_Map_High;
				default: break;
			} break;
		case SensorType::Clt:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_Clt_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_Clt_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_Clt_High;
				default: break;
			} break;
		case SensorType::Iat:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_Iat_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_Iat_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_Iat_High;
				default: break;
			} break;
		case SensorType::FuelEthanolPercent:
			switch (code) {
				case UnexpectedCode::Timeout:      return ObdCode::OBD_FlexSensor_Timeout;
				case UnexpectedCode::Low:          return ObdCode::OBD_FlexSensor_Low;
				case UnexpectedCode::High:         return ObdCode::OBD_FlexSensor_High;
				default: break;
			} break;
		default:
			break;
	}

	return ObdCode::None;
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

	ObdCode code = getCode(type, result.Code);

	if (code != ObdCode::None) {
		warning(code, "Sensor fault: %s %s", Sensor::getSensorName(type), describeUnexpected(result.Code));
	}
}

#if BOARD_EXT_GPIOCHIPS > 0 && EFI_PROD_CODE
#if EFI_ENGINE_CONTROL
static ObdCode getCodeForInjector(int idx, brain_pin_diag_e diag) {
	if (idx < 0 || idx >= MAX_CYLINDER_COUNT) {
		return ObdCode::None;
	}

	if ((diag & PIN_OPEN) || (diag & PIN_SHORT_TO_GND)) {
		return (ObdCode)((int)ObdCode::OBD_Injector_Circuit_1_Low + (idx * 3));
	} else if ((diag & PIN_SHORT_TO_BAT) || (diag & PIN_OVERLOAD)) {
		return (ObdCode)((int)ObdCode::OBD_Injector_Circuit_1_High + (idx * 3));
	}

	/* else common error code */
	return (ObdCode)((int)ObdCode::OBD_Injector_Circuit_1 + idx);
}
#endif // EFI_ENGINE_CONTROL

static ObdCode getCodeForIgnition(int idx, brain_pin_diag_e diag) {
	if (idx < 0 || idx >= MAX_CYLINDER_COUNT) {
		return ObdCode::None;
	}

	// TODO: do something more intelligent with `diag`?
	UNUSED(diag);

	return (ObdCode)((int)ObdCode::OBD_Ignition_Circuit_1 + idx);
}

static uint8_t getTSErrorCode(brain_pin_diag_e diag)
{
	/* Error codes reported to TS:
	 *  0 - output is not used
	 *  1 - ok status/no diagnostic available (TODO: separate codes)
	 * >1 - see brain_pin_diag_e, first least significant 1-bit position + 1 *
	 * Keep in sync with outputDiagErrorList in tunerstudio.template.ini
	 * Note:
	 * diag can be combination of few errors,
	 * while we report only one error to simplify hadling on TS side
	 * find position of least significant 1-bit */
	return __builtin_ffs(diag) + 1;
}
#endif // BOARD_EXT_GPIOCHIPS > 0 && EFI_PROD_CODE

PUBLIC_API_WEAK void boardSensorChecker() {
}

void SensorChecker::onSlowCallback() {
	// Don't check when the ignition is off, or when it was just turned on (let things stabilize)
	// TODO: also inhibit checking if we just did a flash burn, since that blocks the ECU for a few seconds.
	bool shouldCheck = m_ignitionIsOn && m_timeSinceIgnOff.hasElapsedSec(5);
	m_analogSensorsShouldWork = shouldCheck;
	if (!shouldCheck) {
		return;
	}

	// Check sensors
	check(SensorType::Tps1Primary);
	check(SensorType::Tps1Secondary);
	check(SensorType::Tps1);
	check(SensorType::Tps2Primary);
	check(SensorType::Tps2Secondary);
	check(SensorType::Tps2);

	check(SensorType::AcceleratorPedalPrimary);
	check(SensorType::AcceleratorPedalSecondary);
	check(SensorType::AcceleratorPedal);

	check(SensorType::Map);
	check(SensorType::Map2);

	check(SensorType::Clt);
	check(SensorType::Iat);

	check(SensorType::FuelEthanolPercent);

#if EFI_PROD_CODE
	TunerStudioOutputChannels *state = getTunerStudioOutputChannels();
	// only bother checking these if we have GPIO chips actually capable of reporting an error
#if BOARD_EXT_GPIOCHIPS > 0
#if EFI_ENGINE_CONTROL
	// Check injectors
	int unhappyInjector = 0;
	for (size_t i = 0; i < efi::size(enginePins.injectors); i++) {
		InjectorOutputPin& pin = enginePins.injectors[i];

		// Skip not-configured pins
		if (!isBrainPinValid(pin.brainPin)) {
			state->injectorDiagnostic[i] = 0;
			continue;
		}

		auto diag = pin.getDiag();
		if (diag != PIN_OK && diag != PIN_UNKNOWN) {
		    unhappyInjector = 1 + i;
			auto code = getCodeForInjector(i, diag);

			char description[32];
			pinDiag2string(description, efi::size(description), diag);
			warning(code, "Injector %d fault: %s", i + 1, description);
		}
		state->injectorDiagnostic[i] = getTSErrorCode(diag);
	}
	engine->fuelComputer.brokenInjector = unhappyInjector;
	engine->fuelComputer.injectorHwIssue = (unhappyInjector != 0);
#endif // EFI_ENGINE_CONTROL

	// Check ignition
	for (size_t i = 0; i < efi::size(enginePins.injectors); i++) {
		IgnitionOutputPin& pin = enginePins.coils[i];

		// Skip not-configured pins
		if (!isBrainPinValid(pin.brainPin)) {
			state->ignitorDiagnostic[i] = 0;
			continue;
		}

		auto diag = pin.getDiag();
		if (diag != PIN_OK && diag != PIN_UNKNOWN) {
			auto code = getCodeForIgnition(i, diag);

			char description[32];
			pinDiag2string(description, efi::size(description), diag);
			warning(code, "Ignition %d fault: %s", i + 1, description);
		}
		state->ignitorDiagnostic[i] = getTSErrorCode(diag);
	}
#endif // BOARD_EXT_GPIOCHIPS > 0

	// Check ADC(s) and analog inputs
	if (analogGetDiagnostic() < 0) {
		/* TODO: map to more OBD codes? */
		warning(ObdCode::OBD_Sensor_Refence_Voltate_A_Open, "Analog subsystem fault");
		state->isAnalogFailure = true;
	} else {
		state->isAnalogFailure = false;
	}
#endif // EFI_PROD_CODE

  boardSensorChecker();
}

void SensorChecker::onIgnitionStateChanged(bool ignitionOn) {
	m_ignitionIsOn = ignitionOn;

	if (!ignitionOn) {
		// timer keeps track of how long since the state was turned to on (ie, how long ago was it last off)
		m_timeSinceIgnOff.reset();
	}
}
