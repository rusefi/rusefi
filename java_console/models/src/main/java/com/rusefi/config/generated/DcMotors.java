package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/dc_motors.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class DcMotors {
	public static final Field DCOUTPUT0 = Field.create("DCOUTPUT0", 0, FieldType.FLOAT).setBaseOffset(1748);
	public static final Field ISENABLED0_INT = Field.create("ISENABLED0_INT", 4, FieldType.INT8).setScale(1.0).setBaseOffset(1748);
	public static final Field ALIGNMENTFILL_AT_5 = Field.create("ALIGNMENTFILL_AT_5", 5, FieldType.INT8).setScale(1.0).setBaseOffset(1748);
	public static final Field ISENABLED0 = Field.create("ISENABLED0", 8, FieldType.BIT, 0).setBaseOffset(1748);
	public static final Field[] VALUES = {
	DCOUTPUT0,
	ISENABLED0_INT,
	ALIGNMENTFILL_AT_5,
	ISENABLED0,
	};
}
