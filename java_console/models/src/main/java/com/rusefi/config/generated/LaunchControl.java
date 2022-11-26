package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/launch_control_state.txt Sat Nov 26 05:29:50 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class LaunchControl {
	public static final Field RETARDTHRESHOLDRPM = Field.create("RETARDTHRESHOLDRPM", 0, FieldType.INT).setScale(1.0);
	public static final Field COMBINEDCONDITIONS = Field.create("COMBINEDCONDITIONS", 4, FieldType.BIT, 0);
	public static final Field LAUNCHACTIVATEPINSTATE = Field.create("LAUNCHACTIVATEPINSTATE", 4, FieldType.BIT, 1);
	public static final Field ISLAUNCHCONDITION = Field.create("ISLAUNCHCONDITION", 4, FieldType.BIT, 2);
	public static final Field ISSWITCHACTIVATED = Field.create("ISSWITCHACTIVATED", 4, FieldType.BIT, 3);
	public static final Field ISCLUTCHACTIVATED = Field.create("ISCLUTCHACTIVATED", 4, FieldType.BIT, 4);
	public static final Field ISVALIDINPUTPIN = Field.create("ISVALIDINPUTPIN", 4, FieldType.BIT, 5);
	public static final Field ACTIVATESWITCHCONDITION = Field.create("ACTIVATESWITCHCONDITION", 4, FieldType.BIT, 6);
	public static final Field RPMCONDITION = Field.create("RPMCONDITION", 4, FieldType.BIT, 7);
	public static final Field SPEEDCONDITION = Field.create("SPEEDCONDITION", 4, FieldType.BIT, 8);
	public static final Field TPSCONDITION = Field.create("TPSCONDITION", 4, FieldType.BIT, 9);
	public static final Field[] VALUES = {
	RETARDTHRESHOLDRPM,
	COMBINEDCONDITIONS,
	LAUNCHACTIVATEPINSTATE,
	ISLAUNCHCONDITION,
	ISSWITCHACTIVATED,
	ISCLUTCHACTIVATED,
	ISVALIDINPUTPIN,
	ACTIVATESWITCHCONDITION,
	RPMCONDITION,
	SPEEDCONDITION,
	TPSCONDITION,
	};
}
