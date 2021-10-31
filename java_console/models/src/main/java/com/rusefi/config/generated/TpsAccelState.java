package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/tps_accel_state.txt Sun Oct 31 02:08:25 UTC 2021

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TpsAccelState {
	public static final Field TPSFROM = Field.create("TPSFROM", 0, FieldType.FLOAT);
	public static final Field TPSTO = Field.create("TPSTO", 4, FieldType.FLOAT);
	public static final Field DELTATPS = Field.create("DELTATPS", 8, FieldType.FLOAT);
	public static final Field EXTRAFUEL = Field.create("EXTRAFUEL", 12, FieldType.FLOAT);
	public static final Field VALUEFROMTABLE = Field.create("VALUEFROMTABLE", 16, FieldType.FLOAT);
	public static final Field ISABOVEACCELTHRESHOLD = Field.create("ISABOVEACCELTHRESHOLD", 20, FieldType.BIT, 0);
	public static final Field ISBELOWDECELTHRESHOLD = Field.create("ISBELOWDECELTHRESHOLD", 20, FieldType.BIT, 1);
	public static final Field ISTIMETORESETACCUMULATOR = Field.create("ISTIMETORESETACCUMULATOR", 20, FieldType.BIT, 2);
	public static final Field ISFRACTIONALENRICHMENT = Field.create("ISFRACTIONALENRICHMENT", 20, FieldType.BIT, 3);
	public static final Field BELOWEPSILON = Field.create("BELOWEPSILON", 20, FieldType.BIT, 4);
	public static final Field UNUSEDBIT_10_5 = Field.create("UNUSEDBIT_10_5", 20, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_10_6 = Field.create("UNUSEDBIT_10_6", 20, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_10_7 = Field.create("UNUSEDBIT_10_7", 20, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_10_8 = Field.create("UNUSEDBIT_10_8", 20, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_10_9 = Field.create("UNUSEDBIT_10_9", 20, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_10_10 = Field.create("UNUSEDBIT_10_10", 20, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_10_11 = Field.create("UNUSEDBIT_10_11", 20, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_10_12 = Field.create("UNUSEDBIT_10_12", 20, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_10_13 = Field.create("UNUSEDBIT_10_13", 20, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_10_14 = Field.create("UNUSEDBIT_10_14", 20, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_10_15 = Field.create("UNUSEDBIT_10_15", 20, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_10_16 = Field.create("UNUSEDBIT_10_16", 20, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_10_17 = Field.create("UNUSEDBIT_10_17", 20, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_10_18 = Field.create("UNUSEDBIT_10_18", 20, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_10_19 = Field.create("UNUSEDBIT_10_19", 20, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_10_20 = Field.create("UNUSEDBIT_10_20", 20, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_10_21 = Field.create("UNUSEDBIT_10_21", 20, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_10_22 = Field.create("UNUSEDBIT_10_22", 20, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_10_23 = Field.create("UNUSEDBIT_10_23", 20, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_10_24 = Field.create("UNUSEDBIT_10_24", 20, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_10_25 = Field.create("UNUSEDBIT_10_25", 20, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_10_26 = Field.create("UNUSEDBIT_10_26", 20, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_10_27 = Field.create("UNUSEDBIT_10_27", 20, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_10_28 = Field.create("UNUSEDBIT_10_28", 20, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_10_29 = Field.create("UNUSEDBIT_10_29", 20, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_10_30 = Field.create("UNUSEDBIT_10_30", 20, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_10_31 = Field.create("UNUSEDBIT_10_31", 20, FieldType.BIT, 31);
	public static final Field FRACTIONALINJFUEL = Field.create("FRACTIONALINJFUEL", 24, FieldType.FLOAT);
	public static final Field ACCUMULATEDVALUE = Field.create("ACCUMULATEDVALUE", 28, FieldType.FLOAT);
	public static final Field MAXEXTRAPERCYCLE = Field.create("MAXEXTRAPERCYCLE", 32, FieldType.FLOAT);
	public static final Field MAXEXTRAPERPERIOD = Field.create("MAXEXTRAPERPERIOD", 36, FieldType.FLOAT);
	public static final Field MAXINJECTEDPERPERIOD = Field.create("MAXINJECTEDPERPERIOD", 40, FieldType.FLOAT);
	public static final Field CYCLECNT = Field.create("CYCLECNT", 44, FieldType.INT);
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
	UNUSEDBIT_10_5,
	UNUSEDBIT_10_6,
	UNUSEDBIT_10_7,
	UNUSEDBIT_10_8,
	UNUSEDBIT_10_9,
	UNUSEDBIT_10_10,
	UNUSEDBIT_10_11,
	UNUSEDBIT_10_12,
	UNUSEDBIT_10_13,
	UNUSEDBIT_10_14,
	UNUSEDBIT_10_15,
	UNUSEDBIT_10_16,
	UNUSEDBIT_10_17,
	UNUSEDBIT_10_18,
	UNUSEDBIT_10_19,
	UNUSEDBIT_10_20,
	UNUSEDBIT_10_21,
	UNUSEDBIT_10_22,
	UNUSEDBIT_10_23,
	UNUSEDBIT_10_24,
	UNUSEDBIT_10_25,
	UNUSEDBIT_10_26,
	UNUSEDBIT_10_27,
	UNUSEDBIT_10_28,
	UNUSEDBIT_10_29,
	UNUSEDBIT_10_30,
	UNUSEDBIT_10_31,
	FRACTIONALINJFUEL,
	ACCUMULATEDVALUE,
	MAXEXTRAPERCYCLE,
	MAXEXTRAPERPERIOD,
	MAXINJECTEDPERPERIOD,
	CYCLECNT,
	};
}
