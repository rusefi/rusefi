package com.rusefi.config.generated;

import com.rusefi.config.*;

public class ThrottleModel {
	public static final Field THROTTLEUSEWOTMODEL = Field.create("THROTTLEUSEWOTMODEL", 0, FieldType.BIT, 0).setBaseOffset(1360);
	public static final Field THROTTLEMODELCROSSOVERANGLE = Field.create("THROTTLEMODELCROSSOVERANGLE", 4, FieldType.INT16).setScale(0.01).setBaseOffset(1360);
	public static final Field ALIGNMENTFILL_AT_6 = Field.create("ALIGNMENTFILL_AT_6", 6, FieldType.INT8).setScale(1.0).setBaseOffset(1360);
	public static final Field THROTTLEESTIMATEDFLOW = Field.create("THROTTLEESTIMATEDFLOW", 8, FieldType.FLOAT).setBaseOffset(1360);
	public static final Field[] VALUES = {
	THROTTLEUSEWOTMODEL,
	THROTTLEMODELCROSSOVERANGLE,
	ALIGNMENTFILL_AT_6,
	THROTTLEESTIMATEDFLOW,
	};
}
