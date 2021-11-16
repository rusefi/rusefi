#include "pch.h"

#include "adc_subscription.h"
#include "functional_sensor.h"
#include "func_chain.h"
#include "linear_func.h"
#include "resistance_func.h"
#include "thermistor_func.h"

using resist = ResistanceFunc;
using therm = ThermistorFunc;

// Each one could be either linear or thermistor
struct FuncPair {
	LinearFunc linear;
	FuncChain<resist, therm> thermistor;
};

static CCM_OPTIONAL FunctionalSensor clt(SensorType::Clt, MS2NT(10));
static CCM_OPTIONAL FunctionalSensor iat(SensorType::Iat, MS2NT(10));
static CCM_OPTIONAL FunctionalSensor aux1(SensorType::AuxTemp1, MS2NT(10));
static CCM_OPTIONAL FunctionalSensor aux2(SensorType::AuxTemp2, MS2NT(10));

static FuncPair fclt, fiat, faux1, faux2;

static void validateThermistorConfig(const char *msg, thermistor_conf_s& cfg) {
	if (cfg.tempC_1 >= cfg.tempC_2 ||
		cfg.tempC_2 >= cfg.tempC_3) {
		firmwareError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction, "Invalid thermistor %s configuration: please check that temperatures are in the ascending order %f %f %f",
				msg,
				cfg.tempC_1,
				cfg.tempC_2,
				cfg.tempC_3);
	}
}

static SensorConverter& configureTempSensorFunction(const char *msg,
		thermistor_conf_s& cfg, FuncPair& p, bool isLinear) {
	if (isLinear) {
		p.linear.configure(cfg.resistance_1, cfg.tempC_1, cfg.resistance_2, cfg.tempC_2, -50, 250);

		return p.linear;
	} else /* sensor is thermistor */ {
		validateThermistorConfig(msg, cfg);

		p.thermistor.get<resist>().configure(5.0f, cfg.bias_resistor);
		p.thermistor.get<therm>().configure(cfg);

		return p.thermistor;
	}
}

static void configTherm(const char *msg,
		FunctionalSensor &sensor,
					FuncPair &p,
					ThermistorConf &config,
					bool isLinear) {
	// nothing to do if no channel
	if (!isAdcChannelValid(config.adcChannel)) {
		return;
	}

	// Configure the conversion function for this sensor
	sensor.setFunction(configureTempSensorFunction(msg, config.config, p, isLinear));
}

static void configureTempSensor(const char *msg,
								FunctionalSensor &sensor,
								FuncPair &p,
								ThermistorConf &config,
								bool isLinear) {
	auto channel = config.adcChannel;

	// Only register if we have a sensor
	if (!isAdcChannelValid(channel)) {
		return;
	}

	configTherm(msg, sensor, p, config, isLinear);

	// Register & subscribe
	AdcSubscription::SubscribeSensor(sensor, channel, 2);
	sensor.Register();
}

void initThermistors() {
	if (!CONFIG(consumeObdSensors)) {
		configureTempSensor("clt",
						clt,
						fclt,
						CONFIG(clt),
						CONFIG(useLinearCltSensor));

		configureTempSensor("iat",
						iat,
						fiat,
						CONFIG(iat),
						CONFIG(useLinearIatSensor));
	}

	configureTempSensor("aux1",
						aux1,
						faux1,
						CONFIG(auxTempSensor1),
						false);

	configureTempSensor("aux2",
						aux2,
						faux2,
						CONFIG(auxTempSensor2),
						false);
}

void deinitThermistors() {
	AdcSubscription::UnsubscribeSensor(clt);
	AdcSubscription::UnsubscribeSensor(iat);
	AdcSubscription::UnsubscribeSensor(aux1);
	AdcSubscription::UnsubscribeSensor(aux2);
}
