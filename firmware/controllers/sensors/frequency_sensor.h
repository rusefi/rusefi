/**
 * @file frequency_sensor.h
 */
#include "functional_sensor.h"
#include <rusefi/timer.h>
#include "biquad.h"

class FrequencySensor : public FunctionalSensor {
public:
	FrequencySensor(SensorType type, efidur_t timeoutPeriod)
		: FunctionalSensor(type, timeoutPeriod)
	{ }

	void initIfValid(brain_pin_e pin, SensorConverter &converter, float filterParameter);
	void deInit();

	// sad workaround: we are not good at BiQuad configuring
	bool useBiQuad = true;

	void showInfo(const char* sensorName) const override;

	void onEdge(efitick_t nowNt);

#if EFI_UNIT_TEST
	// Static sensors outlive EngineTestHelper's simulated-clock rewind.
	void resetHistoryForUnitTest() {
		m_edgeTimer.init();
		m_filter.reset();
	}
#endif

	int eventCounter = 0;
private:
	Timer m_edgeTimer;
	brain_pin_e m_pin = Gpio::Unassigned;

	Biquad m_filter;
};
