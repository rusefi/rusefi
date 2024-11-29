package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/nitrous_control_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class NitrousController {
	public static final Field ISARMED = Field.create("ISARMED", 0, FieldType.BIT, 0).setBaseOffset(1128);
	public static final Field ISTPSCONDITIONSATISFIED = Field.create("ISTPSCONDITIONSATISFIED", 0, FieldType.BIT, 1).setBaseOffset(1128);
	public static final Field ISCLTCONDITIONSATISFIED = Field.create("ISCLTCONDITIONSATISFIED", 0, FieldType.BIT, 2).setBaseOffset(1128);
	public static final Field ISMAPCONDITIONSATISFIED = Field.create("ISMAPCONDITIONSATISFIED", 0, FieldType.BIT, 3).setBaseOffset(1128);
	public static final Field ISAFRCONDITIONSATISFIED = Field.create("ISAFRCONDITIONSATISFIED", 0, FieldType.BIT, 4).setBaseOffset(1128);
	public static final Field[] VALUES = {
	ISARMED,
	ISTPSCONDITIONSATISFIED,
	ISCLTCONDITIONSATISFIED,
	ISMAPCONDITIONSATISFIED,
	ISAFRCONDITIONSATISFIED,
	};
}
