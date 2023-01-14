package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/main_relay.txt Sat Jan 14 10:41:18 UTC 2023

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class MainRelay {
	public static final Field ISBENCHTEST = Field.create("ISBENCHTEST", 0, FieldType.BIT, 0).setBaseOffset(888);
	public static final Field HASIGNITIONVOLTAGE = Field.create("HASIGNITIONVOLTAGE", 0, FieldType.BIT, 1).setBaseOffset(888);
	public static final Field MAINRELAYSTATE = Field.create("MAINRELAYSTATE", 0, FieldType.BIT, 2).setBaseOffset(888);
	public static final Field DELAYEDSHUTOFFREQUESTED = Field.create("DELAYEDSHUTOFFREQUESTED", 0, FieldType.BIT, 3).setBaseOffset(888);
	public static final Field[] VALUES = {
	ISBENCHTEST,
	HASIGNITIONVOLTAGE,
	MAINRELAYSTATE,
	DELAYEDSHUTOFFREQUESTED,
	};
}
