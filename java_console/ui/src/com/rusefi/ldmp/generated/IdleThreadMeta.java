package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class IdleThreadMeta {
	public static final Request[] CONTENT = new Request[]{
			new TextRequest("Idle_State"),
			new FieldRequest("idleState"),
			new TextRequest("EOL"),
			new TextRequest("Base_Position"),
			new FieldRequest("baseIdlePosition"),
			new TextRequest("Position_with_Adjustments"),
			new FieldRequest("currentIdlePosition"),
			new TextRequest("EOL"),
			new TextRequest("EOL"),
			new SensorRequest("TPS"),
			new TextRequest("Throttle_Up_State"),
			new FieldRequest("throttlePedalUpState"),
			new ConfigRequest("throttlePedalUpPin"),
	};
}