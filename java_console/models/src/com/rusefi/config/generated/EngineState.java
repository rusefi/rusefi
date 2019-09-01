package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Sun Sep 01 11:57:51 EDT 2019

// by class com.rusefi.output.JavaFieldsConsumer
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
	public static final Field IDLESTATE = Field.create("IDLESTATE", 32, FieldType.INT);
	public static final Field CURRENTIDLEPOSITION = Field.create("CURRENTIDLEPOSITION", 36, FieldType.FLOAT);
	public static final Field BASEIDLEPOSITION = Field.create("BASEIDLEPOSITION", 40, FieldType.FLOAT);
	public static final Field THROTTLEPEDALUPSTATE = Field.create("THROTTLEPEDALUPSTATE", 44, FieldType.INT);
	public static final Field TARGETAFR = Field.create("TARGETAFR", 48, FieldType.FLOAT);
	public static final Field ENGINECYCLEDURATIONMS = Field.create("ENGINECYCLEDURATIONMS", 52, FieldType.FLOAT);
	public static final Field MINRPMKCURRENTTPS = Field.create("MINRPMKCURRENTTPS", 56, FieldType.FLOAT);
	public static final Field CURRENTTPSADC = Field.create("CURRENTTPSADC", 60, FieldType.INT);
	public static final Field TPSVOLTAGEMCU = Field.create("TPSVOLTAGEMCU", 64, FieldType.FLOAT);
	public static final Field TPSVOLTAGEBOARD = Field.create("TPSVOLTAGEBOARD", 68, FieldType.FLOAT);
	public static final Field CURRENTBAROCORRECTEDVE = Field.create("CURRENTBAROCORRECTEDVE", 72, FieldType.FLOAT);
	public static final Field BAROCORRECTION = Field.create("BAROCORRECTION", 76, FieldType.FLOAT);
	public static final Field ISCRANKINGSTATE = Field.create("ISCRANKINGSTATE", 80, FieldType.BIT, 0);
	public static final Field HASETBPEDALPOSITIONSENSOR = Field.create("HASETBPEDALPOSITIONSENSOR", 80, FieldType.BIT, 1);
	public static final Field CRANKING_BASEFUEL = Field.create("CRANKING_BASEFUEL", 84, FieldType.FLOAT);
	public static final Field CRANKING_COOLANTTEMPERATURECOEFFICIENT = Field.create("CRANKING_COOLANTTEMPERATURECOEFFICIENT", 88, FieldType.FLOAT);
	public static final Field CRANKING_TPSCOEFFICIENT = Field.create("CRANKING_TPSCOEFFICIENT", 92, FieldType.FLOAT);
	public static final Field CRANKING_DURATIONCOEFFICIENT = Field.create("CRANKING_DURATIONCOEFFICIENT", 96, FieldType.FLOAT);
	public static final Field CRANKING_FUEL = Field.create("CRANKING_FUEL", 100, FieldType.FLOAT);
	public static final Field RUNNING_POSTCRANKINGFUELCORRECTION = Field.create("RUNNING_POSTCRANKINGFUELCORRECTION", 104, FieldType.FLOAT);
	public static final Field RUNNING_INTAKETEMPERATURECOEFFICIENT = Field.create("RUNNING_INTAKETEMPERATURECOEFFICIENT", 108, FieldType.FLOAT);
	public static final Field RUNNING_COOLANTTEMPERATURECOEFFICIENT = Field.create("RUNNING_COOLANTTEMPERATURECOEFFICIENT", 112, FieldType.FLOAT);
	public static final Field RUNNING_TIMESINCECRANKINGINSECS = Field.create("RUNNING_TIMESINCECRANKINGINSECS", 116, FieldType.FLOAT);
	public static final Field RUNNING_INJECTORLAG = Field.create("RUNNING_INJECTORLAG", 120, FieldType.FLOAT);
	public static final Field RUNNING_PIDCORRECTION = Field.create("RUNNING_PIDCORRECTION", 124, FieldType.FLOAT);
	public static final Field RUNNING_BASEFUEL = Field.create("RUNNING_BASEFUEL", 128, FieldType.FLOAT);
	public static final Field RUNNING_FUEL = Field.create("RUNNING_FUEL", 132, FieldType.FLOAT);
	public static final Field[] VALUES = {
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
	THROTTLEPEDALUPSTATE,
	TARGETAFR,
	ENGINECYCLEDURATIONMS,
	MINRPMKCURRENTTPS,
	CURRENTTPSADC,
	TPSVOLTAGEMCU,
	TPSVOLTAGEBOARD,
	CURRENTBAROCORRECTEDVE,
	BAROCORRECTION,
	ISCRANKINGSTATE,
	HASETBPEDALPOSITIONSENSOR,
	CRANKING_BASEFUEL,
	CRANKING_COOLANTTEMPERATURECOEFFICIENT,
	CRANKING_TPSCOEFFICIENT,
	CRANKING_DURATIONCOEFFICIENT,
	CRANKING_FUEL,
	RUNNING_POSTCRANKINGFUELCORRECTION,
	RUNNING_INTAKETEMPERATURECOEFFICIENT,
	RUNNING_COOLANTTEMPERATURECOEFFICIENT,
	RUNNING_TIMESINCECRANKINGINSECS,
	RUNNING_INJECTORLAG,
	RUNNING_PIDCORRECTION,
	RUNNING_BASEFUEL,
	RUNNING_FUEL,
	};
}
