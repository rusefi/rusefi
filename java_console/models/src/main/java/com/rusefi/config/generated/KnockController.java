package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Mon Aug 15 21:21:42 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class KnockController {
	public static final Field M_KNOCKRETARD = Field.create("M_KNOCKRETARD", 0, FieldType.FLOAT);
	public static final Field M_KNOCKTHRESHOLD = Field.create("M_KNOCKTHRESHOLD", 4, FieldType.FLOAT);
	public static final Field M_KNOCKCOUNT = Field.create("M_KNOCKCOUNT", 8, FieldType.INT).setScale(1.0);
	public static final Field[] VALUES = {
	M_KNOCKRETARD,
	M_KNOCKTHRESHOLD,
	M_KNOCKCOUNT,
	};
}
