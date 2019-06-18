package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class TpsMeta {
	public static final Request[] CONTENT = new Request[]{
			new TextRequest("Analog_MCU_reads"),
			new FieldRequest("tpsVoltageMCU"),
			new TextRequest("Volts"),
			new TextRequest("from_pin"),
			new ConfigRequest("tpsADC"),
			new TextRequest("EOL"),
			new TextRequest("Analog_ECU_reads"),
			new FieldRequest("tpsVoltageBoard"),
			new TextRequest("Rdivider"),
			new ConfigRequest("analogInputDividerCoefficient"),
			new TextRequest("EOL"),
			new TextRequest("Current_ADC"),
			new FieldRequest("currentTpsAdc"),
			new TextRequest("interpolate_between"),
			new ConfigRequest("tpsMax"),
			new TextRequest("and"),
			new ConfigRequest("tpsMin"),
	};
}