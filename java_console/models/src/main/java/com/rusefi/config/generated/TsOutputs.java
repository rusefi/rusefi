package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) console/binary/output_channels.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TsOutputs {
	public static final Field SD_PRESENT = Field.create("SD_PRESENT", 0, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field SD_LOGGING_INTERNAL = Field.create("SD_LOGGING_INTERNAL", 0, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field TRIGGERSCOPEREADY = Field.create("TRIGGERSCOPEREADY", 0, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field ANTILAGTRIGGERED = Field.create("ANTILAGTRIGGERED", 0, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field ISO2HEATERON = Field.create("ISO2HEATERON", 0, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field CHECKENGINE = Field.create("CHECKENGINE", 0, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field NEEDBURN = Field.create("NEEDBURN", 0, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field SD_MSD = Field.create("SD_MSD", 0, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field TOOTHLOGREADY = Field.create("TOOTHLOGREADY", 0, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field ISTPSERROR = Field.create("ISTPSERROR", 0, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field ISCLTERROR = Field.create("ISCLTERROR", 0, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field ISMAPERROR = Field.create("ISMAPERROR", 0, FieldType.BIT, 11).setBaseOffset(0);
	public static final Field ISIATERROR = Field.create("ISIATERROR", 0, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field ISTRIGGERERROR = Field.create("ISTRIGGERERROR", 0, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field HASCRITICALERROR = Field.create("HASCRITICALERROR", 0, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field ISWARNNOW = Field.create("ISWARNNOW", 0, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field ISPEDALERROR = Field.create("ISPEDALERROR", 0, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field ISKNOCKCHIPOK = Field.create("ISKNOCKCHIPOK", 0, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field LAUNCHTRIGGERED = Field.create("LAUNCHTRIGGERED", 0, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field ISTPS2ERROR = Field.create("ISTPS2ERROR", 0, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field INJECTORFAULT = Field.create("INJECTORFAULT", 0, FieldType.BIT, 20).setBaseOffset(0);
	public static final Field IGNITIONFAULT = Field.create("IGNITIONFAULT", 0, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field ISUSBCONNECTED = Field.create("ISUSBCONNECTED", 0, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field DFCOACTIVE = Field.create("DFCOACTIVE", 0, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field SD_ACTIVE_WR = Field.create("SD_ACTIVE_WR", 0, FieldType.BIT, 24).setBaseOffset(0);
	public static final Field SD_ACTIVE_RD = Field.create("SD_ACTIVE_RD", 0, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field ISMAPVALID = Field.create("ISMAPVALID", 0, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field TRIGGERPAGEREFRESHFLAG = Field.create("TRIGGERPAGEREFRESHFLAG", 0, FieldType.BIT, 27).setBaseOffset(0);
	public static final Field HASFAULTREPORTFILE = Field.create("HASFAULTREPORTFILE", 0, FieldType.BIT, 28).setBaseOffset(0);
	public static final Field ISANALOGFAILURE = Field.create("ISANALOGFAILURE", 0, FieldType.BIT, 29).setBaseOffset(0);
	public static final Field ISTUNINGNOW = Field.create("ISTUNINGNOW", 0, FieldType.BIT, 30).setBaseOffset(0);
	public static final Field RPMVALUE = Field.create("RPMVALUE", 4, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RPMACCELERATION = Field.create("RPMACCELERATION", 6, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SPEEDTORPMRATIO = Field.create("SPEEDTORPMRATIO", 8, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field INTERNALMCUTEMPERATURE = Field.create("INTERNALMCUTEMPERATURE", 11, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field COOLANT = Field.create("COOLANT", 12, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field INTAKE = Field.create("INTAKE", 14, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field AUXTEMP1 = Field.create("AUXTEMP1", 16, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field AUXTEMP2 = Field.create("AUXTEMP2", 18, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TPSVALUE = Field.create("TPSVALUE", 20, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field THROTTLEPEDALPOSITION = Field.create("THROTTLEPEDALPOSITION", 22, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TPSADC = Field.create("TPSADC", 24, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RAWMAF = Field.create("RAWMAF", 26, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field MAFMEASURED = Field.create("MAFMEASURED", 28, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field MAPVALUE = Field.create("MAPVALUE", 30, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(0);
	public static final Field BAROPRESSURE = Field.create("BAROPRESSURE", 32, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(0);
	public static final Field LAMBDAVALUE = Field.create("LAMBDAVALUE", 34, FieldType.INT16).setScale(1.0E-4).setBaseOffset(0);
	public static final Field VBATT = Field.create("VBATT", 36, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field OILPRESSURE = Field.create("OILPRESSURE", 38, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(0);
	public static final Field VVTPOSITIONB1I = Field.create("VVTPOSITIONB1I", 40, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field ACTUALLASTINJECTION = Field.create("ACTUALLASTINJECTION", 42, FieldType.INT16).setScale(0.0033333333333333335).setBaseOffset(0);
	public static final Field INJECTORDUTYCYCLE = Field.create("INJECTORDUTYCYCLE", 44, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field TEMPLOGGING1 = Field.create("TEMPLOGGING1", 45, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONOFFSET = Field.create("INJECTIONOFFSET", 46, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ENGINEMAKECODENAMECRC16 = Field.create("ENGINEMAKECODENAMECRC16", 48, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field WALLFUELAMOUNT = Field.create("WALLFUELAMOUNT", 50, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field WALLFUELCORRECTIONVALUE = Field.create("WALLFUELCORRECTIONVALUE", 52, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field REVOLUTIONCOUNTERSINCESTART = Field.create("REVOLUTIONCOUNTERSINCESTART", 54, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CANREADCOUNTER = Field.create("CANREADCOUNTER", 56, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPSACCELFUEL = Field.create("TPSACCELFUEL", 58, FieldType.INT16).setScale(0.0033333333333333335).setBaseOffset(0);
	public static final Field CURRENTIGNITIONMODE = Field.create("CURRENTIGNITIONMODE", 60, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CURRENTINJECTIONMODE = Field.create("CURRENTINJECTIONMODE", 61, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field COILDUTYCYCLE = Field.create("COILDUTYCYCLE", 62, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ETB1DUTYCYCLE = Field.create("ETB1DUTYCYCLE", 64, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field FUELTANKLEVEL = Field.create("FUELTANKLEVEL", 66, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TOTALFUELCONSUMPTION = Field.create("TOTALFUELCONSUMPTION", 68, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field FUELFLOWRATE = Field.create("FUELFLOWRATE", 70, FieldType.INT16).setScale(0.005).setBaseOffset(0);
	public static final Field TPS2VALUE = Field.create("TPS2VALUE", 72, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TUNECRC16 = Field.create("TUNECRC16", 74, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEVALUE = Field.create("VEVALUE", 76, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_78 = Field.create("ALIGNMENTFILL_AT_78", 78, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SECONDS = Field.create("SECONDS", 80, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field ENGINEMODE = Field.create("ENGINEMODE", 84, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field FIRMWAREVERSION = Field.create("FIRMWAREVERSION", 88, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field RAWIDLEPOSITIONSENSOR = Field.create("RAWIDLEPOSITIONSENSOR", 92, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWWASTEGATEPOSITION = Field.create("RAWWASTEGATEPOSITION", 94, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field ACCELERATIONLAT = Field.create("ACCELERATIONLAT", 96, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field ACCELERATIONLON = Field.create("ACCELERATIONLON", 98, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field DETECTEDGEAR = Field.create("DETECTEDGEAR", 100, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXTRIGGERREENTRANT = Field.create("MAXTRIGGERREENTRANT", 101, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field RAWLOWFUELPRESSURE = Field.create("RAWLOWFUELPRESSURE", 102, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWHIGHFUELPRESSURE = Field.create("RAWHIGHFUELPRESSURE", 104, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field LOWFUELPRESSURE = Field.create("LOWFUELPRESSURE", 106, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(0);
	public static final Field TCUDESIREDGEAR = Field.create("TCUDESIREDGEAR", 108, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FLEXPERCENT = Field.create("FLEXPERCENT", 109, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field WASTEGATEPOSITIONSENSOR = Field.create("WASTEGATEPOSITIONSENSOR", 110, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field HIGHFUELPRESSURE = Field.create("HIGHFUELPRESSURE", 112, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TEMPLOGGING3 = Field.create("TEMPLOGGING3", 114, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TEMPLOGGING4 = Field.create("TEMPLOGGING4", 115, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CALIBRATIONVALUE = Field.create("CALIBRATIONVALUE", 116, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CALIBRATIONMODE = Field.create("CALIBRATIONMODE", 120, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLESTEPPERTARGETPOSITION = Field.create("IDLESTEPPERTARGETPOSITION", 121, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_122 = Field.create("ALIGNMENTFILL_AT_122", 122, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TOTALTRIGGERERRORCOUNTER = Field.create("TOTALTRIGGERERRORCOUNTER", 124, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field ORDERINGERRORCOUNTER = Field.create("ORDERINGERRORCOUNTER", 128, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field WARNINGCOUNTER = Field.create("WARNINGCOUNTER", 132, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field LASTERRORCODE = Field.create("LASTERRORCODE", 134, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE1 = Field.create("RECENTERRORCODE1", 136, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE2 = Field.create("RECENTERRORCODE2", 138, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE3 = Field.create("RECENTERRORCODE3", 140, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE4 = Field.create("RECENTERRORCODE4", 142, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE5 = Field.create("RECENTERRORCODE5", 144, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE6 = Field.create("RECENTERRORCODE6", 146, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE7 = Field.create("RECENTERRORCODE7", 148, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RECENTERRORCODE8 = Field.create("RECENTERRORCODE8", 150, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DEBUGFLOATFIELD1 = Field.create("DEBUGFLOATFIELD1", 152, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DEBUGFLOATFIELD2 = Field.create("DEBUGFLOATFIELD2", 156, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DEBUGFLOATFIELD3 = Field.create("DEBUGFLOATFIELD3", 160, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DEBUGFLOATFIELD4 = Field.create("DEBUGFLOATFIELD4", 164, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DEBUGFLOATFIELD5 = Field.create("DEBUGFLOATFIELD5", 168, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DEBUGFLOATFIELD6 = Field.create("DEBUGFLOATFIELD6", 172, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DEBUGFLOATFIELD7 = Field.create("DEBUGFLOATFIELD7", 176, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DEBUGINTFIELD1 = Field.create("DEBUGINTFIELD1", 180, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field DEBUGINTFIELD2 = Field.create("DEBUGINTFIELD2", 184, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field DEBUGINTFIELD3 = Field.create("DEBUGINTFIELD3", 188, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field DEBUGINTFIELD4 = Field.create("DEBUGINTFIELD4", 192, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DEBUGINTFIELD5 = Field.create("DEBUGINTFIELD5", 194, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field EGT1 = Field.create("EGT1", 196, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field EGT2 = Field.create("EGT2", 198, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field EGT3 = Field.create("EGT3", 200, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field EGT4 = Field.create("EGT4", 202, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field EGT5 = Field.create("EGT5", 204, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field EGT6 = Field.create("EGT6", 206, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field EGT7 = Field.create("EGT7", 208, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field EGT8 = Field.create("EGT8", 210, FieldType.INT16).setScale(0.25).setBaseOffset(0);
	public static final Field RAWTPS1PRIMARY = Field.create("RAWTPS1PRIMARY", 212, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWCLT = Field.create("RAWCLT", 214, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWIAT = Field.create("RAWIAT", 216, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWOILPRESSURE = Field.create("RAWOILPRESSURE", 218, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWACPRESSURE = Field.create("RAWACPRESSURE", 220, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWFUELLEVEL = Field.create("RAWFUELLEVEL", 222, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field FUELCLOSEDLOOPBINIDX = Field.create("FUELCLOSEDLOOPBINIDX", 224, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_225 = Field.create("ALIGNMENTFILL_AT_225", 225, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field RAWPPSPRIMARY = Field.create("RAWPPSPRIMARY", 228, FieldType.FLOAT).setBaseOffset(0);
	public static final Field RAWPPSSECONDARY = Field.create("RAWPPSSECONDARY", 232, FieldType.FLOAT).setBaseOffset(0);
	public static final Field RAWRAWPPSPRIMARY = Field.create("RAWRAWPPSPRIMARY", 236, FieldType.FLOAT).setBaseOffset(0);
	public static final Field RAWRAWPPSSECONDARY = Field.create("RAWRAWPPSSECONDARY", 240, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLEPOSITIONSENSOR = Field.create("IDLEPOSITIONSENSOR", 244, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field AFRVALUE = Field.create("AFRVALUE", 246, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field AFRVALUE2 = Field.create("AFRVALUE2", 248, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field SMOOTHEDAFRVALUE = Field.create("SMOOTHEDAFRVALUE", 250, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field SMOOTHEDAFRVALUE2 = Field.create("SMOOTHEDAFRVALUE2", 252, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field VSSACCELERATION = Field.create("VSSACCELERATION", 254, FieldType.INT16).setScale(0.0033333333333333335).setBaseOffset(0);
	public static final Field LAMBDAVALUE2 = Field.create("LAMBDAVALUE2", 256, FieldType.INT16).setScale(1.0E-4).setBaseOffset(0);
	public static final Field VVTPOSITIONB1E = Field.create("VVTPOSITIONB1E", 258, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field VVTPOSITIONB2I = Field.create("VVTPOSITIONB2I", 260, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field VVTPOSITIONB2E = Field.create("VVTPOSITIONB2E", 262, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field FUELPIDCORRECTION1 = Field.create("FUELPIDCORRECTION1", 264, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field FUELPIDCORRECTION2 = Field.create("FUELPIDCORRECTION2", 266, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field RAWTPS1SECONDARY = Field.create("RAWTPS1SECONDARY", 268, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWTPS2PRIMARY = Field.create("RAWTPS2PRIMARY", 270, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWTPS2SECONDARY = Field.create("RAWTPS2SECONDARY", 272, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field ACCELERATIONVERT = Field.create("ACCELERATIONVERT", 274, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field GYROYAW = Field.create("GYROYAW", 276, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field VVTTARGETS1 = Field.create("VVTTARGETS1", 278, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTARGETS2 = Field.create("VVTTARGETS2", 279, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTARGETS3 = Field.create("VVTTARGETS3", 280, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTARGETS4 = Field.create("VVTTARGETS4", 281, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TURBOSPEED = Field.create("TURBOSPEED", 282, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL1 = Field.create("IGNITIONADVANCECYL1", 284, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL2 = Field.create("IGNITIONADVANCECYL2", 286, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL3 = Field.create("IGNITIONADVANCECYL3", 288, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL4 = Field.create("IGNITIONADVANCECYL4", 290, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL5 = Field.create("IGNITIONADVANCECYL5", 292, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL6 = Field.create("IGNITIONADVANCECYL6", 294, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL7 = Field.create("IGNITIONADVANCECYL7", 296, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL8 = Field.create("IGNITIONADVANCECYL8", 298, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL9 = Field.create("IGNITIONADVANCECYL9", 300, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL10 = Field.create("IGNITIONADVANCECYL10", 302, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL11 = Field.create("IGNITIONADVANCECYL11", 304, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field IGNITIONADVANCECYL12 = Field.create("IGNITIONADVANCECYL12", 306, FieldType.INT16).setScale(0.02).setBaseOffset(0);
	public static final Field TPS1SPLIT = Field.create("TPS1SPLIT", 308, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TPS2SPLIT = Field.create("TPS2SPLIT", 310, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TPS12SPLIT = Field.create("TPS12SPLIT", 312, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ACCPEDALSPLIT = Field.create("ACCPEDALSPLIT", 314, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field SPARKCUTREASON = Field.create("SPARKCUTREASON", 316, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FUELCUTREASON = Field.create("FUELCUTREASON", 317, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAFESTIMATE = Field.create("MAFESTIMATE", 318, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field INSTANTRPM = Field.create("INSTANTRPM", 320, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RAWMAP = Field.create("RAWMAP", 322, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWAFR = Field.create("RAWAFR", 324, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_326 = Field.create("ALIGNMENTFILL_AT_326", 326, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CALIBRATIONVALUE2 = Field.create("CALIBRATIONVALUE2", 328, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAINVOCATIONCOUNTER = Field.create("LUAINVOCATIONCOUNTER", 332, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field LUALASTCYCLEDURATION = Field.create("LUALASTCYCLEDURATION", 336, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field VSSEDGECOUNTER = Field.create("VSSEDGECOUNTER", 340, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field ISSEDGECOUNTER = Field.create("ISSEDGECOUNTER", 344, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR1 = Field.create("AUXLINEAR1", 348, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR2 = Field.create("AUXLINEAR2", 352, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR3 = Field.create("AUXLINEAR3", 356, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR4 = Field.create("AUXLINEAR4", 360, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FALLBACKMAP = Field.create("FALLBACKMAP", 364, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field INSTANTMAPVALUE = Field.create("INSTANTMAPVALUE", 366, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(0);
	public static final Field MAXLOCKEDDURATION = Field.create("MAXLOCKEDDURATION", 368, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CANWRITEOK = Field.create("CANWRITEOK", 370, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CANWRITENOTOK = Field.create("CANWRITENOTOK", 372, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_374 = Field.create("ALIGNMENTFILL_AT_374", 374, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERPRIMARYFALL = Field.create("TRIGGERPRIMARYFALL", 376, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERPRIMARYRISE = Field.create("TRIGGERPRIMARYRISE", 380, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERSECONDARYFALL = Field.create("TRIGGERSECONDARYFALL", 384, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERSECONDARYRISE = Field.create("TRIGGERSECONDARYRISE", 388, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERVVTFALL = Field.create("TRIGGERVVTFALL", 392, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERVVTRISE = Field.create("TRIGGERVVTRISE", 394, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERVVT2FALL = Field.create("TRIGGERVVT2FALL", 396, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERVVT2RISE = Field.create("TRIGGERVVT2RISE", 398, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STARTERSTATE = Field.create("STARTERSTATE", 400, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STARTERRELAYDISABLE = Field.create("STARTERRELAYDISABLE", 401, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MULTISPARKCOUNTER = Field.create("MULTISPARKCOUNTER", 402, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field EXTIOVERFLOWCOUNT = Field.create("EXTIOVERFLOWCOUNT", 403, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORSTATUS_PTERM = Field.create("ALTERNATORSTATUS_PTERM", 404, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ALTERNATORSTATUS_ITERM = Field.create("ALTERNATORSTATUS_ITERM", 408, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ALTERNATORSTATUS_DTERM = Field.create("ALTERNATORSTATUS_DTERM", 410, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ALTERNATORSTATUS_OUTPUT = Field.create("ALTERNATORSTATUS_OUTPUT", 412, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ALTERNATORSTATUS_ERROR = Field.create("ALTERNATORSTATUS_ERROR", 414, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ALTERNATORSTATUS_RESETCOUNTER = Field.create("ALTERNATORSTATUS_RESETCOUNTER", 416, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field IDLESTATUS_PTERM = Field.create("IDLESTATUS_PTERM", 420, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLESTATUS_ITERM = Field.create("IDLESTATUS_ITERM", 424, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field IDLESTATUS_DTERM = Field.create("IDLESTATUS_DTERM", 426, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field IDLESTATUS_OUTPUT = Field.create("IDLESTATUS_OUTPUT", 428, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field IDLESTATUS_ERROR = Field.create("IDLESTATUS_ERROR", 430, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field IDLESTATUS_RESETCOUNTER = Field.create("IDLESTATUS_RESETCOUNTER", 432, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field ETBSTATUS_PTERM = Field.create("ETBSTATUS_PTERM", 436, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETBSTATUS_ITERM = Field.create("ETBSTATUS_ITERM", 440, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ETBSTATUS_DTERM = Field.create("ETBSTATUS_DTERM", 442, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ETBSTATUS_OUTPUT = Field.create("ETBSTATUS_OUTPUT", 444, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ETBSTATUS_ERROR = Field.create("ETBSTATUS_ERROR", 446, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ETBSTATUS_RESETCOUNTER = Field.create("ETBSTATUS_RESETCOUNTER", 448, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTSTATUS_PTERM = Field.create("BOOSTSTATUS_PTERM", 452, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BOOSTSTATUS_ITERM = Field.create("BOOSTSTATUS_ITERM", 456, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field BOOSTSTATUS_DTERM = Field.create("BOOSTSTATUS_DTERM", 458, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field BOOSTSTATUS_OUTPUT = Field.create("BOOSTSTATUS_OUTPUT", 460, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field BOOSTSTATUS_ERROR = Field.create("BOOSTSTATUS_ERROR", 462, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field BOOSTSTATUS_RESETCOUNTER = Field.create("BOOSTSTATUS_RESETCOUNTER", 464, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field WASTEGATEDCSTATUS_PTERM = Field.create("WASTEGATEDCSTATUS_PTERM", 468, FieldType.FLOAT).setBaseOffset(0);
	public static final Field WASTEGATEDCSTATUS_ITERM = Field.create("WASTEGATEDCSTATUS_ITERM", 472, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field WASTEGATEDCSTATUS_DTERM = Field.create("WASTEGATEDCSTATUS_DTERM", 474, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field WASTEGATEDCSTATUS_OUTPUT = Field.create("WASTEGATEDCSTATUS_OUTPUT", 476, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field WASTEGATEDCSTATUS_ERROR = Field.create("WASTEGATEDCSTATUS_ERROR", 478, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field WASTEGATEDCSTATUS_RESETCOUNTER = Field.create("WASTEGATEDCSTATUS_RESETCOUNTER", 480, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field VVTSTATUS1_PTERM = Field.create("VVTSTATUS1_PTERM", 484, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTSTATUS1_ITERM = Field.create("VVTSTATUS1_ITERM", 488, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS1_DTERM = Field.create("VVTSTATUS1_DTERM", 490, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS1_OUTPUT = Field.create("VVTSTATUS1_OUTPUT", 492, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS1_ERROR = Field.create("VVTSTATUS1_ERROR", 494, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS1_RESETCOUNTER = Field.create("VVTSTATUS1_RESETCOUNTER", 496, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field VVTSTATUS2_PTERM = Field.create("VVTSTATUS2_PTERM", 500, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTSTATUS2_ITERM = Field.create("VVTSTATUS2_ITERM", 504, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS2_DTERM = Field.create("VVTSTATUS2_DTERM", 506, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS2_OUTPUT = Field.create("VVTSTATUS2_OUTPUT", 508, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS2_ERROR = Field.create("VVTSTATUS2_ERROR", 510, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS2_RESETCOUNTER = Field.create("VVTSTATUS2_RESETCOUNTER", 512, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field VVTSTATUS3_PTERM = Field.create("VVTSTATUS3_PTERM", 516, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTSTATUS3_ITERM = Field.create("VVTSTATUS3_ITERM", 520, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS3_DTERM = Field.create("VVTSTATUS3_DTERM", 522, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS3_OUTPUT = Field.create("VVTSTATUS3_OUTPUT", 524, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS3_ERROR = Field.create("VVTSTATUS3_ERROR", 526, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS3_RESETCOUNTER = Field.create("VVTSTATUS3_RESETCOUNTER", 528, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field VVTSTATUS4_PTERM = Field.create("VVTSTATUS4_PTERM", 532, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTSTATUS4_ITERM = Field.create("VVTSTATUS4_ITERM", 536, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS4_DTERM = Field.create("VVTSTATUS4_DTERM", 538, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS4_OUTPUT = Field.create("VVTSTATUS4_OUTPUT", 540, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS4_ERROR = Field.create("VVTSTATUS4_ERROR", 542, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTSTATUS4_RESETCOUNTER = Field.create("VVTSTATUS4_RESETCOUNTER", 544, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field AUXSPEED1 = Field.create("AUXSPEED1", 548, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXSPEED2 = Field.create("AUXSPEED2", 550, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ISSVALUE = Field.create("ISSVALUE", 552, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RAWANALOGINPUT1 = Field.create("RAWANALOGINPUT1", 554, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWANALOGINPUT2 = Field.create("RAWANALOGINPUT2", 556, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWANALOGINPUT3 = Field.create("RAWANALOGINPUT3", 558, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWANALOGINPUT4 = Field.create("RAWANALOGINPUT4", 560, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWANALOGINPUT5 = Field.create("RAWANALOGINPUT5", 562, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWANALOGINPUT6 = Field.create("RAWANALOGINPUT6", 564, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWANALOGINPUT7 = Field.create("RAWANALOGINPUT7", 566, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWANALOGINPUT8 = Field.create("RAWANALOGINPUT8", 568, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field GPPWMOUTPUT1 = Field.create("GPPWMOUTPUT1", 570, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWMOUTPUT2 = Field.create("GPPWMOUTPUT2", 571, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWMOUTPUT3 = Field.create("GPPWMOUTPUT3", 572, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWMOUTPUT4 = Field.create("GPPWMOUTPUT4", 573, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWMXAXIS1 = Field.create("GPPWMXAXIS1", 574, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMXAXIS2 = Field.create("GPPWMXAXIS2", 576, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMXAXIS3 = Field.create("GPPWMXAXIS3", 578, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMXAXIS4 = Field.create("GPPWMXAXIS4", 580, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMYAXIS1 = Field.create("GPPWMYAXIS1", 582, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field GPPWMYAXIS2 = Field.create("GPPWMYAXIS2", 584, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field GPPWMYAXIS3 = Field.create("GPPWMYAXIS3", 586, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field GPPWMYAXIS4 = Field.create("GPPWMYAXIS4", 588, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field RAWBATTERY = Field.create("RAWBATTERY", 590, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field IGNBLENDPARAMETER1 = Field.create("IGNBLENDPARAMETER1", 592, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDPARAMETER2 = Field.create("IGNBLENDPARAMETER2", 594, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDBIAS1 = Field.create("IGNBLENDBIAS1", 596, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field IGNBLENDBIAS2 = Field.create("IGNBLENDBIAS2", 597, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field IGNBLENDOUTPUT1 = Field.create("IGNBLENDOUTPUT1", 598, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field IGNBLENDOUTPUT2 = Field.create("IGNBLENDOUTPUT2", 600, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VEBLENDPARAMETER1 = Field.create("VEBLENDPARAMETER1", 602, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDPARAMETER2 = Field.create("VEBLENDPARAMETER2", 604, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDPARAMETER3 = Field.create("VEBLENDPARAMETER3", 606, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDPARAMETER4 = Field.create("VEBLENDPARAMETER4", 608, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDBIAS1 = Field.create("VEBLENDBIAS1", 610, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDBIAS2 = Field.create("VEBLENDBIAS2", 611, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDBIAS3 = Field.create("VEBLENDBIAS3", 612, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDBIAS4 = Field.create("VEBLENDBIAS4", 613, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDOUTPUT1 = Field.create("VEBLENDOUTPUT1", 614, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VEBLENDOUTPUT2 = Field.create("VEBLENDOUTPUT2", 616, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VEBLENDOUTPUT3 = Field.create("VEBLENDOUTPUT3", 618, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VEBLENDOUTPUT4 = Field.create("VEBLENDOUTPUT4", 620, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDPARAMETER1 = Field.create("BOOSTOPENLOOPBLENDPARAMETER1", 622, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDPARAMETER2 = Field.create("BOOSTOPENLOOPBLENDPARAMETER2", 624, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDBIAS1 = Field.create("BOOSTOPENLOOPBLENDBIAS1", 626, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDBIAS2 = Field.create("BOOSTOPENLOOPBLENDBIAS2", 627, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDOUTPUT1 = Field.create("BOOSTOPENLOOPBLENDOUTPUT1", 628, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDOUTPUT2 = Field.create("BOOSTOPENLOOPBLENDOUTPUT2", 629, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDPARAMETER1 = Field.create("BOOSTCLOSEDLOOPBLENDPARAMETER1", 630, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDPARAMETER2 = Field.create("BOOSTCLOSEDLOOPBLENDPARAMETER2", 632, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDBIAS1 = Field.create("BOOSTCLOSEDLOOPBLENDBIAS1", 634, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDBIAS2 = Field.create("BOOSTCLOSEDLOOPBLENDBIAS2", 635, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDOUTPUT1 = Field.create("BOOSTCLOSEDLOOPBLENDOUTPUT1", 636, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDOUTPUT2 = Field.create("BOOSTCLOSEDLOOPBLENDOUTPUT2", 638, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDPARAMETER1 = Field.create("TARGETAFRBLENDPARAMETER1", 640, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDPARAMETER2 = Field.create("TARGETAFRBLENDPARAMETER2", 642, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDBIAS1 = Field.create("TARGETAFRBLENDBIAS1", 644, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field TARGETAFRBLENDBIAS2 = Field.create("TARGETAFRBLENDBIAS2", 645, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field TARGETAFRBLENDOUTPUT1 = Field.create("TARGETAFRBLENDOUTPUT1", 646, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDOUTPUT2 = Field.create("TARGETAFRBLENDOUTPUT2", 648, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_650 = Field.create("ALIGNMENTFILL_AT_650", 650, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field COILSTATE1 = Field.create("COILSTATE1", 652, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field COILSTATE2 = Field.create("COILSTATE2", 652, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field COILSTATE3 = Field.create("COILSTATE3", 652, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field COILSTATE4 = Field.create("COILSTATE4", 652, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field COILSTATE5 = Field.create("COILSTATE5", 652, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field COILSTATE6 = Field.create("COILSTATE6", 652, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field COILSTATE7 = Field.create("COILSTATE7", 652, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field COILSTATE8 = Field.create("COILSTATE8", 652, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field COILSTATE9 = Field.create("COILSTATE9", 652, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field COILSTATE10 = Field.create("COILSTATE10", 652, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field COILSTATE11 = Field.create("COILSTATE11", 652, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field COILSTATE12 = Field.create("COILSTATE12", 652, FieldType.BIT, 11).setBaseOffset(0);
	public static final Field INJECTORSTATE1 = Field.create("INJECTORSTATE1", 652, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field INJECTORSTATE2 = Field.create("INJECTORSTATE2", 652, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field INJECTORSTATE3 = Field.create("INJECTORSTATE3", 652, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field INJECTORSTATE4 = Field.create("INJECTORSTATE4", 652, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field INJECTORSTATE5 = Field.create("INJECTORSTATE5", 652, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field INJECTORSTATE6 = Field.create("INJECTORSTATE6", 652, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field INJECTORSTATE7 = Field.create("INJECTORSTATE7", 652, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field INJECTORSTATE8 = Field.create("INJECTORSTATE8", 652, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field INJECTORSTATE9 = Field.create("INJECTORSTATE9", 652, FieldType.BIT, 20).setBaseOffset(0);
	public static final Field INJECTORSTATE10 = Field.create("INJECTORSTATE10", 652, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field INJECTORSTATE11 = Field.create("INJECTORSTATE11", 652, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field INJECTORSTATE12 = Field.create("INJECTORSTATE12", 652, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field TRIGGERCHANNEL1 = Field.create("TRIGGERCHANNEL1", 652, FieldType.BIT, 24).setBaseOffset(0);
	public static final Field TRIGGERCHANNEL2 = Field.create("TRIGGERCHANNEL2", 652, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field VVTCHANNEL1 = Field.create("VVTCHANNEL1", 652, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field VVTCHANNEL2 = Field.create("VVTCHANNEL2", 652, FieldType.BIT, 27).setBaseOffset(0);
	public static final Field VVTCHANNEL3 = Field.create("VVTCHANNEL3", 652, FieldType.BIT, 28).setBaseOffset(0);
	public static final Field VVTCHANNEL4 = Field.create("VVTCHANNEL4", 652, FieldType.BIT, 29).setBaseOffset(0);
	public static final Field OUTPUTREQUESTPERIOD = Field.create("OUTPUTREQUESTPERIOD", 656, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field MAPFAST = Field.create("MAPFAST", 660, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES1 = Field.create("LUAGAUGES1", 664, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES2 = Field.create("LUAGAUGES2", 668, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES3 = Field.create("LUAGAUGES3", 672, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES4 = Field.create("LUAGAUGES4", 676, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES5 = Field.create("LUAGAUGES5", 680, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES6 = Field.create("LUAGAUGES6", 684, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES7 = Field.create("LUAGAUGES7", 688, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LUAGAUGES8 = Field.create("LUAGAUGES8", 692, FieldType.FLOAT).setBaseOffset(0);
	public static final Field RAWMAF2 = Field.create("RAWMAF2", 696, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field MAFMEASURED2 = Field.create("MAFMEASURED2", 698, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field SCHEDULINGUSEDCOUNT = Field.create("SCHEDULINGUSEDCOUNT", 700, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEHICLESPEEDKPH = Field.create("VEHICLESPEEDKPH", 702, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEGO = Field.create("GEGO", 704, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field LTFT = Field.create("LTFT", 706, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TESTBENCHITER = Field.create("TESTBENCHITER", 708, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field OILTEMP = Field.create("OILTEMP", 710, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field FUELTEMP = Field.create("FUELTEMP", 712, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field AMBIENTTEMP = Field.create("AMBIENTTEMP", 714, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMP = Field.create("COMPRESSORDISCHARGETEMP", 716, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGEPRESSURE = Field.create("COMPRESSORDISCHARGEPRESSURE", 718, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(0);
	public static final Field THROTTLEINLETPRESSURE = Field.create("THROTTLEINLETPRESSURE", 720, FieldType.INT16).setScale(0.03333333333333333).setBaseOffset(0);
	public static final Field IGNITIONONTIME = Field.create("IGNITIONONTIME", 722, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ENGINERUNTIME = Field.create("ENGINERUNTIME", 724, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DISTANCETRAVELED = Field.create("DISTANCETRAVELED", 726, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field AFRGASOLINESCALE = Field.create("AFRGASOLINESCALE", 728, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field AFR2GASOLINESCALE = Field.create("AFR2GASOLINESCALE", 730, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field WHEELSLIPRATIO = Field.create("WHEELSLIPRATIO", 732, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC1 = Field.create("IGNITORDIAGNOSTIC1", 736, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC2 = Field.create("IGNITORDIAGNOSTIC2", 737, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC3 = Field.create("IGNITORDIAGNOSTIC3", 738, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC4 = Field.create("IGNITORDIAGNOSTIC4", 739, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC5 = Field.create("IGNITORDIAGNOSTIC5", 740, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC6 = Field.create("IGNITORDIAGNOSTIC6", 741, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC7 = Field.create("IGNITORDIAGNOSTIC7", 742, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC8 = Field.create("IGNITORDIAGNOSTIC8", 743, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC9 = Field.create("IGNITORDIAGNOSTIC9", 744, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC10 = Field.create("IGNITORDIAGNOSTIC10", 745, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC11 = Field.create("IGNITORDIAGNOSTIC11", 746, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITORDIAGNOSTIC12 = Field.create("IGNITORDIAGNOSTIC12", 747, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC1 = Field.create("INJECTORDIAGNOSTIC1", 748, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC2 = Field.create("INJECTORDIAGNOSTIC2", 749, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC3 = Field.create("INJECTORDIAGNOSTIC3", 750, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC4 = Field.create("INJECTORDIAGNOSTIC4", 751, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC5 = Field.create("INJECTORDIAGNOSTIC5", 752, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC6 = Field.create("INJECTORDIAGNOSTIC6", 753, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC7 = Field.create("INJECTORDIAGNOSTIC7", 754, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC8 = Field.create("INJECTORDIAGNOSTIC8", 755, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC9 = Field.create("INJECTORDIAGNOSTIC9", 756, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC10 = Field.create("INJECTORDIAGNOSTIC10", 757, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC11 = Field.create("INJECTORDIAGNOSTIC11", 758, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORDIAGNOSTIC12 = Field.create("INJECTORDIAGNOSTIC12", 759, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACTUALLASTINJECTIONSTAGE2 = Field.create("ACTUALLASTINJECTIONSTAGE2", 760, FieldType.INT16).setScale(0.0033333333333333335).setBaseOffset(0);
	public static final Field INJECTORDUTYCYCLESTAGE2 = Field.create("INJECTORDUTYCYCLESTAGE2", 762, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field RAWFLEXFREQ = Field.create("RAWFLEXFREQ", 763, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FASTADCERRORSCOUNT = Field.create("FASTADCERRORSCOUNT", 764, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DEVICEUID = Field.create("DEVICEUID", 765, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33810SPIERRORCOUNTER = Field.create("MC33810SPIERRORCOUNTER", 766, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SADDWELLRATIOCOUNTER = Field.create("SADDWELLRATIOCOUNTER", 768, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPRIMINGCOUNTER = Field.create("INJECTIONPRIMINGCOUNTER", 769, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TEMPLOGGING2 = Field.create("TEMPLOGGING2", 770, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_771 = Field.create("ALIGNMENTFILL_AT_771", 771, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACPRESSURE = Field.create("ACPRESSURE", 772, FieldType.FLOAT).setBaseOffset(0);
	public static final Field RAWAUXANALOG1 = Field.create("RAWAUXANALOG1", 776, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWAUXANALOG2 = Field.create("RAWAUXANALOG2", 778, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWAUXANALOG3 = Field.create("RAWAUXANALOG3", 780, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWAUXANALOG4 = Field.create("RAWAUXANALOG4", 782, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field FASTADCERRORCALLBACKCOUNT = Field.create("FASTADCERRORCALLBACKCOUNT", 784, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SLOWADCERRORSCOUNT = Field.create("SLOWADCERRORSCOUNT", 785, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field RAWAUXTEMP1 = Field.create("RAWAUXTEMP1", 786, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWAUXTEMP2 = Field.create("RAWAUXTEMP2", 788, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RAWAMBIENTTEMP = Field.create("RAWAMBIENTTEMP", 790, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field RTCUNIXEPOCHTIME = Field.create("RTCUNIXEPOCHTIME", 792, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SPARKCUTREASONBLINKER = Field.create("SPARKCUTREASONBLINKER", 796, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FUELCUTREASONBLINKER = Field.create("FUELCUTREASONBLINKER", 797, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HP = Field.create("HP", 798, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TORQUE = Field.create("TORQUE", 800, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_802 = Field.create("ALIGNMENTFILL_AT_802", 802, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MCUSERIAL = Field.create("MCUSERIAL", 804, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field LTITFACTOR = Field.create("LTITFACTOR", 856, FieldType.FLOAT).setBaseOffset(0);
	public static final Field WWTAU = Field.create("WWTAU", 860, FieldType.FLOAT).setBaseOffset(0);
	public static final Field WWBETA = Field.create("WWBETA", 864, FieldType.FLOAT).setBaseOffset(0);
	public static final Field WWALPHA = Field.create("WWALPHA", 868, FieldType.FLOAT).setBaseOffset(0);
	public static final Field[] VALUES = {
	SD_PRESENT,
	SD_LOGGING_INTERNAL,
	TRIGGERSCOPEREADY,
	ANTILAGTRIGGERED,
	ISO2HEATERON,
	CHECKENGINE,
	NEEDBURN,
	SD_MSD,
	TOOTHLOGREADY,
	ISTPSERROR,
	ISCLTERROR,
	ISMAPERROR,
	ISIATERROR,
	ISTRIGGERERROR,
	HASCRITICALERROR,
	ISWARNNOW,
	ISPEDALERROR,
	ISKNOCKCHIPOK,
	LAUNCHTRIGGERED,
	ISTPS2ERROR,
	INJECTORFAULT,
	IGNITIONFAULT,
	ISUSBCONNECTED,
	DFCOACTIVE,
	SD_ACTIVE_WR,
	SD_ACTIVE_RD,
	ISMAPVALID,
	TRIGGERPAGEREFRESHFLAG,
	HASFAULTREPORTFILE,
	ISANALOGFAILURE,
	ISTUNINGNOW,
	RPMVALUE,
	RPMACCELERATION,
	SPEEDTORPMRATIO,
	INTERNALMCUTEMPERATURE,
	COOLANT,
	INTAKE,
	AUXTEMP1,
	AUXTEMP2,
	TPSVALUE,
	THROTTLEPEDALPOSITION,
	TPSADC,
	RAWMAF,
	MAFMEASURED,
	MAPVALUE,
	BAROPRESSURE,
	LAMBDAVALUE,
	VBATT,
	OILPRESSURE,
	VVTPOSITIONB1I,
	ACTUALLASTINJECTION,
	INJECTORDUTYCYCLE,
	TEMPLOGGING1,
	INJECTIONOFFSET,
	ENGINEMAKECODENAMECRC16,
	WALLFUELAMOUNT,
	WALLFUELCORRECTIONVALUE,
	REVOLUTIONCOUNTERSINCESTART,
	CANREADCOUNTER,
	TPSACCELFUEL,
	CURRENTIGNITIONMODE,
	CURRENTINJECTIONMODE,
	COILDUTYCYCLE,
	ETB1DUTYCYCLE,
	FUELTANKLEVEL,
	TOTALFUELCONSUMPTION,
	FUELFLOWRATE,
	TPS2VALUE,
	TUNECRC16,
	VEVALUE,
	ALIGNMENTFILL_AT_78,
	SECONDS,
	ENGINEMODE,
	FIRMWAREVERSION,
	RAWIDLEPOSITIONSENSOR,
	RAWWASTEGATEPOSITION,
	ACCELERATIONLAT,
	ACCELERATIONLON,
	DETECTEDGEAR,
	MAXTRIGGERREENTRANT,
	RAWLOWFUELPRESSURE,
	RAWHIGHFUELPRESSURE,
	LOWFUELPRESSURE,
	TCUDESIREDGEAR,
	FLEXPERCENT,
	WASTEGATEPOSITIONSENSOR,
	HIGHFUELPRESSURE,
	TEMPLOGGING3,
	TEMPLOGGING4,
	CALIBRATIONVALUE,
	CALIBRATIONMODE,
	IDLESTEPPERTARGETPOSITION,
	ALIGNMENTFILL_AT_122,
	TOTALTRIGGERERRORCOUNTER,
	ORDERINGERRORCOUNTER,
	WARNINGCOUNTER,
	LASTERRORCODE,
	RECENTERRORCODE1,
	RECENTERRORCODE2,
	RECENTERRORCODE3,
	RECENTERRORCODE4,
	RECENTERRORCODE5,
	RECENTERRORCODE6,
	RECENTERRORCODE7,
	RECENTERRORCODE8,
	DEBUGFLOATFIELD1,
	DEBUGFLOATFIELD2,
	DEBUGFLOATFIELD3,
	DEBUGFLOATFIELD4,
	DEBUGFLOATFIELD5,
	DEBUGFLOATFIELD6,
	DEBUGFLOATFIELD7,
	DEBUGINTFIELD1,
	DEBUGINTFIELD2,
	DEBUGINTFIELD3,
	DEBUGINTFIELD4,
	DEBUGINTFIELD5,
	EGT1,
	EGT2,
	EGT3,
	EGT4,
	EGT5,
	EGT6,
	EGT7,
	EGT8,
	RAWTPS1PRIMARY,
	RAWCLT,
	RAWIAT,
	RAWOILPRESSURE,
	RAWACPRESSURE,
	RAWFUELLEVEL,
	FUELCLOSEDLOOPBINIDX,
	ALIGNMENTFILL_AT_225,
	RAWPPSPRIMARY,
	RAWPPSSECONDARY,
	RAWRAWPPSPRIMARY,
	RAWRAWPPSSECONDARY,
	IDLEPOSITIONSENSOR,
	AFRVALUE,
	AFRVALUE2,
	SMOOTHEDAFRVALUE,
	SMOOTHEDAFRVALUE2,
	VSSACCELERATION,
	LAMBDAVALUE2,
	VVTPOSITIONB1E,
	VVTPOSITIONB2I,
	VVTPOSITIONB2E,
	FUELPIDCORRECTION1,
	FUELPIDCORRECTION2,
	RAWTPS1SECONDARY,
	RAWTPS2PRIMARY,
	RAWTPS2SECONDARY,
	ACCELERATIONVERT,
	GYROYAW,
	VVTTARGETS1,
	VVTTARGETS2,
	VVTTARGETS3,
	VVTTARGETS4,
	TURBOSPEED,
	IGNITIONADVANCECYL1,
	IGNITIONADVANCECYL2,
	IGNITIONADVANCECYL3,
	IGNITIONADVANCECYL4,
	IGNITIONADVANCECYL5,
	IGNITIONADVANCECYL6,
	IGNITIONADVANCECYL7,
	IGNITIONADVANCECYL8,
	IGNITIONADVANCECYL9,
	IGNITIONADVANCECYL10,
	IGNITIONADVANCECYL11,
	IGNITIONADVANCECYL12,
	TPS1SPLIT,
	TPS2SPLIT,
	TPS12SPLIT,
	ACCPEDALSPLIT,
	SPARKCUTREASON,
	FUELCUTREASON,
	MAFESTIMATE,
	INSTANTRPM,
	RAWMAP,
	RAWAFR,
	ALIGNMENTFILL_AT_326,
	CALIBRATIONVALUE2,
	LUAINVOCATIONCOUNTER,
	LUALASTCYCLEDURATION,
	VSSEDGECOUNTER,
	ISSEDGECOUNTER,
	AUXLINEAR1,
	AUXLINEAR2,
	AUXLINEAR3,
	AUXLINEAR4,
	FALLBACKMAP,
	INSTANTMAPVALUE,
	MAXLOCKEDDURATION,
	CANWRITEOK,
	CANWRITENOTOK,
	ALIGNMENTFILL_AT_374,
	TRIGGERPRIMARYFALL,
	TRIGGERPRIMARYRISE,
	TRIGGERSECONDARYFALL,
	TRIGGERSECONDARYRISE,
	TRIGGERVVTFALL,
	TRIGGERVVTRISE,
	TRIGGERVVT2FALL,
	TRIGGERVVT2RISE,
	STARTERSTATE,
	STARTERRELAYDISABLE,
	MULTISPARKCOUNTER,
	EXTIOVERFLOWCOUNT,
	ALTERNATORSTATUS_PTERM,
	ALTERNATORSTATUS_ITERM,
	ALTERNATORSTATUS_DTERM,
	ALTERNATORSTATUS_OUTPUT,
	ALTERNATORSTATUS_ERROR,
	ALTERNATORSTATUS_RESETCOUNTER,
	IDLESTATUS_PTERM,
	IDLESTATUS_ITERM,
	IDLESTATUS_DTERM,
	IDLESTATUS_OUTPUT,
	IDLESTATUS_ERROR,
	IDLESTATUS_RESETCOUNTER,
	ETBSTATUS_PTERM,
	ETBSTATUS_ITERM,
	ETBSTATUS_DTERM,
	ETBSTATUS_OUTPUT,
	ETBSTATUS_ERROR,
	ETBSTATUS_RESETCOUNTER,
	BOOSTSTATUS_PTERM,
	BOOSTSTATUS_ITERM,
	BOOSTSTATUS_DTERM,
	BOOSTSTATUS_OUTPUT,
	BOOSTSTATUS_ERROR,
	BOOSTSTATUS_RESETCOUNTER,
	WASTEGATEDCSTATUS_PTERM,
	WASTEGATEDCSTATUS_ITERM,
	WASTEGATEDCSTATUS_DTERM,
	WASTEGATEDCSTATUS_OUTPUT,
	WASTEGATEDCSTATUS_ERROR,
	WASTEGATEDCSTATUS_RESETCOUNTER,
	VVTSTATUS1_PTERM,
	VVTSTATUS1_ITERM,
	VVTSTATUS1_DTERM,
	VVTSTATUS1_OUTPUT,
	VVTSTATUS1_ERROR,
	VVTSTATUS1_RESETCOUNTER,
	VVTSTATUS2_PTERM,
	VVTSTATUS2_ITERM,
	VVTSTATUS2_DTERM,
	VVTSTATUS2_OUTPUT,
	VVTSTATUS2_ERROR,
	VVTSTATUS2_RESETCOUNTER,
	VVTSTATUS3_PTERM,
	VVTSTATUS3_ITERM,
	VVTSTATUS3_DTERM,
	VVTSTATUS3_OUTPUT,
	VVTSTATUS3_ERROR,
	VVTSTATUS3_RESETCOUNTER,
	VVTSTATUS4_PTERM,
	VVTSTATUS4_ITERM,
	VVTSTATUS4_DTERM,
	VVTSTATUS4_OUTPUT,
	VVTSTATUS4_ERROR,
	VVTSTATUS4_RESETCOUNTER,
	AUXSPEED1,
	AUXSPEED2,
	ISSVALUE,
	RAWANALOGINPUT1,
	RAWANALOGINPUT2,
	RAWANALOGINPUT3,
	RAWANALOGINPUT4,
	RAWANALOGINPUT5,
	RAWANALOGINPUT6,
	RAWANALOGINPUT7,
	RAWANALOGINPUT8,
	GPPWMOUTPUT1,
	GPPWMOUTPUT2,
	GPPWMOUTPUT3,
	GPPWMOUTPUT4,
	GPPWMXAXIS1,
	GPPWMXAXIS2,
	GPPWMXAXIS3,
	GPPWMXAXIS4,
	GPPWMYAXIS1,
	GPPWMYAXIS2,
	GPPWMYAXIS3,
	GPPWMYAXIS4,
	RAWBATTERY,
	IGNBLENDPARAMETER1,
	IGNBLENDPARAMETER2,
	IGNBLENDBIAS1,
	IGNBLENDBIAS2,
	IGNBLENDOUTPUT1,
	IGNBLENDOUTPUT2,
	VEBLENDPARAMETER1,
	VEBLENDPARAMETER2,
	VEBLENDPARAMETER3,
	VEBLENDPARAMETER4,
	VEBLENDBIAS1,
	VEBLENDBIAS2,
	VEBLENDBIAS3,
	VEBLENDBIAS4,
	VEBLENDOUTPUT1,
	VEBLENDOUTPUT2,
	VEBLENDOUTPUT3,
	VEBLENDOUTPUT4,
	BOOSTOPENLOOPBLENDPARAMETER1,
	BOOSTOPENLOOPBLENDPARAMETER2,
	BOOSTOPENLOOPBLENDBIAS1,
	BOOSTOPENLOOPBLENDBIAS2,
	BOOSTOPENLOOPBLENDOUTPUT1,
	BOOSTOPENLOOPBLENDOUTPUT2,
	BOOSTCLOSEDLOOPBLENDPARAMETER1,
	BOOSTCLOSEDLOOPBLENDPARAMETER2,
	BOOSTCLOSEDLOOPBLENDBIAS1,
	BOOSTCLOSEDLOOPBLENDBIAS2,
	BOOSTCLOSEDLOOPBLENDOUTPUT1,
	BOOSTCLOSEDLOOPBLENDOUTPUT2,
	TARGETAFRBLENDPARAMETER1,
	TARGETAFRBLENDPARAMETER2,
	TARGETAFRBLENDBIAS1,
	TARGETAFRBLENDBIAS2,
	TARGETAFRBLENDOUTPUT1,
	TARGETAFRBLENDOUTPUT2,
	ALIGNMENTFILL_AT_650,
	COILSTATE1,
	COILSTATE2,
	COILSTATE3,
	COILSTATE4,
	COILSTATE5,
	COILSTATE6,
	COILSTATE7,
	COILSTATE8,
	COILSTATE9,
	COILSTATE10,
	COILSTATE11,
	COILSTATE12,
	INJECTORSTATE1,
	INJECTORSTATE2,
	INJECTORSTATE3,
	INJECTORSTATE4,
	INJECTORSTATE5,
	INJECTORSTATE6,
	INJECTORSTATE7,
	INJECTORSTATE8,
	INJECTORSTATE9,
	INJECTORSTATE10,
	INJECTORSTATE11,
	INJECTORSTATE12,
	TRIGGERCHANNEL1,
	TRIGGERCHANNEL2,
	VVTCHANNEL1,
	VVTCHANNEL2,
	VVTCHANNEL3,
	VVTCHANNEL4,
	OUTPUTREQUESTPERIOD,
	MAPFAST,
	LUAGAUGES1,
	LUAGAUGES2,
	LUAGAUGES3,
	LUAGAUGES4,
	LUAGAUGES5,
	LUAGAUGES6,
	LUAGAUGES7,
	LUAGAUGES8,
	RAWMAF2,
	MAFMEASURED2,
	SCHEDULINGUSEDCOUNT,
	VEHICLESPEEDKPH,
	GEGO,
	LTFT,
	TESTBENCHITER,
	OILTEMP,
	FUELTEMP,
	AMBIENTTEMP,
	COMPRESSORDISCHARGETEMP,
	COMPRESSORDISCHARGEPRESSURE,
	THROTTLEINLETPRESSURE,
	IGNITIONONTIME,
	ENGINERUNTIME,
	DISTANCETRAVELED,
	AFRGASOLINESCALE,
	AFR2GASOLINESCALE,
	WHEELSLIPRATIO,
	IGNITORDIAGNOSTIC1,
	IGNITORDIAGNOSTIC2,
	IGNITORDIAGNOSTIC3,
	IGNITORDIAGNOSTIC4,
	IGNITORDIAGNOSTIC5,
	IGNITORDIAGNOSTIC6,
	IGNITORDIAGNOSTIC7,
	IGNITORDIAGNOSTIC8,
	IGNITORDIAGNOSTIC9,
	IGNITORDIAGNOSTIC10,
	IGNITORDIAGNOSTIC11,
	IGNITORDIAGNOSTIC12,
	INJECTORDIAGNOSTIC1,
	INJECTORDIAGNOSTIC2,
	INJECTORDIAGNOSTIC3,
	INJECTORDIAGNOSTIC4,
	INJECTORDIAGNOSTIC5,
	INJECTORDIAGNOSTIC6,
	INJECTORDIAGNOSTIC7,
	INJECTORDIAGNOSTIC8,
	INJECTORDIAGNOSTIC9,
	INJECTORDIAGNOSTIC10,
	INJECTORDIAGNOSTIC11,
	INJECTORDIAGNOSTIC12,
	ACTUALLASTINJECTIONSTAGE2,
	INJECTORDUTYCYCLESTAGE2,
	RAWFLEXFREQ,
	FASTADCERRORSCOUNT,
	DEVICEUID,
	MC33810SPIERRORCOUNTER,
	SADDWELLRATIOCOUNTER,
	INJECTIONPRIMINGCOUNTER,
	TEMPLOGGING2,
	ALIGNMENTFILL_AT_771,
	ACPRESSURE,
	RAWAUXANALOG1,
	RAWAUXANALOG2,
	RAWAUXANALOG3,
	RAWAUXANALOG4,
	FASTADCERRORCALLBACKCOUNT,
	SLOWADCERRORSCOUNT,
	RAWAUXTEMP1,
	RAWAUXTEMP2,
	RAWAMBIENTTEMP,
	RTCUNIXEPOCHTIME,
	SPARKCUTREASONBLINKER,
	FUELCUTREASONBLINKER,
	HP,
	TORQUE,
	ALIGNMENTFILL_AT_802,
	MCUSERIAL,
	LTITFACTOR,
	WWTAU,
	WWBETA,
	WWALPHA,
	};
}
