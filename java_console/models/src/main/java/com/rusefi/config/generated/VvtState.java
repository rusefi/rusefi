package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/vvt.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class VvtState {
	public static final Field VVTTARGET = Field.create("VVTTARGET", 0, FieldType.INT16).setScale(0.1).setBaseOffset(1812);
	public static final Field VVTOUTPUT = Field.create("VVTOUTPUT", 2, FieldType.INT8).setScale(0.5).setBaseOffset(1812);
	public static final Field ALIGNMENTFILL_AT_3 = Field.create("ALIGNMENTFILL_AT_3", 3, FieldType.INT8).setScale(1.0).setBaseOffset(1812);
	public static final Field[] VALUES = {
	VVTTARGET,
	VVTOUTPUT,
	ALIGNMENTFILL_AT_3,
	};
}
