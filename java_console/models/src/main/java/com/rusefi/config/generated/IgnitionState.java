package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/ignition_state.txt Tue Jan 10 21:34:44 UTC 2023

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class IgnitionState {
	public static final Field BASEDWELL = Field.create("BASEDWELL", 0, FieldType.FLOAT).setBaseOffset(1216);
	public static final Field DWELLVOLTAGECORRECTION = Field.create("DWELLVOLTAGECORRECTION", 4, FieldType.FLOAT).setBaseOffset(1216);
	public static final Field LUATIMINGADD = Field.create("LUATIMINGADD", 8, FieldType.FLOAT).setBaseOffset(1216);
	public static final Field LUATIMINGMULT = Field.create("LUATIMINGMULT", 12, FieldType.FLOAT).setBaseOffset(1216);
	public static final Field[] VALUES = {
	BASEDWELL,
	DWELLVOLTAGECORRECTION,
	LUATIMINGADD,
	LUATIMINGMULT,
	};
}
