#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "CanWideband.h"
#include "HaltechWideband.h"
#include "wideband_firmware/for_rusefi/wideband_can.h"

static constexpr uint32_t aem_base    = 0x180;
static constexpr uint32_t rusefi_base = WB_DATA_BASE_ADDR;

CanWideband::CanWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0,	// ID passed here doesn't matter since we override acceptFrame
		type,
		MS2NT(3 * WBO_TX_PERIOD_MS)	// sensor transmits at 100hz, allow a frame to be missed
	)
	, m_sensorIndex(sensorIndex)
{
    faultCode = m_faultCode = static_cast<uint8_t>(wbo::Fault::CanSilent);// silent, initial state is "no one has spoken to us so far"
    isValid = m_isFault = m_afrIsValid = false;
    // wait for first rusEFI WBO standard frame with protocol version field
    fwUnsupported = true;
    fwOutdated = true;
}

can_wbo_type_e CanWideband::sensorType() const {
	return m_sensorIndex ? engineConfiguration->wboType2 : engineConfiguration->wboType1;
}

bool CanWideband::acceptFrame(const CANRxFrame& frame) const {
	if (frame.DLC != 8) {
		return false;
	}

	can_wbo_type_e type = sensorType();

	if (type == DISABLED) {
		return false;
	}

	if ((!CAN_ISX(frame)) && (type == HALTECH)) {
		return CAN_SID(frame) == Haltech::WboChannels::CHA;
	}

	// RusEFI wideband uses standard CAN IDs
	if ((!CAN_ISX(frame)) && (type == RUSEFI)) {
		// 0th sensor is 0x190 and 0x191, 1st sensor is 0x192 and 0x193
		uint32_t rusefiBaseId = rusefi_base + 2 * (engineConfiguration->flipWboChannels ? (1 - m_sensorIndex) : m_sensorIndex);
		return ((CAN_SID(frame) == rusefiBaseId) || (CAN_SID(frame) == rusefiBaseId + 1));
	}

	// AEM uses extended CAN ID
	if ((CAN_ISX(frame)) && (type == AEM)) {
		// 0th sensor is 0x00000180, 1st sensor is 0x00000181, etc
		uint32_t aemXSeriesId = aem_base + m_sensorIndex;
		return (CAN_EID(frame) == aemXSeriesId);
	}

	return false;
}

bool CanWideband::isHeaterAllowed() {
	return sensorType() != HALTECH && ((sensorType() == AEM) || (engine->engineState.heaterControlEnabled));
}

void CanWideband::refreshState() {
	can_wbo_type_e type = sensorType();

	if ((type == RUSEFI) && (!isHeaterAllowed())) {
		faultCode = static_cast<uint8_t>(wbo::Fault::NotAllowed);
		isValid = false;
		// Do not check for any errors while heater is not allowed
		return;
	}

	// Communication timeout is priority error code
	auto value = get();
	if ((!value) && (value.Code == UnexpectedCode::Timeout)) {
		faultCode = static_cast<uint8_t>(wbo::Fault::CanSilent);
		isValid = false;
		return;
	}

	if (type == RUSEFI) {
		// This is RE WBO
		isValid = m_afrIsValid;
		if (m_faultCode != static_cast<uint8_t>(wbo::Fault::None)) {
			// Report error code from WBO
			faultCode = m_faultCode;
			isValid = false;
			return;
		}
	} else if (type == AEM) {
		// This is AEM with two flags only and no debug fields
		heaterDuty = 0;
		pumpDuty = 0;
		tempC = 0;
		nernstVoltage = 0;

		isValid = m_afrIsValid;
		if (m_isFault) {
			faultCode = HACK_INVALID_AEM;
			isValid = false;
			return;
		}
	} else if (type == HALTECH) {
		// TODO implement status handling using Haltech::WboStatus enum from Haltech::decodeStatus
		heaterDuty = 0;
		pumpDuty = 0;
		tempC = 0;
		nernstVoltage = 0;

		isValid = m_afrIsValid;
		if (m_isFault) {
			isValid = false;
			return;
		}
	} else {
		// disabled or analog
		// clear all livedata
		heaterDuty = 0;
		pumpDuty = 0;
		tempC = 0;
		nernstVoltage = 0;
		isValid = false;
		return;
	}

	faultCode = static_cast<uint8_t>(wbo::Fault::None);
}

void CanWideband::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	// accept frame has already guaranteed that this message belongs to us
	// We just have to check if it's AEM or rusEFI
	if (sensorType() == HALTECH) {
		decodeHaltech(frame, nowNt);
	} else if (sensorType() == RUSEFI) {
		uint32_t id = CAN_ID(frame);

		// rusEFI custom format
		if ((id & 0x1) != 0) {
			// low bit is set, this is the "diag" frame
			decodeRusefiDiag(frame);
		} else {
			// low bit not set, this is standard frame
			decodeRusefiStandard(frame, nowNt);
		}
	} else /* if (sensorType() == AEM) */ {
		decodeAemXSeries(frame, nowNt);
	}

	// Do refresh on each CAN packet
	refreshState();
}

/**
 * @return true if valid, false if invalid
 */
bool CanWideband::decodeAemXSeries(const CANRxFrame& frame, efitick_t nowNt) {
	// we don't care
	fwUnsupported = false;
	fwOutdated = false;

	// reports in 0.0001 lambda per LSB
	uint16_t lambdaInt = SWAP_UINT16(frame.data16[0]);
	float lambdaFloat = 0.0001f * lambdaInt;

	// bit 6 indicates sensor fault
	m_isFault = frame.data8[7] & 0x40;
	// bit 7 indicates valid
	m_afrIsValid = frame.data8[6] & 0x80;

	if ((m_isFault) || (!m_afrIsValid)) {
		invalidate();
		return false;
	}

	setValidValue(lambdaFloat, nowNt);
	return true;
}

void CanWideband::decodeHaltech(const CANRxFrame& frame, efitick_t nowNt) {
	fwUnsupported = false;
	fwOutdated = false;

	auto& data = *reinterpret_cast<const Haltech::DataFrame*>(&frame.data8[0]);

	tempC = 0;
	float lambda = Haltech::getLambda(data);

	m_afrIsValid =  Haltech::decodeStatus(data) == Haltech::WboStatus::NoError;
	m_isFault = !m_afrIsValid;

	if (!m_afrIsValid) {
		invalidate();
		return;
	}

	setValidValue(lambda, nowNt);
}

void CanWideband::decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt) {
	auto data = reinterpret_cast<const wbo::StandardData*>(&frame.data8[0]);

	if (data->Version > RUSEFI_WIDEBAND_VERSION) {
		firmwareError(ObdCode::OBD_WB_FW_Mismatch, "Wideband controller index %d has newer protocol version (0x%02x while 0x%02x supported), please update ECU FW!",
			m_sensorIndex, data->Version, RUSEFI_WIDEBAND_VERSION);
		fwUnsupported = true;
		return;
	}

	if (data->Version < RUSEFI_WIDEBAND_VERSION_MIN) {
		firmwareError(ObdCode::OBD_WB_FW_Mismatch, "Wideband controller index %d has outdated protocol version (0x%02x while minimum 0x%02x expected), please update WBO!",
			m_sensorIndex, data->Version, RUSEFI_WIDEBAND_VERSION_MIN);
		fwUnsupported = true;
		return;
	}

	fwUnsupported = false;
	// compatible, but not latest
	fwOutdated = (data->Version != RUSEFI_WIDEBAND_VERSION_MIN);
	// TODO: request and check builddate

	tempC = data->TemperatureC;
	float lambda = 0.0001f * data->Lambda;
	m_afrIsValid = data->Valid & 0x01;

	if (!m_afrIsValid) {
		invalidate();
		return;
	}

	setValidValue(lambda, nowNt);
}

void CanWideband::decodeRusefiDiag(const CANRxFrame& frame) {
	if (fwUnsupported) {
		return;
	}

	auto data = reinterpret_cast<const wbo::DiagData*>(&frame.data8[0]);

	// convert to percent
	heaterDuty = data->HeaterDuty / 2.55f;
	pumpDuty = data->PumpDuty / 2.55f;

	// convert to volts
	nernstVoltage = data->NernstDc * 0.001f;

	// no conversion, just ohms
	esr = data->Esr;

	// This state is handle in refreshState()
    //if (!isHeaterAllowed()) {
    //    m_faultCode = HACK_CRANKING_VALUE;
    //    return;
    //}

	m_faultCode = static_cast<uint8_t>(data->Status);

	if ((data->Status != wbo::Fault::None) && isHeaterAllowed()) {
		auto code = m_sensorIndex == 0 ? ObdCode::Wideband_1_Fault : ObdCode::Wideband_2_Fault;
		warning(code, "Wideband #%d fault: %s", (m_sensorIndex + 1), wbo::describeFault(data->Status));
	}
}

#endif
