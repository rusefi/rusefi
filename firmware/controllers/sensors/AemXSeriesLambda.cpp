#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "AemXSeriesLambda.h"

AemXSeriesWideband::AemXSeriesWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0x180 + sensorIndex,	// 0th sensor is 0x180, others sequential above that
		type,
		MS2NT(21)	// sensor transmits at 100hz, allow a frame to be missed
	)
	, m_sensorIndex(sensorIndex)
{}

void AemXSeriesWideband::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	if (frame.DLC != 8) {
		invalidate();
		return;
	}

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

#endif
