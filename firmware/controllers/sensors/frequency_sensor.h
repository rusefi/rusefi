#include "functional_sensor.h"
#include "timer.h"

class FrequencySensor : public FunctionalSensor {
public:
	FrequencySensor(SensorType type, efitick_t timeoutPeriod, brain_pin_e pin)
		: FunctionalSensor(type, timeoutPeriod)
		, m_pin(pin) 
	{
		
	}

	void init(const char* const msg);
	void deInit();

	void onEdge(efitick_t nowNt);
	brain_pin_e getPin(void);

private:
	Timer m_edgeTimer;
	brain_pin_e m_pin = GPIO_UNASSIGNED;
};
