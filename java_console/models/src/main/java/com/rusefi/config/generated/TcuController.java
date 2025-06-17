package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/tcu/tcu_controller.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TcuController {
	public static final Field TCUCURRENTGEAR = Field.create("TCUCURRENTGEAR", 0, FieldType.INT8).setScale(1.0).setBaseOffset(1084);
	public static final Field ALIGNMENTFILL_AT_1 = Field.create("ALIGNMENTFILL_AT_1", 1, FieldType.INT8).setScale(1.0).setBaseOffset(1084);
	public static final Field TCRATIO = Field.create("TCRATIO", 2, FieldType.INT16).setScale(0.01).setBaseOffset(1084);
	public static final Field LASTSHIFTTIME = Field.create("LASTSHIFTTIME", 4, FieldType.FLOAT).setBaseOffset(1084);
	public static final Field TCU_CURRENTRANGE = Field.create("TCU_CURRENTRANGE", 8, FieldType.INT8).setScale(1.0).setBaseOffset(1084);
	public static final Field PRESSURECONTROLDUTY = Field.create("PRESSURECONTROLDUTY", 9, FieldType.INT8).setScale(1.0).setBaseOffset(1084);
	public static final Field TORQUECONVERTERDUTY = Field.create("TORQUECONVERTERDUTY", 10, FieldType.INT8).setScale(1.0).setBaseOffset(1084);
	public static final Field ALIGNMENTFILL_AT_11 = Field.create("ALIGNMENTFILL_AT_11", 11, FieldType.INT8).setScale(1.0).setBaseOffset(1084);
	public static final Field ISSHIFTING = Field.create("ISSHIFTING", 12, FieldType.BIT, 0).setBaseOffset(1084);
	public static final Field[] VALUES = {
	TCUCURRENTGEAR,
	ALIGNMENTFILL_AT_1,
	TCRATIO,
	LASTSHIFTTIME,
	TCU_CURRENTRANGE,
	PRESSURECONTROLDUTY,
	TORQUECONVERTERDUTY,
	ALIGNMENTFILL_AT_11,
	ISSHIFTING,
	};
}
