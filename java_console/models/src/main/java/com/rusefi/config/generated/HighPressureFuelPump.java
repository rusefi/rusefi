package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/high_pressure_fuel_pump.txt Fri Dec 31 03:07:40 EST 2021

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class HighPressureFuelPump {
	public static final Field M_REQUESTED_PUMP = Field.create("M_REQUESTED_PUMP", 0, FieldType.FLOAT);
	public static final Field FUEL_REQUESTED_PERCENT = Field.create("FUEL_REQUESTED_PERCENT", 4, FieldType.FLOAT);
	public static final Field FUEL_REQUESTED_PERCENT_PI = Field.create("FUEL_REQUESTED_PERCENT_PI", 8, FieldType.FLOAT);
	public static final Field[] VALUES = {
	M_REQUESTED_PUMP,
	FUEL_REQUESTED_PERCENT,
	FUEL_REQUESTED_PERCENT_PI,
	};
}
