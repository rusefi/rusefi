package com.rusefi.config.generated;

// this file was generated automatically by ConfigDefinition.jar based on integration/engine_state.txt Sun Jun 16 22:18:54 EDT 2019

import com.rusefi.config.*;

public class EngineState {
	public static final Field ISTCHARGEAIRMODEL = Field.create("ISTCHARGEAIRMODEL", 0, FieldType.BIT, 0);
	public static final Field AIRMASS = Field.create("AIRMASS", 4, FieldType.FLOAT);
	public static final Field ENGINECYCLEDURATIONMS = Field.create("ENGINECYCLEDURATIONMS", 8, FieldType.FLOAT);
	public static final Field TCHARGE_COFF = Field.create("TCHARGE_COFF", 12, FieldType.FLOAT);
	public static final Field AIRFLOW = Field.create("AIRFLOW", 16, FieldType.FLOAT);
	public static final Field MINRPMKCURRENTTPS = Field.create("MINRPMKCURRENTTPS", 20, FieldType.FLOAT);
	public static final Field[] VALUES = {
	ISTCHARGEAIRMODEL,
	AIRMASS,
	ENGINECYCLEDURATIONMS,
	TCHARGE_COFF,
	AIRFLOW,
	MINRPMKCURRENTTPS,
	};
}
