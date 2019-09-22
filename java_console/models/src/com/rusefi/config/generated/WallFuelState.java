package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/wall_fuel_state.txt Sun Sep 22 08:31:58 EDT 2019

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class WallFuelState {
	public static final Field WALLFUELCORRECTION = Field.create("WALLFUELCORRECTION", 0, FieldType.FLOAT);
	public static final Field WALLFUEL = Field.create("WALLFUEL", 4, FieldType.FLOAT);
	public static final Field[] VALUES = {
	WALLFUELCORRECTION,
	WALLFUEL,
	};
}
