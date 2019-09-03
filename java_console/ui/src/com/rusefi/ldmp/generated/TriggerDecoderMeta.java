package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class TriggerDecoderMeta {
	public static final Request[] CONTENT = new Request[]{
			new TextRequest("Current_Gap"),
			new FieldRequest("Trigger_State", "currentGap"),
			new TextRequest("EOL"),
			new ConfigRequest("TRIGGERINPUTPINS1"),
			new TextRequest("Trigger_1_Fall"),
			new FieldRequest("Trigger_Central", "HWEVENTCOUNTERS1"),
			new TextRequest("Rise"),
			new FieldRequest("Trigger_Central", "HWEVENTCOUNTERS2"),
			new TextRequest("EOL"),
			new ConfigRequest("TRIGGERINPUTPINS2"),
			new TextRequest("Trigger_2_Fall"),
			new FieldRequest("Trigger_Central", "HWEVENTCOUNTERS3"),
			new TextRequest("Rise"),
			new FieldRequest("Trigger_Central", "HWEVENTCOUNTERS4"),
			new TextRequest("EOL"),
			new TextRequest("VVT_1"),
			new ConfigRequest("CAMINPUTS1"),
			new FieldRequest("Trigger_Central", "vvtEventRiseCounter"),
			new FieldRequest("Trigger_Central", "vvtEventFallCounter"),
			new FieldRequest("Trigger_Central", "vvtCamCounter"),
	};
}