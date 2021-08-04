#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "linear_func.h"

static LinearFunc vbattFunc;
static FunctionalSensor vbattSensor(SensorType::BatteryVoltage, /* timeout = */ MS2NT(100));

void initVbatt(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	vbattFunc.configure(0, 0, 1, engineConfiguration->vbattDividerCoeff, 0, 50);
	vbattSensor.setFunction(vbattFunc);

	if (!isAdcChannelValid(engineConfiguration->vbattAdcChannel)) {
		return;
	}

	// adcVoltsPerVolt is set to 1.0 because vbatt doesn't go thru the analog input divider
	AdcSubscription::SubscribeSensor(vbattSensor, CONFIG(vbattAdcChannel), /* filter HZ = */ 20, /* adcVoltsPerVolt = */ 1.0f);

	vbattSensor.Register();
}

void reconfigureVbatt(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	vbattFunc.configure(0, 0, 1, engineConfiguration->vbattDividerCoeff, 0, 50);
}
