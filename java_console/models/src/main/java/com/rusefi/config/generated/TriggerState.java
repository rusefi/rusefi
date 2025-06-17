package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TriggerState {
	public static final Field SYNCHRONIZATIONCOUNTER = Field.create("SYNCHRONIZATIONCOUNTER", 0, FieldType.INT).setScale(1.0).setBaseOffset(1444);
	public static final Field VVTTOOTHDURATIONS0 = Field.create("VVTTOOTHDURATIONS0", 4, FieldType.INT).setScale(1.0).setBaseOffset(1444);
	public static final Field VVTCURRENTPOSITION = Field.create("VVTCURRENTPOSITION", 8, FieldType.FLOAT).setBaseOffset(1444);
	public static final Field VVTTOOTHPOSITION1 = Field.create("VVTTOOTHPOSITION1", 12, FieldType.FLOAT).setBaseOffset(1444);
	public static final Field VVTTOOTHPOSITION2 = Field.create("VVTTOOTHPOSITION2", 16, FieldType.FLOAT).setBaseOffset(1444);
	public static final Field VVTTOOTHPOSITION3 = Field.create("VVTTOOTHPOSITION3", 20, FieldType.FLOAT).setBaseOffset(1444);
	public static final Field VVTTOOTHPOSITION4 = Field.create("VVTTOOTHPOSITION4", 24, FieldType.FLOAT).setBaseOffset(1444);
	public static final Field TRIGGERSYNCGAPRATIO = Field.create("TRIGGERSYNCGAPRATIO", 28, FieldType.FLOAT).setBaseOffset(1444);
	public static final Field TRIGGERSTATEINDEX = Field.create("TRIGGERSTATEINDEX", 32, FieldType.INT8).setScale(1.0).setBaseOffset(1444);
	public static final Field ALIGNMENTFILL_AT_33 = Field.create("ALIGNMENTFILL_AT_33", 33, FieldType.INT8).setScale(1.0).setBaseOffset(1444);
	public static final Field[] VALUES = {
	SYNCHRONIZATIONCOUNTER,
	VVTTOOTHDURATIONS0,
	VVTCURRENTPOSITION,
	VVTTOOTHPOSITION1,
	VVTTOOTHPOSITION2,
	VVTTOOTHPOSITION3,
	VVTTOOTHPOSITION4,
	TRIGGERSYNCGAPRATIO,
	TRIGGERSTATEINDEX,
	ALIGNMENTFILL_AT_33,
	};
}
