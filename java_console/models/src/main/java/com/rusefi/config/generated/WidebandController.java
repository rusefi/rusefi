package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/sensors//wideband_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class WidebandController {
	public static final Field FAULTCODE = Field.create("FAULTCODE", 0, FieldType.INT8).setScale(1.0).setBaseOffset(1768);
	public static final Field HEATERDUTY = Field.create("HEATERDUTY", 1, FieldType.INT8).setScale(1.0).setBaseOffset(1768);
	public static final Field PUMPDUTY = Field.create("PUMPDUTY", 2, FieldType.INT8).setScale(1.0).setBaseOffset(1768);
	public static final Field ALIGNMENTFILL_AT_3 = Field.create("ALIGNMENTFILL_AT_3", 3, FieldType.INT8).setScale(1.0).setBaseOffset(1768);
	public static final Field TEMPC = Field.create("TEMPC", 4, FieldType.INT16).setScale(1.0).setBaseOffset(1768);
	public static final Field NERNSTVOLTAGE = Field.create("NERNSTVOLTAGE", 6, FieldType.INT16).setScale(0.001).setBaseOffset(1768);
	public static final Field ESR = Field.create("ESR", 8, FieldType.INT16).setScale(1.0).setBaseOffset(1768);
	public static final Field ALIGNMENTFILL_AT_10 = Field.create("ALIGNMENTFILL_AT_10", 10, FieldType.INT8).setScale(1.0).setBaseOffset(1768);
	public static final Field[] VALUES = {
	FAULTCODE,
	HEATERDUTY,
	PUMPDUTY,
	ALIGNMENTFILL_AT_3,
	TEMPC,
	NERNSTVOLTAGE,
	ESR,
	ALIGNMENTFILL_AT_10,
	};
}
