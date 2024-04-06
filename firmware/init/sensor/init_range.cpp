#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "resistance_func.h"

#if EFI_TCU

// These aux sensors just read voltage - so the converter function has nothing to do

static FunctionalSensor rangeSensors[] = {
	{ SensorType::RangeInput1, MS2NT(50) },
	{ SensorType::RangeInput2, MS2NT(50) },
	{ SensorType::RangeInput3, MS2NT(50) },
	{ SensorType::RangeInput4, MS2NT(50) },
	{ SensorType::RangeInput5, MS2NT(50) },
	{ SensorType::RangeInput6, MS2NT(50) },
};

static ResistanceFunc rangeFuncs[RANGE_INPUT_COUNT];

static_assert(efi::size(rangeSensors) == RANGE_INPUT_COUNT);

void initRangeSensors() {
	for (size_t i = 0; i < efi::size(engineConfiguration->tcu_rangeAnalogInput); i++) {
		auto channel = engineConfiguration->tcu_rangeAnalogInput[i];

		// Skip unconfigured channels
		if (!isAdcChannelValid(channel)) {
			continue;
		}

		rangeFuncs[i].configure(5.0f, engineConfiguration->tcu_rangeSensorBiasResistor, engineConfiguration->tcu_rangeSensorPulldown);
		
		auto& sensor = rangeSensors[i];
		sensor.setFunction(rangeFuncs[i]);
		sensor.Register();

		AdcSubscription::SubscribeSensor(sensor, channel, 10);
	}
}

#endif
