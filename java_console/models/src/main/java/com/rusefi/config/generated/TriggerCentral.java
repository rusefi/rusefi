package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Sun Oct 17 01:13:40 UTC 2021

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TriggerCentral {
	public static final int HW_EVENT_TYPES = 6;
	public static final Field HWEVENTCOUNTERS1 = Field.create("HWEVENTCOUNTERS1", 0, FieldType.INT);
	public static final Field HWEVENTCOUNTERS2 = Field.create("HWEVENTCOUNTERS2", 4, FieldType.INT);
	public static final Field HWEVENTCOUNTERS3 = Field.create("HWEVENTCOUNTERS3", 8, FieldType.INT);
	public static final Field HWEVENTCOUNTERS4 = Field.create("HWEVENTCOUNTERS4", 12, FieldType.INT);
	public static final Field HWEVENTCOUNTERS5 = Field.create("HWEVENTCOUNTERS5", 16, FieldType.INT);
	public static final Field HWEVENTCOUNTERS6 = Field.create("HWEVENTCOUNTERS6", 20, FieldType.INT);
	public static final Field VVTCAMCOUNTER = Field.create("VVTCAMCOUNTER", 24, FieldType.INT);
	public static final Field VVTEVENTRISECOUNTER = Field.create("VVTEVENTRISECOUNTER", 28, FieldType.INT);
	public static final Field VVTEVENTFALLCOUNTER = Field.create("VVTEVENTFALLCOUNTER", 32, FieldType.INT);
	public static final Field[] VALUES = {
	HWEVENTCOUNTERS1,
	HWEVENTCOUNTERS2,
	HWEVENTCOUNTERS3,
	HWEVENTCOUNTERS4,
	HWEVENTCOUNTERS5,
	HWEVENTCOUNTERS6,
	VVTCAMCOUNTER,
	VVTEVENTRISECOUNTER,
	VVTEVENTFALLCOUNTER,
	};
}
