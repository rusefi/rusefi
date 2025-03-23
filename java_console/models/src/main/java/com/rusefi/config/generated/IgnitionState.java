package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/ignition/ignition_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class IgnitionState {
	public static final Field BASEDWELL = Field.create("BASEDWELL", 0, FieldType.FLOAT).setBaseOffset(928);
	public static final Field SPARKDWELL = Field.create("SPARKDWELL", 4, FieldType.FLOAT).setBaseOffset(928);
	public static final Field DWELLDURATIONANGLE = Field.create("DWELLDURATIONANGLE", 8, FieldType.FLOAT).setBaseOffset(928);
	public static final Field CLTTIMINGCORRECTION = Field.create("CLTTIMINGCORRECTION", 12, FieldType.INT16).setScale(0.01).setBaseOffset(928);
	public static final Field TIMINGIATCORRECTION = Field.create("TIMINGIATCORRECTION", 14, FieldType.INT16).setScale(0.01).setBaseOffset(928);
	public static final Field TIMINGPIDCORRECTION = Field.create("TIMINGPIDCORRECTION", 16, FieldType.INT16).setScale(0.01).setBaseOffset(928);
	public static final Field DFCOTIMINGRETARD = Field.create("DFCOTIMINGRETARD", 18, FieldType.INT16).setScale(0.01).setBaseOffset(928);
	public static final Field BASEIGNITIONADVANCE = Field.create("BASEIGNITIONADVANCE", 20, FieldType.INT16).setScale(0.02).setBaseOffset(928);
	public static final Field CORRECTEDIGNITIONADVANCE = Field.create("CORRECTEDIGNITIONADVANCE", 22, FieldType.INT16).setScale(0.02).setBaseOffset(928);
	public static final Field TRACTIONADVANCEDROP = Field.create("TRACTIONADVANCEDROP", 24, FieldType.INT16).setScale(0.02).setBaseOffset(928);
	public static final Field ALIGNMENTFILL_AT_26 = Field.create("ALIGNMENTFILL_AT_26", 26, FieldType.INT8).setScale(1.0).setBaseOffset(928);
	public static final Field DWELLVOLTAGECORRECTION = Field.create("DWELLVOLTAGECORRECTION", 28, FieldType.FLOAT).setBaseOffset(928);
	public static final Field LUATIMINGADD = Field.create("LUATIMINGADD", 32, FieldType.FLOAT).setBaseOffset(928);
	public static final Field LUATIMINGMULT = Field.create("LUATIMINGMULT", 36, FieldType.FLOAT).setBaseOffset(928);
	public static final Field LUAIGNITIONSKIP = Field.create("LUAIGNITIONSKIP", 40, FieldType.BIT, 0).setBaseOffset(928);
	public static final Field TRAILINGSPARKANGLE = Field.create("TRAILINGSPARKANGLE", 44, FieldType.INT16).setScale(0.01).setBaseOffset(928);
	public static final Field ALIGNMENTFILL_AT_46 = Field.create("ALIGNMENTFILL_AT_46", 46, FieldType.INT8).setScale(1.0).setBaseOffset(928);
	public static final Field[] VALUES = {
	BASEDWELL,
	SPARKDWELL,
	DWELLDURATIONANGLE,
	CLTTIMINGCORRECTION,
	TIMINGIATCORRECTION,
	TIMINGPIDCORRECTION,
	DFCOTIMINGRETARD,
	BASEIGNITIONADVANCE,
	CORRECTEDIGNITIONADVANCE,
	TRACTIONADVANCEDROP,
	ALIGNMENTFILL_AT_26,
	DWELLVOLTAGECORRECTION,
	LUATIMINGADD,
	LUATIMINGMULT,
	LUAIGNITIONSKIP,
	TRAILINGSPARKANGLE,
	ALIGNMENTFILL_AT_46,
	};
}
