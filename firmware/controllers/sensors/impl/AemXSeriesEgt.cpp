#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "AemXSeriesEgt.h"

static constexpr uint32_t aem_egt_base = 0x000A0305;
#define AEM_EGT_DEFAULT_PERIOD_MS 100	/* 10 Hz */

AemXSeriesEgt::AemXSeriesEgt(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		aem_egt_base + sensorIndex,
		type,
		MS2NT(3 * AEM_EGT_DEFAULT_PERIOD_MS)	// sensor transmits at 10hz, allow a frame to be missed
	)
{
	// nope
}

void AemXSeriesEgt::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	// No status flags, just a value
	uint16_t egt = SWAP_UINT16(frame.data16[0]);

	setValidValue((float)egt, nowNt);
}

#endif
