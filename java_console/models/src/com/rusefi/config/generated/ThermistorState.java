package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/thermistor.txt Mon Jun 17 20:32:33 EDT 2019

import com.rusefi.config.*;

public class ThermistorState {
	public static final Field RESISTANCE = Field.create("RESISTANCE", 0, FieldType.FLOAT);
	public static final Field VOLTAGEMCU = Field.create("VOLTAGEMCU", 4, FieldType.FLOAT);
	public static final Field VOLTAGEBOARD = Field.create("VOLTAGEBOARD", 8, FieldType.FLOAT);
	public static final Field[] VALUES = {
	RESISTANCE,
	VOLTAGEMCU,
	VOLTAGEBOARD,
	};
}
