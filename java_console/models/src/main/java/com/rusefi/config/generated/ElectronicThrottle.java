package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Mon Aug 15 21:21:43 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class ElectronicThrottle {
	public static final Field IDLEPOSITION = Field.create("IDLEPOSITION", 0, FieldType.FLOAT);
	public static final Field TRIM = Field.create("TRIM", 4, FieldType.FLOAT);
	public static final Field LUAADJUSTMENT = Field.create("LUAADJUSTMENT", 8, FieldType.FLOAT);
	public static final Field ETBFEEDFORWARD = Field.create("ETBFEEDFORWARD", 12, FieldType.FLOAT);
	public static final Field ETBINTEGRALERROR = Field.create("ETBINTEGRALERROR", 16, FieldType.FLOAT);
	public static final Field ETBCURRENTTARGET = Field.create("ETBCURRENTTARGET", 20, FieldType.FLOAT);
	public static final Field[] VALUES = {
	IDLEPOSITION,
	TRIM,
	LUAADJUSTMENT,
	ETBFEEDFORWARD,
	ETBINTEGRALERROR,
	ETBCURRENTTARGET,
	};
}
