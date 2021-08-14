#include "pch.h"

#include "adc_subscription.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "redundant_ford_tps.h"
#include "proxy_sensor.h"
#include "linear_func.h"
#include "tps.h"

LinearFunc tpsFunc1p(TPS_TS_CONVERSION);
LinearFunc tpsFunc1s(TPS_TS_CONVERSION);
LinearFunc tpsFunc2p(TPS_TS_CONVERSION);
LinearFunc tpsFunc2s(TPS_TS_CONVERSION);

FunctionalSensor tpsSens1p(SensorType::Tps1Primary, MS2NT(10));
FunctionalSensor tpsSens1s(SensorType::Tps1Secondary, MS2NT(10));
FunctionalSensor tpsSens2p(SensorType::Tps2Primary, MS2NT(10));
FunctionalSensor tpsSens2s(SensorType::Tps2Secondary, MS2NT(10));

// Used in case of "normal", non-Ford ETB TPS
RedundantSensor tps1(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary);
RedundantSensor tps2(SensorType::Tps2, SensorType::Tps2Primary, SensorType::Tps2Secondary);

// Used only in case of weird Ford-style ETB TPS
RedundantFordTps fordTps1(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary);
RedundantFordTps fordTps2(SensorType::Tps2, SensorType::Tps2Primary, SensorType::Tps2Secondary);

LinearFunc pedalFuncPrimary;
LinearFunc pedalFuncSecondary;
FunctionalSensor pedalSensorPrimary(SensorType::AcceleratorPedalPrimary, MS2NT(10));
FunctionalSensor pedalSensorSecondary(SensorType::AcceleratorPedalSecondary, MS2NT(10));

RedundantSensor pedal(SensorType::AcceleratorPedal, SensorType::AcceleratorPedalPrimary, SensorType::AcceleratorPedalSecondary);

// This sensor indicates the driver's throttle intent - Pedal if we have one, TPS if not.
ProxySensor driverIntent(SensorType::DriverThrottleIntent);

// These sensors are TPS-like, so handle them in here too
LinearFunc wastegateFunc(PACK_MULT_VOLTAGE);
LinearFunc idlePosFunc(PACK_MULT_VOLTAGE);
FunctionalSensor wastegateSens(SensorType::WastegatePosition, MS2NT(10));
FunctionalSensor idlePosSens(SensorType::IdlePosition, MS2NT(10));

static bool configureTps(LinearFunc& func, adc_channel_e channel, float closed, float open, float min, float max, const char* msg) {
	// Only configure if we have a channel
	if (!isAdcChannelValid(channel)) {
		return false;
	}

	float scaledClosed = closed / func.getDivideInput();
	float scaledOpen = open / func.getDivideInput();

	float split = absF(scaledOpen - scaledClosed);

	// If the voltage for closed vs. open is very near, something is wrong with your calibration
	if (split < 0.5f) {
		firmwareError(OBD_Throttle_Position_Sensor_Circuit_Malfunction, "\"%s\" problem: open %.2f/closed %.2f cal values are too close together. Check your wiring!", msg,
				open,
				closed);
		return false;
	}

	func.configure(
		closed, 0,
		open, 100, 
		min, max
	);

	return true;
}

static bool initTpsFunc(LinearFunc& func, FunctionalSensor& sensor, adc_channel_e channel, float closed, float open, float min, float max) {
	// If the configuration was invalid, don't continue to configure the sensor
	if (!configureTps(func, channel, closed, open, min, max, sensor.getSensorName())) {
		return false;
	}

	sensor.setFunction(func);

	AdcSubscription::SubscribeSensor(sensor, channel, 200);

	return sensor.Register();
}

static void initTpsFuncAndRedund(RedundantSensor& redund, RedundantFordTps* fordTps, bool isFordTps, LinearFunc& func, FunctionalSensor& sensor, adc_channel_e channel, float closed, float open, float min, float max) {
	bool hasSecond = initTpsFunc(func, sensor, channel, closed, open, min, max);

	if (isFordTps && fordTps) {
		fordTps->configure(5.0f, 52.6f);
		fordTps->Register();
	} else {
		redund.configure(5.0f, !hasSecond);
		redund.Register();
	}
}

void initTps(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	percent_t min = CONFIG(tpsErrorDetectionTooLow);
	percent_t max = CONFIG(tpsErrorDetectionTooHigh);

	if (!CONFIG(consumeObdSensors)) {
		// primary TPS sensors
		initTpsFunc(tpsFunc1p, tpsSens1p, CONFIG(tps1_1AdcChannel), CONFIG(tpsMin), CONFIG(tpsMax), min, max);
		initTpsFunc(tpsFunc2p, tpsSens2p, CONFIG(tps2_1AdcChannel), CONFIG(tps2Min), CONFIG(tps2Max), min, max);

		// Secondary TPS sensors (and redundant combining)
		bool isFordTps = CONFIG(useFordRedundantTps);
		initTpsFuncAndRedund(tps1, &fordTps1, isFordTps, tpsFunc1s, tpsSens1s, CONFIG(tps1_2AdcChannel), CONFIG(tps1SecondaryMin), CONFIG(tps1SecondaryMax), min, max);
		initTpsFuncAndRedund(tps2, &fordTps2, isFordTps, tpsFunc2s, tpsSens2s, CONFIG(tps2_2AdcChannel), CONFIG(tps2SecondaryMin), CONFIG(tps2SecondaryMax), min, max);

		// Pedal sensors
		initTpsFunc(pedalFuncPrimary, pedalSensorPrimary, CONFIG(throttlePedalPositionAdcChannel), CONFIG(throttlePedalUpVoltage), CONFIG(throttlePedalWOTVoltage), min, max);
		initTpsFuncAndRedund(pedal, nullptr, false, pedalFuncSecondary, pedalSensorSecondary, CONFIG(throttlePedalPositionSecondAdcChannel), CONFIG(throttlePedalSecondaryUpVoltage), CONFIG(throttlePedalSecondaryWOTVoltage), min, max);

		// TPS-like stuff that isn't actually a TPS
		initTpsFunc(wastegateFunc, wastegateSens, CONFIG(wastegatePositionSensor), CONFIG(wastegatePositionMin), CONFIG(wastegatePositionMax), min, max);
		initTpsFunc(idlePosFunc, idlePosSens, CONFIG(idlePositionSensor), CONFIG(idlePositionMin), CONFIG(idlePositionMax), min, max);
	}

	// Route the pedal or TPS to driverIntent as appropriate
	if (isAdcChannelValid(CONFIG(throttlePedalPositionAdcChannel))) {
		driverIntent.setProxiedSensor(SensorType::AcceleratorPedal);
	} else {
		driverIntent.setProxiedSensor(SensorType::Tps1);
	}

	driverIntent.Register();
}

void deinitTps() {
	AdcSubscription::UnsubscribeSensor(tpsSens1p);
	AdcSubscription::UnsubscribeSensor(tpsSens1s);

	AdcSubscription::UnsubscribeSensor(tpsSens2p);
	AdcSubscription::UnsubscribeSensor(tpsSens2s);

	AdcSubscription::UnsubscribeSensor(pedalSensorPrimary);
	AdcSubscription::UnsubscribeSensor(pedalSensorSecondary);

	AdcSubscription::UnsubscribeSensor(wastegateSens);
	AdcSubscription::UnsubscribeSensor(idlePosSens);
}
