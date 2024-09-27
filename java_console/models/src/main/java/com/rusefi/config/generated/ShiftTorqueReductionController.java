package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/shift_torque_reduction_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class ShiftTorqueReductionController {
	public static final Field ISTORQUEREDUCTIONTRIGGERPINVALID = Field.create("ISTORQUEREDUCTIONTRIGGERPINVALID", 0, FieldType.BIT, 0).setBaseOffset(1052);
	public static final Field TORQUEREDUCTIONTRIGGERPINSTATE = Field.create("TORQUEREDUCTIONTRIGGERPINSTATE", 0, FieldType.BIT, 1).setBaseOffset(1052);
	public static final Field[] VALUES = {
	ISTORQUEREDUCTIONTRIGGERPINVALID,
	TORQUEREDUCTIONTRIGGERPINSTATE,
	};
}
