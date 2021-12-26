package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Sun Dec 26 12:40:21 EST 2021

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class InjectorModel {
	public static final Field M_DEADTIME = Field.create("M_DEADTIME", 0, FieldType.FLOAT);
	public static final Field PRESSUREDELTA = Field.create("PRESSUREDELTA", 4, FieldType.FLOAT);
	public static final Field PRESSURERATIO = Field.create("PRESSURERATIO", 8, FieldType.FLOAT);
	public static final Field[] VALUES = {
	M_DEADTIME,
	PRESSUREDELTA,
	PRESSURERATIO,
	};
}
