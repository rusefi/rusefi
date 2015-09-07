#include "main.h"
#include "engine_configuration.h"
#include "engine_math.h"
#include "adc_inputs.h"
#include "interpolation.h"
#include "error_handling.h"
#include "map.h"

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "digital_input_hw.h"
#include "pin_repository.h"
#endif

#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)

EXTERN_ENGINE;

static Logging *logger;

static FastInterpolation customMap;
static efitick_t prevWidthTimeNt = 0;

static float mapFreq = 0;

/**
 * @brief	MAP value decoded for a 1.83 Honda sensor
 * -6.64kPa at zero volts
 * 182.78kPa at 5 volts
 *
 * about 3 volts at 100kPa
 *
 * @returns kPa value
 */
static FastInterpolation denso183(0, -6.64, 5, 182.78);

/**
 * MAP sensor output voltage of 3.0v = a gauge reading of 0 in. Hg
 * MAP sensor output voltage of 0.5v = a gauge reading of 27 in. Hg
 */
static FastInterpolation honda3bar(0.5, 91.422, 3.0, 0);

static FastInterpolation subyDenso(0, 0, 5, 200);

static FastInterpolation gm3bar(0.631, 40, 4.914, 304);

static FastInterpolation mpx4250(0, 8, 5, 260);

static FastInterpolation mpx4100(0.3, 20, 4.9, 105);

static FastInterpolation dodgeNeon2003(0.5, 0, 4.5, 100);

/**
 * We hold a reference to current decoder to reduce code branching
 * to lookup decoder each time we need to decode
 */
static FastInterpolation *mapDecoder;

float decodePressure(float voltage, air_pressure_sensor_config_s * config) {
	switch (config->type) {
	case MT_CUSTOM:
		// todo: migrate to 'FastInterpolation customMap'
		return interpolate(0, config->valueAt0, 5, config->valueAt5, voltage);
	case MT_DENSO183:
		return denso183.getValue(voltage);
	case MT_MPX4250:
		return mpx4250.getValue(voltage);
	case MT_HONDA3BAR:
		return honda3bar.getValue(voltage);
	case MT_DODGE_NEON_2003:
		return dodgeNeon2003.getValue(voltage);
	case MT_SUBY_DENSO:
		return subyDenso.getValue(voltage);
	case MT_GM_3_BAR:
		return gm3bar.getValue(voltage);
	case MT_MPX4100:
		return mpx4100.getValue(voltage);
	default:
		firmwareError("Unknown MAP type: %d", config->type);
		return NAN;
	}
}

/**
 * This function adds an error if MAP sensor value is outside of expected range
 * @return unchanged mapKPa parameter
 */
float validateMap(float mapKPa DECLARE_ENGINE_PARAMETER_S) {
	if (cisnan(mapKPa) || mapKPa < CONFIG(mapErrorLowValue) || mapKPa > CONFIG(mapErrorHighValue)) {
		warning(OBD_PCM_Processor_Fault, "invalid MAP value: %f", mapKPa);
		return 0;
	}
	return mapKPa;
}

/**
 * @brief	MAP value decoded according to current settings
 * @returns kPa value
 */
float getMapByVoltage(float voltage DECLARE_ENGINE_PARAMETER_S) {
	// todo: migrate to mapDecoder once parameter listeners are ready
	air_pressure_sensor_config_s * apConfig = &engineConfiguration->map.sensor;
	return decodePressure(voltage, apConfig);
}

/**
 * @return Manifold Absolute Pressure, in kPa
 */
float getRawMap(DECLARE_ENGINE_PARAMETER_F) {
	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		return interpolate(boardConfiguration->mapFrequency0Kpa, 0, boardConfiguration->mapFrequency100Kpa, 100, mapFreq);
	}

	float voltage = getVoltageDivided("map", engineConfiguration->map.sensor.hwChannel);
	return getMapByVoltage(voltage PASS_ENGINE_PARAMETER);
}

bool_t hasBaroSensor(DECLARE_ENGINE_PARAMETER_F) {
	return engineConfiguration->hasBaroSensor && engineConfiguration->baroSensor.hwChannel != EFI_ADC_NONE;
}

float getBaroPressure(DECLARE_ENGINE_PARAMETER_F) {
	float voltage = getVoltageDivided("baro", engineConfiguration->baroSensor.hwChannel);
	return decodePressure(voltage, &engineConfiguration->baroSensor);
}

static FastInterpolation *getDecoder(air_pressure_sensor_type_e type) {
	switch (type) {
	case MT_CUSTOM:
		return &customMap;
	case MT_DENSO183:
		return &denso183;
	case MT_MPX4250:
		return &mpx4250;
	case MT_HONDA3BAR:
		return &honda3bar;
	case MT_DODGE_NEON_2003:
		return &dodgeNeon2003;
	case MT_SUBY_DENSO:
		return &subyDenso;
	case MT_GM_3_BAR:
		return &gm3bar;
	default:
		firmwareError("Unknown MAP type: %d", type);
		return &customMap;
	}
}

static void applyConfiguration(DECLARE_ENGINE_PARAMETER_F) {
	air_pressure_sensor_config_s * apConfig = &engineConfiguration->map.sensor;
	customMap.init(0, apConfig->valueAt0, 5, apConfig->valueAt5);
	mapDecoder = getDecoder(engineConfiguration->map.sensor.type);
}

static void digitalMapWidthCallback(void) {
	efitick_t nowNt = getTimeNowNt();

	mapFreq = 1000000.0 / NT2US(nowNt - prevWidthTimeNt);

	prevWidthTimeNt = nowNt;
}

#if EFI_PROD_CODE || defined(__DOXYGEN__)
static void printMAPInfo(void) {
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	scheduleMsg(logger, "instant value=%fkPa", getRawMap());

	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		scheduleMsg(logger, "instant value=%fHz @ %s", mapFreq, hwPortname(boardConfiguration->frequencyReportingMapInputPin));
	} else {
		scheduleMsg(logger, "map type=%d/%s MAP=%fkPa", engineConfiguration->map.sensor.type,
				getAir_pressure_sensor_type_e(engineConfiguration->map.sensor.type),
				getMap());

		if (engineConfiguration->map.sensor.type == MT_CUSTOM) {
			scheduleMsg(logger, "at0=%f at5=%f", engineConfiguration->map.sensor.valueAt0,
					engineConfiguration->map.sensor.valueAt5);
		}
	}

	if (hasBaroSensor(PASS_ENGINE_PARAMETER_F)) {
		scheduleMsg(logger, "baro type=%d value=%f", engineConfiguration->baroSensor.type, getBaroPressure());
		if (engineConfiguration->baroSensor.type == MT_CUSTOM) {
			scheduleMsg(logger, "min=%f max=%f", engineConfiguration->baroSensor.valueAt0,
					engineConfiguration->baroSensor.valueAt5);
		}
	}
#endif /* EFI_ANALOG_SENSORS */
}
#endif /* EFI_PROD_CODE */


void initMapDecoder(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
	logger = sharedLogger;
	applyConfiguration(PASS_ENGINE_PARAMETER_F);
	//engine->configurationListeners.registerCallback(applyConfiguration);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		digital_input_s* digitalMapInput = initWaveAnalyzerDriver("map freq", boardConfiguration->frequencyReportingMapInputPin);
		startInputDriver(digitalMapInput, true);

		digitalMapInput->widthListeners.registerCallback((VoidInt) digitalMapWidthCallback, NULL);
	}

	addConsoleAction("mapinfo", printMAPInfo);
#endif
}

#else /* EFI_ANALOG_SENSORS */

void initMapDecoder(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
}

#endif /* EFI_ANALOG_SENSORS */
