package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class SpeedDensityMeta {
	public static final Request[] CONTENT = new Request[]{
			new IfRequest("isTChargeAirModel",
				new Request[]{
				new TextRequest("interpolate_Air_Flow"),
				new FieldRequest("airFlow"),
				new TextRequest("Between"),
				new TextRequest("tChargeAirCoefMin"),
				new ConfigRequest("tChargeAirCoefMin"),
				new TextRequest("tChargeAirFlowMax"),
				new ConfigRequest("tChargeAirFlowMax"),
				new TextRequest("tChargeAirCoefMax"),
				new ConfigRequest("tChargeAirCoefMax"),
},
				new Request[]{
				new TextRequest("interpolate_RPM"),
				new SensorRequest("RPM"),
				new TextRequest("and_TPS"),
				new SensorRequest("TPS"),
				new TextRequest("EOL"),
				new TextRequest("Between"),
				new TextRequest("tChargeMinRpmMinTps"),
				new ConfigRequest("tChargeMinRpmMinTps"),
				new TextRequest("EOL"),
				new TextRequest("tChargeMinRpmMaxTps"),
				new ConfigRequest("tChargeMinRpmMaxTps"),
				new ConfigRequest("tChargeMaxRpmMinTps"),
				new ConfigRequest("tChargeMaxRpmMaxTps"),
}),
	};
}