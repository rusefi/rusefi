package com.rusefi.ldmp.generated;

import com.rusefi.ldmp.*;

public class ThermistorsMeta {
	public static final Request[] CONTENT = new Request[]{
			new TextRequest("Analog_MCU_reads"),
			new FieldRequest("voltageMCU"),
			new TextRequest("from_pin"),
			new ConfigRequest("adcChannel"),
			new TextRequest("EOL"),
			new TextRequest("Analog_ECU_reads"),
			new FieldRequest("voltageBoard"),
			new TextRequest("Rdivider"),
			new ConfigRequest("analogInputDividerCoefficient"),
			new TextRequest("EOL"),
			new TextRequest("Measured_resistance"),
			new FieldRequest("resistance"),
	};
}