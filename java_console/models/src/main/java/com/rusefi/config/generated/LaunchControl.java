package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/launch_control_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class LaunchControl {
	public static final Field RETARDTHRESHOLDRPM = Field.create("RETARDTHRESHOLDRPM", 0, FieldType.INT).setScale(1.0).setBaseOffset(1116);
	public static final Field LAUNCHACTIVATEPINSTATE = Field.create("LAUNCHACTIVATEPINSTATE", 4, FieldType.BIT, 0).setBaseOffset(1116);
	public static final Field ISPRELAUNCHCONDITION = Field.create("ISPRELAUNCHCONDITION", 4, FieldType.BIT, 1).setBaseOffset(1116);
	public static final Field ISLAUNCHCONDITION = Field.create("ISLAUNCHCONDITION", 4, FieldType.BIT, 2).setBaseOffset(1116);
	public static final Field ISSWITCHACTIVATED = Field.create("ISSWITCHACTIVATED", 4, FieldType.BIT, 3).setBaseOffset(1116);
	public static final Field ISCLUTCHACTIVATED = Field.create("ISCLUTCHACTIVATED", 4, FieldType.BIT, 4).setBaseOffset(1116);
	public static final Field ISBRAKEPEDALACTIVATED = Field.create("ISBRAKEPEDALACTIVATED", 4, FieldType.BIT, 5).setBaseOffset(1116);
	public static final Field ISVALIDINPUTPIN = Field.create("ISVALIDINPUTPIN", 4, FieldType.BIT, 6).setBaseOffset(1116);
	public static final Field ACTIVATESWITCHCONDITION = Field.create("ACTIVATESWITCHCONDITION", 4, FieldType.BIT, 7).setBaseOffset(1116);
	public static final Field RPMLAUNCHCONDITION = Field.create("RPMLAUNCHCONDITION", 4, FieldType.BIT, 8).setBaseOffset(1116);
	public static final Field RPMPRELAUNCHCONDITION = Field.create("RPMPRELAUNCHCONDITION", 4, FieldType.BIT, 9).setBaseOffset(1116);
	public static final Field SPEEDCONDITION = Field.create("SPEEDCONDITION", 4, FieldType.BIT, 10).setBaseOffset(1116);
	public static final Field TPSCONDITION = Field.create("TPSCONDITION", 4, FieldType.BIT, 11).setBaseOffset(1116);
	public static final Field[] VALUES = {
	RETARDTHRESHOLDRPM,
	LAUNCHACTIVATEPINSTATE,
	ISPRELAUNCHCONDITION,
	ISLAUNCHCONDITION,
	ISSWITCHACTIVATED,
	ISCLUTCHACTIVATED,
	ISBRAKEPEDALACTIVATED,
	ISVALIDINPUTPIN,
	ACTIVATESWITCHCONDITION,
	RPMLAUNCHCONDITION,
	RPMPRELAUNCHCONDITION,
	SPEEDCONDITION,
	TPSCONDITION,
	};
}
