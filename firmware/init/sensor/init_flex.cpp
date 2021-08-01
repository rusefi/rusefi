#include "init.h"
#include "pin_repository.h"
#include "engine.h"
#include "frequency_sensor.h"
#include "biquad.h"

class FlexConverter : public SensorConverter {
public:
	FlexConverter() {
		// Update rate is 50-150hz, so this actually filters at 0.5-1.5hz -3db depending on E%, which is ok
		m_filter.configureLowpass(100, 1);
	}

	SensorResult convert(float frequency) const override {
		// Sensor should only report 50-150hz, significantly outside that range indicates a problem
		// it changes to 200hz+ to indicate methanol "contamination"
		if (frequency > 45 && frequency < 155) {
			float flexPct = clampF(0, frequency - 50, 100);

			return m_filter.filter(flexPct);
		} else {
			return unexpected;
		}
	}

private:
	mutable Biquad m_filter;
};

static FrequencySensor flexSensor(SensorType::FuelEthanolPercent, MS2NT(500));
static FlexConverter converter;

// https://rusefi.com/forum/viewtopic.php?p=37452&sid=829804c90d5b2e1fecd1b900cf1b1811#p37452

void initFlexSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	auto pin = CONFIG(flexSensorPin);

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(pin)) {
		return;
	}

	flexSensor.setFunction(converter);
	flexSensor.init(pin);
	flexSensor.Register();
}
