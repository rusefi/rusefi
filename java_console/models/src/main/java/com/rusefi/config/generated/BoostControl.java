package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/boost_control.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class BoostControl {
	public static final Field ISTPSINVALID = Field.create("ISTPSINVALID", 0, FieldType.BIT, 0).setBaseOffset(1144);
	public static final Field M_SHOULDRESETPID = Field.create("M_SHOULDRESETPID", 0, FieldType.BIT, 1).setBaseOffset(1144);
	public static final Field ISBELOWCLOSEDLOOPTHRESHOLD = Field.create("ISBELOWCLOSEDLOOPTHRESHOLD", 0, FieldType.BIT, 2).setBaseOffset(1144);
	public static final Field ISNOTCLOSEDLOOP = Field.create("ISNOTCLOSEDLOOP", 0, FieldType.BIT, 3).setBaseOffset(1144);
	public static final Field ISZERORPM = Field.create("ISZERORPM", 0, FieldType.BIT, 4).setBaseOffset(1144);
	public static final Field HASINITBOOST = Field.create("HASINITBOOST", 0, FieldType.BIT, 5).setBaseOffset(1144);
	public static final Field RPMTOOLOW = Field.create("RPMTOOLOW", 0, FieldType.BIT, 6).setBaseOffset(1144);
	public static final Field TPSTOOLOW = Field.create("TPSTOOLOW", 0, FieldType.BIT, 7).setBaseOffset(1144);
	public static final Field MAPTOOLOW = Field.create("MAPTOOLOW", 0, FieldType.BIT, 8).setBaseOffset(1144);
	public static final Field ISPLANTVALID = Field.create("ISPLANTVALID", 0, FieldType.BIT, 9).setBaseOffset(1144);
	public static final Field ISBOOSTCONTROLLED = Field.create("ISBOOSTCONTROLLED", 0, FieldType.BIT, 10).setBaseOffset(1144);
	public static final Field LUATARGETADD = Field.create("LUATARGETADD", 4, FieldType.INT16).setScale(0.5).setBaseOffset(1144);
	public static final Field BOOSTOUTPUT = Field.create("BOOSTOUTPUT", 6, FieldType.INT16).setScale(0.01).setBaseOffset(1144);
	public static final Field LUATARGETMULT = Field.create("LUATARGETMULT", 8, FieldType.FLOAT).setBaseOffset(1144);
	public static final Field OPENLOOPPART = Field.create("OPENLOOPPART", 12, FieldType.FLOAT).setBaseOffset(1144);
	public static final Field LUAOPENLOOPADD = Field.create("LUAOPENLOOPADD", 16, FieldType.FLOAT).setBaseOffset(1144);
	public static final Field BOOSTCONTROLLERCLOSEDLOOPPART = Field.create("BOOSTCONTROLLERCLOSEDLOOPPART", 20, FieldType.INT8).setScale(0.5).setBaseOffset(1144);
	public static final Field ALIGNMENTFILL_AT_21 = Field.create("ALIGNMENTFILL_AT_21", 21, FieldType.INT8).setScale(1.0).setBaseOffset(1144);
	public static final Field BOOSTCONTROLTARGET = Field.create("BOOSTCONTROLTARGET", 22, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(1144);
	public static final Field[] VALUES = {
	ISTPSINVALID,
	M_SHOULDRESETPID,
	ISBELOWCLOSEDLOOPTHRESHOLD,
	ISNOTCLOSEDLOOP,
	ISZERORPM,
	HASINITBOOST,
	RPMTOOLOW,
	TPSTOOLOW,
	MAPTOOLOW,
	ISPLANTVALID,
	ISBOOSTCONTROLLED,
	LUATARGETADD,
	BOOSTOUTPUT,
	LUATARGETMULT,
	OPENLOOPPART,
	LUAOPENLOOPADD,
	BOOSTCONTROLLERCLOSEDLOOPPART,
	ALIGNMENTFILL_AT_21,
	BOOSTCONTROLTARGET,
	};
}
