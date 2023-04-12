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
	if (cisnan(mapKPa) || mapKPa > 110 || mapKPa < 60) {
		warning(ObdCode::OBD_Barometric_Press_Circ, "Invalid start-up baro pressure = %.2fkPa", mapKPa);
		return NAN;
	}
	return mapKPa;
}

#if EFI_PROD_CODE

extern int mapMinBufferLength;

static void printMAPInfo() {
#if EFI_ANALOG_SENSORS
	efiPrintf("instant value=%.2fkPa", Sensor::getOrZero(SensorType::Map));

#if EFI_MAP_AVERAGING
	efiPrintf("map type=%d/%s MAP=%.2fkPa mapMinBufferLength=%d", engineConfiguration->map.sensor.type,
			getAir_pressure_sensor_type_e(engineConfiguration->map.sensor.type),
			Sensor::getOrZero(SensorType::Map),
			mapMinBufferLength);
#endif // EFI_MAP_AVERAGING

	adc_channel_e mapAdc = engineConfiguration->map.sensor.hwChannel;
	char pinNameBuffer[16];

	efiPrintf("MAP %.2fv @%s", getVoltage("mapinfo", mapAdc),
			getPinNameByAdcChannel("map", mapAdc, pinNameBuffer));
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

void initMapDecoder() {
	if (engineConfiguration->useFixedBaroCorrFromMap) {
		// Read initial MAP sensor value and store it for Baro correction.
		float storedInitialBaroPressure = Sensor::get(SensorType::MapSlow).value_or(101.325);
		efiPrintf("Get initial baro MAP pressure = %.2fkPa", storedInitialBaroPressure);
		// validate if it's within a reasonable range (the engine should not be spinning etc.)
		storedInitialBaroPressure = validateBaroMap(storedInitialBaroPressure);
		if (!cisnan(storedInitialBaroPressure)) {
			efiPrintf("Using this fixed MAP pressure to override the baro correction!");

			// TODO: do literally anything other than this
			Sensor::setMockValue(SensorType::BarometricPressure, storedInitialBaroPressure);
		} else {
			efiPrintf("The baro pressure is invalid. The fixed baro correction will be disabled!");
		}
	}

#if EFI_PROD_CODE
	addConsoleAction("mapinfo", printMAPInfo);
#endif
}

#else /* EFI_ANALOG_SENSORS */

void initMapDecoder() {
}

#endif /* EFI_ANALOG_SENSORS */
