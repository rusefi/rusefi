package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/ignition_state.txt Mon Aug 15 21:21:43 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class IgnitionState {
	public static final Field BASEDWELL = Field.create("BASEDWELL", 0, FieldType.FLOAT);
	public static final Field DWELLVOLTAGECORRECTION = Field.create("DWELLVOLTAGECORRECTION", 4, FieldType.FLOAT);
	public static final Field LUATIMINGADD = Field.create("LUATIMINGADD", 8, FieldType.FLOAT);
	public static final Field LUATIMINGMULT = Field.create("LUATIMINGMULT", 12, FieldType.FLOAT);
	public static final Field[] VALUES = {
	BASEDWELL,
	DWELLVOLTAGECORRECTION,
	LUATIMINGADD,
	LUATIMINGMULT,
	};
}
