package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class FuelMathMeta {
	public static final Request[] CONTENT = new Request[]{
			new IfRequest("isCranking",
				new Request[]{
				new FieldRequest("cranking_durationCoefficient"),
				new FieldRequest("cranking_coolantTemperatureCoefficient"),
				new FieldRequest("cranking_tpsCoefficient"),
				new FieldRequest("cranking_fuel"),
},
				new Request[]{
}),
	};
}