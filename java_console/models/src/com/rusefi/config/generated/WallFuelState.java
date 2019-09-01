package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/wall_fuel_state.txt Sat Jul 20 12:28:05 EDT 2019

// by class com.rusefi.output.JavaFieldsConsumer
import com.rusefi.config.*;

public class WallFuelState {
	public static final Field WALLFUELCORRECTION = Field.create("WALLFUELCORRECTION", 0, FieldType.FLOAT);
	public static final Field WALLFUEL = Field.create("WALLFUEL", 4, FieldType.FLOAT);
	public static final Field[] VALUES = {
	WALLFUELCORRECTION,
	WALLFUEL,
	};
}
