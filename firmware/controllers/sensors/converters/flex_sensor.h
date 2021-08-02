#include "sensor_converter_func.h"
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
