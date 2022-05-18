package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators\electronic_throttle.txt Thu Apr 14 20:48:22 EDT 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class ElectronicThrottle {
	public static final Field IDLEPOSITION = Field.create("IDLEPOSITION", 0, FieldType.FLOAT);
	public static final Field TRIM = Field.create("TRIM", 4, FieldType.FLOAT);
	public static final Field LUAADJUSTMENT = Field.create("LUAADJUSTMENT", 8, FieldType.FLOAT);
	public static final Field[] VALUES = {
	IDLEPOSITION,
	TRIM,
	LUAADJUSTMENT,
	};
}
