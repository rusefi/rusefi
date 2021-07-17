
#if EFI_CAN_SUPPORT
#include "AemXSeriesLambda.h"
#include "efilib.h"

#if EFI_TUNER_STUDIO
#endif

AemXSeriesWideband::AemXSeriesWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0x180 + sensorIndex,	// 0th sensor is 0x180, others sequential above that
		type,
		MS2NT(21)	// sensor transmits at 100hz, allow a frame to be missed
	)
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
	if (isRusefiController && CONFIG(debugMode) == DBG_RUSEFI_WIDEBAND) {
		float pumpDuty = frame.data8[2] / 255.0f;
		float sensorEsr = frame.data8[3] * 4;
		float nernstVoltage = frame.data8[4] / 200.0f;

		tsOutputChannels.debugFloatField1 = pumpDuty;
		tsOutputChannels.debugFloatField2 = sensorEsr;
		tsOutputChannels.debugFloatField3 = nernstVoltage;
		tsOutputChannels.debugFloatField4 = lambdaFloat;
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
