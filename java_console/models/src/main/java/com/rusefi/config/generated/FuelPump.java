package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/fuel_pump_control.txt Wed Dec 21 23:51:25 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class FuelPump {
	public static final Field ISPRIME = Field.create("ISPRIME", 0, FieldType.BIT, 0).setBaseOffset(876);
	public static final Field ENGINETURNEDRECENTLY = Field.create("ENGINETURNEDRECENTLY", 0, FieldType.BIT, 1).setBaseOffset(876);
	public static final Field ISFUELPUMPON = Field.create("ISFUELPUMPON", 0, FieldType.BIT, 2).setBaseOffset(876);
	public static final Field IGNITIONON = Field.create("IGNITIONON", 0, FieldType.BIT, 3).setBaseOffset(876);
	public static final Field[] VALUES = {
	ISPRIME,
	ENGINETURNEDRECENTLY,
	ISFUELPUMPON,
	IGNITIONON,
	};
}
