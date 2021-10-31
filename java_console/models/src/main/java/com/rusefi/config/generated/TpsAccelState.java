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
	public static final Field UNUSEDBIT_9_4 = Field.create("UNUSEDBIT_9_4", 20, FieldType.BIT, 4);
	public static final Field UNUSEDBIT_9_5 = Field.create("UNUSEDBIT_9_5", 20, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_9_6 = Field.create("UNUSEDBIT_9_6", 20, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_9_7 = Field.create("UNUSEDBIT_9_7", 20, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_9_8 = Field.create("UNUSEDBIT_9_8", 20, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_9_9 = Field.create("UNUSEDBIT_9_9", 20, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_9_10 = Field.create("UNUSEDBIT_9_10", 20, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_9_11 = Field.create("UNUSEDBIT_9_11", 20, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_9_12 = Field.create("UNUSEDBIT_9_12", 20, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_9_13 = Field.create("UNUSEDBIT_9_13", 20, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_9_14 = Field.create("UNUSEDBIT_9_14", 20, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_9_15 = Field.create("UNUSEDBIT_9_15", 20, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_9_16 = Field.create("UNUSEDBIT_9_16", 20, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_9_17 = Field.create("UNUSEDBIT_9_17", 20, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_9_18 = Field.create("UNUSEDBIT_9_18", 20, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_9_19 = Field.create("UNUSEDBIT_9_19", 20, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_9_20 = Field.create("UNUSEDBIT_9_20", 20, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_9_21 = Field.create("UNUSEDBIT_9_21", 20, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_9_22 = Field.create("UNUSEDBIT_9_22", 20, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_9_23 = Field.create("UNUSEDBIT_9_23", 20, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_9_24 = Field.create("UNUSEDBIT_9_24", 20, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_9_25 = Field.create("UNUSEDBIT_9_25", 20, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_9_26 = Field.create("UNUSEDBIT_9_26", 20, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_9_27 = Field.create("UNUSEDBIT_9_27", 20, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_9_28 = Field.create("UNUSEDBIT_9_28", 20, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_9_29 = Field.create("UNUSEDBIT_9_29", 20, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_9_30 = Field.create("UNUSEDBIT_9_30", 20, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_9_31 = Field.create("UNUSEDBIT_9_31", 20, FieldType.BIT, 31);
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
	UNUSEDBIT_9_4,
	UNUSEDBIT_9_5,
	UNUSEDBIT_9_6,
	UNUSEDBIT_9_7,
	UNUSEDBIT_9_8,
	UNUSEDBIT_9_9,
	UNUSEDBIT_9_10,
	UNUSEDBIT_9_11,
	UNUSEDBIT_9_12,
	UNUSEDBIT_9_13,
	UNUSEDBIT_9_14,
	UNUSEDBIT_9_15,
	UNUSEDBIT_9_16,
	UNUSEDBIT_9_17,
	UNUSEDBIT_9_18,
	UNUSEDBIT_9_19,
	UNUSEDBIT_9_20,
	UNUSEDBIT_9_21,
	UNUSEDBIT_9_22,
	UNUSEDBIT_9_23,
	UNUSEDBIT_9_24,
	UNUSEDBIT_9_25,
	UNUSEDBIT_9_26,
	UNUSEDBIT_9_27,
	UNUSEDBIT_9_28,
	UNUSEDBIT_9_29,
	UNUSEDBIT_9_30,
	UNUSEDBIT_9_31,
	FRACTIONALINJFUEL,
	ACCUMULATEDVALUE,
	MAXEXTRAPERCYCLE,
	MAXEXTRAPERPERIOD,
	MAXINJECTEDPERPERIOD,
	CYCLECNT,
	};
}
