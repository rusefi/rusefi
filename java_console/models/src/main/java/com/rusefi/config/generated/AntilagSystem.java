package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/antilag_system_state.txt Wed Dec 21 23:51:25 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class AntilagSystem {
	public static final Field ISANTILAGCONDITION = Field.create("ISANTILAGCONDITION", 0, FieldType.BIT, 0).setBaseOffset(828);
	public static final Field ALSMINRPMCONDITION = Field.create("ALSMINRPMCONDITION", 0, FieldType.BIT, 1).setBaseOffset(828);
	public static final Field ALSMAXRPMCONDITION = Field.create("ALSMAXRPMCONDITION", 0, FieldType.BIT, 2).setBaseOffset(828);
	public static final Field ALSMINCLTCONDITION = Field.create("ALSMINCLTCONDITION", 0, FieldType.BIT, 3).setBaseOffset(828);
	public static final Field ALSMAXCLTCONDITION = Field.create("ALSMAXCLTCONDITION", 0, FieldType.BIT, 4).setBaseOffset(828);
	public static final Field ALSMAXTPSCONDITION = Field.create("ALSMAXTPSCONDITION", 0, FieldType.BIT, 5).setBaseOffset(828);
	public static final Field ANTILAGCOMBINEDCONDITIONS = Field.create("ANTILAGCOMBINEDCONDITIONS", 0, FieldType.BIT, 6).setBaseOffset(828);
	public static final Field ISALSSWITCHACTIVATED = Field.create("ISALSSWITCHACTIVATED", 0, FieldType.BIT, 7).setBaseOffset(828);
	public static final Field ALSACTIVATEPINSTATE = Field.create("ALSACTIVATEPINSTATE", 0, FieldType.BIT, 8).setBaseOffset(828);
	public static final Field ALSSWITCHCONDITION = Field.create("ALSSWITCHCONDITION", 0, FieldType.BIT, 9).setBaseOffset(828);
	public static final Field FUELALSCORRECTION = Field.create("FUELALSCORRECTION", 4, FieldType.FLOAT).setBaseOffset(828);
	public static final Field[] VALUES = {
	ISANTILAGCONDITION,
	ALSMINRPMCONDITION,
	ALSMAXRPMCONDITION,
	ALSMINCLTCONDITION,
	ALSMAXCLTCONDITION,
	ALSMAXTPSCONDITION,
	ANTILAGCOMBINEDCONDITIONS,
	ISALSSWITCHACTIVATED,
	ALSACTIVATEPINSTATE,
	ALSSWITCHCONDITION,
	FUELALSCORRECTION,
	};
}
