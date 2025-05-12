/**
 * @file	map_averaging.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "engine_module.h"
#include "stored_value_sensor.h"
#include "sensor_converter_func.h"
#include "scheduler.h"

#if EFI_MAP_AVERAGING

#if HAL_USE_ADC
void mapAveragingAdcCallback(float instantVoltage);
#endif



// allow smoothing up to number of cylinders
#define MAX_MAP_BUFFER_LENGTH (MAX_CYLINDER_COUNT)
/**
* here we have averaging start and averaging end points for each cylinder
* TODO: migrate to AngleBasedEvent, see also #7869
*/
struct sampler {
	scheduling_s startTimer;
	scheduling_s endTimer;
};

void startMapAveraging(sampler* s);
#endif /* EFI_MAP_AVERAGING */

class MapAverager : public StoredValueSensor {
public:
	MapAverager(SensorType type, efidur_t timeout)
		: StoredValueSensor(type, timeout)
	{
	}

	void start();
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
};

MapAverager& getMapAvg(size_t idx);
float filterMapValue(float value);

class MapAveragingModule : public EngineModule {
public:
	void onConfigurationChange(engine_configuration_s const * previousConfig) override;

	void onFastCallback() override;
	void onEnginePhase(float rpm,
						efitick_t edgeTimestamp,
						float currentPhase,
						float nextPhase) override;
	void init();
	void submitSample(float voltsMap1, float voltsMap2);
	sampler samplers[MAX_CYLINDER_COUNT][2];
};
