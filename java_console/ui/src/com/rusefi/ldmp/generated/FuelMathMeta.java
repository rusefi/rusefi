package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class FuelMathMeta {
	public static final Request[] CONTENT = new Request[]{
			new IfRequest("isCrankingState",
				new Request[]{
				new TextRequest("Duration_coef"),
				new FieldRequest("cranking_durationCoefficient"),
				new TextRequest("eol"),
				new TextRequest("Coolant_coef"),
				new FieldRequest("cranking_coolantTemperatureCoefficient"),
				new SensorRequest("CLT"),
				new TextRequest("eol"),
				new TextRequest("TPS_coef"),
				new FieldRequest("cranking_tpsCoefficient"),
				new SensorRequest("TPS"),
				new TextRequest("eol"),
				new TextRequest("Cranking_fuel"),
				new FieldRequest("cranking_fuel"),
},
				new Request[]{
				new TextRequest("Base_fuel"),
				new FieldRequest("running_baseFuel"),
				new TextRequest("eol"),
				new TextRequest("Intake_coef"),
				new FieldRequest("running_intakeTemperatureCoefficient"),
				new SensorRequest("IAT"),
				new TextRequest("eol"),
				new TextRequest("Coolant_coef"),
				new FieldRequest("running_coolantTemperatureCoefficient"),
				new SensorRequest("CLT"),
				new TextRequest("eol"),
				new TextRequest("Post_cranking_coef"),
				new FieldRequest("running_postCrankingFuelCorrection"),
				new TextRequest("eol"),
				new TextRequest("eol"),
				new TextRequest("Running_fuel"),
				new FieldRequest("running_fuel"),
				new TextRequest("eol"),
				new TextRequest("Injector_lag"),
				new FieldRequest("running_injectorLag"),
				new SensorRequest("VBATT"),
}),
	};
}