#include "global.h"
#include "AemXSeriesLambda.h"
#include "efilib.h"

AemXSeriesWideband::AemXSeriesWideband(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0x180 + sensorIndex,	// 0th sensor is 0x180, others sequential above that
		type,
		MS2NT(21)	// sensor transmits at 100hz, allow a frame to be missed
	)
{}

SensorResult AemXSeriesWideband::decodeFrame(const CANRxFrame& frame) {
	if (frame.DLC != 8) {
		return unexpected;
	}

	// bit 6 indicates sensor fault
	bool sensorFault = frame.data8[7] & 0x40;
	if (sensorFault) {
		return unexpected;
	}

	// bit 7 indicates valid
	bool valid = frame.data8[6] & 0x80;
	if (!valid) {
		return unexpected;
	}

	// reports in 0.0001 lambda per LSB
	return 0.0001f * SWAP_UINT16(frame.data16[0]);
}
