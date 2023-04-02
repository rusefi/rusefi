package com.rusefi.config.generated;

import com.rusefi.config.*;

public class HighPressureFuelPump {
	public static final Field M_REQUESTED_PUMP = Field.create("M_REQUESTED_PUMP", 0, FieldType.FLOAT).setBaseOffset(904);
	public static final Field FUEL_REQUESTED_PERCENT = Field.create("FUEL_REQUESTED_PERCENT", 4, FieldType.FLOAT).setBaseOffset(904);
	public static final Field FUEL_REQUESTED_PERCENT_PI = Field.create("FUEL_REQUESTED_PERCENT_PI", 8, FieldType.FLOAT).setBaseOffset(904);
	public static final Field NOVALVE = Field.create("NOVALVE", 12, FieldType.BIT, 0).setBaseOffset(904);
	public static final Field ANGLEABOVEMIN = Field.create("ANGLEABOVEMIN", 12, FieldType.BIT, 1).setBaseOffset(904);
	public static final Field ISHPFPINACTIVE = Field.create("ISHPFPINACTIVE", 12, FieldType.BIT, 2).setBaseOffset(904);
	public static final Field NEXTLOBE = Field.create("NEXTLOBE", 16, FieldType.FLOAT).setBaseOffset(904);
	public static final Field DI_NEXTSTART = Field.create("DI_NEXTSTART", 20, FieldType.FLOAT).setBaseOffset(904);
	public static final Field[] VALUES = {
	M_REQUESTED_PUMP,
	FUEL_REQUESTED_PERCENT,
	FUEL_REQUESTED_PERCENT_PI,
	NOVALVE,
	ANGLEABOVEMIN,
	ISHPFPINACTIVE,
	NEXTLOBE,
	DI_NEXTSTART,
	};
}
