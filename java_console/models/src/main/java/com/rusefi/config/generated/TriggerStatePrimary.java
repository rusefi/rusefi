package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_state_primary.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TriggerStatePrimary {
	public static final Field CAMRESYNCCOUNTER = Field.create("CAMRESYNCCOUNTER", 0, FieldType.INT8).setScale(1.0).setBaseOffset(1572);
	public static final Field ALIGNMENTFILL_AT_1 = Field.create("ALIGNMENTFILL_AT_1", 1, FieldType.INT8).setScale(1.0).setBaseOffset(1572);
	public static final Field M_HASSYNCHRONIZEDPHASE = Field.create("M_HASSYNCHRONIZEDPHASE", 4, FieldType.BIT, 0).setBaseOffset(1572);
	public static final Field[] VALUES = {
	CAMRESYNCCOUNTER,
	ALIGNMENTFILL_AT_1,
	M_HASSYNCHRONIZEDPHASE,
	};
}
