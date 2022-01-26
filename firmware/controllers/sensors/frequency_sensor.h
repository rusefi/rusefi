#include "functional_sensor.h"
#include "timer.h"
#include "biquad.h"

class FrequencySensor : public FunctionalSensor {
public:
	FrequencySensor(SensorType type, efitick_t timeoutPeriod, float filterParameter)
		: FunctionalSensor(type, timeoutPeriod)
	{
		m_filter.configureLowpass(1, filterParameter);
	}

	void init(brain_pin_e pin);
	void deInit();

	void onEdge(efitick_t nowNt);

private:
	Timer m_edgeTimer;
	brain_pin_e m_pin = GPIO_UNASSIGNED;

	Biquad m_filter;
};
