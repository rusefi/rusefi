/**
 * @file flex_sensor.h
 */
#include "sensor_type.h"
#include "functional_sensor.h"
#include "sensor_converter_func.h"
#include "biquad.h"
#include <rusefi/timer.h>

class FlexConverter : public SensorConverter {
public:
	FlexConverter() {
		// Update rate is 50-150hz, so this actually filters at 0.5-1.5hz -3db depending on E%, which is ok
		m_filter.configureLowpass(100, 1);
	}

	SensorResult convert(float frequency) const override {
		// Sensor should only report 50-150hz, significantly outside that range indicates a problem
		// it changes to 200hz+ to indicate methanol "contamination"
		if (frequency < 45) {
			return UnexpectedCode::Low;
		}

		if (frequency > 155) {
			return UnexpectedCode::High;
		}

		float flexPct = clampF(0, frequency - 50, 100);

		return m_filter.filter(flexPct);
	}

private:
	mutable Biquad m_filter;
};

class FlexSensor {
public:
	FlexSensor(efidur_t timeoutPeriod)
		: flexSensor(SensorType::FuelEthanolPercent, timeoutPeriod)
		, flexFuelTemp(SensorType::FuelTemperature, timeoutPeriod)
	{
		// 0.01 means filter bandwidth of ~1hz with ~100hz sensor
		flexTempFilter.configureLowpass(1, 0.01f);
		flexSensor.setFunction(converter);
	}
	void Register(bool withTempSensor);
	void unregister();
	void callback(efitick_t nowNt, bool value);
	void debug();

private:
	FunctionalSensor flexSensor;
	StoredValueSensor flexFuelTemp;
	FlexConverter converter;

	Biquad flexTempFilter;

	Timer flexFreq, flexPulse;

	bool gotRising = false;

	int lowFlexCallbackCounter = 0;
	int flexCallbackCounter = 0;

	float frequency;
	float pulseWidthUs;
	efitick_t latestCallbackTime;
};
