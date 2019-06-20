package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Wed Jun 19 22:24:15 EDT 2019

import com.rusefi.config.*;

public class EngineState {
	public static final Field ISTCHARGEAIRMODEL = Field.create("ISTCHARGEAIRMODEL", 0, FieldType.BIT, 0);
	public static final Field AIRMASSINONECYLINDER = Field.create("AIRMASSINONECYLINDER", 4, FieldType.FLOAT);
	public static final Field TCHARGE = Field.create("TCHARGE", 8, FieldType.FLOAT);
	public static final Field TCHARGEK = Field.create("TCHARGEK", 12, FieldType.FLOAT);
	public static final Field TCHARGE_COFF = Field.create("TCHARGE_COFF", 16, FieldType.FLOAT);
	public static final Field AIRFLOW = Field.create("AIRFLOW", 20, FieldType.FLOAT);
	public static final Field MANIFOLDAIRPRESSUREACCELERATIONADJUSTMENT = Field.create("MANIFOLDAIRPRESSUREACCELERATIONADJUSTMENT", 24, FieldType.FLOAT);
	public static final Field ADJUSTEDMANIFOLDAIRPRESSURE = Field.create("ADJUSTEDMANIFOLDAIRPRESSURE", 28, FieldType.FLOAT);
	public static final Field TARGETAFR = Field.create("TARGETAFR", 32, FieldType.FLOAT);
	public static final Field ENGINECYCLEDURATIONMS = Field.create("ENGINECYCLEDURATIONMS", 36, FieldType.FLOAT);
	public static final Field MINRPMKCURRENTTPS = Field.create("MINRPMKCURRENTTPS", 40, FieldType.FLOAT);
	public static final Field CURRENTTPSADC = Field.create("CURRENTTPSADC", 44, FieldType.INT);
	public static final Field TPSVOLTAGEMCU = Field.create("TPSVOLTAGEMCU", 48, FieldType.FLOAT);
	public static final Field TPSVOLTAGEBOARD = Field.create("TPSVOLTAGEBOARD", 52, FieldType.FLOAT);
	public static final Field CURRENTBAROCORRECTEDVE = Field.create("CURRENTBAROCORRECTEDVE", 56, FieldType.FLOAT);
	public static final Field[] VALUES = {
	ISTCHARGEAIRMODEL,
	AIRMASSINONECYLINDER,
	TCHARGE,
	TCHARGEK,
	TCHARGE_COFF,
	AIRFLOW,
	MANIFOLDAIRPRESSUREACCELERATIONADJUSTMENT,
	ADJUSTEDMANIFOLDAIRPRESSURE,
	TARGETAFR,
	ENGINECYCLEDURATIONMS,
	MINRPMKCURRENTTPS,
	CURRENTTPSADC,
	TPSVOLTAGEMCU,
	TPSVOLTAGEBOARD,
	CURRENTBAROCORRECTEDVE,
	};
}
