package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/idle_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class IdleState {
	public static final Field IDLESTATE = Field.create("IDLESTATE", 0, FieldType.INT).setScale(1.0).setBaseOffset(1640);
	public static final Field BASEIDLEPOSITION = Field.create("BASEIDLEPOSITION", 4, FieldType.FLOAT).setBaseOffset(1640);
	public static final Field IACBYTPSTAPER = Field.create("IACBYTPSTAPER", 8, FieldType.FLOAT).setBaseOffset(1640);
	public static final Field MIGHTRESETPID = Field.create("MIGHTRESETPID", 12, FieldType.BIT, 0).setBaseOffset(1640);
	public static final Field SHOULDRESETPID = Field.create("SHOULDRESETPID", 12, FieldType.BIT, 1).setBaseOffset(1640);
	public static final Field WASRESETPID = Field.create("WASRESETPID", 12, FieldType.BIT, 2).setBaseOffset(1640);
	public static final Field MUSTRESETPID = Field.create("MUSTRESETPID", 12, FieldType.BIT, 3).setBaseOffset(1640);
	public static final Field ISCRANKING = Field.create("ISCRANKING", 12, FieldType.BIT, 4).setBaseOffset(1640);
	public static final Field ISIACTABLEFORCOASTING = Field.create("ISIACTABLEFORCOASTING", 12, FieldType.BIT, 5).setBaseOffset(1640);
	public static final Field NOTIDLING = Field.create("NOTIDLING", 12, FieldType.BIT, 6).setBaseOffset(1640);
	public static final Field NEEDRESET = Field.create("NEEDRESET", 12, FieldType.BIT, 7).setBaseOffset(1640);
	public static final Field ISINDEADZONE = Field.create("ISINDEADZONE", 12, FieldType.BIT, 8).setBaseOffset(1640);
	public static final Field ISBLIPPING = Field.create("ISBLIPPING", 12, FieldType.BIT, 9).setBaseOffset(1640);
	public static final Field USECLOSEDLOOP = Field.create("USECLOSEDLOOP", 12, FieldType.BIT, 10).setBaseOffset(1640);
	public static final Field BADTPS = Field.create("BADTPS", 12, FieldType.BIT, 11).setBaseOffset(1640);
	public static final Field LOOKSLIKERUNNING = Field.create("LOOKSLIKERUNNING", 12, FieldType.BIT, 12).setBaseOffset(1640);
	public static final Field LOOKSLIKECOASTING = Field.create("LOOKSLIKECOASTING", 12, FieldType.BIT, 13).setBaseOffset(1640);
	public static final Field LOOKSLIKECRANKTOIDLE = Field.create("LOOKSLIKECRANKTOIDLE", 12, FieldType.BIT, 14).setBaseOffset(1640);
	public static final Field ISRUNNING = Field.create("ISRUNNING", 12, FieldType.BIT, 15).setBaseOffset(1640);
	public static final Field ISIDLECOASTING = Field.create("ISIDLECOASTING", 12, FieldType.BIT, 16).setBaseOffset(1640);
	public static final Field ISIDLECLOSEDLOOP = Field.create("ISIDLECLOSEDLOOP", 12, FieldType.BIT, 17).setBaseOffset(1640);
	public static final Field IDLETARGET = Field.create("IDLETARGET", 16, FieldType.INT).setScale(1.0).setBaseOffset(1640);
	public static final Field TARGETRPMBYCLT = Field.create("TARGETRPMBYCLT", 20, FieldType.INT).setScale(1.0).setBaseOffset(1640);
	public static final Field TARGETRPMAC = Field.create("TARGETRPMAC", 24, FieldType.INT).setScale(1.0).setBaseOffset(1640);
	public static final Field IACBYRPMTAPER = Field.create("IACBYRPMTAPER", 28, FieldType.FLOAT).setBaseOffset(1640);
	public static final Field LUAADD = Field.create("LUAADD", 32, FieldType.FLOAT).setBaseOffset(1640);
	public static final Field IDLECLOSEDLOOP = Field.create("IDLECLOSEDLOOP", 36, FieldType.FLOAT).setBaseOffset(1640);
	public static final Field CURRENTIDLEPOSITION = Field.create("CURRENTIDLEPOSITION", 40, FieldType.FLOAT).setBaseOffset(1640);
	public static final Field IDLETARGETAIRMASS = Field.create("IDLETARGETAIRMASS", 44, FieldType.INT16).setScale(1.0).setBaseOffset(1640);
	public static final Field IDLETARGETFLOW = Field.create("IDLETARGETFLOW", 46, FieldType.INT16).setScale(0.01).setBaseOffset(1640);
	public static final Field[] VALUES = {
	IDLESTATE,
	BASEIDLEPOSITION,
	IACBYTPSTAPER,
	MIGHTRESETPID,
	SHOULDRESETPID,
	WASRESETPID,
	MUSTRESETPID,
	ISCRANKING,
	ISIACTABLEFORCOASTING,
	NOTIDLING,
	NEEDRESET,
	ISINDEADZONE,
	ISBLIPPING,
	USECLOSEDLOOP,
	BADTPS,
	LOOKSLIKERUNNING,
	LOOKSLIKECOASTING,
	LOOKSLIKECRANKTOIDLE,
	ISRUNNING,
	ISIDLECOASTING,
	ISIDLECLOSEDLOOP,
	IDLETARGET,
	TARGETRPMBYCLT,
	TARGETRPMAC,
	IACBYRPMTAPER,
	LUAADD,
	IDLECLOSEDLOOP,
	CURRENTIDLEPOSITION,
	IDLETARGETAIRMASS,
	IDLETARGETFLOW,
	};
}
