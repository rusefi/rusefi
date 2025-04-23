/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * EGO Exhaust Gas Oxygen, also known as AFR Air/Fuel Ratio :)
 *
 * rusEfi has three options for wideband:
 * 1) integration with external widebands using liner analog signal wire
 * 2) 8-point interpolation curve to emulate a wide-band with a narrow-band sensor.
 * 3) CJ125 internal wideband controller is known to work with both 4.2 and 4.9
 *
 */
#include "pch.h"


bool hasAfrSensor() {
	if (engineConfiguration->enableAemXSeries) {
		return true;
	}

	return isAdcChannelValid(engineConfiguration->afr.hwChannel);
}

extern float InnovateLC2AFR;

float getAfrInterpolation(float lambdaSensorVolts, afr_sensor_s * sensor){
	return interpolateMsg("AFR", sensor->v1, sensor->value1, sensor->v2, sensor->value2, lambdaSensorVolts)
			+ engineConfiguration->egoValueShift;
}

float getAfr(SensorType type) {
	afr_sensor_s * sensor = &engineConfiguration->afr;
	float lambdaSensorVolts = 0;
	float interpolatedAfr = 0;

	switch (type) {
  	case SensorType::Lambda1: {
    	if (isAdcChannelValid(engineConfiguration->afr.hwChannel)) {
      		lambdaSensorVolts = adcGetScaledVoltage("ego", engineConfiguration->afr.hwChannel);
      		interpolatedAfr = getAfrInterpolation(lambdaSensorVolts, sensor);
    		// todo set SmoothedLambda1
      		return interpolatedAfr;
    	}
    	return 0; // invalid adc channel
  	}
	case SensorType::Lambda2: {
		if (isAdcChannelValid(engineConfiguration->afr.hwChannel2)) {
			lambdaSensorVolts = adcGetScaledVoltage("ego", engineConfiguration->afr.hwChannel2);
			interpolatedAfr = getAfrInterpolation(lambdaSensorVolts, sensor);
			// todo set SmoothedLambda2
			return interpolatedAfr;
		}
		return 0; // invalid adc channel
	}
  	default:
    	return 0; // invalid sensor type
  	}
	return 0;
}

// this method is only used for canned tunes now! User-facing selection is defined in tunerstudio.template.ini using settingSelector
static void initEgoSensor(afr_sensor_s *sensor, ego_sensor_e type) {

	switch (type) {
	case ES_BPSX_D1:
		/**
		 * This decodes BPSX D1 Wideband Controller analog signal
		 */
		sensor->v1 = 0;
		sensor->value1 = 9;
		sensor->v2 = 5;
		sensor->value2 = 19;
		break;

	case ES_Innovate_MTX_L:
		sensor->v1 = 0;
		sensor->value1 = 7.35;
		sensor->v2 = 5;
		sensor->value2 = 22.39;
		break;
	case ES_14Point7_Free:
		sensor->v1 = 0;
		sensor->value1 = 9.996;
		sensor->v2 = 5;
		sensor->value2 = 19.992;
		break;
		// technically 14Point7 and PLX use the same scale
	case ES_PLX:
		sensor->v1 = 0;
		sensor->value1 = 10;
		sensor->v2 = 5;
		sensor->value2 = 20;
		break;
	case ES_AEM:
		sensor->v1 = 0.5;
		sensor->value1 = 8.5;
		sensor->v2 = 4.5;
		sensor->value2 = 18;
		break;
	default:
		firmwareError(ObdCode::CUSTOM_EGO_TYPE, "Unexpected EGO %d", type);
		break;
	}
}

void setEgoSensor(ego_sensor_e type) {
	engineConfiguration->afr_type = type;
	initEgoSensor(&engineConfiguration->afr, type);
}
