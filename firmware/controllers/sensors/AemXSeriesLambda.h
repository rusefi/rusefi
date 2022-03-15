#pragma once

#include "can_sensor.h"

class AemXSeriesWideband final : public CanSensorBase {
public:
	AemXSeriesWideband(uint8_t sensorIndex, SensorType type);

	bool acceptFrame(const CANRxFrame& frame) const override;

protected:
	// Dispatches to one of the three decoders below
	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override;

	// Decode an actual AEM controller, or a rusEFI controller sending AEM format
	void decodeAemXSeries(const CANRxFrame& frame, efitick_t nowNt);

	// Decode rusEFI custom format
	void decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt);
	void decodeRusefiDiag(const CANRxFrame& frame);

private:
	const uint8_t m_sensorIndex;
};
