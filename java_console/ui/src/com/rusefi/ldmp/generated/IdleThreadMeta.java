package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class IdleThreadMeta {
	public static final Request[] CONTENT = new Request[]{
			new TextRequest("Idle_State"),
			new FieldRequest("Engine","idleState"),
			new TextRequest("EOL"),
			new TextRequest("Base_Position"),
			new FieldRequest("Engine","baseIdlePosition"),
			new TextRequest("Position_with_Adjustments"),
			new FieldRequest("Engine","currentIdlePosition"),
			new TextRequest("EOL"),
			new TextRequest("EOL"),
			new SensorRequest("TPS"),
			new TextRequest("EOL"),
			new TextRequest("Throttle_Up_State"),
			new FieldRequest("Engine","throttlePedalUpState"),
			new ConfigRequest("throttlePedalUpPin"),
	};
}