#include "stored_value_sensor.h"
#include "timer.h"
#include "rusefi_hw_enums.h"
#include "biquad.h"

class FlexFuelSensor : public StoredValueSensor {
public:
	FlexFuelSensor();

	void init(brain_pin_e pin);

	void onEdge(efitick_t nowNt);

	void showInfo(Logging* /*logger*/, const char* /*sensorName*/) const override { }

private:
	Timer m_edgeTimer;

	Biquad m_filter;
};
