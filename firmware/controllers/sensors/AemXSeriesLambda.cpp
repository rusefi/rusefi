#include "pch.h"

#if EFI_CAN_SUPPORT
#include "AemXSeriesLambda.h"

static constexpr uint32_t aem_base    = 0x180;
static constexpr uint32_t rusefi_base = 0x190;

AemXSeriesWideband::AemXSeriesWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		aem_base + sensorIndex,	// 0th sensor is 0x180, others sequential above that
		type,
		MS2NT(21)	// sensor transmits at 100hz, allow a frame to be missed
	)
	, m_sensorIndex(sensorIndex)
{}

bool AemXSeriesWideband::acceptFrame(const CANRxFrame& frame) const {
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
	if (frame.DLC != 8) {
		invalidate();
		return;
	}

	int32_t id = CAN_ID(frame);

	// accept frame has already checked if the message belongs to
	// this sensor index to us, we just have to check if it's AEM or rusEFI
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

	// This bit is a reserved bit on AEM - but is set on rusEfi's controller
	bool isRusefiController = frame.data8[7] & 0x80;

#if EFI_TUNER_STUDIO
	// rusEfi controller sends some extra diagnostic data about its internal workings
	if (isRusefiController && engineConfiguration->debugMode == DBG_RUSEFI_WIDEBAND) {
		float pumpDuty = frame.data8[2] / 255.0f;
		float nernstVoltage = frame.data8[4] / 200.0f;

		engine->outputChannels.debugFloatField1 = pumpDuty;
		engine->outputChannels.debugFloatField3 = nernstVoltage;
	}

	if (isRusefiController) {
		float wbEsr = frame.data8[3] * 4;

		// TODO: convert ESR to temperature
		engine->outputChannels.wbTemperature[m_sensorIndex] = wbEsr;

		// TODO: decode heater duty
		engine->outputChannels.wbHeaterDuty[m_sensorIndex] = 0;
	}
#endif

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

// TODO: include rusEFI wideband file directly
namespace wbo
{
struct StandardData
{
    uint8_t Version;
    uint8_t Valid;

    uint16_t Lambda;
    uint16_t TemperatureC;

    uint16_t pad;
};

struct DiagData
{
    uint16_t Esr;
    uint16_t NernstDc;
    uint8_t PumpDuty;
    uint8_t Status;

    uint16_t pad;
};
} // namespace wbo

void AemXSeriesWideband::decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt) {
	auto data = reinterpret_cast<const wbo::StandardData*>(&frame.data8[0]);

	// TODO: enforce version check
	//bool versionValid = data->Version != RUSEFI_WIDEBAND_VERSION;

	float lambda = 0.0001f * data->Lambda;
	engine->outputChannels.wbTemperature[m_sensorIndex] = data->TemperatureC;

	bool valid = data->Valid != 0;

	if (valid) {
		setValidValue(lambda, nowNt);
	} else {
		invalidate();
		return;
	}
}

void AemXSeriesWideband::decodeRusefiDiag(const CANRxFrame& frame) {
	auto data = reinterpret_cast<const wbo::DiagData*>(&frame.data8[0]);

	if (m_sensorIndex == 0 || engineConfiguration->debugMode == DBG_RUSEFI_WIDEBAND) {
		engine->outputChannels.debugFloatField1 = data->PumpDuty / 255.0f;
		engine->outputChannels.debugFloatField3 = data->NernstDc / 1000.0f;
	}
}

#endif
