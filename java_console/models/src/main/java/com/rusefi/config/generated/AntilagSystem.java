package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/antilag_system_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class AntilagSystem {
	public static final Field ISANTILAGCONDITION = Field.create("ISANTILAGCONDITION", 0, FieldType.BIT, 0).setBaseOffset(1132);
	public static final Field ALSMINRPMCONDITION = Field.create("ALSMINRPMCONDITION", 0, FieldType.BIT, 1).setBaseOffset(1132);
	public static final Field ALSMAXRPMCONDITION = Field.create("ALSMAXRPMCONDITION", 0, FieldType.BIT, 2).setBaseOffset(1132);
	public static final Field ALSMINCLTCONDITION = Field.create("ALSMINCLTCONDITION", 0, FieldType.BIT, 3).setBaseOffset(1132);
	public static final Field ALSMAXCLTCONDITION = Field.create("ALSMAXCLTCONDITION", 0, FieldType.BIT, 4).setBaseOffset(1132);
	public static final Field ALSMAXTHROTTLEINTENTCONDITION = Field.create("ALSMAXTHROTTLEINTENTCONDITION", 0, FieldType.BIT, 5).setBaseOffset(1132);
	public static final Field ISALSSWITCHACTIVATED = Field.create("ISALSSWITCHACTIVATED", 0, FieldType.BIT, 6).setBaseOffset(1132);
	public static final Field ALSACTIVATEPINSTATE = Field.create("ALSACTIVATEPINSTATE", 0, FieldType.BIT, 7).setBaseOffset(1132);
	public static final Field ALSSWITCHCONDITION = Field.create("ALSSWITCHCONDITION", 0, FieldType.BIT, 8).setBaseOffset(1132);
	public static final Field ALSTIMERCONDITION = Field.create("ALSTIMERCONDITION", 0, FieldType.BIT, 9).setBaseOffset(1132);
	public static final Field FUELALSCORRECTION = Field.create("FUELALSCORRECTION", 4, FieldType.FLOAT).setBaseOffset(1132);
	public static final Field TIMINGALSCORRECTION = Field.create("TIMINGALSCORRECTION", 8, FieldType.INT16).setScale(0.01).setBaseOffset(1132);
	public static final Field TIMINGALSSKIP = Field.create("TIMINGALSSKIP", 10, FieldType.INT16).setScale(0.01).setBaseOffset(1132);
	public static final Field[] VALUES = {
	ISANTILAGCONDITION,
	ALSMINRPMCONDITION,
	ALSMAXRPMCONDITION,
	ALSMINCLTCONDITION,
	ALSMAXCLTCONDITION,
	ALSMAXTHROTTLEINTENTCONDITION,
	ISALSSWITCHACTIVATED,
	ALSACTIVATEPINSTATE,
	ALSSWITCHCONDITION,
	ALSTIMERCONDITION,
	FUELALSCORRECTION,
	TIMINGALSCORRECTION,
	TIMINGALSSKIP,
	};
}
