package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Fri Aug 26 01:07:49 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class ElectronicThrottle {
	public static final Field IDLEPOSITION = Field.create("IDLEPOSITION", 0, FieldType.FLOAT);
	public static final Field TRIM = Field.create("TRIM", 4, FieldType.FLOAT);
	public static final Field LUAADJUSTMENT = Field.create("LUAADJUSTMENT", 8, FieldType.FLOAT);
	public static final Field ETBFEEDFORWARD = Field.create("ETBFEEDFORWARD", 12, FieldType.FLOAT);
	public static final Field ETBINTEGRALERROR = Field.create("ETBINTEGRALERROR", 16, FieldType.FLOAT);
	public static final Field ETBCURRENTTARGET = Field.create("ETBCURRENTTARGET", 20, FieldType.FLOAT);
	public static final Field ETBREVLIMITACTIVE = Field.create("ETBREVLIMITACTIVE", 24, FieldType.BIT, 0);
	public static final Field UNUSEDBIT_7_1 = Field.create("UNUSEDBIT_7_1", 24, FieldType.BIT, 1);
	public static final Field UNUSEDBIT_7_2 = Field.create("UNUSEDBIT_7_2", 24, FieldType.BIT, 2);
	public static final Field UNUSEDBIT_7_3 = Field.create("UNUSEDBIT_7_3", 24, FieldType.BIT, 3);
	public static final Field UNUSEDBIT_7_4 = Field.create("UNUSEDBIT_7_4", 24, FieldType.BIT, 4);
	public static final Field UNUSEDBIT_7_5 = Field.create("UNUSEDBIT_7_5", 24, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_7_6 = Field.create("UNUSEDBIT_7_6", 24, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_7_7 = Field.create("UNUSEDBIT_7_7", 24, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_7_8 = Field.create("UNUSEDBIT_7_8", 24, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_7_9 = Field.create("UNUSEDBIT_7_9", 24, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_7_10 = Field.create("UNUSEDBIT_7_10", 24, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_7_11 = Field.create("UNUSEDBIT_7_11", 24, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_7_12 = Field.create("UNUSEDBIT_7_12", 24, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_7_13 = Field.create("UNUSEDBIT_7_13", 24, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_7_14 = Field.create("UNUSEDBIT_7_14", 24, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_7_15 = Field.create("UNUSEDBIT_7_15", 24, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_7_16 = Field.create("UNUSEDBIT_7_16", 24, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_7_17 = Field.create("UNUSEDBIT_7_17", 24, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_7_18 = Field.create("UNUSEDBIT_7_18", 24, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_7_19 = Field.create("UNUSEDBIT_7_19", 24, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_7_20 = Field.create("UNUSEDBIT_7_20", 24, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_7_21 = Field.create("UNUSEDBIT_7_21", 24, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_7_22 = Field.create("UNUSEDBIT_7_22", 24, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_7_23 = Field.create("UNUSEDBIT_7_23", 24, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_7_24 = Field.create("UNUSEDBIT_7_24", 24, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_7_25 = Field.create("UNUSEDBIT_7_25", 24, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_7_26 = Field.create("UNUSEDBIT_7_26", 24, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_7_27 = Field.create("UNUSEDBIT_7_27", 24, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_7_28 = Field.create("UNUSEDBIT_7_28", 24, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_7_29 = Field.create("UNUSEDBIT_7_29", 24, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_7_30 = Field.create("UNUSEDBIT_7_30", 24, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_7_31 = Field.create("UNUSEDBIT_7_31", 24, FieldType.BIT, 31);
	public static final Field[] VALUES = {
	IDLEPOSITION,
	TRIM,
	LUAADJUSTMENT,
	ETBFEEDFORWARD,
	ETBINTEGRALERROR,
	ETBCURRENTTARGET,
	ETBREVLIMITACTIVE,
	UNUSEDBIT_7_1,
	UNUSEDBIT_7_2,
	UNUSEDBIT_7_3,
	UNUSEDBIT_7_4,
	UNUSEDBIT_7_5,
	UNUSEDBIT_7_6,
	UNUSEDBIT_7_7,
	UNUSEDBIT_7_8,
	UNUSEDBIT_7_9,
	UNUSEDBIT_7_10,
	UNUSEDBIT_7_11,
	UNUSEDBIT_7_12,
	UNUSEDBIT_7_13,
	UNUSEDBIT_7_14,
	UNUSEDBIT_7_15,
	UNUSEDBIT_7_16,
	UNUSEDBIT_7_17,
	UNUSEDBIT_7_18,
	UNUSEDBIT_7_19,
	UNUSEDBIT_7_20,
	UNUSEDBIT_7_21,
	UNUSEDBIT_7_22,
	UNUSEDBIT_7_23,
	UNUSEDBIT_7_24,
	UNUSEDBIT_7_25,
	UNUSEDBIT_7_26,
	UNUSEDBIT_7_27,
	UNUSEDBIT_7_28,
	UNUSEDBIT_7_29,
	UNUSEDBIT_7_30,
	UNUSEDBIT_7_31,
	};
}
