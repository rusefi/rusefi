package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Sun Jul 21 18:03:23 EDT 2019

// by class com.rusefi.output.JavaFieldsConsumer
import com.rusefi.config.*;

public class EngineState {
	public static final Field ITERM = Field.create("ITERM", 0, FieldType.FLOAT);
	public static final Field DTERM = Field.create("DTERM", 4, FieldType.FLOAT);
	public static final Field TARGET = Field.create("TARGET", 8, FieldType.FLOAT);
	public static final Field INPUT = Field.create("INPUT", 12, FieldType.FLOAT);
	public static final Field OUTPUT = Field.create("OUTPUT", 16, FieldType.FLOAT);
	public static final Field ISTCHARGEAIRMODEL = Field.create("ISTCHARGEAIRMODEL", 0, FieldType.BIT, 0);
	public static final Field AIRMASSINONECYLINDER = Field.create("AIRMASSINONECYLINDER", 4, FieldType.FLOAT);
	public static final Field TCHARGE = Field.create("TCHARGE", 8, FieldType.FLOAT);
	public static final Field TCHARGEK = Field.create("TCHARGEK", 12, FieldType.FLOAT);
	public static final Field TCHARGE_COFF = Field.create("TCHARGE_COFF", 16, FieldType.FLOAT);
	public static final Field AIRFLOW = Field.create("AIRFLOW", 20, FieldType.FLOAT);
	public static final Field MANIFOLDAIRPRESSUREACCELERATIONADJUSTMENT = Field.create("MANIFOLDAIRPRESSUREACCELERATIONADJUSTMENT", 24, FieldType.FLOAT);
	public static final Field ADJUSTEDMANIFOLDAIRPRESSURE = Field.create("ADJUSTEDMANIFOLDAIRPRESSURE", 28, FieldType.FLOAT);
	public static final Field IDLESTATE = Field.create("IDLESTATE", 32, FieldType.INT);
	public static final Field CURRENTIDLEPOSITION = Field.create("CURRENTIDLEPOSITION", 36, FieldType.FLOAT);
	public static final Field BASEIDLEPOSITION = Field.create("BASEIDLEPOSITION", 40, FieldType.FLOAT);
	public static final Field THROTTLEUPSTATE = Field.create("THROTTLEUPSTATE", 44, FieldType.INT);
	public static final Field TARGETAFR = Field.create("TARGETAFR", 48, FieldType.FLOAT);
	public static final Field ENGINECYCLEDURATIONMS = Field.create("ENGINECYCLEDURATIONMS", 52, FieldType.FLOAT);
	public static final Field MINRPMKCURRENTTPS = Field.create("MINRPMKCURRENTTPS", 56, FieldType.FLOAT);
	public static final Field CURRENTTPSADC = Field.create("CURRENTTPSADC", 60, FieldType.INT);
	public static final Field TPSVOLTAGEMCU = Field.create("TPSVOLTAGEMCU", 64, FieldType.FLOAT);
	public static final Field TPSVOLTAGEBOARD = Field.create("TPSVOLTAGEBOARD", 68, FieldType.FLOAT);
	public static final Field CURRENTBAROCORRECTEDVE = Field.create("CURRENTBAROCORRECTEDVE", 72, FieldType.FLOAT);
	public static final Field[] VALUES = {
	ITERM,
	DTERM,
	TARGET,
	INPUT,
	OUTPUT,
	ISTCHARGEAIRMODEL,
	AIRMASSINONECYLINDER,
	TCHARGE,
	TCHARGEK,
	TCHARGE_COFF,
	AIRFLOW,
	MANIFOLDAIRPRESSUREACCELERATIONADJUSTMENT,
	ADJUSTEDMANIFOLDAIRPRESSURE,
	IDLESTATE,
	CURRENTIDLEPOSITION,
	BASEIDLEPOSITION,
	THROTTLEUPSTATE,
	TARGETAFR,
	ENGINECYCLEDURATIONMS,
	MINRPMKCURRENTTPS,
	CURRENTTPSADC,
	TPSVOLTAGEMCU,
	TPSVOLTAGEBOARD,
	CURRENTBAROCORRECTEDVE,
	};
}
