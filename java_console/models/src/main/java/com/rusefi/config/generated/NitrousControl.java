package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/nitrous_control_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class NitrousControl {
	public static final Field ISNITROUSARMED = Field.create("ISNITROUSARMED", 0, FieldType.BIT, 0).setBaseOffset(1176);
	public static final Field ISNITROUSSPEEDCONDITION = Field.create("ISNITROUSSPEEDCONDITION", 0, FieldType.BIT, 1).setBaseOffset(1176);
	public static final Field ISNITROUSTPSCONDITION = Field.create("ISNITROUSTPSCONDITION", 0, FieldType.BIT, 2).setBaseOffset(1176);
	public static final Field ISNITROUSCLTCONDITION = Field.create("ISNITROUSCLTCONDITION", 0, FieldType.BIT, 3).setBaseOffset(1176);
	public static final Field ISNITROUSMAPCONDITION = Field.create("ISNITROUSMAPCONDITION", 0, FieldType.BIT, 4).setBaseOffset(1176);
	public static final Field ISNITROUSAFRCONDITION = Field.create("ISNITROUSAFRCONDITION", 0, FieldType.BIT, 5).setBaseOffset(1176);
	public static final Field ISNITROUSRPMCONDITION = Field.create("ISNITROUSRPMCONDITION", 0, FieldType.BIT, 6).setBaseOffset(1176);
	public static final Field ISNITROUSCONDITION = Field.create("ISNITROUSCONDITION", 0, FieldType.BIT, 7).setBaseOffset(1176);
	public static final Field[] VALUES = {
	ISNITROUSARMED,
	ISNITROUSSPEEDCONDITION,
	ISNITROUSTPSCONDITION,
	ISNITROUSCLTCONDITION,
	ISNITROUSMAPCONDITION,
	ISNITROUSAFRCONDITION,
	ISNITROUSRPMCONDITION,
	ISNITROUSCONDITION,
	};
}
