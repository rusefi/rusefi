package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/idle_state.txt Tue Jan 11 14:08:37 EST 2022

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
	public static final Field BADTPS = Field.create("BADTPS", 16, FieldType.BIT, 11);
	public static final Field LOOKSLIKERUNNING = Field.create("LOOKSLIKERUNNING", 16, FieldType.BIT, 12);
	public static final Field LOOKSLIKECOASTING = Field.create("LOOKSLIKECOASTING", 16, FieldType.BIT, 13);
	public static final Field LOOKSLIKECRANKTOIDLE = Field.create("LOOKSLIKECRANKTOIDLE", 16, FieldType.BIT, 14);
	public static final Field USEINSTANTRPMFORIDLE = Field.create("USEINSTANTRPMFORIDLE", 16, FieldType.BIT, 15);
	public static final Field ISVERBOSEIAC = Field.create("ISVERBOSEIAC", 16, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_21_17 = Field.create("UNUSEDBIT_21_17", 16, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_21_18 = Field.create("UNUSEDBIT_21_18", 16, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_21_19 = Field.create("UNUSEDBIT_21_19", 16, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_21_20 = Field.create("UNUSEDBIT_21_20", 16, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_21_21 = Field.create("UNUSEDBIT_21_21", 16, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_21_22 = Field.create("UNUSEDBIT_21_22", 16, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_21_23 = Field.create("UNUSEDBIT_21_23", 16, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_21_24 = Field.create("UNUSEDBIT_21_24", 16, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_21_25 = Field.create("UNUSEDBIT_21_25", 16, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_21_26 = Field.create("UNUSEDBIT_21_26", 16, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_21_27 = Field.create("UNUSEDBIT_21_27", 16, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_21_28 = Field.create("UNUSEDBIT_21_28", 16, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_21_29 = Field.create("UNUSEDBIT_21_29", 16, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_21_30 = Field.create("UNUSEDBIT_21_30", 16, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_21_31 = Field.create("UNUSEDBIT_21_31", 16, FieldType.BIT, 31);
	public static final Field TARGETRPMBYCLT = Field.create("TARGETRPMBYCLT", 20, FieldType.INT);
	public static final Field TARGETRPMACBUMP = Field.create("TARGETRPMACBUMP", 24, FieldType.INT);
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
	BADTPS,
	LOOKSLIKERUNNING,
	LOOKSLIKECOASTING,
	LOOKSLIKECRANKTOIDLE,
	USEINSTANTRPMFORIDLE,
	ISVERBOSEIAC,
	UNUSEDBIT_21_17,
	UNUSEDBIT_21_18,
	UNUSEDBIT_21_19,
	UNUSEDBIT_21_20,
	UNUSEDBIT_21_21,
	UNUSEDBIT_21_22,
	UNUSEDBIT_21_23,
	UNUSEDBIT_21_24,
	UNUSEDBIT_21_25,
	UNUSEDBIT_21_26,
	UNUSEDBIT_21_27,
	UNUSEDBIT_21_28,
	UNUSEDBIT_21_29,
	UNUSEDBIT_21_30,
	UNUSEDBIT_21_31,
	TARGETRPMBYCLT,
	TARGETRPMACBUMP,
	};
}
