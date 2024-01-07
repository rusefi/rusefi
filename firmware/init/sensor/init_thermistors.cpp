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

static FunctionalSensor clt(SensorType::Clt, MS2NT(10));
static FunctionalSensor iat(SensorType::Iat, MS2NT(10));
static FunctionalSensor aux1(SensorType::AuxTemp1, MS2NT(10));
static FunctionalSensor aux2(SensorType::AuxTemp2, MS2NT(10));

static FunctionalSensor oilTempSensor(SensorType::OilTemperature, MS2NT(10));
static FunctionalSensor fuelTempSensor(SensorType::FuelTemperature, MS2NT(10));
static FunctionalSensor ambientTempSensor(SensorType::AmbientTemperature, MS2NT(10));
static FunctionalSensor compressorDischargeTemp(SensorType::CompressorDischargeTemperature, MS2NT(10));

static FuncPair fclt, fiat, faux1, faux2, foil, ffuel, fambient, fcdt;

static void validateThermistorConfig(const char *msg, thermistor_conf_s& cfg) {
	if (cfg.tempC_1 >= cfg.tempC_2 ||
		cfg.tempC_2 >= cfg.tempC_3) {
		firmwareError(ObdCode::OBD_ThermistorConfig, "Invalid thermistor %s configuration: please check that temperatures are in the ascending order %f %f %f",
				msg,
				cfg.tempC_1,
				cfg.tempC_2,
				cfg.tempC_3);
	}
}

static SensorConverter& configureTempSensorFunction(const char *msg,
		thermistor_conf_s& cfg, FuncPair& p, bool isLinear, bool isPulldown) {
	if (isLinear) {
		p.linear.configure(cfg.resistance_1, cfg.tempC_1, cfg.resistance_2, cfg.tempC_2, -50, 250);

		return p.linear;
	} else /* sensor is thermistor */ {
		validateThermistorConfig(msg, cfg);

		p.thermistor.get<resist>().configure(5.0f, cfg.bias_resistor, isPulldown);
		p.thermistor.get<therm>().configure(cfg);

		return p.thermistor;
	}
}

static void configTherm(const char *msg,
		FunctionalSensor &sensor,
					FuncPair &p,
					ThermistorConf &cfg,
					bool isLinear,
					bool isPulldown) {
	// nothing to do if no channel
	if (!isAdcChannelValid(cfg.adcChannel)) {
		return;
	}

	// Configure the conversion function for this sensor
	sensor.setFunction(configureTempSensorFunction(msg, cfg.config, p, isLinear, isPulldown));
}

static void configureTempSensor(const char *msg,
								FunctionalSensor &sensor,
								FuncPair &p,
								ThermistorConf &cfg,
								bool isLinear,
								bool isPulldown = false) {
	auto channel = cfg.adcChannel;

	// Only register if we have a sensor
	if (!isAdcChannelValid(channel)) {
		return;
	}

	configTherm(msg, sensor, p, cfg, isLinear, isPulldown);

	// Register & subscribe
	AdcSubscription::SubscribeSensor(sensor, channel, 2);
	sensor.Register();
}

void initThermistors() {
	configureTempSensor("clt",
						clt,
						fclt,
						engineConfiguration->clt,
						engineConfiguration->useLinearCltSensor,
						engineConfiguration->cltSensorPulldown);

	configureTempSensor("iat",
						iat,
						fiat,
						engineConfiguration->iat,
						engineConfiguration->useLinearIatSensor,
						engineConfiguration->iatSensorPulldown);

	configureTempSensor("oil temp",
						oilTempSensor,
						faux2,
						engineConfiguration->oilTempSensor,
						false);

	configureTempSensor("fuel temp",
						fuelTempSensor,
						ffuel,
						engineConfiguration->fuelTempSensor,
						false);

	configureTempSensor("ambient temp",
						ambientTempSensor,
						fambient,
						engineConfiguration->ambientTempSensor,
						false);

	configureTempSensor("compressor discharge temp",
						compressorDischargeTemp,
						fcdt,
						engineConfiguration->compressorDischargeTemperature,
						false);

	configureTempSensor("aux1",
						aux1,
						faux1,
						engineConfiguration->auxTempSensor1,
						false);

	configureTempSensor("aux2",
						aux2,
						faux2,
						engineConfiguration->auxTempSensor2,
						false);
}

void deinitThermistors() {
	AdcSubscription::UnsubscribeSensor(clt, engineConfiguration->clt.adcChannel);
	AdcSubscription::UnsubscribeSensor(iat, engineConfiguration->iat.adcChannel);
	AdcSubscription::UnsubscribeSensor(oilTempSensor, engineConfiguration->oilTempSensor.adcChannel);
	AdcSubscription::UnsubscribeSensor(fuelTempSensor, engineConfiguration->fuelTempSensor.adcChannel);
	AdcSubscription::UnsubscribeSensor(ambientTempSensor, engineConfiguration->ambientTempSensor.adcChannel);
	AdcSubscription::UnsubscribeSensor(compressorDischargeTemp, engineConfiguration->compressorDischargeTemperature.adcChannel);
	AdcSubscription::UnsubscribeSensor(aux1, engineConfiguration->auxTempSensor1.adcChannel);
	AdcSubscription::UnsubscribeSensor(aux2, engineConfiguration->auxTempSensor2.adcChannel);
}
