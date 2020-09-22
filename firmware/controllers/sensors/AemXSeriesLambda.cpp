#include "global.h"

#if EFI_CAN_SUPPORT
#include "AemXSeriesLambda.h"
#include "efilib.h"

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

	// reports in 0.0001 lambda per LSB
	uint16_t lambdaInt = SWAP_UINT16(frame.data16[0]);
	setValidValue(0.0001f * lambdaInt, nowNt);
}

#endif
