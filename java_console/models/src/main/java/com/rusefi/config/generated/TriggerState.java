package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Sun May 08 12:02:50 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TriggerState {
	public static final Field CURRENTGAP = Field.create("CURRENTGAP", 0, FieldType.FLOAT);
	public static final Field TOTALREVOLUTIONCOUNTER = Field.create("TOTALREVOLUTIONCOUNTER", 4, FieldType.INT).setScale(1.0);
	public static final Field VVTSYNCGAPRATIO = Field.create("VVTSYNCGAPRATIO", 8, FieldType.FLOAT);
	public static final Field VVTCURRENTPOSITION = Field.create("VVTCURRENTPOSITION", 12, FieldType.FLOAT);
	public static final Field TRIGGERSTATEINDEX = Field.create("TRIGGERSTATEINDEX", 16, FieldType.INT8).setScale(1.0);
	public static final Field VVTCOUNTER = Field.create("VVTCOUNTER", 17, FieldType.INT8).setScale(1.0);
	public static final Field VVTSYNCCOUNTER = Field.create("VVTSYNCCOUNTER", 18, FieldType.INT8).setScale(1.0);
	public static final Field VVTSTATEINDEX = Field.create("VVTSTATEINDEX", 19, FieldType.INT8).setScale(1.0);
	public static final Field[] VALUES = {
	CURRENTGAP,
	TOTALREVOLUTIONCOUNTER,
	VVTSYNCGAPRATIO,
	VVTCURRENTPOSITION,
	TRIGGERSTATEINDEX,
	VVTCOUNTER,
	VVTSYNCCOUNTER,
	VVTSTATEINDEX,
	};
}
