/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * EGO Exhaust Gas Oxygen, also known as AFR Air/Fuel Ratio :) connectet over analog input
 *
 */
#include "pch.h"

StoredValueSensor smoothedLambda1Sensor(SensorType::SmoothedLambda1, MS2NT(500));
StoredValueSensor smoothedLambda2Sensor(SensorType::SmoothedLambda2, MS2NT(500));

ExpAverage expAverageLambda1;
ExpAverage expAverageLambda2;

#include "cyclic_buffer.h"

bool hasAfrSensor() {
	if (engineConfiguration->enableAemXSeries) {
		return true;
	}

	return isAdcChannelValid(engineConfiguration->afr.hwChannel);
}

float getAfr(SensorType type) {
	afr_sensor_s * sensor = &engineConfiguration->afr;

	if (!isAdcChannelValid(type == SensorType::Lambda1 ? engineConfiguration->afr.hwChannel : engineConfiguration->afr.hwChannel2)) {
		return 0;
	}

	auto volts = adcGetScaledVoltage("ego", type == SensorType::Lambda1 ? sensor->hwChannel : sensor->hwChannel2);

	float interpolatedAfr = interpolateMsg("AFR", sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts.value_or(0));

	switch (type) {
		case SensorType::Lambda1: {
			expAverageLambda1.setSmoothingFactor(engineConfiguration->afrExpAverageAlpha);
			smoothedLambda1Sensor.setValidValue(expAverageLambda1.initOrAverage(interpolatedAfr), getTimeNowNt());
			break;
		}
		case SensorType::Lambda2: {
			expAverageLambda2.setSmoothingFactor(engineConfiguration->afrExpAverageAlpha);
			smoothedLambda2Sensor.setValidValue(expAverageLambda2.initOrAverage(interpolatedAfr), getTimeNowNt());
			break;
		}
		default:
			break;
	}

	return interpolateMsg("AFR", sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts.value_or(0))
			+ engineConfiguration->egoValueShift;
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
