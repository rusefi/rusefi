package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Tue Sep 24 18:13:29 EDT 2019

// by class com.rusefi.output.FileJavaFieldsConsumer
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
	public static final Field ETBIDLEADDITION = Field.create("ETBIDLEADDITION", 48, FieldType.FLOAT);
	public static final Field TARGETAFR = Field.create("TARGETAFR", 52, FieldType.FLOAT);
	public static final Field ENGINECYCLEDURATIONMS = Field.create("ENGINECYCLEDURATIONMS", 56, FieldType.FLOAT);
	public static final Field MINRPMKCURRENTTPS = Field.create("MINRPMKCURRENTTPS", 60, FieldType.FLOAT);
	public static final Field CURRENTTPSADC = Field.create("CURRENTTPSADC", 64, FieldType.INT);
	public static final Field TPSVOLTAGEMCU = Field.create("TPSVOLTAGEMCU", 68, FieldType.FLOAT);
	public static final Field TPSVOLTAGEBOARD = Field.create("TPSVOLTAGEBOARD", 72, FieldType.FLOAT);
	public static final Field CURRENTBAROCORRECTEDVE = Field.create("CURRENTBAROCORRECTEDVE", 76, FieldType.FLOAT);
	public static final Field INJECTORFLOWCORRECTION = Field.create("INJECTORFLOWCORRECTION", 80, FieldType.FLOAT);
	public static final Field BAROCORRECTION = Field.create("BAROCORRECTION", 84, FieldType.FLOAT);
	public static final Field ISCRANKINGSTATE = Field.create("ISCRANKINGSTATE", 88, FieldType.BIT, 0);
	public static final Field HASETBPEDALPOSITIONSENSOR = Field.create("HASETBPEDALPOSITIONSENSOR", 88, FieldType.BIT, 1);
	public static final Field ISAUTOMATICIDLE = Field.create("ISAUTOMATICIDLE", 88, FieldType.BIT, 2);
	public static final Field CRANKING_BASEFUEL = Field.create("CRANKING_BASEFUEL", 92, FieldType.FLOAT);
	public static final Field CRANKING_COOLANTTEMPERATURECOEFFICIENT = Field.create("CRANKING_COOLANTTEMPERATURECOEFFICIENT", 96, FieldType.FLOAT);
	public static final Field CRANKING_TPSCOEFFICIENT = Field.create("CRANKING_TPSCOEFFICIENT", 100, FieldType.FLOAT);
	public static final Field CRANKING_DURATIONCOEFFICIENT = Field.create("CRANKING_DURATIONCOEFFICIENT", 104, FieldType.FLOAT);
	public static final Field CRANKING_FUEL = Field.create("CRANKING_FUEL", 108, FieldType.FLOAT);
	public static final Field RUNNING_POSTCRANKINGFUELCORRECTION = Field.create("RUNNING_POSTCRANKINGFUELCORRECTION", 112, FieldType.FLOAT);
	public static final Field RUNNING_INTAKETEMPERATURECOEFFICIENT = Field.create("RUNNING_INTAKETEMPERATURECOEFFICIENT", 116, FieldType.FLOAT);
	public static final Field RUNNING_COOLANTTEMPERATURECOEFFICIENT = Field.create("RUNNING_COOLANTTEMPERATURECOEFFICIENT", 120, FieldType.FLOAT);
	public static final Field RUNNING_TIMESINCECRANKINGINSECS = Field.create("RUNNING_TIMESINCECRANKINGINSECS", 124, FieldType.FLOAT);
	public static final Field RUNNING_INJECTORLAG = Field.create("RUNNING_INJECTORLAG", 128, FieldType.FLOAT);
	public static final Field RUNNING_PIDCORRECTION = Field.create("RUNNING_PIDCORRECTION", 132, FieldType.FLOAT);
	public static final Field RUNNING_BASEFUEL = Field.create("RUNNING_BASEFUEL", 136, FieldType.FLOAT);
	public static final Field RUNNING_FUEL = Field.create("RUNNING_FUEL", 140, FieldType.FLOAT);
	public static final Field ETBFEEDFORWARD = Field.create("ETBFEEDFORWARD", 144, FieldType.FLOAT);
	public static final Field TARGETFROMTABLE = Field.create("TARGETFROMTABLE", 148, FieldType.FLOAT);
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
	ETBIDLEADDITION,
	TARGETAFR,
	ENGINECYCLEDURATIONMS,
	MINRPMKCURRENTTPS,
	CURRENTTPSADC,
	TPSVOLTAGEMCU,
	TPSVOLTAGEBOARD,
	CURRENTBAROCORRECTEDVE,
	INJECTORFLOWCORRECTION,
	BAROCORRECTION,
	ISCRANKINGSTATE,
	HASETBPEDALPOSITIONSENSOR,
	ISAUTOMATICIDLE,
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
	ETBFEEDFORWARD,
	TARGETFROMTABLE,
	};
}
