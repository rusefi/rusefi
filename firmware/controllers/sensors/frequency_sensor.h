#include "functional_sensor.h"
#include "timer.h"

class FrequencySensor : public FunctionalSensor {
public:
	FrequencySensor(SensorType type, efitick_t timeoutPeriod)
		: FunctionalSensor(type, timeoutPeriod) { }

	void init(brain_pin_e pin);

	void onEdge(efitick_t nowNt);

private:
	Timer m_edgeTimer;
};
