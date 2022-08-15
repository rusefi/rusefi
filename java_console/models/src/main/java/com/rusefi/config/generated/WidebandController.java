package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/sensors//wideband_state.txt Mon Aug 15 18:43:34 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class WidebandController {
	public static final int BANKS_COUNT = 1;
	public static final int CAM_INPUTS_COUNT_padding = 3;
	public static final int CAMS_PER_BANK = 1;
	public static final int CAMS_PER_BANK_padding = 1;
	public static final Field FAULTCODE = Field.create("FAULTCODE", 0, FieldType.INT8).setScale(1.0);
	public static final Field HEATERDUTY = Field.create("HEATERDUTY", 1, FieldType.INT8).setScale(1.0);
	public static final Field PUMPDUTY = Field.create("PUMPDUTY", 2, FieldType.INT8).setScale(1.0);
	public static final Field ALIGNMENTFILL_AT_3 = Field.create("ALIGNMENTFILL_AT_3", 3, FieldType.INT8).setScale(1.0);
	public static final Field TEMPC = Field.create("TEMPC", 4, FieldType.INT16).setScale(1.0);
	public static final Field NERNSTVOLTAGE = Field.create("NERNSTVOLTAGE", 6, FieldType.INT16).setScale(0.001);
	public static final Field ESR = Field.create("ESR", 8, FieldType.INT16).setScale(1.0);
	public static final Field ALIGNMENTFILL_AT_10 = Field.create("ALIGNMENTFILL_AT_10", 10, FieldType.INT8).setScale(1.0);
	public static final Field[] VALUES = {
	FAULTCODE,
	HEATERDUTY,
	PUMPDUTY,
	ALIGNMENTFILL_AT_3,
	TEMPC,
	NERNSTVOLTAGE,
	ESR,
	ALIGNMENTFILL_AT_10,
	};
}
