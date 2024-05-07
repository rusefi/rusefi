package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/wall_fuel_state.txt Tue May 07 17:41:25 UTC 2024

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class WallFuelState {
	public static final Field WALLFUELCORRECTION = Field.create("WALLFUELCORRECTION", 0, FieldType.FLOAT).setBaseOffset(1532);
	public static final Field WALLFUEL = Field.create("WALLFUEL", 4, FieldType.FLOAT).setBaseOffset(1532);
	public static final Field[] VALUES = {
	WALLFUELCORRECTION,
	WALLFUEL,
	};
}
