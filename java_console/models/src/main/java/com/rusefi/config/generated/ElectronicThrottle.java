package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Wed Nov 23 01:41:35 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class ElectronicThrottle {
	public static final Field IDLEPOSITION = Field.create("IDLEPOSITION", 0, FieldType.FLOAT);
	public static final Field TRIM = Field.create("TRIM", 4, FieldType.FLOAT);
	public static final Field LUAADJUSTMENT = Field.create("LUAADJUSTMENT", 8, FieldType.FLOAT);
	public static final Field ETBFEEDFORWARD = Field.create("ETBFEEDFORWARD", 12, FieldType.FLOAT);
	public static final Field ETBINTEGRALERROR = Field.create("ETBINTEGRALERROR", 16, FieldType.FLOAT);
	public static final Field ETBCURRENTTARGET = Field.create("ETBCURRENTTARGET", 20, FieldType.FLOAT);
	public static final Field ETBCURRENTADJUSTEDTARGET = Field.create("ETBCURRENTADJUSTEDTARGET", 24, FieldType.FLOAT);
	public static final Field ETBREVLIMITACTIVE = Field.create("ETBREVLIMITACTIVE", 28, FieldType.BIT, 0);
	public static final Field UNUSEDBIT_8_1 = Field.create("UNUSEDBIT_8_1", 28, FieldType.BIT, 1);
	public static final Field UNUSEDBIT_8_2 = Field.create("UNUSEDBIT_8_2", 28, FieldType.BIT, 2);
	public static final Field UNUSEDBIT_8_3 = Field.create("UNUSEDBIT_8_3", 28, FieldType.BIT, 3);
	public static final Field UNUSEDBIT_8_4 = Field.create("UNUSEDBIT_8_4", 28, FieldType.BIT, 4);
	public static final Field UNUSEDBIT_8_5 = Field.create("UNUSEDBIT_8_5", 28, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_8_6 = Field.create("UNUSEDBIT_8_6", 28, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_8_7 = Field.create("UNUSEDBIT_8_7", 28, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_8_8 = Field.create("UNUSEDBIT_8_8", 28, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_8_9 = Field.create("UNUSEDBIT_8_9", 28, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_8_10 = Field.create("UNUSEDBIT_8_10", 28, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_8_11 = Field.create("UNUSEDBIT_8_11", 28, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_8_12 = Field.create("UNUSEDBIT_8_12", 28, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_8_13 = Field.create("UNUSEDBIT_8_13", 28, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_8_14 = Field.create("UNUSEDBIT_8_14", 28, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_8_15 = Field.create("UNUSEDBIT_8_15", 28, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_8_16 = Field.create("UNUSEDBIT_8_16", 28, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_8_17 = Field.create("UNUSEDBIT_8_17", 28, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_8_18 = Field.create("UNUSEDBIT_8_18", 28, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_8_19 = Field.create("UNUSEDBIT_8_19", 28, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_8_20 = Field.create("UNUSEDBIT_8_20", 28, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_8_21 = Field.create("UNUSEDBIT_8_21", 28, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_8_22 = Field.create("UNUSEDBIT_8_22", 28, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_8_23 = Field.create("UNUSEDBIT_8_23", 28, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_8_24 = Field.create("UNUSEDBIT_8_24", 28, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_8_25 = Field.create("UNUSEDBIT_8_25", 28, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_8_26 = Field.create("UNUSEDBIT_8_26", 28, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_8_27 = Field.create("UNUSEDBIT_8_27", 28, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_8_28 = Field.create("UNUSEDBIT_8_28", 28, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_8_29 = Field.create("UNUSEDBIT_8_29", 28, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_8_30 = Field.create("UNUSEDBIT_8_30", 28, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_8_31 = Field.create("UNUSEDBIT_8_31", 28, FieldType.BIT, 31);
	public static final Field[] VALUES = {
	IDLEPOSITION,
	TRIM,
	LUAADJUSTMENT,
	ETBFEEDFORWARD,
	ETBINTEGRALERROR,
	ETBCURRENTTARGET,
	ETBCURRENTADJUSTEDTARGET,
	ETBREVLIMITACTIVE,
	UNUSEDBIT_8_1,
	UNUSEDBIT_8_2,
	UNUSEDBIT_8_3,
	UNUSEDBIT_8_4,
	UNUSEDBIT_8_5,
	UNUSEDBIT_8_6,
	UNUSEDBIT_8_7,
	UNUSEDBIT_8_8,
	UNUSEDBIT_8_9,
	UNUSEDBIT_8_10,
	UNUSEDBIT_8_11,
	UNUSEDBIT_8_12,
	UNUSEDBIT_8_13,
	UNUSEDBIT_8_14,
	UNUSEDBIT_8_15,
	UNUSEDBIT_8_16,
	UNUSEDBIT_8_17,
	UNUSEDBIT_8_18,
	UNUSEDBIT_8_19,
	UNUSEDBIT_8_20,
	UNUSEDBIT_8_21,
	UNUSEDBIT_8_22,
	UNUSEDBIT_8_23,
	UNUSEDBIT_8_24,
	UNUSEDBIT_8_25,
	UNUSEDBIT_8_26,
	UNUSEDBIT_8_27,
	UNUSEDBIT_8_28,
	UNUSEDBIT_8_29,
	UNUSEDBIT_8_30,
	UNUSEDBIT_8_31,
	};
}
