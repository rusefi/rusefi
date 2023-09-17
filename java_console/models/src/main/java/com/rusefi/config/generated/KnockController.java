package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Sun Sep 17 14:00:21 UTC 2023

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class KnockController {
	public static final Field M_KNOCKRETARD = Field.create("M_KNOCKRETARD", 0, FieldType.FLOAT).setBaseOffset(904);
	public static final Field M_KNOCKTHRESHOLD = Field.create("M_KNOCKTHRESHOLD", 4, FieldType.FLOAT).setBaseOffset(904);
	public static final Field M_KNOCKCOUNT = Field.create("M_KNOCKCOUNT", 8, FieldType.INT).setScale(1.0).setBaseOffset(904);
	public static final Field M_MAXIMUMRETARD = Field.create("M_MAXIMUMRETARD", 12, FieldType.FLOAT).setBaseOffset(904);
	public static final Field[] VALUES = {
	M_KNOCKRETARD,
	M_KNOCKTHRESHOLD,
	M_KNOCKCOUNT,
	M_MAXIMUMRETARD,
	};
}
