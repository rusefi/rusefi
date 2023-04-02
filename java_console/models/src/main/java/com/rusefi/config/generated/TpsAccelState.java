package com.rusefi.config.generated;

import com.rusefi.config.*;

public class TpsAccelState {
	public static final Field TPSFROM = Field.create("TPSFROM", 0, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field TPSTO = Field.create("TPSTO", 4, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field DELTATPS = Field.create("DELTATPS", 8, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field EXTRAFUEL = Field.create("EXTRAFUEL", 12, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field VALUEFROMTABLE = Field.create("VALUEFROMTABLE", 16, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field ISABOVEACCELTHRESHOLD = Field.create("ISABOVEACCELTHRESHOLD", 20, FieldType.BIT, 0).setBaseOffset(1128);
	public static final Field ISBELOWDECELTHRESHOLD = Field.create("ISBELOWDECELTHRESHOLD", 20, FieldType.BIT, 1).setBaseOffset(1128);
	public static final Field ISTIMETORESETACCUMULATOR = Field.create("ISTIMETORESETACCUMULATOR", 20, FieldType.BIT, 2).setBaseOffset(1128);
	public static final Field ISFRACTIONALENRICHMENT = Field.create("ISFRACTIONALENRICHMENT", 20, FieldType.BIT, 3).setBaseOffset(1128);
	public static final Field BELOWEPSILON = Field.create("BELOWEPSILON", 20, FieldType.BIT, 4).setBaseOffset(1128);
	public static final Field TOOSHORT = Field.create("TOOSHORT", 20, FieldType.BIT, 5).setBaseOffset(1128);
	public static final Field FRACTIONALINJFUEL = Field.create("FRACTIONALINJFUEL", 24, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field ACCUMULATEDVALUE = Field.create("ACCUMULATEDVALUE", 28, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field MAXEXTRAPERCYCLE = Field.create("MAXEXTRAPERCYCLE", 32, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field MAXEXTRAPERPERIOD = Field.create("MAXEXTRAPERPERIOD", 36, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field MAXINJECTEDPERPERIOD = Field.create("MAXINJECTEDPERPERIOD", 40, FieldType.FLOAT).setBaseOffset(1128);
	public static final Field CYCLECNT = Field.create("CYCLECNT", 44, FieldType.INT).setScale(1.0).setBaseOffset(1128);
	public static final Field[] VALUES = {
	TPSFROM,
	TPSTO,
	DELTATPS,
	EXTRAFUEL,
	VALUEFROMTABLE,
	ISABOVEACCELTHRESHOLD,
	ISBELOWDECELTHRESHOLD,
	ISTIMETORESETACCUMULATOR,
	ISFRACTIONALENRICHMENT,
	BELOWEPSILON,
	TOOSHORT,
	FRACTIONALINJFUEL,
	ACCUMULATEDVALUE,
	MAXEXTRAPERCYCLE,
	MAXEXTRAPERPERIOD,
	MAXINJECTEDPERPERIOD,
	CYCLECNT,
	};
}
