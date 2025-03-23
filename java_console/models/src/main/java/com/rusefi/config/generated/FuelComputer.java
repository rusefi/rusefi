package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class FuelComputer {
	public static final Field TOTALFUELCORRECTION = Field.create("TOTALFUELCORRECTION", 0, FieldType.FLOAT).setBaseOffset(876);
	public static final Field RUNNING_POSTCRANKINGFUELCORRECTION = Field.create("RUNNING_POSTCRANKINGFUELCORRECTION", 4, FieldType.FLOAT).setBaseOffset(876);
	public static final Field RUNNING_INTAKETEMPERATURECOEFFICIENT = Field.create("RUNNING_INTAKETEMPERATURECOEFFICIENT", 8, FieldType.FLOAT).setBaseOffset(876);
	public static final Field RUNNING_COOLANTTEMPERATURECOEFFICIENT = Field.create("RUNNING_COOLANTTEMPERATURECOEFFICIENT", 12, FieldType.FLOAT).setBaseOffset(876);
	public static final Field RUNNING_TIMESINCECRANKINGINSECS = Field.create("RUNNING_TIMESINCECRANKINGINSECS", 16, FieldType.FLOAT).setBaseOffset(876);
	public static final Field RUNNING_BASEFUEL = Field.create("RUNNING_BASEFUEL", 20, FieldType.INT16).setScale(0.01).setBaseOffset(876);
	public static final Field RUNNING_FUEL = Field.create("RUNNING_FUEL", 22, FieldType.INT16).setScale(0.01).setBaseOffset(876);
	public static final Field AFRTABLEYAXIS = Field.create("AFRTABLEYAXIS", 24, FieldType.INT16).setScale(0.01).setBaseOffset(876);
	public static final Field TARGETLAMBDA = Field.create("TARGETLAMBDA", 26, FieldType.INT16).setScale(1.0E-4).setBaseOffset(876);
	public static final Field TARGETAFR = Field.create("TARGETAFR", 28, FieldType.INT16).setScale(0.001).setBaseOffset(876);
	public static final Field STOICHIOMETRICRATIO = Field.create("STOICHIOMETRICRATIO", 30, FieldType.INT16).setScale(0.001).setBaseOffset(876);
	public static final Field SDTCHARGE_COFF = Field.create("SDTCHARGE_COFF", 32, FieldType.FLOAT).setBaseOffset(876);
	public static final Field SDAIRMASSINONECYLINDER = Field.create("SDAIRMASSINONECYLINDER", 36, FieldType.FLOAT).setBaseOffset(876);
	public static final Field NORMALIZEDCYLINDERFILLING = Field.create("NORMALIZEDCYLINDERFILLING", 40, FieldType.FLOAT).setBaseOffset(876);
	public static final Field BROKENINJECTOR = Field.create("BROKENINJECTOR", 44, FieldType.INT8).setScale(1.0).setBaseOffset(876);
	public static final Field IDEALENGINETORQUE = Field.create("IDEALENGINETORQUE", 46, FieldType.INT16).setScale(1.0).setBaseOffset(876);
	public static final Field INJECTORHWISSUE = Field.create("INJECTORHWISSUE", 48, FieldType.BIT, 0).setBaseOffset(876);
	public static final Field[] VALUES = {
	TOTALFUELCORRECTION,
	RUNNING_POSTCRANKINGFUELCORRECTION,
	RUNNING_INTAKETEMPERATURECOEFFICIENT,
	RUNNING_COOLANTTEMPERATURECOEFFICIENT,
	RUNNING_TIMESINCECRANKINGINSECS,
	RUNNING_BASEFUEL,
	RUNNING_FUEL,
	AFRTABLEYAXIS,
	TARGETLAMBDA,
	TARGETAFR,
	STOICHIOMETRICRATIO,
	SDTCHARGE_COFF,
	SDAIRMASSINONECYLINDER,
	NORMALIZEDCYLINDERFILLING,
	BROKENINJECTOR,
	IDEALENGINETORQUE,
	INJECTORHWISSUE,
	};
}
