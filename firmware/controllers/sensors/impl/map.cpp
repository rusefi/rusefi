/**
 * @file map.cpp
 *
 * See also map_averaging.cpp
 *
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "pch.h"

#if EFI_ANALOG_SENSORS

/**
 * This function checks if Baro/MAP sensor value is inside of expected range
 * @return unchanged mapKPa parameter or NaN
 */
static float validateBaroMap(float mapKPa) {
	// Highest interstate is the Eisenhower Tunnel at 11158 feet -> 66 kpa
	// Lowest point is the Dead Sea, -1411 feet -> 106 kpa
	if (std::isnan(mapKPa) || mapKPa > 110 || mapKPa < 60) {
		warning(ObdCode::OBD_Barometric_Press_Circ, "Invalid start-up baro pressure = %.2fkPa", mapKPa);
		return NAN;
	}
	return mapKPa;
}

#if EFI_PROD_CODE

static void printMAPInfo() {
#if EFI_ANALOG_SENSORS
	efiPrintf("instant value=%.2fkPa", Sensor::getOrZero(SensorType::Map));

#if EFI_MAP_AVERAGING && defined (MODULE_MAP_AVERAGING)
	efiPrintf("map type=%d/%s MAP=%.2fkPa", engineConfiguration->map.sensor.type,
			getAir_pressure_sensor_type_e(engineConfiguration->map.sensor.type),
			Sensor::getOrZero(SensorType::Map));
#endif // EFI_MAP_AVERAGING

	adc_channel_e mapAdc = engineConfiguration->map.sensor.hwChannel;
	char pinNameBuffer[16];

	efiPrintf("MAP %.2fv @%s",
			adcGetRawVoltage("mapinfo", mapAdc).value_or(0),
			getPinNameByAdcChannel("map", mapAdc, pinNameBuffer, sizeof(pinNameBuffer)));
	if (engineConfiguration->map.sensor.type == MT_CUSTOM) {
		efiPrintf("at %.2fv=%.2f at %.2fv=%.2f",
				engineConfiguration->mapLowValueVoltage,
				engineConfiguration->map.sensor.lowValue,
				engineConfiguration->mapHighValueVoltage,
				engineConfiguration->map.sensor.highValue);
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

// How long after start-up we keep waiting for the first valid MAP sample before giving up on
// the "grab baro value from MAP" feature. The slow MAP channel is sampled at 100Hz, so this is
// only about tolerating a slow ADC/sensor start-up, not about a running engine.
static constexpr float baroFromMapTimeoutSec = 3;

static bool baroFromMapPending = false;
static Timer baroFromMapTimer;

/**
 * Latch the current MAP reading as the fixed barometric pressure.
 *
 * This has to happen from the slow callback and not from initMapDecoder(): sensors are registered
 * just a few instructions earlier, so at init time the ADC has not delivered a single MAP sample
 * yet and MapSlow is still invalid. Defaulting to STD_ATMOSPHERE back then latched 101.325kPa
 * forever, no matter what the MAP sensor reported a moment later - see #9744.
 */
void updateFixedBaroFromMap() {
	if (!baroFromMapPending) {
		return;
	}

	// MAP only reads atmospheric pressure while the engine is not turning
	if (Sensor::getOrZero(SensorType::Rpm) > 0) {
		baroFromMapPending = false;
		efiPrintf("Engine started before a valid MAP sample, the fixed baro correction will be disabled!");
		return;
	}

	SensorResult mapKPa = Sensor::get(SensorType::MapSlow);
	if (!mapKPa) {
		if (baroFromMapTimer.hasElapsedSec(baroFromMapTimeoutSec)) {
			baroFromMapPending = false;
			warning(ObdCode::OBD_Barometric_Press_Circ, "No valid MAP sample for start-up baro pressure");
		}
		return;
	}

	// whatever this sample says, we only get one shot at it
	baroFromMapPending = false;

	efiPrintf("Get initial baro MAP pressure = %.2fkPa", mapKPa.Value);
	// validate if it's within a reasonable range (the engine should not be spinning etc.)
	float storedInitialBaroPressure = validateBaroMap(mapKPa.Value);
	if (!std::isnan(storedInitialBaroPressure)) {
		efiPrintf("Using this fixed MAP pressure to override the baro correction!");

		// TODO: do literally anything other than this
		Sensor::setMockValue(SensorType::BarometricPressure, storedInitialBaroPressure);
	} else {
		efiPrintf("The baro pressure is invalid. The fixed baro correction will be disabled!");
	}
}

void initMapDecoder() {
	// the actual grab is deferred to updateFixedBaroFromMap(), MAP has no valid sample yet
	baroFromMapPending = engineConfiguration->useFixedBaroCorrFromMap;
	baroFromMapTimer.reset();

#if EFI_PROD_CODE
	addConsoleAction("mapinfo", printMAPInfo);
#endif
}

#else /* EFI_ANALOG_SENSORS */

void initMapDecoder() {
}

void updateFixedBaroFromMap() {
}

#endif /* EFI_ANALOG_SENSORS */
