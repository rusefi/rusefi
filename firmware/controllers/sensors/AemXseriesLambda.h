#pragma once

#include "can_sensor.h"

class AemXSeriesWideband final : public CanSensorBase {
public:
	AemXSeriesWideband(uint8_t sensorIndex, SensorType type);

protected:
	SensorResult decodeFrame(const CANRxFrame& frame) override;
};
