package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class SpeedDensityMeta {
	public static final Request[] CONTENT = new Request[]{
			new IfRequest("isTChargeAirModel",
				new Request[]{
				new TextRequest("interpolate_Air_Flow"),
				new FieldRequest("","airFlow"),
				new TextRequest("Between"),
				new ConfigRequest("tChargeAirCoefMin"),
				new ConfigRequest("tChargeAirFlowMax"),
				new ConfigRequest("tChargeAirCoefMax"),
},
				new Request[]{
				new TextRequest("interpolate_3D"),
				new SensorRequest("RPM"),
				new TextRequest("and"),
				new SensorRequest("TPS"),
				new TextRequest("EOL"),
				new TextRequest("Between"),
				new ConfigRequest("tChargeMinRpmMinTps"),
				new ConfigRequest("tChargeMinRpmMaxTps"),
				new TextRequest("EOL"),
				new ConfigRequest("tChargeMaxRpmMinTps"),
				new ConfigRequest("tChargeMaxRpmMaxTps"),
}),
	};
}