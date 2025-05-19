#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "AemXSeriesLambda.h"
#include "wideband_firmware/for_rusefi/wideband_can.h"

static constexpr uint32_t aem_base    = 0x180;
static constexpr uint32_t rusefi_base = WB_DATA_BASE_ADDR;

AemXSeriesWideband::AemXSeriesWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0,	// ID passed here doesn't matter since we override acceptFrame
		type,
		MS2NT(3 * WBO_TX_PERIOD_MS)	// sensor transmits at 100hz, allow a frame to be missed
	)
	, m_sensorIndex(sensorIndex)
{
    faultCode = m_faultCode = HACK_SILENT_VALUE;// silent, initial state is "no one has spoken to us so far"
    m_isValid = m_isFault = m_isAem = false;
}

static can_wbo_type_e getCanWboSensorType(size_t index)
{
	return index ? engineConfiguration->wboType2 : engineConfiguration->wboType1;
}

bool AemXSeriesWideband::acceptFrame(const CANRxFrame& frame) const {
	if (frame.DLC != 8) {
		return false;
	}

	can_wbo_type_e type = getCanWboSensorType(m_sensorIndex);

	if (type == DISABLED) {
		return false;
	}

	uint32_t id = CAN_ID(frame);

	// 0th sensor is 0x180, 1st sensor is 0x181, etc
	uint32_t aemXSeriesId = aem_base + m_sensorIndex;

	// 0th sensor is 0x190 and 0x191, 1st sensor is 0x192 and 0x193
	uint32_t rusefiBaseId = rusefi_base + 2 * (engineConfiguration->flipWboChannels ? (1 - m_sensorIndex) : m_sensorIndex);

	if (((type == AUTODETECT) || (type == RUSEFI))
		((id == rusefiBaseId) || (id == rusefiBaseId + 1))) {
		return true;
	}

	if (((type == AUTODETECT) || (type == AEM)) &&
		(id == aemXSeriesId)) {
		return true;
	}

	return false;
}

bool AemXSeriesWideband::isHeaterAllowed() {
	return ((m_isAem) || (engine->engineState.heaterControlEnabled));
}

void AemXSeriesWideband::refreshState() {
	if (!isHeaterAllowed()) {
		// Do not check for any errors while heater is not allowed
		faultCode = HACK_CRANKING_VALUE;
		return;
	}

	if (!m_isAem) {
		// This is RE WBO
		if (m_faultCode != static_cast<uint8_t>(wbo::Fault::None)) {
			// Report error code from WBO
			faultCode = m_faultCode;
		} else {
			// No fault code reported from WBO
			auto value = get();
			if ((!value) && (value.Code == UnexpectedCode::Timeout)) {
				faultCode = HACK_SILENT_VALUE;
			} else if (!m_isValid) {
				// But no valid AFR too
				faultCode = HACK_INVALID_RE;
			} else {
				faultCode = static_cast<uint8_t>(wbo::Fault::None);
			}
		}
	} else {
		// This is AEM with two flags only
		if (m_isFault) {
			// TODO:
			faultCode = HACK_INVALID_AEM;
			return;
		}
		if (!m_isValid) {
			faultCode = HACK_INVALID_AEM;
			return;
		}
		faultCode = HACK_VALID_AEM;

		// .. and no debug fields
		heaterDuty = 0;
		pumpDuty = 0;
		nernstVoltage = 0;
	}
}

void AemXSeriesWideband::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	uint32_t id = CAN_ID(frame);

	// accept frame has already guaranteed that this message belongs to us
	// We just have to check if it's AEM or rusEFI
	m_isAem = (id < rusefi_base);

	if (m_isAem) {
		decodeAemXSeries(frame, nowNt);
	} else {
		// rusEFI custom format
		if ((id & 0x1) != 0) {
			// low bit is set, this is the "diag" frame
			decodeRusefiDiag(frame);
		} else {
			// low bit not set, this is standard frame
			decodeRusefiStandard(frame, nowNt);
		}
	}

	// Do refresh on each CAN packet
	refreshState();
}

/**
 * @return true if valid, false if invalid
 */
bool AemXSeriesWideband::decodeAemXSeries(const CANRxFrame& frame, efitick_t nowNt) {
	// reports in 0.0001 lambda per LSB
	uint16_t lambdaInt = SWAP_UINT16(frame.data16[0]);
	float lambdaFloat = 0.0001f * lambdaInt;

	// bit 6 indicates sensor fault
	m_isFault = frame.data8[7] & 0x40;
	// bit 7 indicates valid
	m_isValid = frame.data8[6] & 0x80;

	if ((m_isFault) || (!m_isValid)) {
		invalidate();
		return false;
	}

	setValidValue(lambdaFloat, nowNt);
	return true;
}

void AemXSeriesWideband::decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt) {
	auto data = reinterpret_cast<const wbo::StandardData*>(&frame.data8[0]);

	if (data->Version != RUSEFI_WIDEBAND_VERSION) {
		firmwareError(ObdCode::OBD_WB_FW_Mismatch, "Wideband controller index %d has wrong protocol version (0x%02x while 0x%02x expected), please update!",
			m_sensorIndex, data->Version, RUSEFI_WIDEBAND_VERSION);
		return;
	}

	tempC = data->TemperatureC;
	float lambda = 0.0001f * data->Lambda;
	m_isValid = data->Valid != 0;

	if (!m_isValid) {
		invalidate();
		return;
	}

	setValidValue(lambda, nowNt);
}

void AemXSeriesWideband::decodeRusefiDiag(const CANRxFrame& frame) {
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
