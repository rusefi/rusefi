package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) util/math/pid_state.txt Sun Oct 17 01:13:39 UTC 2021

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class PidState {
	public static final Field ITERM = Field.create("ITERM", 0, FieldType.FLOAT);
	public static final Field DTERM = Field.create("DTERM", 4, FieldType.FLOAT);
	public static final Field TARGET = Field.create("TARGET", 8, FieldType.FLOAT);
	public static final Field INPUT = Field.create("INPUT", 12, FieldType.FLOAT);
	public static final Field OUTPUT = Field.create("OUTPUT", 16, FieldType.FLOAT);
	public static final Field ERRORAMPLIFICATIONCOEF = Field.create("ERRORAMPLIFICATIONCOEF", 20, FieldType.FLOAT);
	public static final Field PREVIOUSERROR = Field.create("PREVIOUSERROR", 24, FieldType.FLOAT);
	public static final Field[] VALUES = {
	ITERM,
	DTERM,
	TARGET,
	INPUT,
	OUTPUT,
	ERRORAMPLIFICATIONCOEF,
	PREVIOUSERROR,
	};
}
