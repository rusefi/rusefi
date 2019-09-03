package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class TriggerDecoderMeta {
	public static final Request[] CONTENT = new Request[]{
			new ConfigRequest("TRIGGERINPUTPINS1"),
			new TextRequest("Trigger_1_Fall"),
			new FieldRequest("Trigger", "HWEVENTCOUNTERS1"),
			new TextRequest("Rise"),
			new FieldRequest("Trigger", "HWEVENTCOUNTERS2"),
			new TextRequest("EOL"),
			new ConfigRequest("TRIGGERINPUTPINS2"),
			new TextRequest("Trigger_2_Fall"),
			new FieldRequest("Trigger", "HWEVENTCOUNTERS3"),
			new TextRequest("Rise"),
			new FieldRequest("Trigger", "HWEVENTCOUNTERS4"),
			new TextRequest("EOL"),
			new TextRequest("VVT_1"),
			new ConfigRequest("CAMINPUTS1"),
			new FieldRequest("Trigger", "vvtEventRiseCounter"),
			new FieldRequest("Trigger", "vvtEventFallCounter"),
			new FieldRequest("Trigger", "vvtCamCounter"),
	};
}