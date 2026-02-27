#include "pch.h"
#include "ego.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "AemXSeriesLambda.h"
#include "wideband_firmware/for_rusefi/wideband_can.h"
#include "rusefi_wideband.h"

static constexpr uint32_t aem_base    = 0x180;
static constexpr uint32_t rusefi_base = WB_DATA_BASE_ADDR;
// sensor transmits at 100hz, allow a frame to be missed
static constexpr efidur_t sensor_timeout = MS2NT(3 * WBO_TX_PERIOD_MS);

AemXSeriesWideband::AemXSeriesWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0,	// ID passed here doesn't matter since we override acceptFrame
		type,
		sensor_timeout
	)
	, m_sensorIndex(sensorIndex)
{
    stateCode = m_stateCode = static_cast<uint8_t>(wbo::Status::CanSilent);// silent, initial state is "no one has spoken to us so far"
    isValid = m_isFault = m_afrIsValid = false;
    // wait for first rusEFI WBO standard frame with protocol version field
    fwUnsupported = true;
    // Do not light "FW: need update" indicator until we get some data from WBO
    fwOutdated = false;
}

can_wbo_type_e AemXSeriesWideband::sensorType() const {
	return engineConfiguration->canWbo[m_sensorIndex].type;
}

uint32_t AemXSeriesWideband::getReCanId() const {
	// 0th sensor is 0x190 and 0x191, 1st sensor is 0x192 and 0x193
	return rusefi_base + 2 * engineConfiguration->canWbo[m_sensorIndex].reId;
}

uint32_t AemXSeriesWideband::getAemCanId() const {
	// 0th sensor is 0x00000180, 1st sensor is 0x00000181, etc
	return aem_base + engineConfiguration->canWbo[m_sensorIndex].aemId;
}

bool AemXSeriesWideband::acceptFrame(const size_t busIndex, const CANRxFrame& frame) const {
#if !EFI_UNIT_TEST
	if (busIndex != getWidebandBus()) {
		return false;
	}
#endif

	if (frame.DLC != 8) {
		return false;
	}

	can_wbo_type_e type = sensorType();

	if (type == DISABLED) {
		return false;
	}

	// RusEFI wideband uses standard CAN IDs
	if ((!CAN_ISX(frame)) && (type == RUSEFI)) {
		// 0th sensor is 0x190 and 0x191, 1st sensor is 0x192 and 0x193
		uint32_t rusefiBaseId = getReCanId();
		return ((CAN_SID(frame) == rusefiBaseId) || (CAN_SID(frame) == rusefiBaseId + 1));
	}

	// AEM uses extended CAN ID
	if ((CAN_ISX(frame)) && (type == AEM)) {
		// 0th sensor is 0x00000180, 1st sensor is 0x00000181, etc
		uint32_t aemXSeriesId = getAemCanId();
		return (CAN_EID(frame) == aemXSeriesId);
	}

	return false;
}

bool AemXSeriesWideband::isHeaterAllowed() {
	return ((sensorType() == AEM) || (engine->engineState.heaterControlEnabled));
}

void AemXSeriesWideband::refreshState() {
	can_wbo_type_e type = sensorType();

	// Report ECU to WBO allow state
	allowed = isHeaterAllowed();
	if (getTimeNowNt() - sensor_timeout > m_lastUpdate) {
		canSilent = true;
		isValid = false;
		return;
	}
	canSilent = false;

	if (type == RUSEFI) {
		// This is RE WBO
		isValid = m_afrIsValid;
		// Report state code from WBO
		stateCode = m_stateCode;
		return;
	} else if (type == AEM) {
		// This is AEM with two flags only and no debug fields
		heaterDuty = 0;
		pumpDuty = 0;
		tempC = 0;
		nernstVoltage = 0;

		if (m_isFault) {
			stateCode = HACK_INVALID_AEM;
			isValid = false;
		} else {
			// TODO: better status code?
			stateCode = m_afrIsValid ?
				static_cast<uint8_t>(wbo::Status::RunningClosedLoop) :
				static_cast<uint8_t>(wbo::Status::Warmup);
			isValid = m_afrIsValid;
		}
		return;
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

	// non configured
	stateCode = static_cast<uint8_t>(wbo::Status::CanSilent);
}

void AemXSeriesWideband::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	bool accepted = false;
	// accept frame has already guaranteed that this message belongs to us
	// We just have to check if it's AEM or rusEFI
	if (sensorType() == RUSEFI) {
		uint32_t id = CAN_ID(frame);
		hasSeenRx = true;

		// rusEFI custom format
		if ((id & 0x1) != 0) {
			// low bit is set, this is the "diag" frame
			decodeRusefiDiag(frame);
		} else {
			// low bit not set, this is standard frame
			accepted = decodeRusefiStandard(frame, nowNt);
		}
	} else /* if (sensorType() == AEM) */ {
		accepted = decodeAemXSeries(frame, nowNt);
	}

	if (accepted) {
		m_lastUpdate = nowNt;
	}

	// Do refresh on each CAN packet
	refreshState();
}

/**
 * @return true if valid, false if invalid
 */
bool AemXSeriesWideband::decodeAemXSeries(const CANRxFrame& frame, efitick_t nowNt) {
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
	refreshSmoothedLambda(lambdaFloat);
	return true;
}

bool AemXSeriesWideband::decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt) {
	auto data = reinterpret_cast<const wbo::StandardData*>(&frame.data8[0]);

	if (data->Version > RUSEFI_WIDEBAND_VERSION) {
		firmwareError(ObdCode::OBD_WB_FW_Mismatch, "Wideband controller index %d has newer protocol version (0x%02x while 0x%02x supported), please update ECU FW!",
			m_sensorIndex, data->Version, RUSEFI_WIDEBAND_VERSION);
		fwUnsupported = true;
		return false;
	}

	if (data->Version < RUSEFI_WIDEBAND_VERSION_MIN) {
		firmwareError(ObdCode::OBD_WB_FW_Mismatch, "Wideband controller index %d has outdated protocol version (0x%02x while minimum 0x%02x expected), please update WBO!",
			m_sensorIndex, data->Version, RUSEFI_WIDEBAND_VERSION_MIN);
		fwUnsupported = true;
		return false;
	}

	fwUnsupported = false;
	// compatible, but not latest
	fwOutdated = (data->Version != RUSEFI_WIDEBAND_VERSION);
	// TODO: request and check builddate

	tempC = data->TemperatureC;
	float lambda = 0.0001f * data->Lambda;
	m_afrIsValid = data->Valid & 0x01;

	if (!m_afrIsValid) {
		invalidate();
	} else {
		setValidValue(lambda, nowNt);
		refreshSmoothedLambda(lambda);
	}

	return true;
}

void AemXSeriesWideband::refreshSmoothedLambda(float lambda) {
	switch (type()) {
	case SensorType::Lambda1: {
			expAverageLambda1.setSmoothingFactor(engineConfiguration->afrExpAverageAlpha);
			smoothedLambda1Sensor.setValidValue(expAverageLambda1.initOrAverage(lambda), getTimeNowNt());
			break;
	}
	case SensorType::Lambda2: {
			expAverageLambda2.setSmoothingFactor(engineConfiguration->afrExpAverageAlpha);
			smoothedLambda2Sensor.setValidValue(expAverageLambda2.initOrAverage(lambda), getTimeNowNt());
			break;
	}
	default:
		break;
	}
}

void AemXSeriesWideband::decodeRusefiDiag(const CANRxFrame& frame) {
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
    //    m_stateCode = HACK_CRANKING_VALUE;
    //    return;
    //}

	m_stateCode = static_cast<uint8_t>(data->status);

	if ((isStatusError(static_cast<wbo::Status>(data->status))) && isHeaterAllowed()) {
		auto code = m_sensorIndex == 0 ? ObdCode::Wideband_1_Fault : ObdCode::Wideband_2_Fault;
		warning(code, "Wideband #%d fault: %s", (m_sensorIndex + 1), wbo::describeStatus(data->status));
	}
}

#endif
