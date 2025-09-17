package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/electronic_throttle.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class ElectronicThrottle {
	public static final Field TARGETWITHIDLEPOSITION = Field.create("TARGETWITHIDLEPOSITION", 0, FieldType.FLOAT).setBaseOffset(1636);
	public static final Field TRIM = Field.create("TRIM", 4, FieldType.FLOAT).setBaseOffset(1636);
	public static final Field LUAADJUSTMENT = Field.create("LUAADJUSTMENT", 8, FieldType.FLOAT).setBaseOffset(1636);
	public static final Field M_WASTEGATEPOSITION = Field.create("M_WASTEGATEPOSITION", 12, FieldType.FLOAT).setBaseOffset(1636);
	public static final Field ETBFEEDFORWARD = Field.create("ETBFEEDFORWARD", 16, FieldType.FLOAT).setBaseOffset(1636);
	public static final Field ETBCURRENTTARGET = Field.create("ETBCURRENTTARGET", 20, FieldType.FLOAT).setBaseOffset(1636);
	public static final Field M_ADJUSTEDTARGET = Field.create("M_ADJUSTEDTARGET", 24, FieldType.INT16).setScale(0.01).setBaseOffset(1636);
	public static final Field ALIGNMENTFILL_AT_26 = Field.create("ALIGNMENTFILL_AT_26", 26, FieldType.INT8).setScale(1.0).setBaseOffset(1636);
	public static final Field ETBREVLIMITACTIVE = Field.create("ETBREVLIMITACTIVE", 28, FieldType.BIT, 0).setBaseOffset(1636);
	public static final Field JAMDETECTED = Field.create("JAMDETECTED", 28, FieldType.BIT, 1).setBaseOffset(1636);
	public static final Field VALIDPLANTPOSITION = Field.create("VALIDPLANTPOSITION", 28, FieldType.BIT, 2).setBaseOffset(1636);
	public static final Field ETBTPSERRORCOUNTER = Field.create("ETBTPSERRORCOUNTER", 32, FieldType.INT16).setScale(1.0).setBaseOffset(1636);
	public static final Field ETBPPSERRORCOUNTER = Field.create("ETBPPSERRORCOUNTER", 34, FieldType.INT16).setScale(1.0).setBaseOffset(1636);
	public static final Field ETBERRORCODE = Field.create("ETBERRORCODE", 36, FieldType.INT8).setScale(1.0).setBaseOffset(1636);
	public static final Field ETBERRORCODEBLINKER = Field.create("ETBERRORCODEBLINKER", 37, FieldType.INT8).setScale(1.0).setBaseOffset(1636);
	public static final Field TCETBDROP = Field.create("TCETBDROP", 38, FieldType.INT8).setScale(1.0).setBaseOffset(1636);
	public static final Field ALIGNMENTFILL_AT_39 = Field.create("ALIGNMENTFILL_AT_39", 39, FieldType.INT8).setScale(1.0).setBaseOffset(1636);
	public static final Field JAMTIMER = Field.create("JAMTIMER", 40, FieldType.INT16).setScale(0.01).setBaseOffset(1636);
	public static final Field ADJUSTEDETBTARGET = Field.create("ADJUSTEDETBTARGET", 42, FieldType.INT8).setScale(1.0).setBaseOffset(1636);
	public static final Field STATE = Field.create("STATE", 43, FieldType.INT8).setScale(1.0).setBaseOffset(1636);
	public static final Field[] VALUES = {
	TARGETWITHIDLEPOSITION,
	TRIM,
	LUAADJUSTMENT,
	M_WASTEGATEPOSITION,
	ETBFEEDFORWARD,
	ETBCURRENTTARGET,
	M_ADJUSTEDTARGET,
	ALIGNMENTFILL_AT_26,
	ETBREVLIMITACTIVE,
	JAMDETECTED,
	VALIDPLANTPOSITION,
	ETBTPSERRORCOUNTER,
	ETBPPSERRORCOUNTER,
	ETBERRORCODE,
	ETBERRORCODEBLINKER,
	TCETBDROP,
	ALIGNMENTFILL_AT_39,
	JAMTIMER,
	ADJUSTEDETBTARGET,
	STATE,
	};
}
