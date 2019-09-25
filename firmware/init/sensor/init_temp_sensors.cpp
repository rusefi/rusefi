
#include "engine_configuration_generated_structures.h"

#include "adc_subscription.h"
#include "converters/linear_func.h"
#include "converters/converter_chain.h"
#include "converters/thermistor_func.h"
#include "converters/resistance_func.h"
#include "engine.h"
#include "tunerstudio_configuration.h"
#include "functional_sensor.h"

EXTERN_ENGINE;


extern TunerStudioOutputChannels tsOutputChannels;


using therm = ThermistorFunc;
using resist = ResistanceFunc;


union FuncPair {
	FuncPair() { }

	FunctionalSensor<LinearFunc> linear;
	FunctionalSensor<ConverterChain<resist, therm>> thermistor;
};

static SensorConversionFunction* configureTempSensorFunction(thermistor_conf_s& cfg, FuncPair& p, bool isLinear) {
	if (isLinear) {
		p.linear.f().configure(cfg.resistance_1, cfg.tempC_1, cfg.resistance_2, cfg.tempC_2, -50, 250);

		return &p.linear;
	} else /* sensor is thermistor */{
		p.thermistor = ConverterChain<resist, therm>();
		
		p.thermistor.configure<resist>(5, 2700);
		p.thermistor.

		return &p.thermistor;
	}
}

static void configureTempSensor(FunctionalSensor& sensor, FuncPair& p, ThermistorConf& config, bool isLinear, float* reportLoc) {
	auto channel = config.adcChannel;
	
	// Check if channel is set - ignore this sensor if not
	if (channel == EFI_ADC_NONE) {
		return;
	}

	// Configure the conversion function for this sensor
	sensor.setFunction(configureTempSensorFunction(config.config, p, isLinear));

	// Set report location
	sensor.setReportingLocation(reportLoc);

	// Register & subscribe
	if (!sensor.Register()) {
		// uhh?
		return;
	}

	AdcSubscription::SubscribeSensor(sensor, channel);
}

void initTempSensors() {
	static FunctionalSensor  clt(SensorType::Clt);
	static FunctionalSensor  iat(SensorType::Iat);
	static FunctionalSensor aux1(SensorType::AuxTemp1);
	static FunctionalSensor aux2(SensorType::AuxTemp2);

	static FuncPair fclt, fiat, faux1, faux2;

	configureTempSensor(clt,  fclt,  engineConfiguration->clt, engineConfiguration->useLinearCltSensor, &tsOutputChannels.coolantTemperature);
	configureTempSensor(iat,  fiat,  engineConfiguration->iat, engineConfiguration->useLinearIatSensor, &tsOutputChannels.intakeAirTemperature);
	configureTempSensor(aux1, faux1, engineConfiguration->auxTempSensor1, false,                        nullptr /* fixme! */);
	configureTempSensor(aux2, faux2, engineConfiguration->auxTempSensor2, false,                        nullptr /* fixme! */);
}
