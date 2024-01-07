package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/algo/wall_fuel_state.txt Sun Jan 07 19:53:44 UTC 2024

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class WallFuelState {
	public static final Field WALLFUELCORRECTION = Field.create("WALLFUELCORRECTION", 0, FieldType.FLOAT).setBaseOffset(1304);
	public static final Field WALLFUEL = Field.create("WALLFUEL", 4, FieldType.FLOAT).setBaseOffset(1304);
	public static final Field[] VALUES = {
	WALLFUELCORRECTION,
	WALLFUEL,
	};
}
