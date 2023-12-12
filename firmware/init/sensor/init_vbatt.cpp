#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "linear_func.h"

static FunctionalSensor vbattSensor(SensorType::BatteryVoltage, /* timeout = */ MS2NT(100));
static FunctionalSensor ignBattSensor(SensorType::IgnKeyVoltage, /* timeout = */ MS2NT(100));

void initVbatt() {
  {
    static LinearFunc vbattFunc; // static with scoped visibility just to reduce change of code defect
	  vbattFunc.configure(0, 0, 1, engineConfiguration->vbattDividerCoeff, 0, 50);
	  vbattSensor.setFunction(vbattFunc);
	}

#if defined(IGN_KEY_DIVIDER)
  {
    static LinearFunc ignBattFunc; // static with scoped visibility just to reduce change of code defect
	  ignBattFunc.configure(0, 0, 1, IGN_KEY_DIVIDER, 0, 50);
	  ignBattSensor.setFunction(ignBattFunc);
	}
#endif //	IGN_KEY_DIVIDER


	if (isAdcChannelValid(engineConfiguration->vbattAdcChannel)) {
	  // adcVoltsPerVolt is set to 1.0 because vbatt doesn't go thru the analog input divider
	  AdcSubscription::SubscribeSensor(vbattSensor, engineConfiguration->vbattAdcChannel, /* filter HZ = */ 20, /* adcVoltsPerVolt = */ 1.0f);
	  vbattSensor.Register();
	}

#if defined(IGN_KEY_DIVIDER)
	if (isAdcChannelValid(engineConfiguration->ignKeyAdcChannel)) {
	  AdcSubscription::SubscribeSensor(ignBattSensor, engineConfiguration->ignKeyAdcChannel, /* filter HZ = */ 20, /* adcVoltsPerVolt = */ 1.0f);
	  ignBattSensor.Register();
	}
#endif //	IGN_KEY_DIVIDER
}

void deinitVbatt() {
	AdcSubscription::UnsubscribeSensor(vbattSensor, engineConfiguration->vbattAdcChannel);
#if defined(IGN_KEY_DIVIDER)
	AdcSubscription::UnsubscribeSensor(ignBattSensor, engineConfiguration->ignKeyAdcChannel);
#endif //	IGN_KEY_DIVIDER
}
