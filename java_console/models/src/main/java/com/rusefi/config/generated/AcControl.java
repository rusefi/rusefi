package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/ac_control.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class AcControl {
	public static final Field ACBUTTONSTATE = Field.create("ACBUTTONSTATE", 0, FieldType.INT8).setScale(1.0).setBaseOffset(1168);
	public static final Field ALIGNMENTFILL_AT_1 = Field.create("ALIGNMENTFILL_AT_1", 1, FieldType.INT8).setScale(1.0).setBaseOffset(1168);
	public static final Field M_ACENABLED = Field.create("M_ACENABLED", 4, FieldType.BIT, 0).setBaseOffset(1168);
	public static final Field ENGINETOOSLOW = Field.create("ENGINETOOSLOW", 4, FieldType.BIT, 1).setBaseOffset(1168);
	public static final Field ENGINETOOFAST = Field.create("ENGINETOOFAST", 4, FieldType.BIT, 2).setBaseOffset(1168);
	public static final Field NOCLT = Field.create("NOCLT", 4, FieldType.BIT, 3).setBaseOffset(1168);
	public static final Field ENGINETOOHOT = Field.create("ENGINETOOHOT", 4, FieldType.BIT, 4).setBaseOffset(1168);
	public static final Field TPSTOOHIGH = Field.create("TPSTOOHIGH", 4, FieldType.BIT, 5).setBaseOffset(1168);
	public static final Field ISDISABLEDBYLUA = Field.create("ISDISABLEDBYLUA", 4, FieldType.BIT, 6).setBaseOffset(1168);
	public static final Field ACCOMPRESSORSTATE = Field.create("ACCOMPRESSORSTATE", 4, FieldType.BIT, 7).setBaseOffset(1168);
	public static final Field ACPRESSURETOOLOW = Field.create("ACPRESSURETOOLOW", 4, FieldType.BIT, 8).setBaseOffset(1168);
	public static final Field ACPRESSURETOOHIGH = Field.create("ACPRESSURETOOHIGH", 4, FieldType.BIT, 9).setBaseOffset(1168);
	public static final Field[] VALUES = {
	ACBUTTONSTATE,
	ALIGNMENTFILL_AT_1,
	M_ACENABLED,
	ENGINETOOSLOW,
	ENGINETOOFAST,
	NOCLT,
	ENGINETOOHOT,
	TPSTOOHIGH,
	ISDISABLEDBYLUA,
	ACCOMPRESSORSTATE,
	ACPRESSURETOOLOW,
	ACPRESSURETOOHIGH,
	};
}
