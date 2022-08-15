package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/tps_accel_state.txt Mon Aug 15 21:21:43 UTC 2022

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
	public static final Field TOOSHORT = Field.create("TOOSHORT", 20, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_11_6 = Field.create("UNUSEDBIT_11_6", 20, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_11_7 = Field.create("UNUSEDBIT_11_7", 20, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_11_8 = Field.create("UNUSEDBIT_11_8", 20, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_11_9 = Field.create("UNUSEDBIT_11_9", 20, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_11_10 = Field.create("UNUSEDBIT_11_10", 20, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_11_11 = Field.create("UNUSEDBIT_11_11", 20, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_11_12 = Field.create("UNUSEDBIT_11_12", 20, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_11_13 = Field.create("UNUSEDBIT_11_13", 20, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_11_14 = Field.create("UNUSEDBIT_11_14", 20, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_11_15 = Field.create("UNUSEDBIT_11_15", 20, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_11_16 = Field.create("UNUSEDBIT_11_16", 20, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_11_17 = Field.create("UNUSEDBIT_11_17", 20, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_11_18 = Field.create("UNUSEDBIT_11_18", 20, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_11_19 = Field.create("UNUSEDBIT_11_19", 20, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_11_20 = Field.create("UNUSEDBIT_11_20", 20, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_11_21 = Field.create("UNUSEDBIT_11_21", 20, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_11_22 = Field.create("UNUSEDBIT_11_22", 20, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_11_23 = Field.create("UNUSEDBIT_11_23", 20, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_11_24 = Field.create("UNUSEDBIT_11_24", 20, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_11_25 = Field.create("UNUSEDBIT_11_25", 20, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_11_26 = Field.create("UNUSEDBIT_11_26", 20, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_11_27 = Field.create("UNUSEDBIT_11_27", 20, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_11_28 = Field.create("UNUSEDBIT_11_28", 20, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_11_29 = Field.create("UNUSEDBIT_11_29", 20, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_11_30 = Field.create("UNUSEDBIT_11_30", 20, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_11_31 = Field.create("UNUSEDBIT_11_31", 20, FieldType.BIT, 31);
	public static final Field FRACTIONALINJFUEL = Field.create("FRACTIONALINJFUEL", 24, FieldType.FLOAT);
	public static final Field ACCUMULATEDVALUE = Field.create("ACCUMULATEDVALUE", 28, FieldType.FLOAT);
	public static final Field MAXEXTRAPERCYCLE = Field.create("MAXEXTRAPERCYCLE", 32, FieldType.FLOAT);
	public static final Field MAXEXTRAPERPERIOD = Field.create("MAXEXTRAPERPERIOD", 36, FieldType.FLOAT);
	public static final Field MAXINJECTEDPERPERIOD = Field.create("MAXINJECTEDPERPERIOD", 40, FieldType.FLOAT);
	public static final Field CYCLECNT = Field.create("CYCLECNT", 44, FieldType.INT).setScale(1.0);
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
	UNUSEDBIT_11_6,
	UNUSEDBIT_11_7,
	UNUSEDBIT_11_8,
	UNUSEDBIT_11_9,
	UNUSEDBIT_11_10,
	UNUSEDBIT_11_11,
	UNUSEDBIT_11_12,
	UNUSEDBIT_11_13,
	UNUSEDBIT_11_14,
	UNUSEDBIT_11_15,
	UNUSEDBIT_11_16,
	UNUSEDBIT_11_17,
	UNUSEDBIT_11_18,
	UNUSEDBIT_11_19,
	UNUSEDBIT_11_20,
	UNUSEDBIT_11_21,
	UNUSEDBIT_11_22,
	UNUSEDBIT_11_23,
	UNUSEDBIT_11_24,
	UNUSEDBIT_11_25,
	UNUSEDBIT_11_26,
	UNUSEDBIT_11_27,
	UNUSEDBIT_11_28,
	UNUSEDBIT_11_29,
	UNUSEDBIT_11_30,
	UNUSEDBIT_11_31,
	FRACTIONALINJFUEL,
	ACCUMULATEDVALUE,
	MAXEXTRAPERCYCLE,
	MAXEXTRAPERPERIOD,
	MAXINJECTEDPERPERIOD,
	CYCLECNT,
	};
}
