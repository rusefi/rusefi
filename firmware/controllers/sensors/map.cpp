/**
 * @file map.cpp
 *
 * See also map_averaging.cpp
 *
 * @author Andrey Belomutskiy, (c) 2012-2018
 */
#include "main.h"
#include "engine_configuration.h"
#include "engine_math.h"
#include "analog_input.h"
#include "interpolation.h"
#include "error_handling.h"
#include "map.h"
#include "engine_controller.h"

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

// See 'useFixedBaroCorrFromMap'
static float storedInitialBaroPressure = NAN;

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

static FastInterpolation mpx4250A(0.25, 20, 4.875, 250);

static FastInterpolation mpx4100(0.3, 20, 4.9, 105);

/**
 * http://easyautodiagnostics.com/chrysler/2.0L-2.4L/map-sensor-diagnostic-test-1
 * or maybe
 * https://books.google.com/books?id=3q85p56_PxIC page 132
 * https://books.google.com/books?id=3q85p56_PxIC&q=chrysler+map#v=snippet&q=chrysler%20map&f=false
 */
//static FastInterpolation dodgeNeon2003(0.5 /* volts */, 0 /* kPa */, 4.5 /* volts */ , 100 /* kPa */);
static FastInterpolation dodgeNeon2003(0.4 /* volts */, 15.34 /* kPa */, 4.5 /* volts */ , 100 /* kPa */);

static FastInterpolation densoToyota(3.7 - 2 /* volts */, 33.322271 /* kPa */, 3.7 /* volts */ , 100 /* kPa */);

/**
 * We hold a reference to current decoder to reduce code branching
 * to lookup decoder each time we need to decode
 */
static FastInterpolation *mapDecoder;

static FastInterpolation *getDecoder(air_pressure_sensor_type_e type);

float decodePressure(float voltage, air_pressure_sensor_config_s * mapConfig DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (mapConfig->type) {
	case MT_CUSTOM:
		// todo: migrate to 'FastInterpolation customMap'
		return interpolate(engineConfiguration->mapLowValueVoltage, mapConfig->lowValue,
				engineConfiguration->mapHighValueVoltage, mapConfig->highValue, voltage);
	case MT_DENSO183:
	case MT_MPX4250:
	case MT_MPX4250A:
	case MT_HONDA3BAR:
	case MT_DODGE_NEON_2003:
	case MT_SUBY_DENSO:
	case MT_GM_3_BAR:
	case MT_TOYOTA_89420_02010:
	case MT_MPX4100:
		return getDecoder(mapConfig->type)->getValue(voltage);
	default:
		firmwareError(CUSTOM_ERR_MAP_TYPE, "Unknown MAP type: p %d", mapConfig->type);
		return NAN;
	}
}

/**
 * This function adds an error if MAP sensor value is outside of expected range
 * @return unchanged mapKPa parameter
 */
float validateMap(float mapKPa DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (cisnan(mapKPa) || mapKPa < CONFIG(mapErrorDetectionTooLow) || mapKPa > CONFIG(mapErrorDetectionTooHigh)) {
		warning(OBD_Manifold_Absolute_Pressure_Circuit_Malfunction, "unexpected MAP value: %.2f", mapKPa);
		return 0;
	}
	return mapKPa;
}

/**
 * This function checks if Baro/MAP sensor value is inside of expected range
 * @return unchanged mapKPa parameter or NaN
 */
float validateBaroMap(float mapKPa DECLARE_ENGINE_PARAMETER_SUFFIX) {
	const float atmoPressure = 100.0f;
	const float atmoPressureRange = 15.0f;	// 85..115
	if (cisnan(mapKPa) || absF(mapKPa - atmoPressure) > atmoPressureRange)
		return NAN;
	return mapKPa;
}

/**
 * @brief	MAP value decoded according to current settings
 * @returns kPa value
 */
float getMapByVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENABLE_MOCK_ADC || defined(__DOXYGEN__)
	int mapChannel = engineConfiguration->map.sensor.hwChannel;
	if (engine->engineState.mockAdcState.hasMockAdc[mapChannel])
		voltage = adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(mapChannel) * engineConfiguration->analogInputDividerCoefficient);
#endif

	// todo: migrate to mapDecoder once parameter listeners are ready
	air_pressure_sensor_config_s * apConfig = &engineConfiguration->map.sensor;
	return decodePressure(voltage, apConfig PASS_ENGINE_PARAMETER_SUFFIX);
}

/**
 * @return Manifold Absolute Pressure, in kPa
 */
float getRawMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		return interpolate(boardConfiguration->mapFrequency0Kpa, 0, boardConfiguration->mapFrequency100Kpa, 100, mapFreq);
	}

	float voltage = getVoltageDivided("map", engineConfiguration->map.sensor.hwChannel);
	return getMapByVoltage(voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

/**
 * Returns true if a real Baro sensor is present.
 * Also if 'useFixedBaroCorrFromMap' option is enabled, and we have the initial pressure value stored and passed validation.
 */
bool hasBaroSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->baroSensor.hwChannel != EFI_ADC_NONE || !cisnan(storedInitialBaroPressure);
}

bool hasMapSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->map.sensor.hwChannel != EFI_ADC_NONE;
}

float getBaroPressure(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// Override the real Baro sensor with the stored initial MAP value, if the option is set.
	if (CONFIG(useFixedBaroCorrFromMap))
		return storedInitialBaroPressure;
	float voltage = getVoltageDivided("baro", engineConfiguration->baroSensor.hwChannel);
	return decodePressure(voltage, &engineConfiguration->baroSensor PASS_ENGINE_PARAMETER_SUFFIX);
}

static FastInterpolation *getDecoder(air_pressure_sensor_type_e type) {
	switch (type) {
	case MT_CUSTOM:
		return &customMap;
	case MT_DENSO183:
		return &denso183;
	case MT_MPX4250:
		return &mpx4250;
	case MT_MPX4250A:
		return &mpx4250A;
	case MT_HONDA3BAR:
		return &honda3bar;
	case MT_DODGE_NEON_2003:
		return &dodgeNeon2003;
	case MT_SUBY_DENSO:
		return &subyDenso;
	case MT_GM_3_BAR:
		return &gm3bar;
	case MT_TOYOTA_89420_02010:
		return &densoToyota;
	default:
		firmwareError(CUSTOM_ERR_MAP_TYPE, "Unknown MAP type: d %d", type);
		return &customMap;
	}
}

static void applyConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	air_pressure_sensor_config_s * apConfig = &engineConfiguration->map.sensor;
	customMap.init(0, apConfig->lowValue, 5, apConfig->highValue);
	mapDecoder = getDecoder(engineConfiguration->map.sensor.type);
}

static void digitalMapWidthCallback(void) {
	efitick_t nowNt = getTimeNowNt();

	mapFreq = 1000000.0 / NT2US(nowNt - prevWidthTimeNt);

	prevWidthTimeNt = nowNt;
}

#if EFI_PROD_CODE || defined(__DOXYGEN__)

extern int mapMinBufferLength;

static void printMAPInfo(void) {
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	scheduleMsg(logger, "instant value=%.2fkPa", getRawMap());


	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		scheduleMsg(logger, "instant value=%.2fHz @ %s", mapFreq, hwPortname(boardConfiguration->frequencyReportingMapInputPin));
	} else {
		scheduleMsg(logger, "map type=%d/%s MAP=%.2fkPa mapMinBufferLength=%d", engineConfiguration->map.sensor.type,
				getAir_pressure_sensor_type_e(engineConfiguration->map.sensor.type),
				getMap(),
				mapMinBufferLength);

		adc_channel_e mapAdc = engineConfiguration->map.sensor.hwChannel;
		static char pinNameBuffer[16];

		scheduleMsg(logger, "MAP %.2fv @%s", getVoltage("mapinfo", mapAdc),
				getPinNameByAdcChannel("map", mapAdc, pinNameBuffer));
		if (engineConfiguration->map.sensor.type == MT_CUSTOM) {
			scheduleMsg(logger, "at %.2fv=%.2f at %.2fv=%.2f",
					engineConfiguration->mapLowValueVoltage,
					engineConfiguration->map.sensor.lowValue,
					engineConfiguration->mapHighValueVoltage,
					engineConfiguration->map.sensor.highValue);
		}
	}

	if (hasBaroSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		scheduleMsg(logger, "baro type=%d value=%.2f", engineConfiguration->baroSensor.type, getBaroPressure());
		if (engineConfiguration->baroSensor.type == MT_CUSTOM) {
			scheduleMsg(logger, "min=%.2f@%.2f max=%.2f@%.2f",
					engineConfiguration->baroSensor.lowValue,
					engineConfiguration->mapLowValueVoltage,
					engineConfiguration->baroSensor.highValue,
					engineConfiguration->mapHighValueVoltage);
		}
	}
#endif /* EFI_ANALOG_SENSORS */
}
#endif /* EFI_PROD_CODE */


void initMapDecoder(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;
	applyConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
	//engine->configurationListeners.registerCallback(applyConfiguration);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		digital_input_s* digitalMapInput = addWaveAnalyzerDriver("map freq", boardConfiguration->frequencyReportingMapInputPin);
		startInputDriver(digitalMapInput, true);

		digitalMapInput->widthListeners.registerCallback((VoidInt) digitalMapWidthCallback, NULL);
	}

	if (CONFIG(useFixedBaroCorrFromMap)) {
		// Read initial MAP sensor value and store it for Baro correction.
		storedInitialBaroPressure = getRawMap(PASS_ENGINE_PARAMETER_SIGNATURE);
		scheduleMsg(logger, "Get initial baro MAP pressure = %.2fkPa", storedInitialBaroPressure);
		// validate if it's within a reasonable range (the engine should not be spinning etc.)
		storedInitialBaroPressure = validateBaroMap(storedInitialBaroPressure);
		if (!cisnan(storedInitialBaroPressure)) {
			scheduleMsg(logger, "Using this fixed MAP pressure to override the baro correction!");
		} else {
			scheduleMsg(logger, "The baro pressure is invalid. The fixed baro correction will be disabled!");
		}
	}
	
	addConsoleAction("mapinfo", printMAPInfo);
#endif
}

#else /* EFI_ANALOG_SENSORS */

void initMapDecoder(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
}

#endif /* EFI_ANALOG_SENSORS */
