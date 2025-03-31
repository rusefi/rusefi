package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/sensors/sent_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class SentState {
	public static final Field VALUE0 = Field.create("VALUE0", 0, FieldType.INT16).setScale(1.0).setBaseOffset(1812);
	public static final Field VALUE1 = Field.create("VALUE1", 2, FieldType.INT16).setScale(1.0).setBaseOffset(1812);
	public static final Field ERRORRATE = Field.create("ERRORRATE", 4, FieldType.FLOAT).setBaseOffset(1812);
	public static final Field[] VALUES = {
	VALUE0,
	VALUE1,
	ERRORRATE,
	};
}
