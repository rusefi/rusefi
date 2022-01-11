package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/idle_state.txt Tue Jan 11 01:55:22 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class IdleState {
	public static final Field IDLESTATE = Field.create("IDLESTATE", 0, FieldType.INT);
	public static final Field CURRENTIDLEPOSITION = Field.create("CURRENTIDLEPOSITION", 4, FieldType.FLOAT);
	public static final Field BASEIDLEPOSITION = Field.create("BASEIDLEPOSITION", 8, FieldType.FLOAT);
	public static final Field THROTTLEPEDALUPSTATE = Field.create("THROTTLEPEDALUPSTATE", 12, FieldType.INT);
	public static final Field MIGHTRESETPID = Field.create("MIGHTRESETPID", 16, FieldType.BIT, 0);
	public static final Field SHOULDRESETPID = Field.create("SHOULDRESETPID", 16, FieldType.BIT, 1);
	public static final Field WASRESETPID = Field.create("WASRESETPID", 16, FieldType.BIT, 2);
	public static final Field MUSTRESETPID = Field.create("MUSTRESETPID", 16, FieldType.BIT, 3);
	public static final Field ISCOASTING = Field.create("ISCOASTING", 16, FieldType.BIT, 4);
	public static final Field USEIACTABLEFORCOASTING = Field.create("USEIACTABLEFORCOASTING", 16, FieldType.BIT, 5);
	public static final Field NOTIDLING = Field.create("NOTIDLING", 16, FieldType.BIT, 6);
	public static final Field NEEDRESET = Field.create("NEEDRESET", 16, FieldType.BIT, 7);
	public static final Field ISINDEADZONE = Field.create("ISINDEADZONE", 16, FieldType.BIT, 8);
	public static final Field ISBLIPPING = Field.create("ISBLIPPING", 16, FieldType.BIT, 9);
	public static final Field USECLOSEDLOOP = Field.create("USECLOSEDLOOP", 16, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_15_11 = Field.create("UNUSEDBIT_15_11", 16, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_15_12 = Field.create("UNUSEDBIT_15_12", 16, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_15_13 = Field.create("UNUSEDBIT_15_13", 16, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_15_14 = Field.create("UNUSEDBIT_15_14", 16, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_15_15 = Field.create("UNUSEDBIT_15_15", 16, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_15_16 = Field.create("UNUSEDBIT_15_16", 16, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_15_17 = Field.create("UNUSEDBIT_15_17", 16, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_15_18 = Field.create("UNUSEDBIT_15_18", 16, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_15_19 = Field.create("UNUSEDBIT_15_19", 16, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_15_20 = Field.create("UNUSEDBIT_15_20", 16, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_15_21 = Field.create("UNUSEDBIT_15_21", 16, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_15_22 = Field.create("UNUSEDBIT_15_22", 16, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_15_23 = Field.create("UNUSEDBIT_15_23", 16, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_15_24 = Field.create("UNUSEDBIT_15_24", 16, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_15_25 = Field.create("UNUSEDBIT_15_25", 16, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_15_26 = Field.create("UNUSEDBIT_15_26", 16, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_15_27 = Field.create("UNUSEDBIT_15_27", 16, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_15_28 = Field.create("UNUSEDBIT_15_28", 16, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_15_29 = Field.create("UNUSEDBIT_15_29", 16, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_15_30 = Field.create("UNUSEDBIT_15_30", 16, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_15_31 = Field.create("UNUSEDBIT_15_31", 16, FieldType.BIT, 31);
	public static final Field[] VALUES = {
	IDLESTATE,
	CURRENTIDLEPOSITION,
	BASEIDLEPOSITION,
	THROTTLEPEDALUPSTATE,
	MIGHTRESETPID,
	SHOULDRESETPID,
	WASRESETPID,
	MUSTRESETPID,
	ISCOASTING,
	USEIACTABLEFORCOASTING,
	NOTIDLING,
	NEEDRESET,
	ISINDEADZONE,
	ISBLIPPING,
	USECLOSEDLOOP,
	UNUSEDBIT_15_11,
	UNUSEDBIT_15_12,
	UNUSEDBIT_15_13,
	UNUSEDBIT_15_14,
	UNUSEDBIT_15_15,
	UNUSEDBIT_15_16,
	UNUSEDBIT_15_17,
	UNUSEDBIT_15_18,
	UNUSEDBIT_15_19,
	UNUSEDBIT_15_20,
	UNUSEDBIT_15_21,
	UNUSEDBIT_15_22,
	UNUSEDBIT_15_23,
	UNUSEDBIT_15_24,
	UNUSEDBIT_15_25,
	UNUSEDBIT_15_26,
	UNUSEDBIT_15_27,
	UNUSEDBIT_15_28,
	UNUSEDBIT_15_29,
	UNUSEDBIT_15_30,
	UNUSEDBIT_15_31,
	};
}
