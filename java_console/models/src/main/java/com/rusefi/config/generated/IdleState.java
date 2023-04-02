package com.rusefi.config.generated;

import com.rusefi.config.*;

public class IdleState {
	public static final Field IDLESTATE = Field.create("IDLESTATE", 0, FieldType.INT).setScale(1.0).setBaseOffset(1260);
	public static final Field CURRENTIDLEPOSITION = Field.create("CURRENTIDLEPOSITION", 4, FieldType.FLOAT).setBaseOffset(1260);
	public static final Field BASEIDLEPOSITION = Field.create("BASEIDLEPOSITION", 8, FieldType.FLOAT).setBaseOffset(1260);
	public static final Field IACBYTPSTAPER = Field.create("IACBYTPSTAPER", 12, FieldType.FLOAT).setBaseOffset(1260);
	public static final Field THROTTLEPEDALUPSTATE = Field.create("THROTTLEPEDALUPSTATE", 16, FieldType.INT).setScale(1.0).setBaseOffset(1260);
	public static final Field MIGHTRESETPID = Field.create("MIGHTRESETPID", 20, FieldType.BIT, 0).setBaseOffset(1260);
	public static final Field SHOULDRESETPID = Field.create("SHOULDRESETPID", 20, FieldType.BIT, 1).setBaseOffset(1260);
	public static final Field WASRESETPID = Field.create("WASRESETPID", 20, FieldType.BIT, 2).setBaseOffset(1260);
	public static final Field MUSTRESETPID = Field.create("MUSTRESETPID", 20, FieldType.BIT, 3).setBaseOffset(1260);
	public static final Field ISCRANKING = Field.create("ISCRANKING", 20, FieldType.BIT, 4).setBaseOffset(1260);
	public static final Field ISIACTABLEFORCOASTING = Field.create("ISIACTABLEFORCOASTING", 20, FieldType.BIT, 5).setBaseOffset(1260);
	public static final Field NOTIDLING = Field.create("NOTIDLING", 20, FieldType.BIT, 6).setBaseOffset(1260);
	public static final Field NEEDRESET = Field.create("NEEDRESET", 20, FieldType.BIT, 7).setBaseOffset(1260);
	public static final Field ISINDEADZONE = Field.create("ISINDEADZONE", 20, FieldType.BIT, 8).setBaseOffset(1260);
	public static final Field ISBLIPPING = Field.create("ISBLIPPING", 20, FieldType.BIT, 9).setBaseOffset(1260);
	public static final Field USECLOSEDLOOP = Field.create("USECLOSEDLOOP", 20, FieldType.BIT, 10).setBaseOffset(1260);
	public static final Field BADTPS = Field.create("BADTPS", 20, FieldType.BIT, 11).setBaseOffset(1260);
	public static final Field LOOKSLIKERUNNING = Field.create("LOOKSLIKERUNNING", 20, FieldType.BIT, 12).setBaseOffset(1260);
	public static final Field LOOKSLIKECOASTING = Field.create("LOOKSLIKECOASTING", 20, FieldType.BIT, 13).setBaseOffset(1260);
	public static final Field LOOKSLIKECRANKTOIDLE = Field.create("LOOKSLIKECRANKTOIDLE", 20, FieldType.BIT, 14).setBaseOffset(1260);
	public static final Field ISVERBOSEIAC = Field.create("ISVERBOSEIAC", 20, FieldType.BIT, 15).setBaseOffset(1260);
	public static final Field ISIDLECOASTING = Field.create("ISIDLECOASTING", 20, FieldType.BIT, 16).setBaseOffset(1260);
	public static final Field TARGETRPMBYCLT = Field.create("TARGETRPMBYCLT", 24, FieldType.INT).setScale(1.0).setBaseOffset(1260);
	public static final Field TARGETRPMACBUMP = Field.create("TARGETRPMACBUMP", 28, FieldType.INT).setScale(1.0).setBaseOffset(1260);
	public static final Field IACBYRPMTAPER = Field.create("IACBYRPMTAPER", 32, FieldType.FLOAT).setBaseOffset(1260);
	public static final Field LUAADD = Field.create("LUAADD", 36, FieldType.FLOAT).setBaseOffset(1260);
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
	ISVERBOSEIAC,
	ISIDLECOASTING,
	TARGETRPMBYCLT,
	TARGETRPMACBUMP,
	IACBYRPMTAPER,
	LUAADD,
	};
}
