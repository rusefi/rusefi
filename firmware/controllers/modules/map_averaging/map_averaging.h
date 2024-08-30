/**
 * @file	map_averaging.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "sensor_converter_func.h"

void initMapAveraging();

// allow smoothing up to number of cylinders
#define MAX_MAP_BUFFER_LENGTH (MAX_CYLINDER_COUNT)

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

class MapAveragingModule : public EngineModule {
public:
	void onConfigurationChange(engine_configuration_s const * previousConfig);

	void onFastCallback() override;
	void onEnginePhase(float rpm,
						efitick_t edgeTimestamp,
						float currentPhase,
						float nextPhase) override;

	void submitSample(float volts);
};
