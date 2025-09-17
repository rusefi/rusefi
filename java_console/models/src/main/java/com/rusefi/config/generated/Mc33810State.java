package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) hw_layer/drivers/gpio//mc33810_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class Mc33810State {
	public static final Field SPARKDURATION1 = Field.create("SPARKDURATION1", 0, FieldType.FLOAT).setBaseOffset(1780);
	public static final Field SPARKDURATION2 = Field.create("SPARKDURATION2", 4, FieldType.FLOAT).setBaseOffset(1780);
	public static final Field SPARKDURATION3 = Field.create("SPARKDURATION3", 8, FieldType.FLOAT).setBaseOffset(1780);
	public static final Field SPARKDURATION4 = Field.create("SPARKDURATION4", 12, FieldType.FLOAT).setBaseOffset(1780);
	public static final Field[] VALUES = {
	SPARKDURATION1,
	SPARKDURATION2,
	SPARKDURATION3,
	SPARKDURATION4,
	};
}
