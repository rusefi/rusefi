package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/idle_state.txt Sun Aug 21 04:23:45 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class IdleState {
	public static final Field IDLESTATE = Field.create("IDLESTATE", 0, FieldType.INT).setScale(1.0);
	public static final Field CURRENTIDLEPOSITION = Field.create("CURRENTIDLEPOSITION", 4, FieldType.FLOAT);
	public static final Field BASEIDLEPOSITION = Field.create("BASEIDLEPOSITION", 8, FieldType.FLOAT);
	public static final Field IACBYTPSTAPER = Field.create("IACBYTPSTAPER", 12, FieldType.FLOAT);
	public static final Field THROTTLEPEDALUPSTATE = Field.create("THROTTLEPEDALUPSTATE", 16, FieldType.INT).setScale(1.0);
	public static final Field MIGHTRESETPID = Field.create("MIGHTRESETPID", 20, FieldType.BIT, 0);
	public static final Field SHOULDRESETPID = Field.create("SHOULDRESETPID", 20, FieldType.BIT, 1);
	public static final Field WASRESETPID = Field.create("WASRESETPID", 20, FieldType.BIT, 2);
	public static final Field MUSTRESETPID = Field.create("MUSTRESETPID", 20, FieldType.BIT, 3);
	public static final Field ISCOASTING = Field.create("ISCOASTING", 20, FieldType.BIT, 4);
	public static final Field USEIACTABLEFORCOASTING = Field.create("USEIACTABLEFORCOASTING", 20, FieldType.BIT, 5);
	public static final Field NOTIDLING = Field.create("NOTIDLING", 20, FieldType.BIT, 6);
	public static final Field NEEDRESET = Field.create("NEEDRESET", 20, FieldType.BIT, 7);
	public static final Field ISINDEADZONE = Field.create("ISINDEADZONE", 20, FieldType.BIT, 8);
	public static final Field ISBLIPPING = Field.create("ISBLIPPING", 20, FieldType.BIT, 9);
	public static final Field USECLOSEDLOOP = Field.create("USECLOSEDLOOP", 20, FieldType.BIT, 10);
	public static final Field BADTPS = Field.create("BADTPS", 20, FieldType.BIT, 11);
	public static final Field LOOKSLIKERUNNING = Field.create("LOOKSLIKERUNNING", 20, FieldType.BIT, 12);
	public static final Field LOOKSLIKECOASTING = Field.create("LOOKSLIKECOASTING", 20, FieldType.BIT, 13);
	public static final Field LOOKSLIKECRANKTOIDLE = Field.create("LOOKSLIKECRANKTOIDLE", 20, FieldType.BIT, 14);
	public static final Field USEINSTANTRPMFORIDLE = Field.create("USEINSTANTRPMFORIDLE", 20, FieldType.BIT, 15);
	public static final Field ISVERBOSEIAC = Field.create("ISVERBOSEIAC", 20, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_22_17 = Field.create("UNUSEDBIT_22_17", 20, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_22_18 = Field.create("UNUSEDBIT_22_18", 20, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_22_19 = Field.create("UNUSEDBIT_22_19", 20, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_22_20 = Field.create("UNUSEDBIT_22_20", 20, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_22_21 = Field.create("UNUSEDBIT_22_21", 20, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_22_22 = Field.create("UNUSEDBIT_22_22", 20, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_22_23 = Field.create("UNUSEDBIT_22_23", 20, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_22_24 = Field.create("UNUSEDBIT_22_24", 20, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_22_25 = Field.create("UNUSEDBIT_22_25", 20, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_22_26 = Field.create("UNUSEDBIT_22_26", 20, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_22_27 = Field.create("UNUSEDBIT_22_27", 20, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_22_28 = Field.create("UNUSEDBIT_22_28", 20, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_22_29 = Field.create("UNUSEDBIT_22_29", 20, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_22_30 = Field.create("UNUSEDBIT_22_30", 20, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_22_31 = Field.create("UNUSEDBIT_22_31", 20, FieldType.BIT, 31);
	public static final Field TARGETRPMBYCLT = Field.create("TARGETRPMBYCLT", 24, FieldType.INT).setScale(1.0);
	public static final Field TARGETRPMACBUMP = Field.create("TARGETRPMACBUMP", 28, FieldType.INT).setScale(1.0);
	public static final Field IACBYRPMTAPER = Field.create("IACBYRPMTAPER", 32, FieldType.FLOAT);
	public static final Field LUAADD = Field.create("LUAADD", 36, FieldType.FLOAT);
	public static final Field[] VALUES = {
	IDLESTATE,
	CURRENTIDLEPOSITION,
	BASEIDLEPOSITION,
	IACBYTPSTAPER,
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
	UNUSEDBIT_22_17,
	UNUSEDBIT_22_18,
	UNUSEDBIT_22_19,
	UNUSEDBIT_22_20,
	UNUSEDBIT_22_21,
	UNUSEDBIT_22_22,
	UNUSEDBIT_22_23,
	UNUSEDBIT_22_24,
	UNUSEDBIT_22_25,
	UNUSEDBIT_22_26,
	UNUSEDBIT_22_27,
	UNUSEDBIT_22_28,
	UNUSEDBIT_22_29,
	UNUSEDBIT_22_30,
	UNUSEDBIT_22_31,
	TARGETRPMBYCLT,
	TARGETRPMACBUMP,
	IACBYRPMTAPER,
	LUAADD,
	};
}
