/**
 * @file    filtered_sensor.h
 * @brief   A StoredValueSensor that applies exponential moving average filtering
 *          to another sensor's value on demand.
 *
 * The filter is advanced once per call to update(), decoupling the filter rate
 * from the consumer read rate.  This ensures the EMA smoothing factor has a
 * predictable effect regardless of how many consumers read the sensor.
 */

#pragma once

#include "stored_value_sensor.h"
#include "exp_average.h"

class FilteredSensor : public StoredValueSensor {
public:
	FilteredSensor(SensorType type, efidur_t timeoutNt, SensorType sourceType)
		: StoredValueSensor(type, timeoutNt)
		, m_sourceType(sourceType)
	{
	}

	void setSmoothingFactor(float alpha) {
		m_expAverage.setSmoothingFactor(alpha);
	}

	void update() {
		SensorResult source = Sensor::get(m_sourceType);
		if (!source) {
			invalidate(source.Code);
			return;
		}
		float filtered = m_expAverage.initOrAverage(source.Value);
		setValidValue(filtered, getTimeNowNt());
	}

	void reset() {
		m_expAverage.reset();
		invalidate();
	}

	bool isRedundant() const override {
		return Sensor::isRedundant(m_sourceType);
	}

	bool hasSensor() const override {
		return Sensor::hasSensor(m_sourceType);
	}

	void showInfo(const char* sensorName) const override;

private:
	SensorType m_sourceType;
	ExpAverage m_expAverage;
};
