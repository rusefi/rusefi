package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/fan_control.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class FanControl {
	public static final Field CRANKING = Field.create("CRANKING", 0, FieldType.BIT, 0).setBaseOffset(1184);
	public static final Field NOTRUNNING = Field.create("NOTRUNNING", 0, FieldType.BIT, 1).setBaseOffset(1184);
	public static final Field DISABLEDWHILEENGINESTOPPED = Field.create("DISABLEDWHILEENGINESTOPPED", 0, FieldType.BIT, 2).setBaseOffset(1184);
	public static final Field BROKENCLT = Field.create("BROKENCLT", 0, FieldType.BIT, 3).setBaseOffset(1184);
	public static final Field ENABLEDFORAC = Field.create("ENABLEDFORAC", 0, FieldType.BIT, 4).setBaseOffset(1184);
	public static final Field ENABLEDACOLD = Field.create("ENABLEDACOLD", 0, FieldType.BIT, 5).setBaseOffset(1184);
	public static final Field ACENABLETIME = Field.create("ACENABLETIME", 4, FieldType.INT).setScale(1.0).setBaseOffset(1184);
	public static final Field HOT = Field.create("HOT", 8, FieldType.BIT, 0).setBaseOffset(1184);
	public static final Field COLD = Field.create("COLD", 8, FieldType.BIT, 1).setBaseOffset(1184);
	public static final Field DISABLEDBYSPEED = Field.create("DISABLEDBYSPEED", 8, FieldType.BIT, 2).setBaseOffset(1184);
	public static final Field RADIATORFANSTATUS = Field.create("RADIATORFANSTATUS", 12, FieldType.INT8).setScale(1.0).setBaseOffset(1184);
	public static final Field ALIGNMENTFILL_AT_13 = Field.create("ALIGNMENTFILL_AT_13", 13, FieldType.INT8).setScale(1.0).setBaseOffset(1184);
	public static final Field[] VALUES = {
	CRANKING,
	NOTRUNNING,
	DISABLEDWHILEENGINESTOPPED,
	BROKENCLT,
	ENABLEDFORAC,
	ENABLEDACOLD,
	ACENABLETIME,
	HOT,
	COLD,
	DISABLEDBYSPEED,
	RADIATORFANSTATUS,
	ALIGNMENTFILL_AT_13,
	};
}
