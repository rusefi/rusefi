#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "linear_func.h"

static LinearFunc gearboxposSensorFunc;
static FunctionalSensor gearboxposSensor(SensorType::GearboxPositionSensor, /* timeout = */ MS2NT(50));

static void initGearboxPosition(LinearFunc& func, FunctionalSensor& sensor, const linear_sensor_s& cfg, float bandwidth) {
  auto channel = cfg.hwChannel;

  // Only register if we have a sensor
  if (!isAdcChannelValid(channel)) {
    return;
  }

  float val1 = cfg.value1;
  float val2 = cfg.value2;

  // Limit to max given pressure - val1 or val2 could be larger
  // (sensor may be backwards, high voltage = low pressure)
  float greaterOutput = val1 > val2 ? val1 : val2;

  // Allow slightly negative output (-5kpa) so as to not fail the sensor when engine is off
  func.configure(cfg.v1, val1, cfg.v2, val2, /*minOutput*/ -5, greaterOutput);

  sensor.setFunction(func);

  AdcSubscription::SubscribeSensor(sensor, channel, bandwidth);

  sensor.Register();
}

void initGearboxPosition() {
  initGearboxPosition(gearboxposSensorFunc, gearboxposSensor, engineConfiguration->gearboxPosition, 10);
}

void deinitGearboxPosition() {
  AdcSubscription::UnsubscribeSensor(gearboxposSensor, engineConfiguration->gearboxPosition.hwChannel);
}
