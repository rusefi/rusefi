#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "AemXSeriesLambda.h"

static constexpr uint32_t aem_base    = 0x180;
static constexpr uint32_t rusefi_base = 0x190;

AemXSeriesWideband::AemXSeriesWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0,	// ID passed here doesn't matter since we override acceptFrame
		type,
		MS2NT(21)	// sensor transmits at 100hz, allow a frame to be missed
	)
	, m_sensorIndex(sensorIndex)
{}

bool AemXSeriesWideband::acceptFrame(const CANRxFrame& frame) const {
	if (frame.DLC != 8) {
		return false;
	}
	
	uint32_t id = CAN_ID(frame);

	// 0th sensor is 0x180, 1st sensor is 0x181, etc
	uint32_t aemXSeriesId = aem_base + m_sensorIndex;

	// 0th sensor is 0x190 and 0x191, 1st sensor is 0x192 and 0x193
	uint32_t rusefiBaseId = rusefi_base + 2 * m_sensorIndex;

	return 
		id == aemXSeriesId ||
		id == rusefiBaseId ||
		id == rusefiBaseId + 1;
}

void AemXSeriesWideband::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	int32_t id = CAN_ID(frame);

	// accept frame has already guaranteed that this message belongs to us
	// We just have to check if it's AEM or rusEFI
	if (id < rusefi_base) {
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
}

void AemXSeriesWideband::decodeAemXSeries(const CANRxFrame& frame, efitick_t nowNt) {
	// reports in 0.0001 lambda per LSB
	uint16_t lambdaInt = SWAP_UINT16(frame.data16[0]);
	float lambdaFloat = 0.0001f * lambdaInt;

	// bit 6 indicates sensor fault
	bool sensorFault = frame.data8[7] & 0x40;
	if (sensorFault) {
		invalidate();
		return;
	}

	// bit 7 indicates valid
	bool valid = frame.data8[6] & 0x80;
	if (!valid) {
		invalidate();
		return;
	}

	setValidValue(lambdaFloat, nowNt);
}

#include "wideband_firmware/for_rusefi/wideband_can.h"

void AemXSeriesWideband::decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt) {
	auto data = reinterpret_cast<const wbo::StandardData*>(&frame.data8[0]);

	if (data->Version != RUSEFI_WIDEBAND_VERSION) {
		firmwareError(OBD_WB_FW_Mismatch, "Wideband controller index %d has wrong firmware version, please update!", m_sensorIndex);
		return;
	}

	tempC = data->TemperatureC;

	float lambda = 0.0001f * data->Lambda;
	bool valid = data->Valid != 0;

	if (valid) {
		setValidValue(lambda, nowNt);
	} else {
		invalidate();
	}
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

	faultCode = static_cast<uint8_t>(data->Status);
}

#endif
