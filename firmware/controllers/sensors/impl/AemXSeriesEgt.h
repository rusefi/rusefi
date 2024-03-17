#pragma once

#include "can_sensor.h"

class AemXSeriesEgt final : public CanSensorBase {
public:
	AemXSeriesEgt(uint8_t sensorIndex, SensorType type);

protected:
	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override;
};
