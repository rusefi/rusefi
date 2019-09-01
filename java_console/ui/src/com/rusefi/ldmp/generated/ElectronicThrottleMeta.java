package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class ElectronicThrottleMeta {
	public static final Request[] CONTENT = new Request[]{
			new IfRequest("Engine", "hasEtbPedalPositionSensor",
				new Request[]{
				new TextRequest("Electronic_Throttle"),
				new SensorRequest("TPS"),
				new TextRequest("eol"),
				new TextRequest("Pedal"),
				new SensorRequest("PPS"),
				new ConfigRequest("throttlePedalPositionAdcChannel"),
				new TextRequest("eol"),
				new TextRequest("Feed_forward"),
				new FieldRequest("Engine", "etbFeedForward"),
				new TextRequest("eol"),
				new TextRequest("Output"),
				new FieldRequest("ETB_pid", "output"),
				new TextRequest("iTerm"),
				new FieldRequest("ETB_pid", "iTerm"),
				new TextRequest("eol"),
				new TextRequest("Settings"),
				new ConfigRequest("ETB_PFACTOR"),
				new ConfigRequest("ETB_IFACTOR"),
				new ConfigRequest("ETB_DFACTOR"),
				new TextRequest("eol"),
				new ConfigRequest("ETB_OFFSET"),
				new ConfigRequest("ETB_PERIODMS"),
},
				new Request[]{
				new TextRequest("No_Pedal_Sensor"),
}),
	};
}