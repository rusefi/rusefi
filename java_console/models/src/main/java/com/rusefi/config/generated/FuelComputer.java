package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Thu Sep 01 23:17:10 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class FuelComputer {
	public static final Field CURRENTAFRLOAD = Field.create("CURRENTAFRLOAD", 0, FieldType.FLOAT);
	public static final Field TARGETLAMBDA = Field.create("TARGETLAMBDA", 4, FieldType.FLOAT);
	public static final Field STOICHIOMETRICRATIO = Field.create("STOICHIOMETRICRATIO", 8, FieldType.FLOAT);
	public static final Field TARGETAFR_T = Field.create("TARGETAFR_T", 12, FieldType.FLOAT);
	public static final Field[] VALUES = {
	CURRENTAFRLOAD,
	TARGETLAMBDA,
	STOICHIOMETRICRATIO,
	TARGETAFR_T,
	};
}
