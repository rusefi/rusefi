/**
 * @file map.cpp
 *
 * See also map_averaging.cpp
 *
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "engine_configuration.h"
#include "engine_math.h"
#include "adc_inputs.h"
#include "interpolation.h"
#include "map.h"
#include "engine_controller.h"
#include "sensor.h"

#if EFI_PROD_CODE
#include "digital_input_icu.h"
#include "digital_input_exti.h"
#include "pin_repository.h"
#endif

#if EFI_ANALOG_SENSORS

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

static FastInterpolation gm2bar(0, 8.8, 5, 208);

static FastInterpolation gm1bar(0, 10, 5, 105);

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
 * Open question how to get this Miata NB2 sensor read MAP
 */
static FastInterpolation mazda1bar(0 /* volts */, 15 /* kPa */, 5 /* volts */ , 115 /* kPa */);

/**
 * Bosch 2.5 Bar TMap Map Sensor with IAT
 */
static FastInterpolation bosch2_5(0.4 /* volts */, 20 /* kPa */, 4.65 /* volts */ , 250 /* kPa */);

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
		return interpolateMsg("map", engineConfiguration->mapLowValueVoltage, mapConfig->lowValue,
				engineConfiguration->mapHighValueVoltage, mapConfig->highValue, voltage);
	case MT_DENSO183:
	case MT_MPX4250:
	case MT_MPX4250A:
	case MT_HONDA3BAR:
	case MT_DODGE_NEON_2003:
	case MT_SUBY_DENSO:
	case MT_GM_3_BAR:
	case MT_GM_2_BAR:
	case MT_GM_1_BAR:
	case MT_TOYOTA_89420_02010:
	case MT_MPX4100:
	case MT_BOSCH_2_5:
	case MT_MAZDA_1_BAR:
		return getDecoder(mapConfig->type)->getValue(voltage);
	default:
		firmwareError(CUSTOM_ERR_MAP_TYPE, "Unknown MAP type: pressure %d", mapConfig->type);
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
static float validateBaroMap(float mapKPa DECLARE_ENGINE_PARAMETER_SUFFIX) {
	const float atmoPressure = 100.0f;
	const float atmoPressureRange = 15.0f;	// 85..115
	if (cisnan(mapKPa) || absF(mapKPa - atmoPressure) > atmoPressureRange) {
		warning(OBD_Barometric_Press_Circ, "Invalid start-up baro pressure = %.2fkPa", mapKPa);
		return NAN;
	}
	return mapKPa;
}

/**
 * @brief	MAP value decoded according to current settings
 * @returns kPa value
 */
float getMapByVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENABLE_MOCK_ADC
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
		return interpolateMsg("rmap", CONFIG(mapFrequency0Kpa), 0, CONFIG(mapFrequency100Kpa), 100, mapFreq);
	}

	float voltage = getVoltageDivided("map", engineConfiguration->map.sensor.hwChannel PASS_ENGINE_PARAMETER_SUFFIX);
	return getMapByVoltage(voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

bool hasMapSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return isAdcChannelValid(engineConfiguration->map.sensor.hwChannel);
}

float getBaroPressure(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// Override the real Baro sensor with the stored initial MAP value, if the option is set.
	if (CONFIG(useFixedBaroCorrFromMap))
		return storedInitialBaroPressure;
	float voltage = getVoltageDivided("baro", engineConfiguration->baroSensor.hwChannel PASS_ENGINE_PARAMETER_SUFFIX);
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
	case MT_MPX4100:
		return &mpx4100;
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
	case MT_GM_2_BAR:
		return &gm2bar;
	case MT_GM_1_BAR:
		return &gm1bar;
	case MT_TOYOTA_89420_02010:
		return &densoToyota;
	case MT_MAZDA_1_BAR:
		return &mazda1bar;
	case MT_BOSCH_2_5:
		return &bosch2_5;
	default:
		firmwareError(CUSTOM_ERR_MAP_TYPE, "Unknown MAP type: decoder %d", type);
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

#if EFI_PROD_CODE

extern int mapMinBufferLength;

static void printMAPInfo(void) {
#if EFI_ANALOG_SENSORS
	efiPrintf("instant value=%.2fkPa", getRawMap());


	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		efiPrintf("instant value=%.2fHz @ %s", mapFreq, hwPortname(CONFIG(frequencyReportingMapInputPin)));
	} else {
#if EFI_MAP_AVERAGING
		efiPrintf("map type=%d/%s MAP=%.2fkPa mapMinBufferLength=%d", engineConfiguration->map.sensor.type,
				getAir_pressure_sensor_type_e(engineConfiguration->map.sensor.type),
				getMap(),
				mapMinBufferLength);
#endif // EFI_MAP_AVERAGING

		adc_channel_e mapAdc = engineConfiguration->map.sensor.hwChannel;
		static char pinNameBuffer[16];

		efiPrintf("MAP %.2fv @%s", getVoltage("mapinfo", mapAdc PASS_ENGINE_PARAMETER_SUFFIX),
				getPinNameByAdcChannel("map", mapAdc, pinNameBuffer));
		if (engineConfiguration->map.sensor.type == MT_CUSTOM) {
			efiPrintf("at %.2fv=%.2f at %.2fv=%.2f",
					engineConfiguration->mapLowValueVoltage,
					engineConfiguration->map.sensor.lowValue,
					engineConfiguration->mapHighValueVoltage,
					engineConfiguration->map.sensor.highValue);
		}
	}

	if (Sensor::hasSensor(SensorType::BarometricPressure)) {
		efiPrintf("baro type=%d value=%.2f", engineConfiguration->baroSensor.type, Sensor::get(SensorType::BarometricPressure).value_or(-1));
		if (engineConfiguration->baroSensor.type == MT_CUSTOM) {
			efiPrintf("min=%.2f@%.2f max=%.2f@%.2f",
					engineConfiguration->baroSensor.lowValue,
					engineConfiguration->mapLowValueVoltage,
					engineConfiguration->baroSensor.highValue,
					engineConfiguration->mapHighValueVoltage);
		}
	}
#endif /* EFI_ANALOG_SENSORS */
}
#endif /* EFI_PROD_CODE */


void initMapDecoder(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	applyConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
	//engine->configurationListeners.registerCallback(applyConfiguration);

	if (engineConfiguration->hasFrequencyReportingMapSensor) {
#if EFI_ICU_INPUTS
		digital_input_s* digitalMapInput = startDigitalCapture("MAP freq", CONFIG(frequencyReportingMapInputPin));

		digitalMapInput->setWidthCallback((VoidInt) digitalMapWidthCallback, NULL);
#else
 #if EFI_PROD_CODE
	efiExtiEnablePin(
				"Frequency MAP",
				CONFIG(frequencyReportingMapInputPin),
				PAL_EVENT_MODE_RISING_EDGE,
				(palcallback_t)digitalMapWidthCallback,
				nullptr
			);
 #endif /* EFI_PROD_CODE */
#endif /* HAL_USE_ICU */
	}

	if (CONFIG(useFixedBaroCorrFromMap)) {
		// Read initial MAP sensor value and store it for Baro correction.
		storedInitialBaroPressure = getRawMap(PASS_ENGINE_PARAMETER_SIGNATURE);
		efiPrintf("Get initial baro MAP pressure = %.2fkPa", storedInitialBaroPressure);
		// validate if it's within a reasonable range (the engine should not be spinning etc.)
		storedInitialBaroPressure = validateBaroMap(storedInitialBaroPressure PASS_ENGINE_PARAMETER_SUFFIX);
		if (!cisnan(storedInitialBaroPressure)) {
			efiPrintf("Using this fixed MAP pressure to override the baro correction!");
		} else {
			efiPrintf("The baro pressure is invalid. The fixed baro correction will be disabled!");
		}
	}
	
#if EFI_PROD_CODE
	addConsoleAction("mapinfo", printMAPInfo);
#endif
}

#else /* EFI_ANALOG_SENSORS */

void initMapDecoder(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
}

#endif /* EFI_ANALOG_SENSORS */
