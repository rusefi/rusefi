package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class ElectronicThrottleMeta {
	public static final Request[] CONTENT = new Request[]{
			new IfRequest("hasEtbPedalPositionSensor",
				new Request[]{
				new SensorRequest("TPS"),
				new TextRequest("eol"),
				new TextRequest("Pedal"),
				new SensorRequest("PPS"),
				new TextRequest("eol"),
				new TextRequest("Output"),
				new FieldRequest("output"),
				new TextRequest("iTerm"),
				new FieldRequest("iTerm"),
				new TextRequest("eol"),
				new TextRequest("Settings"),
				new ConfigRequest("ETB_PFACTOR"),
				new ConfigRequest("ETB_IFACTOR"),
				new ConfigRequest("ETB_DFACTOR"),
},
				new Request[]{
				new TextRequest("No_Pedal_Sensor"),
}),
	};
}