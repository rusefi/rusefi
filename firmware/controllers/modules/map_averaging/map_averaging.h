/**
 * @file	map_averaging.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "sensor_converter_func.h"

#if EFI_MAP_AVERAGING

#if HAL_USE_ADC
void mapAveragingAdcCallback(float instantVoltage);
#endif

void initMapAveraging();
void refreshMapAveragingPreCalc();

void mapAveragingTriggerCallback(efitick_t edgeTimestamp, angle_t currentPhase, angle_t nextPhase);

// allow smoothing up to number of cylinders
#define MAX_MAP_BUFFER_LENGTH (MAX_CYLINDER_COUNT)

#endif /* EFI_MAP_AVERAGING */

class MapAverager : public StoredValueSensor {
public:
	MapAverager(SensorType type, efidur_t timeout)
		: StoredValueSensor(type, timeout)
	{
	}

	void start(uint8_t cylinderIndex);
	void stop();

	SensorResult submit(float sensorVolts);

	void setFunction(SensorConverter& func) {
		m_function = &func;
	}

	void showInfo(const char* sensorName) const override;

private:
	SensorConverter* m_function = nullptr;

	bool m_isAveraging = false;
	size_t m_counter = 0;
	size_t m_lastCounter = 0;
	float m_sum = 0;
	uint8_t m_cylinderIndex = 0;
};

MapAverager& getMapAvg(size_t idx);
