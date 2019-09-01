package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class IdleThreadMeta {
	public static final Request[] CONTENT = new Request[]{
			new TextRequest("Idle_State"),
			new FieldRequest("Engine", "idleState"),
			new TextRequest("EOL"),
			new TextRequest("Base_Position"),
			new FieldRequest("Engine", "baseIdlePosition"),
			new TextRequest("Position_with_Adjustments"),
			new FieldRequest("Engine", "currentIdlePosition"),
			new TextRequest("EOL"),
			new TextRequest("EOL"),
			new SensorRequest("TPS"),
			new TextRequest("EOL"),
			new TextRequest("Throttle_Up_State"),
			new FieldRequest("Engine", "throttlePedalUpState"),
			new ConfigRequest("throttlePedalUpPin"),
			new TextRequest("eol"),
			new IfRequest("Engine", "isAutomaticIdle",
				new Request[]{
				new TextRequest("Output"),
				new FieldRequest("idle_pid", "output"),
				new TextRequest("iTerm"),
				new FieldRequest("idle_pid", "iTerm"),
				new TextRequest("eol"),
				new TextRequest("Settings"),
				new ConfigRequest("IDLERPMPID_PFACTOR"),
				new ConfigRequest("IDLERPMPID_IFACTOR"),
				new ConfigRequest("IDLERPMPID_DFACTOR"),
				new ConfigRequest("IDLERPMPID_OFFSET"),
},
				new Request[]{
				new TextRequest("Manual_idle_control"),
}),
	};
}