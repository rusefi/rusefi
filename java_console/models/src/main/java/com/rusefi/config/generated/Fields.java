package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class Fields {
	public static final Field ENGINETYPE = Field.create("ENGINETYPE", 0, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STARTBUTTONSUPPRESSONSTARTUPMS = Field.create("STARTBUTTONSUPPRESSONSTARTUPMS", 2, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SENSORSNIFFERRPMTHRESHOLD = Field.create("SENSORSNIFFERRPMTHRESHOLD", 4, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field LAUNCHRPM = Field.create("LAUNCHRPM", 6, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RPMHARDLIMIT = Field.create("RPMHARDLIMIT", 8, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ENGINESNIFFERRPMTHRESHOLD = Field.create("ENGINESNIFFERRPMTHRESHOLD", 10, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MULTISPARKMAXRPM = Field.create("MULTISPARKMAXRPM", 12, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field MAXACRPM = Field.create("MAXACRPM", 13, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field MAXACTPS = Field.create("MAXACTPS", 14, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXACCLT = Field.create("MAXACCLT", 15, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field COMPRESSIONRATIO = Field.create("COMPRESSIONRATIO", 16, FieldType.FLOAT).setBaseOffset(0);
	public static final Field WASTEGATEPOSITIONMIN = Field.create("WASTEGATEPOSITIONMIN", 20, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field WASTEGATEPOSITIONMAX = Field.create("WASTEGATEPOSITIONMAX", 22, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEPOSITIONMIN = Field.create("IDLEPOSITIONMIN", 24, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEPOSITIONMAX = Field.create("IDLEPOSITIONMAX", 26, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final String[] output_pin_e = {"NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "MC33972_1", "MC33972_2", "MC33972_3", "MC33972_4", "MC33972_5", "MC33972_6", "MC33972_7", "MC33972_8", "MC33972_9", "MC33972_10", "MC33972_11", "MC33972_12", "MC33972_13", "MC33972_14", "MC33972_15", "MC33972_16", "MC33972_17", "MC33972_18", "MC33972_19", "MC33972_20", "MC33972_21", "MC33972_22", "TLE8888_1", "TLE8888_2", "TLE8888_3", "TLE8888_4", "TLE8888_5", "TLE8888_6", "TLE8888_7", "TLE8888_8", "TLE8888_9", "TLE8888_10", "TLE8888_11", "TLE8888_12", "TLE8888_13", "TLE8888_14", "TLE8888_15", "TLE8888_16", "TLE8888_17", "TLE8888_18", "TLE8888_19", "TLE8888_20", "TLE8888_21", "TLE8888_22", "TLE8888_23", "TLE8888_24", "TLE8888_25", "TLE8888_26", "TLE8888_27", "TLE8888_28", "INVALID", "INVALID", "INVALID", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16", "L215", "L216", "L217", "L218", "L219", "L220", "L221", "L222", "L223", "L224", "L225", "L226", "L227", "L228", "L229", "L230", "L231", "L232", "L233", "L234", "L235", "L236", "L237", "L238", "L239", "L240", "L241", "L242", "L243", "L244", "L245", "L246", "L247", "L248", "can0", "can1", "can2", "can3", "can4", "can5", "can6", "can7", "protected_0", "protected_1", "protected_2", "protected_3", "protected_4", "protected_5", "protected_6", "protected_7", "protected_8", "protected_9", "protected_10", "protected_11", "protected_12", "protected_13", "protected_14", "protected_15", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "MC33810_0_OUT_0", "MC33810_0_OUT_1", "MC33810_0_OUT_2", "MC33810_0_OUT_3", "MC33810_0_GD_0", "MC33810_0_GD_1", "MC33810_0_GD_2", "MC33810_0_GD_3"};
	public static final Field MAINRELAYPIN = Field.create("MAINRELAYPIN", 28, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final String[] Gpio = {"NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"};
	public static final Field SDCARDCSPIN = Field.create("SDCARDCSPIN", 30, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CANTXPIN = Field.create("CANTXPIN", 32, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CANRXPIN = Field.create("CANRXPIN", 34, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final String[] switch_input_pin_e = {"NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"};
	public static final Field TORQUEREDUCTIONTRIGGERPIN = Field.create("TORQUEREDUCTIONTRIGGERPIN", 36, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LAUNCHFUELADDERPERCENT = Field.create("LAUNCHFUELADDERPERCENT", 38, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ETBJAMTIMEOUT = Field.create("ETBJAMTIMEOUT", 39, FieldType.INT8).setScale(0.02).setBaseOffset(0);
	public static final Field TACHOUTPUTPIN = Field.create("TACHOUTPUTPIN", 40, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final String[] pin_output_mode_e = {"default", "default inverted", "open collector", "open collector inverted"};
	public static final Field TACHOUTPUTPINMODE = Field.create("TACHOUTPUTPINMODE", 42, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ACIDLEEXTRAOFFSET = Field.create("ACIDLEEXTRAOFFSET", 43, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MULTISPARKMAXSPARKINGANGLE = Field.create("MULTISPARKMAXSPARKINGANGLE", 44, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MULTISPARKMAXEXTRASPARKCOUNT = Field.create("MULTISPARKMAXEXTRASPARKCOUNT", 45, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VVTCONTROLMINRPM = Field.create("VVTCONTROLMINRPM", 46, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTOR_FLOW = Field.create("INJECTOR_FLOW", 48, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTOR_BATTLAGCORRBATTBINS = Field.create("INJECTOR_BATTLAGCORRBATTBINS", 52, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field INJECTOR_BATTLAGCORRPRESSBINS = Field.create("INJECTOR_BATTLAGCORRPRESSBINS", 68, FieldType.INT).setScale(0.1).setBaseOffset(0);
	public static final Field INJECTOR_BATTLAGCORRTABLE = Field.create("INJECTOR_BATTLAGCORRTABLE", 76, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field INJECTORSECONDARY_FLOW = Field.create("INJECTORSECONDARY_FLOW", 108, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORSECONDARY_BATTLAGCORRBATTBINS = Field.create("INJECTORSECONDARY_BATTLAGCORRBATTBINS", 112, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field INJECTORSECONDARY_BATTLAGCORRPRESSBINS = Field.create("INJECTORSECONDARY_BATTLAGCORRPRESSBINS", 128, FieldType.INT).setScale(0.1).setBaseOffset(0);
	public static final Field INJECTORSECONDARY_BATTLAGCORRTABLE = Field.create("INJECTORSECONDARY_BATTLAGCORRTABLE", 136, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field ISFORCEDINDUCTION = Field.create("ISFORCEDINDUCTION", 168, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field USEFORDREDUNDANTTPS = Field.create("USEFORDREDUNDANTTPS", 168, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field ENABLEKLINE = Field.create("ENABLEKLINE", 168, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field OVERRIDETRIGGERGAPS = Field.create("OVERRIDETRIGGERGAPS", 168, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field ENABLEFAN1WITHAC = Field.create("ENABLEFAN1WITHAC", 168, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field ENABLEFAN2WITHAC = Field.create("ENABLEFAN2WITHAC", 168, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field DISABLEFAN1WHENSTOPPED = Field.create("DISABLEFAN1WHENSTOPPED", 168, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field DISABLEFAN2WHENSTOPPED = Field.create("DISABLEFAN2WHENSTOPPED", 168, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field ENABLETRAILINGSPARKS = Field.create("ENABLETRAILINGSPARKS", 168, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field ETB_USE_TWO_WIRES = Field.create("ETB_USE_TWO_WIRES", 168, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field ISDOUBLESOLENOIDIDLE = Field.create("ISDOUBLESOLENOIDIDLE", 168, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field USEEEPROM = Field.create("USEEEPROM", 168, FieldType.BIT, 11).setBaseOffset(0);
	public static final Field USECICPIDFORIDLE = Field.create("USECICPIDFORIDLE", 168, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field USETLE8888_CRANKING_HACK = Field.create("USETLE8888_CRANKING_HACK", 168, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field KICKSTARTCRANKING = Field.create("KICKSTARTCRANKING", 168, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field USESEPARATEIDLETABLESFORCRANKINGTAPER = Field.create("USESEPARATEIDLETABLESFORCRANKINGTAPER", 168, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field LAUNCHCONTROLENABLED = Field.create("LAUNCHCONTROLENABLED", 168, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field DONOTFILTERTRIGGEREDGENOISE = Field.create("DONOTFILTERTRIGGEREDGENOISE", 168, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field ANTILAGENABLED = Field.create("ANTILAGENABLED", 168, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field USERUNNINGMATHFORCRANKING = Field.create("USERUNNINGMATHFORCRANKING", 168, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field DISPLAYLOGICLEVELSINENGINESNIFFER = Field.create("DISPLAYLOGICLEVELSINENGINESNIFFER", 168, FieldType.BIT, 20).setBaseOffset(0);
	public static final Field USETLE8888_STEPPER = Field.create("USETLE8888_STEPPER", 168, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field USESCRIPTTABLEFORCANSNIFFINGFILTERING = Field.create("USESCRIPTTABLEFORCANSNIFFINGFILTERING", 168, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field VERBOSECAN = Field.create("VERBOSECAN", 168, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field ARTIFICIALTESTMISFIRE = Field.create("ARTIFICIALTESTMISFIRE", 168, FieldType.BIT, 24).setBaseOffset(0);
	public static final Field USEFORDREDUNDANTPPS = Field.create("USEFORDREDUNDANTPPS", 168, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field CLTSENSORPULLDOWN = Field.create("CLTSENSORPULLDOWN", 168, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field IATSENSORPULLDOWN = Field.create("IATSENSORPULLDOWN", 168, FieldType.BIT, 27).setBaseOffset(0);
	public static final Field ALLOWIDENTICALPPS = Field.create("ALLOWIDENTICALPPS", 168, FieldType.BIT, 28).setBaseOffset(0);
	public static final Field OVERRIDEVVTTRIGGERGAPS = Field.create("OVERRIDEVVTTRIGGERGAPS", 168, FieldType.BIT, 29).setBaseOffset(0);
	public static final Field USESPIIMU = Field.create("USESPIIMU", 168, FieldType.BIT, 30).setBaseOffset(0);
	public static final Field ENABLESTAGEDINJECTION = Field.create("ENABLESTAGEDINJECTION", 168, FieldType.BIT, 31).setBaseOffset(0);
	public static final Field TPSMIN = Field.create("TPSMIN", 172, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPSMAX = Field.create("TPSMAX", 174, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPSERRORDETECTIONTOOLOW = Field.create("TPSERRORDETECTIONTOOLOW", 176, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPSERRORDETECTIONTOOHIGH = Field.create("TPSERRORDETECTIONTOOHIGH", 178, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CRANKING_BASEFUEL = Field.create("CRANKING_BASEFUEL", 180, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKING_RPM = Field.create("CRANKING_RPM", 184, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CRANKING_ALIGNMENTFILL_AT_6 = Field.create("CRANKING_ALIGNMENTFILL_AT_6", 186, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONDWELLFORCRANKINGMS = Field.create("IGNITIONDWELLFORCRANKINGMS", 188, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETBREVLIMITSTART = Field.create("ETBREVLIMITSTART", 192, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBREVLIMITRANGE = Field.create("ETBREVLIMITRANGE", 194, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MAP_SAMPLINGANGLEBINS = Field.create("MAP_SAMPLINGANGLEBINS", 196, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAP_SAMPLINGANGLE = Field.create("MAP_SAMPLINGANGLE", 228, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAP_SAMPLINGWINDOWBINS = Field.create("MAP_SAMPLINGWINDOWBINS", 260, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAP_SAMPLINGWINDOW = Field.create("MAP_SAMPLINGWINDOW", 292, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAP_SENSOR_LOWVALUE = Field.create("MAP_SENSOR_LOWVALUE", 324, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAP_SENSOR_HIGHVALUE = Field.create("MAP_SENSOR_HIGHVALUE", 328, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAP_SENSOR_TYPE = Field.create("MAP_SENSOR_TYPE", 332, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final String[] adc_channel_e = {"Disabled", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5"};
	public static final Field MAP_SENSOR_HWCHANNEL = Field.create("MAP_SENSOR_HWCHANNEL", 333, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field MAP_SENSOR_ALIGNMENTFILL_AT_10 = Field.create("MAP_SENSOR_ALIGNMENTFILL_AT_10", 334, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CLT_TEMPC_1 = Field.create("CLT_TEMPC_1", 336, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLT_TEMPC_2 = Field.create("CLT_TEMPC_2", 340, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLT_TEMPC_3 = Field.create("CLT_TEMPC_3", 344, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLT_RESISTANCE_1 = Field.create("CLT_RESISTANCE_1", 348, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLT_RESISTANCE_2 = Field.create("CLT_RESISTANCE_2", 352, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLT_RESISTANCE_3 = Field.create("CLT_RESISTANCE_3", 356, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLT_BIAS_RESISTOR = Field.create("CLT_BIAS_RESISTOR", 360, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLT_ADCCHANNEL = Field.create("CLT_ADCCHANNEL", 364, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field CLT_ALIGNMENTFILL_AT_29 = Field.create("CLT_ALIGNMENTFILL_AT_29", 365, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IAT_TEMPC_1 = Field.create("IAT_TEMPC_1", 368, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IAT_TEMPC_2 = Field.create("IAT_TEMPC_2", 372, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IAT_TEMPC_3 = Field.create("IAT_TEMPC_3", 376, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IAT_RESISTANCE_1 = Field.create("IAT_RESISTANCE_1", 380, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IAT_RESISTANCE_2 = Field.create("IAT_RESISTANCE_2", 384, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IAT_RESISTANCE_3 = Field.create("IAT_RESISTANCE_3", 388, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IAT_BIAS_RESISTOR = Field.create("IAT_BIAS_RESISTOR", 392, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IAT_ADCCHANNEL = Field.create("IAT_ADCCHANNEL", 396, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IAT_ALIGNMENTFILL_AT_29 = Field.create("IAT_ALIGNMENTFILL_AT_29", 397, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field LAUNCHTIMINGRETARD = Field.create("LAUNCHTIMINGRETARD", 400, FieldType.FLOAT).setBaseOffset(0);
	public static final Field HIP9011PRESCALER = Field.create("HIP9011PRESCALER", 404, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TUNINGDETECTOR = Field.create("TUNINGDETECTOR", 407, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATOR_ITERMMIN = Field.create("ALTERNATOR_ITERMMIN", 408, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATOR_ITERMMAX = Field.create("ALTERNATOR_ITERMMAX", 410, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DISPLACEMENT = Field.create("DISPLACEMENT", 412, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field TRIGGERSIMULATORRPM = Field.create("TRIGGERSIMULATORRPM", 414, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERSCOUNT = Field.create("CYLINDERSCOUNT", 416, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field FIRINGORDER = Field.create("FIRINGORDER", 420, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field JUSTATEMPTEST = Field.create("JUSTATEMPTEST", 421, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAPSYNCTHRESHOLD = Field.create("MAPSYNCTHRESHOLD", 422, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TORQUEREDUCTIONIGNITIONCUT = Field.create("TORQUEREDUCTIONIGNITIONCUT", 423, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBORE = Field.create("CYLINDERBORE", 424, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] engine_load_mode_e = {"Speed Density", "MAF Air Charge", "Alpha-N", "Lua"};
	public static final Field FUELALGORITHM = Field.create("FUELALGORITHM", 428, FieldType.INT8, engine_load_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALSMAXTPS = Field.create("ALSMAXTPS", 429, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CRANKINGINJECTIONMODE = Field.create("CRANKINGINJECTIONMODE", 430, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONMODE = Field.create("INJECTIONMODE", 431, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCONTROLMINRPM = Field.create("BOOSTCONTROLMINRPM", 432, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCONTROLMINTPS = Field.create("BOOSTCONTROLMINTPS", 434, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_435 = Field.create("ALIGNMENTFILL_AT_435", 435, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCONTROLMINMAP = Field.create("BOOSTCONTROLMINMAP", 436, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_438 = Field.create("ALIGNMENTFILL_AT_438", 438, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CRANKINGTIMINGANGLE = Field.create("CRANKINGTIMINGANGLE", 440, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IGNITIONMODE = Field.create("IGNITIONMODE", 444, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GAPTRACKINGLENGTHOVERRIDE = Field.create("GAPTRACKINGLENGTHOVERRIDE", 445, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXIDLEVSS = Field.create("MAXIDLEVSS", 446, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CAMDECODER2JZPRECISION = Field.create("CAMDECODER2JZPRECISION", 447, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MINOILPRESSUREAFTERSTART = Field.create("MINOILPRESSUREAFTERSTART", 448, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TIMINGMODE = Field.create("TIMINGMODE", 450, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CANNBCTYPE = Field.create("CANNBCTYPE", 451, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FIXEDMODETIMING = Field.create("FIXEDMODETIMING", 452, FieldType.FLOAT).setBaseOffset(0);
	public static final Field GLOBALTRIGGERANGLEOFFSET = Field.create("GLOBALTRIGGERANGLEOFFSET", 456, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ANALOGINPUTDIVIDERCOEFFICIENT = Field.create("ANALOGINPUTDIVIDERCOEFFICIENT", 460, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VBATTDIVIDERCOEFF = Field.create("VBATTDIVIDERCOEFF", 464, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FANONTEMPERATURE = Field.create("FANONTEMPERATURE", 468, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FANOFFTEMPERATURE = Field.create("FANOFFTEMPERATURE", 472, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ACRPIN = Field.create("ACRPIN", 476, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_478 = Field.create("ALIGNMENTFILL_AT_478", 478, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DRIVEWHEELREVPERKM = Field.create("DRIVEWHEELREVPERKM", 480, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CANSLEEPPERIODMS = Field.create("CANSLEEPPERIODMS", 484, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field BYFIRMWAREVERSION = Field.create("BYFIRMWAREVERSION", 488, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TPS1_1ADCCHANNEL = Field.create("TPS1_1ADCCHANNEL", 492, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VBATTADCCHANNEL = Field.create("VBATTADCCHANNEL", 493, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field FUELLEVELSENSOR = Field.create("FUELLEVELSENSOR", 494, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TPS2_1ADCCHANNEL = Field.create("TPS2_1ADCCHANNEL", 495, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_DERIVATIVEFILTERLOSS = Field.create("IDLE_DERIVATIVEFILTERLOSS", 496, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] trigger_type_e = {"custom toothed wheel", "Ford Aspire", "Dodge Neon 1995", "Miata NA", "INVALID", "GM_7X", "Daihatsu 3 cylinder", "Mazda SOHC 4", "60-2", "36-1", "Mercedes Two Segment", "Single Tooth", "EZ30", "INVALID", "Dodge Neon 2003", "Mazda DOHC 1+4", "1+1", "INVALID", "Half Moon", "Dodge Ram 1+16", "60-2 Wrong Polarity", "Benelli Tre", "Dodge Stratus", "36_2_2_2", "Nissan Primera", "dev 2JZ 3/34 simulator", "Rover K", "GM 24x 5 degree", "Honda CBR 600 Cam", "INVALID", "Honda CBR 600 custom", "3-1 skipped", "Dodge Neon 2003 crank", "Miata NB", "INVALID", "INVALID", "Subaru 7+6", "Jeep 18-2-2-2", "12crank/24cam", "Dodge Neon 1995 crank only", "Jeep XJ 4 cyl", "FiatIAQ_P8", "Mazda Z5", "INVALID", "Renix 44-2-2", "Renix 66-2-2-2", "Honda K 12+1", "INVALID", "36-2", "Subaru SVX", "Suzuki K6A", "Subaru 7 without 6", "INVALID", "INVALID", "GM 60-2-2-2", "Skoda Favorit", "Barra 3+1 Cam", "Kawa KX450F", "Nissan VQ35", "INVALID", "Nissan VQ30", "Nissan QR25", "INVALID", "Subaru SVX Crank 1", "Subaru SVX Cam VVT", "Ford PIP", "Suzuki G13B", "Honda K 4+1", "Nissan MR18 Crank", "32-2", "36-2-1", "36-2-1-1", "3-0", "INVALID", "GM 24x 3 degree", "60-2-2 F3R", "Mitsu 4G63 Crank", "x2 30 Deg camshaft BTDC", "INVALID", "Mitsu 6G72 Cam", "6-0", "Daihatsu 4 cylinder", "INVALID", "dev", "Nissan HR", "Arctic Cat", "HR12 in", "HONDA J30A2", "trg88"};
	public static final Field TRIGGER_TYPE = Field.create("TRIGGER_TYPE", 500, FieldType.INT, trigger_type_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGER_CUSTOMTOTALTOOTHCOUNT = Field.create("TRIGGER_CUSTOMTOTALTOOTHCOUNT", 504, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGER_CUSTOMSKIPPEDTOOTHCOUNT = Field.create("TRIGGER_CUSTOMSKIPPEDTOOTHCOUNT", 508, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field AIRBYRPMTAPER = Field.create("AIRBYRPMTAPER", 512, FieldType.FLOAT).setBaseOffset(0);
	public static final Field HIP9011SPIDEVICE = Field.create("HIP9011SPIDEVICE", 516, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCONTROLSAFEDUTYCYCLE = Field.create("BOOSTCONTROLSAFEDUTYCYCLE", 517, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAFADCCHANNEL = Field.create("MAFADCCHANNEL", 518, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ACRREVOLUTIONS = Field.create("ACRREVOLUTIONS", 519, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CALIBRATIONBIRTHDAY = Field.create("CALIBRATIONBIRTHDAY", 520, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field ADCVCC = Field.create("ADCVCC", 524, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAPCAMDETECTIONANGLEPOSITION = Field.create("MAPCAMDETECTIONANGLEPOSITION", 528, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] brain_input_pin_e = {"NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"};
	public static final Field CAMINPUTS1 = Field.create("CAMINPUTS1", 532, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field CAMINPUTS2 = Field.create("CAMINPUTS2", 534, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field CAMINPUTS3 = Field.create("CAMINPUTS3", 536, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field CAMINPUTS4 = Field.create("CAMINPUTS4", 538, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field AFR_HWCHANNEL = Field.create("AFR_HWCHANNEL", 540, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AFR_HWCHANNEL2 = Field.create("AFR_HWCHANNEL2", 541, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AFR_ALIGNMENTFILL_AT_2 = Field.create("AFR_ALIGNMENTFILL_AT_2", 542, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AFR_V1 = Field.create("AFR_V1", 544, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AFR_VALUE1 = Field.create("AFR_VALUE1", 548, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AFR_V2 = Field.create("AFR_V2", 552, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AFR_VALUE2 = Field.create("AFR_VALUE2", 556, FieldType.FLOAT).setBaseOffset(0);
	public static final Field THROTTLEPEDALPOSITIONADCCHANNEL = Field.create("THROTTLEPEDALPOSITIONADCCHANNEL", 560, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ETBSPLIT = Field.create("ETBSPLIT", 561, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field TLE6240_CS = Field.create("TLE6240_CS", 562, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TLE6240_CSPINMODE = Field.create("TLE6240_CSPINMODE", 564, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MC33810_CSPINMODE = Field.create("MC33810_CSPINMODE", 565, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_566 = Field.create("ALIGNMENTFILL_AT_566", 566, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BAROSENSOR_LOWVALUE = Field.create("BAROSENSOR_LOWVALUE", 568, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BAROSENSOR_HIGHVALUE = Field.create("BAROSENSOR_HIGHVALUE", 572, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BAROSENSOR_TYPE = Field.create("BAROSENSOR_TYPE", 576, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BAROSENSOR_HWCHANNEL = Field.create("BAROSENSOR_HWCHANNEL", 577, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BAROSENSOR_ALIGNMENTFILL_AT_10 = Field.create("BAROSENSOR_ALIGNMENTFILL_AT_10", 578, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_SOLENOIDFREQUENCY = Field.create("IDLE_SOLENOIDFREQUENCY", 580, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_SOLENOIDPIN = Field.create("IDLE_SOLENOIDPIN", 584, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_STEPPERDIRECTIONPIN = Field.create("IDLE_STEPPERDIRECTIONPIN", 586, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_STEPPERSTEPPIN = Field.create("IDLE_STEPPERSTEPPIN", 588, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_SOLENOIDPINMODE = Field.create("IDLE_SOLENOIDPINMODE", 590, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_ALIGNMENTFILL_AT_11 = Field.create("IDLE_ALIGNMENTFILL_AT_11", 591, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field KNOCKRETARDAGGRESSION = Field.create("KNOCKRETARDAGGRESSION", 592, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field KNOCKRETARDREAPPLYRATE = Field.create("KNOCKRETARDREAPPLYRATE", 593, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ENGINESYNCCAM = Field.create("ENGINESYNCCAM", 594, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SDCARDCSPINMODE = Field.create("SDCARDCSPINMODE", 595, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field VSSGEARRATIO = Field.create("VSSGEARRATIO", 596, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field VSSFILTERRECIPROCAL = Field.create("VSSFILTERRECIPROCAL", 598, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VSSTOOTHCOUNT = Field.create("VSSTOOTHCOUNT", 599, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final String[] ve_override_e = {"None", "MAP", "TPS"};
	public static final Field IDLEVEOVERRIDEMODE = Field.create("IDLEVEOVERRIDEMODE", 600, FieldType.INT8, ve_override_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_601 = Field.create("ALIGNMENTFILL_AT_601", 601, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field L9779_CS = Field.create("L9779_CS", 602, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS1 = Field.create("INJECTIONPINS1", 604, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS2 = Field.create("INJECTIONPINS2", 606, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS3 = Field.create("INJECTIONPINS3", 608, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS4 = Field.create("INJECTIONPINS4", 610, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS5 = Field.create("INJECTIONPINS5", 612, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS6 = Field.create("INJECTIONPINS6", 614, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS7 = Field.create("INJECTIONPINS7", 616, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS8 = Field.create("INJECTIONPINS8", 618, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS9 = Field.create("INJECTIONPINS9", 620, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS10 = Field.create("INJECTIONPINS10", 622, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS11 = Field.create("INJECTIONPINS11", 624, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINS12 = Field.create("INJECTIONPINS12", 626, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS1 = Field.create("IGNITIONPINS1", 628, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS2 = Field.create("IGNITIONPINS2", 630, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS3 = Field.create("IGNITIONPINS3", 632, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS4 = Field.create("IGNITIONPINS4", 634, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS5 = Field.create("IGNITIONPINS5", 636, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS6 = Field.create("IGNITIONPINS6", 638, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS7 = Field.create("IGNITIONPINS7", 640, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS8 = Field.create("IGNITIONPINS8", 642, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS9 = Field.create("IGNITIONPINS9", 644, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS10 = Field.create("IGNITIONPINS10", 646, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS11 = Field.create("IGNITIONPINS11", 648, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINS12 = Field.create("IGNITIONPINS12", 650, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINMODE = Field.create("INJECTIONPINMODE", 652, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONPINMODE = Field.create("IGNITIONPINMODE", 653, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field FUELPUMPPIN = Field.create("FUELPUMPPIN", 654, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field FUELPUMPPINMODE = Field.create("FUELPUMPPINMODE", 656, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field GAPVVTTRACKINGLENGTHOVERRIDE = Field.create("GAPVVTTRACKINGLENGTHOVERRIDE", 657, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MALFUNCTIONINDICATORPIN = Field.create("MALFUNCTIONINDICATORPIN", 658, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field MALFUNCTIONINDICATORPINMODE = Field.create("MALFUNCTIONINDICATORPINMODE", 660, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field FANPINMODE = Field.create("FANPINMODE", 661, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field FANPIN = Field.create("FANPIN", 662, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field CLUTCHDOWNPIN = Field.create("CLUTCHDOWNPIN", 664, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORCONTROLPIN = Field.create("ALTERNATORCONTROLPIN", 666, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORCONTROLPINMODE = Field.create("ALTERNATORCONTROLPINMODE", 668, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final String[] pin_input_mode_e = {"DEFAULT", "PULLUP", "PULLDOWN", "INVALID", "INVERTED DEFAULT", "INVERTED PULLUP", "INVERTED PULLDOWN"};
	public static final Field CLUTCHDOWNPINMODE = Field.create("CLUTCHDOWNPINMODE", 669, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field DIGITALPOTENTIOMETERCHIPSELECT1 = Field.create("DIGITALPOTENTIOMETERCHIPSELECT1", 670, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field DIGITALPOTENTIOMETERCHIPSELECT2 = Field.create("DIGITALPOTENTIOMETERCHIPSELECT2", 672, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field DIGITALPOTENTIOMETERCHIPSELECT3 = Field.create("DIGITALPOTENTIOMETERCHIPSELECT3", 674, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field DIGITALPOTENTIOMETERCHIPSELECT4 = Field.create("DIGITALPOTENTIOMETERCHIPSELECT4", 676, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ELECTRONICTHROTTLEPIN1MODE = Field.create("ELECTRONICTHROTTLEPIN1MODE", 678, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855SPIDEVICE = Field.create("MAX31855SPIDEVICE", 679, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DEBUGTRIGGERSYNC = Field.create("DEBUGTRIGGERSYNC", 680, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field DIGITALPOTENTIOMETERSPIDEVICE = Field.create("DIGITALPOTENTIOMETERSPIDEVICE", 682, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BRAKEPEDALPINMODE = Field.create("BRAKEPEDALPINMODE", 683, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MC33972_CS = Field.create("MC33972_CS", 684, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MC33972_CSPINMODE = Field.create("MC33972_CSPINMODE", 686, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXFASTSENSOR1_ADCCHANNEL = Field.create("AUXFASTSENSOR1_ADCCHANNEL", 687, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TPS1_2ADCCHANNEL = Field.create("TPS1_2ADCCHANNEL", 688, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TPS2_2ADCCHANNEL = Field.create("TPS2_2ADCCHANNEL", 689, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field THROTTLEPEDALPOSITIONSECONDADCCHANNEL = Field.create("THROTTLEPEDALPOSITIONSECONDADCCHANNEL", 690, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final String[] ego_sensor_e = {"BPSX", "Innovate", "14Point7", "INVALID", "PLX", "Custom", "AEM"};
	public static final Field AFR_TYPE = Field.create("AFR_TYPE", 691, FieldType.INT8, ego_sensor_e).setScale(1.0).setBaseOffset(0);
	public static final Field MC33810_CS1 = Field.create("MC33810_CS1", 692, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_694 = Field.create("ALIGNMENTFILL_AT_694", 694, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLE_ANTIWINDUPFREQ = Field.create("IDLE_ANTIWINDUPFREQ", 696, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERINPUTPINS1 = Field.create("TRIGGERINPUTPINS1", 700, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERINPUTPINS2 = Field.create("TRIGGERINPUTPINS2", 702, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_T_MIN_BOOST = Field.create("MC33_T_MIN_BOOST", 704, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field HIP9011CSPINMODE = Field.create("HIP9011CSPINMODE", 706, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_707 = Field.create("ALIGNMENTFILL_AT_707", 707, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FINALGEARRATIO = Field.create("FINALGEARRATIO", 708, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TCUINPUTSPEEDSENSORPIN = Field.create("TCUINPUTSPEEDSENSORPIN", 710, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCUINPUTSPEEDSENSORTEETH = Field.create("TCUINPUTSPEEDSENSORTEETH", 712, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_713 = Field.create("ALIGNMENTFILL_AT_713", 713, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERSIMULATORPINS1 = Field.create("TRIGGERSIMULATORPINS1", 714, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERSIMULATORPINS2 = Field.create("TRIGGERSIMULATORPINS2", 716, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field FORDINJECTORSMALLPULSESLOPE = Field.create("FORDINJECTORSMALLPULSESLOPE", 718, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field TRIGGERSIMULATORPINMODES1 = Field.create("TRIGGERSIMULATORPINMODES1", 720, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERSIMULATORPINMODES2 = Field.create("TRIGGERSIMULATORPINMODES2", 721, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MAF2ADCCHANNEL = Field.create("MAF2ADCCHANNEL", 722, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_723 = Field.create("ALIGNMENTFILL_AT_723", 723, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field O2HEATERPIN = Field.create("O2HEATERPIN", 724, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field O2HEATERPINMODETODO = Field.create("O2HEATERPINMODETODO", 726, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONMINRPM = Field.create("LAMBDAPROTECTIONMINRPM", 727, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONMINLOAD = Field.create("LAMBDAPROTECTIONMINLOAD", 728, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_729 = Field.create("ALIGNMENTFILL_AT_729", 729, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IS_ENABLED_SPI_1 = Field.create("IS_ENABLED_SPI_1", 732, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field IS_ENABLED_SPI_2 = Field.create("IS_ENABLED_SPI_2", 732, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field IS_ENABLED_SPI_3 = Field.create("IS_ENABLED_SPI_3", 732, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field ISSDCARDENABLED = Field.create("ISSDCARDENABLED", 732, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field RUSEFIVERBOSE29B = Field.create("RUSEFIVERBOSE29B", 732, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field RETHROWHARDFAULT = Field.create("RETHROWHARDFAULT", 732, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field ISHIP9011ENABLED = Field.create("ISHIP9011ENABLED", 732, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field REQUIREFOOTONBRAKETOCRANK = Field.create("REQUIREFOOTONBRAKETOCRANK", 732, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field VERBOSEQUAD = Field.create("VERBOSEQUAD", 732, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field USESTEPPERIDLE = Field.create("USESTEPPERIDLE", 732, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field ENABLEDSTEP1LIMITER = Field.create("ENABLEDSTEP1LIMITER", 732, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field USETPICADVANCEDMODE = Field.create("USETPICADVANCEDMODE", 732, FieldType.BIT, 11).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONENABLE = Field.create("LAMBDAPROTECTIONENABLE", 732, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field VERBOSETLE8888 = Field.create("VERBOSETLE8888", 732, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field ENABLEVERBOSECANTX = Field.create("ENABLEVERBOSECANTX", 732, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field EXTERNALRUSEFIGDIMODULE = Field.create("EXTERNALRUSEFIGDIMODULE", 732, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field FLIPWBOCHANNELS = Field.create("FLIPWBOCHANNELS", 732, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field MEASUREMAPONLYINONECYLINDER = Field.create("MEASUREMAPONLYINONECYLINDER", 732, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field STEPPERFORCEPARKINGEVERYRESTART = Field.create("STEPPERFORCEPARKINGEVERYRESTART", 732, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field ISFASTERENGINESPINUPENABLED = Field.create("ISFASTERENGINESPINUPENABLED", 732, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field COASTINGFUELCUTENABLED = Field.create("COASTINGFUELCUTENABLED", 732, FieldType.BIT, 20).setBaseOffset(0);
	public static final Field USEIACTABLEFORCOASTING = Field.create("USEIACTABLEFORCOASTING", 732, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field USENOISELESSTRIGGERDECODER = Field.create("USENOISELESSTRIGGERDECODER", 732, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field USEIDLETIMINGPIDCONTROL = Field.create("USEIDLETIMINGPIDCONTROL", 732, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field DISABLEETBWHENENGINESTOPPED = Field.create("DISABLEETBWHENENGINESTOPPED", 732, FieldType.BIT, 24).setBaseOffset(0);
	public static final Field IS_ENABLED_SPI_4 = Field.create("IS_ENABLED_SPI_4", 732, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field PAUSEETBCONTROL = Field.create("PAUSEETBCONTROL", 732, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field TPSTPSPERCENTMODE = Field.create("TPSTPSPERCENTMODE", 732, FieldType.BIT, 27).setBaseOffset(0);
	public static final Field VERBOSEKLINE = Field.create("VERBOSEKLINE", 732, FieldType.BIT, 28).setBaseOffset(0);
	public static final Field IDLEINCREMENTALPIDCIC = Field.create("IDLEINCREMENTALPIDCIC", 732, FieldType.BIT, 29).setBaseOffset(0);
	public static final Field ENABLEAEMXSERIES = Field.create("ENABLEAEMXSERIES", 732, FieldType.BIT, 30).setBaseOffset(0);
	public static final Field MODELEDFLOWIDLE = Field.create("MODELEDFLOWIDLE", 732, FieldType.BIT, 31).setBaseOffset(0);
	public static final Field LOGICANALYZERPINS1 = Field.create("LOGICANALYZERPINS1", 736, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LOGICANALYZERPINS2 = Field.create("LOGICANALYZERPINS2", 738, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LOGICANALYZERPINS3 = Field.create("LOGICANALYZERPINS3", 740, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LOGICANALYZERPINS4 = Field.create("LOGICANALYZERPINS4", 742, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field MAINRELAYPINMODE = Field.create("MAINRELAYPINMODE", 744, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_745 = Field.create("ALIGNMENTFILL_AT_745", 745, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HIP9011CSPIN = Field.create("HIP9011CSPIN", 746, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field HIP9011INTHOLDPIN = Field.create("HIP9011INTHOLDPIN", 748, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field HIP9011INTHOLDPINMODE = Field.create("HIP9011INTHOLDPINMODE", 750, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_751 = Field.create("ALIGNMENTFILL_AT_751", 751, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VERBOSECANBASEADDRESS = Field.create("VERBOSECANBASEADDRESS", 752, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_HVOLT = Field.create("MC33_HVOLT", 756, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_757 = Field.create("ALIGNMENTFILL_AT_757", 757, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MINIMUMBOOSTCLOSEDLOOPMAP = Field.create("MINIMUMBOOSTCLOSEDLOOPMAP", 758, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field INITIALIGNITIONCUTPERCENT = Field.create("INITIALIGNITIONCUTPERCENT", 760, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FINALIGNITIONCUTPERCENTBEFORELAUNCH = Field.create("FINALIGNITIONCUTPERCENTBEFORELAUNCH", 761, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final String[] gppwm_channel_e = {"Zero", "TPS", "MAP", "CLT", "IAT", "Fuel Load", "Ignition Load", "Aux Temp 1", "Aux Temp 2", "Accel Pedal", "Battery Voltage", "VVT 1 In Actual", "VVT 1 Ex Actual", "VVT 2 In Actual", "VVT 2 Ex Actual", "Ethanol (Flex) %", "Aux Linear 1", "Aux Linear 2", "GPPWM Output 1", "GPPWM Output 2", "GPPWM Output 3", "GPPWM Output 4", "Lua Gauge 1", "Lua Gauge 2", "RPM", "Gear (detected)", "Baro pressure", "EGT 1", "EGT 2", "Aux Linear 3", "Aux Linear 4", "Vehicle speed", "Oil pressure", "Oil temp", "Fuel Pressure"};
	public static final Field BOOSTOPENLOOPYAXIS = Field.create("BOOSTOPENLOOPYAXIS", 762, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field L9779SPIDEVICE = Field.create("L9779SPIDEVICE", 763, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IMUTYPE = Field.create("IMUTYPE", 764, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_765 = Field.create("ALIGNMENTFILL_AT_765", 765, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEPIDRPMUPPERLIMIT = Field.create("IDLEPIDRPMUPPERLIMIT", 766, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field APPLYNONLINEARBELOWPULSE = Field.create("APPLYNONLINEARBELOWPULSE", 768, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field LPS25BAROSENSORSCL = Field.create("LPS25BAROSENSORSCL", 770, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field LPS25BAROSENSORSDA = Field.create("LPS25BAROSENSORSDA", 772, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field VEHICLESPEEDSENSORINPUTPIN = Field.create("VEHICLESPEEDSENSORINPUTPIN", 774, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field CLUTCHUPPIN = Field.create("CLUTCHUPPIN", 776, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORNONLINEARMODE = Field.create("INJECTORNONLINEARMODE", 778, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CLUTCHUPPINMODE = Field.create("CLUTCHUPPINMODE", 779, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS1 = Field.create("MAX31855_CS1", 780, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS2 = Field.create("MAX31855_CS2", 782, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS3 = Field.create("MAX31855_CS3", 784, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS4 = Field.create("MAX31855_CS4", 786, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS5 = Field.create("MAX31855_CS5", 788, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS6 = Field.create("MAX31855_CS6", 790, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS7 = Field.create("MAX31855_CS7", 792, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MAX31855_CS8 = Field.create("MAX31855_CS8", 794, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field FLEXSENSORPIN = Field.create("FLEXSENSORPIN", 796, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TORQUEREDUCTIONARMINGRPM = Field.create("TORQUEREDUCTIONARMINGRPM", 798, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDIRECTIONPINMODE = Field.create("STEPPERDIRECTIONPINMODE", 800, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MC33972SPIDEVICE = Field.create("MC33972SPIDEVICE", 801, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STOICHRATIOSECONDARY = Field.create("STOICHRATIOSECONDARY", 802, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ETBMAXIMUMPOSITION = Field.create("ETBMAXIMUMPOSITION", 803, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SDCARDLOGFREQUENCY = Field.create("SDCARDLOGFREQUENCY", 804, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEPOSITIONCHANNEL = Field.create("IDLEPOSITIONCHANNEL", 806, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_807 = Field.create("ALIGNMENTFILL_AT_807", 807, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field LAUNCHCORRECTIONSENDRPM = Field.create("LAUNCHCORRECTIONSENDRPM", 808, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STARTERRELAYDISABLEPIN = Field.create("STARTERRELAYDISABLEPIN", 810, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field STARTERRELAYDISABLEPINMODE = Field.create("STARTERRELAYDISABLEPINMODE", 812, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_813 = Field.create("ALIGNMENTFILL_AT_813", 813, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SECONDSOLENOIDPIN = Field.create("SECONDSOLENOIDPIN", 814, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field STARTSTOPBUTTONPIN = Field.create("STARTSTOPBUTTONPIN", 816, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONRESTORERPM = Field.create("LAMBDAPROTECTIONRESTORERPM", 818, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field ACRELAYPINMODE = Field.create("ACRELAYPINMODE", 819, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MAPMINBUFFERLENGTH = Field.create("MAPMINBUFFERLENGTH", 820, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEPIDDEACTIVATIONTPSTHRESHOLD = Field.create("IDLEPIDDEACTIVATIONTPSTHRESHOLD", 824, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERPARKINGEXTRASTEPS = Field.create("STEPPERPARKINGEXTRASTEPS", 826, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPS1SECONDARYMIN = Field.create("TPS1SECONDARYMIN", 828, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPS1SECONDARYMAX = Field.create("TPS1SECONDARYMAX", 830, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ANTILAGRPMTRESHOLD = Field.create("ANTILAGRPMTRESHOLD", 832, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STARTCRANKINGDURATION = Field.create("STARTCRANKINGDURATION", 834, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERERRORPIN = Field.create("TRIGGERERRORPIN", 836, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERERRORPINMODE = Field.create("TRIGGERERRORPINMODE", 838, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_839 = Field.create("ALIGNMENTFILL_AT_839", 839, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACRELAYPIN = Field.create("ACRELAYPIN", 840, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONMINTPS = Field.create("LAMBDAPROTECTIONMINTPS", 842, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONTIMEOUT = Field.create("LAMBDAPROTECTIONTIMEOUT", 843, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field SCRIPTSETTING1 = Field.create("SCRIPTSETTING1", 844, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTING2 = Field.create("SCRIPTSETTING2", 848, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTING3 = Field.create("SCRIPTSETTING3", 852, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTING4 = Field.create("SCRIPTSETTING4", 856, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTING5 = Field.create("SCRIPTSETTING5", 860, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTING6 = Field.create("SCRIPTSETTING6", 864, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTING7 = Field.create("SCRIPTSETTING7", 868, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTING8 = Field.create("SCRIPTSETTING8", 872, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field SPI1MOSIPIN = Field.create("SPI1MOSIPIN", 876, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI1MISOPIN = Field.create("SPI1MISOPIN", 878, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI1SCKPIN = Field.create("SPI1SCKPIN", 880, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI2MOSIPIN = Field.create("SPI2MOSIPIN", 882, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI2MISOPIN = Field.create("SPI2MISOPIN", 884, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI2SCKPIN = Field.create("SPI2SCKPIN", 886, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI3MOSIPIN = Field.create("SPI3MOSIPIN", 888, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI3MISOPIN = Field.create("SPI3MISOPIN", 890, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI3SCKPIN = Field.create("SPI3SCKPIN", 892, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CDMINPUTPIN = Field.create("CDMINPUTPIN", 894, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CONSOLEUARTDEVICE = Field.create("CONSOLEUARTDEVICE", 896, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final String[] sensor_chart_e = {"none", "trigger", "INVALID", "RPM ACCEL", "DETAILED RPM", "Fast Aux1"};
	public static final Field SENSORCHARTMODE = Field.create("SENSORCHARTMODE", 897, FieldType.INT8, sensor_chart_e).setScale(1.0).setBaseOffset(0);
	public static final String[] maf_sensor_type_e = {"v0", "v1", "v2", "v3"};
	public static final Field MAFSENSORTYPE = Field.create("MAFSENSORTYPE", 898, FieldType.INT8, maf_sensor_type_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_899 = Field.create("ALIGNMENTFILL_AT_899", 899, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLERETURNTARGETRAMP = Field.create("IDLERETURNTARGETRAMP", 900, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field USEHBRIDGESTODRIVEIDLESTEPPER = Field.create("USEHBRIDGESTODRIVEIDLESTEPPER", 900, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field MULTISPARKENABLE = Field.create("MULTISPARKENABLE", 900, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field ENABLELAUNCHRETARD = Field.create("ENABLELAUNCHRETARD", 900, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field CANINPUTBCM = Field.create("CANINPUTBCM", 900, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field CONSUMEOBDSENSORS = Field.create("CONSUMEOBDSENSORS", 900, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field ENABLECANVSS = Field.create("ENABLECANVSS", 900, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field SUPPRESSWBOWORKAROUND7048 = Field.create("SUPPRESSWBOWORKAROUND7048", 900, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field STFTIGNOREERRORMAGNITUDE = Field.create("STFTIGNOREERRORMAGNITUDE", 900, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field VVTBOOLEANFORVERYSPECIALCASES = Field.create("VVTBOOLEANFORVERYSPECIALCASES", 900, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field ENABLESOFTWAREKNOCK = Field.create("ENABLESOFTWAREKNOCK", 900, FieldType.BIT, 11).setBaseOffset(0);
	public static final Field VERBOSEVVTDECODING = Field.create("VERBOSEVVTDECODING", 900, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field INVERTCAMVVTSIGNAL = Field.create("INVERTCAMVVTSIGNAL", 900, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field ALPHANUSEIAT = Field.create("ALPHANUSEIAT", 900, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field KNOCKBANKCYL1 = Field.create("KNOCKBANKCYL1", 900, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field KNOCKBANKCYL2 = Field.create("KNOCKBANKCYL2", 900, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field KNOCKBANKCYL3 = Field.create("KNOCKBANKCYL3", 900, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field KNOCKBANKCYL4 = Field.create("KNOCKBANKCYL4", 900, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field KNOCKBANKCYL5 = Field.create("KNOCKBANKCYL5", 900, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field KNOCKBANKCYL6 = Field.create("KNOCKBANKCYL6", 900, FieldType.BIT, 20).setBaseOffset(0);
	public static final Field KNOCKBANKCYL7 = Field.create("KNOCKBANKCYL7", 900, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field KNOCKBANKCYL8 = Field.create("KNOCKBANKCYL8", 900, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field KNOCKBANKCYL9 = Field.create("KNOCKBANKCYL9", 900, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field KNOCKBANKCYL10 = Field.create("KNOCKBANKCYL10", 900, FieldType.BIT, 24).setBaseOffset(0);
	public static final Field KNOCKBANKCYL11 = Field.create("KNOCKBANKCYL11", 900, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field KNOCKBANKCYL12 = Field.create("KNOCKBANKCYL12", 900, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field TCUENABLED = Field.create("TCUENABLED", 900, FieldType.BIT, 27).setBaseOffset(0);
	public static final Field CANBROADCASTUSECHANNELTWO = Field.create("CANBROADCASTUSECHANNELTWO", 900, FieldType.BIT, 28).setBaseOffset(0);
	public static final Field USERAWOUTPUTTODRIVEIDLESTEPPER = Field.create("USERAWOUTPUTTODRIVEIDLESTEPPER", 900, FieldType.BIT, 29).setBaseOffset(0);
	public static final Field VERBOSECAN2 = Field.create("VERBOSECAN2", 900, FieldType.BIT, 30).setBaseOffset(0);
	public static final Field ETBIO1_DIRECTIONPIN1 = Field.create("ETBIO1_DIRECTIONPIN1", 904, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIO1_DIRECTIONPIN2 = Field.create("ETBIO1_DIRECTIONPIN2", 906, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIO1_CONTROLPIN = Field.create("ETBIO1_CONTROLPIN", 908, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIO1_DISABLEPIN = Field.create("ETBIO1_DISABLEPIN", 910, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIO2_DIRECTIONPIN1 = Field.create("ETBIO2_DIRECTIONPIN1", 912, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIO2_DIRECTIONPIN2 = Field.create("ETBIO2_DIRECTIONPIN2", 914, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIO2_CONTROLPIN = Field.create("ETBIO2_CONTROLPIN", 916, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIO2_DISABLEPIN = Field.create("ETBIO2_DISABLEPIN", 918, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCONTROLPIN = Field.create("BOOSTCONTROLPIN", 920, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCONTROLPINMODE = Field.create("BOOSTCONTROLPINMODE", 922, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_923 = Field.create("ALIGNMENTFILL_AT_923", 923, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALSACTIVATEPIN = Field.create("ALSACTIVATEPIN", 924, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LAUNCHACTIVATEPIN = Field.create("LAUNCHACTIVATEPIN", 926, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTPID_PFACTOR = Field.create("BOOSTPID_PFACTOR", 928, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BOOSTPID_IFACTOR = Field.create("BOOSTPID_IFACTOR", 932, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BOOSTPID_DFACTOR = Field.create("BOOSTPID_DFACTOR", 936, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BOOSTPID_OFFSET = Field.create("BOOSTPID_OFFSET", 940, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTPID_PERIODMS = Field.create("BOOSTPID_PERIODMS", 942, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTPID_MINVALUE = Field.create("BOOSTPID_MINVALUE", 944, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTPID_MAXVALUE = Field.create("BOOSTPID_MAXVALUE", 946, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTTYPE = Field.create("BOOSTTYPE", 948, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONKEYDIGITALPINMODE = Field.create("IGNITIONKEYDIGITALPINMODE", 949, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONKEYDIGITALPIN = Field.create("IGNITIONKEYDIGITALPIN", 950, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTPWMFREQUENCY = Field.create("BOOSTPWMFREQUENCY", 952, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final String[] launchActivationMode_e = {"Launch Button", "Clutch Down Switch", "Speed Based", "Brake Pedal"};
	public static final Field LAUNCHACTIVATIONMODE = Field.create("LAUNCHACTIVATIONMODE", 956, FieldType.INT8, launchActivationMode_e).setScale(1.0).setBaseOffset(0);
	public static final String[] antiLagActivationMode_e = {"Switch Input", "Always Active"};
	public static final Field ANTILAGACTIVATIONMODE = Field.create("ANTILAGACTIVATIONMODE", 957, FieldType.INT8, antiLagActivationMode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TPSACCELLOOKBACK = Field.create("TPSACCELLOOKBACK", 958, FieldType.INT8).setScale(0.05).setBaseOffset(0);
	public static final Field NOFUELTRIMAFTERDFCOTIME = Field.create("NOFUELTRIMAFTERDFCOTIME", 959, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field LAUNCHSPEEDTHRESHOLD = Field.create("LAUNCHSPEEDTHRESHOLD", 960, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field LAUNCHRPMWINDOW = Field.create("LAUNCHRPMWINDOW", 964, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGEREVENTSTIMEOUTMS = Field.create("TRIGGEREVENTSTIMEOUTMS", 968, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAPEXPAVERAGEALPHA = Field.create("MAPEXPAVERAGEALPHA", 972, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAGICNUMBERAVAILABLEFORDEVTRICKS = Field.create("MAGICNUMBERAVAILABLEFORDEVTRICKS", 976, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TURBOCHARGERFILTER = Field.create("TURBOCHARGERFILTER", 980, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LAUNCHTPSTHRESHOLD = Field.create("LAUNCHTPSTHRESHOLD", 984, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field LAUNCHACTIVATEDELAY = Field.create("LAUNCHACTIVATEDELAY", 988, FieldType.FLOAT).setBaseOffset(0);
	public static final Field STFT_MAXIDLEREGIONRPM = Field.create("STFT_MAXIDLEREGIONRPM", 992, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field STFT_ALIGNMENTFILL_AT_1 = Field.create("STFT_ALIGNMENTFILL_AT_1", 993, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_MAXOVERRUNLOAD = Field.create("STFT_MAXOVERRUNLOAD", 994, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_MINPOWERLOAD = Field.create("STFT_MINPOWERLOAD", 996, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_DEADBAND = Field.create("STFT_DEADBAND", 998, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field STFT_MINCLT = Field.create("STFT_MINCLT", 999, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_MINAFR = Field.create("STFT_MINAFR", 1000, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field STFT_MAXAFR = Field.create("STFT_MAXAFR", 1001, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field STFT_STARTUPDELAY = Field.create("STFT_STARTUPDELAY", 1002, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_ALIGNMENTFILL_AT_11 = Field.create("STFT_ALIGNMENTFILL_AT_11", 1003, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS1_MAXADD = Field.create("STFT_CELLCFGS1_MAXADD", 1004, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS1_MAXREMOVE = Field.create("STFT_CELLCFGS1_MAXREMOVE", 1005, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS1_TIMECONSTANT = Field.create("STFT_CELLCFGS1_TIMECONSTANT", 1006, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field STFT_CELLCFGS2_MAXADD = Field.create("STFT_CELLCFGS2_MAXADD", 1008, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS2_MAXREMOVE = Field.create("STFT_CELLCFGS2_MAXREMOVE", 1009, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS2_TIMECONSTANT = Field.create("STFT_CELLCFGS2_TIMECONSTANT", 1010, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field STFT_CELLCFGS3_MAXADD = Field.create("STFT_CELLCFGS3_MAXADD", 1012, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS3_MAXREMOVE = Field.create("STFT_CELLCFGS3_MAXREMOVE", 1013, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS3_TIMECONSTANT = Field.create("STFT_CELLCFGS3_TIMECONSTANT", 1014, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field STFT_CELLCFGS4_MAXADD = Field.create("STFT_CELLCFGS4_MAXADD", 1016, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS4_MAXREMOVE = Field.create("STFT_CELLCFGS4_MAXREMOVE", 1017, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STFT_CELLCFGS4_TIMECONSTANT = Field.create("STFT_CELLCFGS4_TIMECONSTANT", 1018, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field STEPPERDCIO1_DIRECTIONPIN1 = Field.create("STEPPERDCIO1_DIRECTIONPIN1", 1020, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCIO1_DIRECTIONPIN2 = Field.create("STEPPERDCIO1_DIRECTIONPIN2", 1022, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCIO1_CONTROLPIN = Field.create("STEPPERDCIO1_CONTROLPIN", 1024, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCIO1_DISABLEPIN = Field.create("STEPPERDCIO1_DISABLEPIN", 1026, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCIO2_DIRECTIONPIN1 = Field.create("STEPPERDCIO2_DIRECTIONPIN1", 1028, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCIO2_DIRECTIONPIN2 = Field.create("STEPPERDCIO2_DIRECTIONPIN2", 1030, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCIO2_CONTROLPIN = Field.create("STEPPERDCIO2_CONTROLPIN", 1032, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCIO2_DISABLEPIN = Field.create("STEPPERDCIO2_DISABLEPIN", 1034, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ENGINEMAKE = Field.create("ENGINEMAKE", 1036, 32, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field ENGINECODE = Field.create("ENGINECODE", 1068, 32, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field VEHICLENAME = Field.create("VEHICLENAME", 1100, 32, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID1 = Field.create("TCU_SOLENOID1", 1132, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID2 = Field.create("TCU_SOLENOID2", 1134, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID3 = Field.create("TCU_SOLENOID3", 1136, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID4 = Field.create("TCU_SOLENOID4", 1138, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID5 = Field.create("TCU_SOLENOID5", 1140, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID6 = Field.create("TCU_SOLENOID6", 1142, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field ETBFUNCTIONS1 = Field.create("ETBFUNCTIONS1", 1144, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ETBFUNCTIONS2 = Field.create("ETBFUNCTIONS2", 1145, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DRV8860SPIDEVICE = Field.create("DRV8860SPIDEVICE", 1146, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1147 = Field.create("ALIGNMENTFILL_AT_1147", 1147, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DRV8860_CS = Field.create("DRV8860_CS", 1148, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field DRV8860_CSPINMODE = Field.create("DRV8860_CSPINMODE", 1150, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1151 = Field.create("ALIGNMENTFILL_AT_1151", 1151, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DRV8860_MISO = Field.create("DRV8860_MISO", 1152, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS1 = Field.create("LUAOUTPUTPINS1", 1154, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS2 = Field.create("LUAOUTPUTPINS2", 1156, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS3 = Field.create("LUAOUTPUTPINS3", 1158, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS4 = Field.create("LUAOUTPUTPINS4", 1160, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS5 = Field.create("LUAOUTPUTPINS5", 1162, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS6 = Field.create("LUAOUTPUTPINS6", 1164, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS7 = Field.create("LUAOUTPUTPINS7", 1166, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUAOUTPUTPINS8 = Field.create("LUAOUTPUTPINS8", 1168, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1170 = Field.create("ALIGNMENTFILL_AT_1170", 1170, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VVTOFFSETS1 = Field.create("VVTOFFSETS1", 1172, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTOFFSETS2 = Field.create("VVTOFFSETS2", 1176, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTOFFSETS3 = Field.create("VVTOFFSETS3", 1180, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTOFFSETS4 = Field.create("VVTOFFSETS4", 1184, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VRTHRESHOLD1_RPMBINS = Field.create("VRTHRESHOLD1_RPMBINS", 1188, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field VRTHRESHOLD1_VALUES = Field.create("VRTHRESHOLD1_VALUES", 1194, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field VRTHRESHOLD1_PIN = Field.create("VRTHRESHOLD1_PIN", 1200, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field VRTHRESHOLD1_ALIGNMENTFILL_AT_14 = Field.create("VRTHRESHOLD1_ALIGNMENTFILL_AT_14", 1202, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VRTHRESHOLD2_RPMBINS = Field.create("VRTHRESHOLD2_RPMBINS", 1204, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field VRTHRESHOLD2_VALUES = Field.create("VRTHRESHOLD2_VALUES", 1210, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field VRTHRESHOLD2_PIN = Field.create("VRTHRESHOLD2_PIN", 1216, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field VRTHRESHOLD2_ALIGNMENTFILL_AT_14 = Field.create("VRTHRESHOLD2_ALIGNMENTFILL_AT_14", 1218, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMNOTE1 = Field.create("GPPWMNOTE1", 1220, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMNOTE2 = Field.create("GPPWMNOTE2", 1236, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMNOTE3 = Field.create("GPPWMNOTE3", 1252, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWMNOTE4 = Field.create("GPPWMNOTE4", 1268, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field TPS2SECONDARYMIN = Field.create("TPS2SECONDARYMIN", 1284, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPS2SECONDARYMAX = Field.create("TPS2SECONDARYMAX", 1286, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field WIDEBANDONSECONDBUS = Field.create("WIDEBANDONSECONDBUS", 1288, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field FUELCLOSEDLOOPCORRECTIONENABLED = Field.create("FUELCLOSEDLOOPCORRECTIONENABLED", 1288, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field ALWAYSWRITESDCARD = Field.create("ALWAYSWRITESDCARD", 1288, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field KNOCKDETECTIONUSEDOUBLEFREQUENCY = Field.create("KNOCKDETECTIONUSEDOUBLEFREQUENCY", 1288, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field YESUNDERSTANDLOCKING = Field.create("YESUNDERSTANDLOCKING", 1288, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field SILENTTRIGGERERROR = Field.create("SILENTTRIGGERERROR", 1288, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field USELINEARCLTSENSOR = Field.create("USELINEARCLTSENSOR", 1288, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field CANREADENABLED = Field.create("CANREADENABLED", 1288, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field CANWRITEENABLED = Field.create("CANWRITEENABLED", 1288, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field USELINEARIATSENSOR = Field.create("USELINEARIATSENSOR", 1288, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field ENABLEOILPRESSUREPROTECT = Field.create("ENABLEOILPRESSUREPROTECT", 1288, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field TACHPULSEDURATIONASDUTYCYCLE = Field.create("TACHPULSEDURATIONASDUTYCYCLE", 1288, FieldType.BIT, 11).setBaseOffset(0);
	public static final Field ISALTERNATORCONTROLENABLED = Field.create("ISALTERNATORCONTROLENABLED", 1288, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field INVERTPRIMARYTRIGGERSIGNAL = Field.create("INVERTPRIMARYTRIGGERSIGNAL", 1288, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field INVERTSECONDARYTRIGGERSIGNAL = Field.create("INVERTSECONDARYTRIGGERSIGNAL", 1288, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field CUTFUELONHARDLIMIT = Field.create("CUTFUELONHARDLIMIT", 1288, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field CUTSPARKONHARDLIMIT = Field.create("CUTSPARKONHARDLIMIT", 1288, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field LAUNCHFUELCUTENABLE = Field.create("LAUNCHFUELCUTENABLE", 1288, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field LAUNCHSPARKCUTENABLE = Field.create("LAUNCHSPARKCUTENABLE", 1288, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field TORQUEREDUCTIONENABLED = Field.create("TORQUEREDUCTIONENABLED", 1288, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field LIMITTORQUEREDUCTIONTIME = Field.create("LIMITTORQUEREDUCTIONTIME", 1288, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field VERBOSEISOTP = Field.create("VERBOSEISOTP", 1288, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field ENGINESNIFFERFOCUSONINPUTS = Field.create("ENGINESNIFFERFOCUSONINPUTS", 1288, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field TWOSTROKE = Field.create("TWOSTROKE", 1288, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field SKIPPEDWHEELONCAM = Field.create("SKIPPEDWHEELONCAM", 1288, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field HIPOUTPUTCHANNEL = Field.create("HIPOUTPUTCHANNEL", 1292, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1293 = Field.create("ALIGNMENTFILL_AT_1293", 1293, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACSWITCH = Field.create("ACSWITCH", 1294, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field VREFADCCHANNEL = Field.create("VREFADCCHANNEL", 1296, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ETBNEUTRALPOSITION = Field.create("ETBNEUTRALPOSITION", 1297, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEMODE = Field.create("IDLEMODE", 1298, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1299 = Field.create("ALIGNMENTFILL_AT_1299", 1299, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ISINJECTIONENABLED = Field.create("ISINJECTIONENABLED", 1300, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field ISIGNITIONENABLED = Field.create("ISIGNITIONENABLED", 1300, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field ISCYLINDERCLEANUPENABLED = Field.create("ISCYLINDERCLEANUPENABLED", 1300, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field COMPLEXWALLMODEL = Field.create("COMPLEXWALLMODEL", 1300, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field ALWAYSINSTANTRPM = Field.create("ALWAYSINSTANTRPM", 1300, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field ISMAPAVERAGINGENABLED = Field.create("ISMAPAVERAGINGENABLED", 1300, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field USESEPARATEADVANCEFORIDLE = Field.create("USESEPARATEADVANCEFORIDLE", 1300, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field ISWAVEANALYZERENABLED = Field.create("ISWAVEANALYZERENABLED", 1300, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field USESEPARATEVEFORIDLE = Field.create("USESEPARATEVEFORIDLE", 1300, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field VERBOSETRIGGERSYNCHDETAILS = Field.create("VERBOSETRIGGERSYNCHDETAILS", 1300, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field HONDAK = Field.create("HONDAK", 1300, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field TWOWIREBATCHIGNITION = Field.create("TWOWIREBATCHIGNITION", 1300, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field USEFIXEDBAROCORRFROMMAP = Field.create("USEFIXEDBAROCORRFROMMAP", 1300, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field USESEPARATEADVANCEFORCRANKING = Field.create("USESEPARATEADVANCEFORCRANKING", 1300, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field USEADVANCECORRECTIONSFORCRANKING = Field.create("USEADVANCECORRECTIONSFORCRANKING", 1300, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field FLEXCRANKING = Field.create("FLEXCRANKING", 1300, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field USEIACPIDMULTTABLE = Field.create("USEIACPIDMULTTABLE", 1300, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field ISBOOSTCONTROLENABLED = Field.create("ISBOOSTCONTROLENABLED", 1300, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field LAUNCHSMOOTHRETARD = Field.create("LAUNCHSMOOTHRETARD", 1300, FieldType.BIT, 20).setBaseOffset(0);
	public static final Field ISPHASESYNCREQUIREDFORIGNITION = Field.create("ISPHASESYNCREQUIREDFORIGNITION", 1300, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field USECLTBASEDRPMLIMIT = Field.create("USECLTBASEDRPMLIMIT", 1300, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field FORCEO2HEATING = Field.create("FORCEO2HEATING", 1300, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field INVERTVVTCONTROLINTAKE = Field.create("INVERTVVTCONTROLINTAKE", 1300, FieldType.BIT, 24).setBaseOffset(0);
	public static final Field INVERTVVTCONTROLEXHAUST = Field.create("INVERTVVTCONTROLEXHAUST", 1300, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field USEBIQUADONAUXSPEEDSENSORS = Field.create("USEBIQUADONAUXSPEEDSENSORS", 1300, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field SDTRIGGERLOG = Field.create("SDTRIGGERLOG", 1300, FieldType.BIT, 27).setBaseOffset(0);
	public static final Field STEPPER_DC_USE_TWO_WIRES = Field.create("STEPPER_DC_USE_TWO_WIRES", 1300, FieldType.BIT, 29).setBaseOffset(0);
	public static final Field WATCHOUTFORLINEARTIME = Field.create("WATCHOUTFORLINEARTIME", 1300, FieldType.BIT, 30).setBaseOffset(0);
	public static final Field ENGINECHARTSIZE = Field.create("ENGINECHARTSIZE", 1304, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TURBOSPEEDSENSORMULTIPLIER = Field.create("TURBOSPEEDSENSORMULTIPLIER", 1308, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CAMINPUTSDEBUG1 = Field.create("CAMINPUTSDEBUG1", 1312, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CAMINPUTSDEBUG2 = Field.create("CAMINPUTSDEBUG2", 1314, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CAMINPUTSDEBUG3 = Field.create("CAMINPUTSDEBUG3", 1316, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CAMINPUTSDEBUG4 = Field.create("CAMINPUTSDEBUG4", 1318, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ACIDLERPMTARGET = Field.create("ACIDLERPMTARGET", 1320, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field WARNINGPERIOD = Field.create("WARNINGPERIOD", 1322, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field KNOCKDETECTIONWINDOWSTART = Field.create("KNOCKDETECTIONWINDOWSTART", 1324, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLESTEPPERREACTIONTIME = Field.create("IDLESTEPPERREACTIONTIME", 1328, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLESTEPPERTOTALSTEPS = Field.create("IDLESTEPPERTOTALSTEPS", 1332, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TORQUEREDUCTIONARMINGAPP = Field.create("TORQUEREDUCTIONARMINGAPP", 1336, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TACHPULSEDURACTIONMS = Field.create("TACHPULSEDURACTIONMS", 1340, FieldType.FLOAT).setBaseOffset(0);
	public static final Field WWAETAU = Field.create("WWAETAU", 1344, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ALTERNATORCONTROL_PFACTOR = Field.create("ALTERNATORCONTROL_PFACTOR", 1348, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ALTERNATORCONTROL_IFACTOR = Field.create("ALTERNATORCONTROL_IFACTOR", 1352, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ALTERNATORCONTROL_DFACTOR = Field.create("ALTERNATORCONTROL_DFACTOR", 1356, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ALTERNATORCONTROL_OFFSET = Field.create("ALTERNATORCONTROL_OFFSET", 1360, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORCONTROL_PERIODMS = Field.create("ALTERNATORCONTROL_PERIODMS", 1362, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORCONTROL_MINVALUE = Field.create("ALTERNATORCONTROL_MINVALUE", 1364, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORCONTROL_MAXVALUE = Field.create("ALTERNATORCONTROL_MAXVALUE", 1366, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETB_PFACTOR = Field.create("ETB_PFACTOR", 1368, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETB_IFACTOR = Field.create("ETB_IFACTOR", 1372, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETB_DFACTOR = Field.create("ETB_DFACTOR", 1376, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETB_OFFSET = Field.create("ETB_OFFSET", 1380, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETB_PERIODMS = Field.create("ETB_PERIODMS", 1382, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETB_MINVALUE = Field.create("ETB_MINVALUE", 1384, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETB_MAXVALUE = Field.create("ETB_MAXVALUE", 1386, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERINPUTDEBUGPINS1 = Field.create("TRIGGERINPUTDEBUGPINS1", 1388, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERINPUTDEBUGPINS2 = Field.create("TRIGGERINPUTDEBUGPINS2", 1390, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field AIRTAPERRPMRANGE = Field.create("AIRTAPERRPMRANGE", 1392, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TURBOSPEEDSENSORINPUTPIN = Field.create("TURBOSPEEDSENSORINPUTPIN", 1394, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TPS2MIN = Field.create("TPS2MIN", 1396, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPS2MAX = Field.create("TPS2MAX", 1398, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STARTERCONTROLPIN = Field.create("STARTERCONTROLPIN", 1400, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field STARTSTOPBUTTONMODE = Field.create("STARTSTOPBUTTONMODE", 1402, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1403 = Field.create("ALIGNMENTFILL_AT_1403", 1403, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33816_FLAG0 = Field.create("MC33816_FLAG0", 1404, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TACHPULSEPERREV = Field.create("TACHPULSEPERREV", 1406, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field MAPERRORDETECTIONTOOLOW = Field.create("MAPERRORDETECTIONTOOLOW", 1408, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAPERRORDETECTIONTOOHIGH = Field.create("MAPERRORDETECTIONTOOHIGH", 1412, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MULTISPARKSPARKDURATION = Field.create("MULTISPARKSPARKDURATION", 1416, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field MULTISPARKDWELL = Field.create("MULTISPARKDWELL", 1418, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field IDLERPMPID_PFACTOR = Field.create("IDLERPMPID_PFACTOR", 1420, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLERPMPID_IFACTOR = Field.create("IDLERPMPID_IFACTOR", 1424, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLERPMPID_DFACTOR = Field.create("IDLERPMPID_DFACTOR", 1428, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLERPMPID_OFFSET = Field.create("IDLERPMPID_OFFSET", 1432, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLERPMPID_PERIODMS = Field.create("IDLERPMPID_PERIODMS", 1434, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLERPMPID_MINVALUE = Field.create("IDLERPMPID_MINVALUE", 1436, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLERPMPID_MAXVALUE = Field.create("IDLERPMPID_MAXVALUE", 1438, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field WWAEBETA = Field.create("WWAEBETA", 1440, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BINARYSERIALTXPIN = Field.create("BINARYSERIALTXPIN", 1444, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field BINARYSERIALRXPIN = Field.create("BINARYSERIALRXPIN", 1446, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field AUXVALVES1 = Field.create("AUXVALVES1", 1448, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field AUXVALVES2 = Field.create("AUXVALVES2", 1450, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TCUUPSHIFTBUTTONPIN = Field.create("TCUUPSHIFTBUTTONPIN", 1452, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCUDOWNSHIFTBUTTONPIN = Field.create("TCUDOWNSHIFTBUTTONPIN", 1454, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field THROTTLEPEDALUPVOLTAGE = Field.create("THROTTLEPEDALUPVOLTAGE", 1456, FieldType.FLOAT).setBaseOffset(0);
	public static final Field THROTTLEPEDALWOTVOLTAGE = Field.create("THROTTLEPEDALWOTVOLTAGE", 1460, FieldType.FLOAT).setBaseOffset(0);
	public static final Field STARTUPFUELPUMPDURATION = Field.create("STARTUPFUELPUMPDURATION", 1464, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEPIDRPMDEADZONE = Field.create("IDLEPIDRPMDEADZONE", 1466, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEMAXIMUMAIRMASS = Field.create("IDLEMAXIMUMAIRMASS", 1468, FieldType.INT8).setScale(2.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1469 = Field.create("ALIGNMENTFILL_AT_1469", 1469, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TORQUEREDUCTIONTIME = Field.create("TORQUEREDUCTIONTIME", 1472, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MC33810DISABLERECOVERYMODE = Field.create("MC33810DISABLERECOVERYMODE", 1476, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field MC33810GPGD0MODE = Field.create("MC33810GPGD0MODE", 1476, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field MC33810GPGD1MODE = Field.create("MC33810GPGD1MODE", 1476, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field MC33810GPGD2MODE = Field.create("MC33810GPGD2MODE", 1476, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field MC33810GPGD3MODE = Field.create("MC33810GPGD3MODE", 1476, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field ENABLEEXTENDEDCANBROADCAST = Field.create("ENABLEEXTENDEDCANBROADCAST", 1476, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field LUACANRXWORKAROUND = Field.create("LUACANRXWORKAROUND", 1476, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field FLEXSENSORINVERTED = Field.create("FLEXSENSORINVERTED", 1476, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field USEHARDSKIPINTRACTION = Field.create("USEHARDSKIPINTRACTION", 1476, FieldType.BIT, 8).setBaseOffset(0);
	public static final Field USEAUXSPEEDFORSLIPRATIO = Field.create("USEAUXSPEEDFORSLIPRATIO", 1476, FieldType.BIT, 9).setBaseOffset(0);
	public static final Field USEVSSASSECONDWHEELSPEED = Field.create("USEVSSASSECONDWHEELSPEED", 1476, FieldType.BIT, 10).setBaseOffset(0);
	public static final Field IS_ENABLED_SPI_5 = Field.create("IS_ENABLED_SPI_5", 1476, FieldType.BIT, 11).setBaseOffset(0);
	public static final Field IS_ENABLED_SPI_6 = Field.create("IS_ENABLED_SPI_6", 1476, FieldType.BIT, 12).setBaseOffset(0);
	public static final Field ENABLEAEMXSERIESEGT = Field.create("ENABLEAEMXSERIESEGT", 1476, FieldType.BIT, 13).setBaseOffset(0);
	public static final Field STARTREQUESTPININVERTED = Field.create("STARTREQUESTPININVERTED", 1476, FieldType.BIT, 14).setBaseOffset(0);
	public static final Field TCU_RANGESENSORPULLDOWN = Field.create("TCU_RANGESENSORPULLDOWN", 1476, FieldType.BIT, 15).setBaseOffset(0);
	public static final Field DEVBIT01 = Field.create("DEVBIT01", 1476, FieldType.BIT, 16).setBaseOffset(0);
	public static final Field DEVBIT0 = Field.create("DEVBIT0", 1476, FieldType.BIT, 17).setBaseOffset(0);
	public static final Field DEVBIT1 = Field.create("DEVBIT1", 1476, FieldType.BIT, 18).setBaseOffset(0);
	public static final Field DEVBIT2 = Field.create("DEVBIT2", 1476, FieldType.BIT, 19).setBaseOffset(0);
	public static final Field DEVBIT3 = Field.create("DEVBIT3", 1476, FieldType.BIT, 20).setBaseOffset(0);
	public static final Field DEVBIT4 = Field.create("DEVBIT4", 1476, FieldType.BIT, 21).setBaseOffset(0);
	public static final Field DEVBIT5 = Field.create("DEVBIT5", 1476, FieldType.BIT, 22).setBaseOffset(0);
	public static final Field DEVBIT6 = Field.create("DEVBIT6", 1476, FieldType.BIT, 23).setBaseOffset(0);
	public static final Field DEVBIT7 = Field.create("DEVBIT7", 1476, FieldType.BIT, 24).setBaseOffset(0);
	public static final Field INVERTEXHAUSTCAMVVTSIGNAL = Field.create("INVERTEXHAUSTCAMVVTSIGNAL", 1476, FieldType.BIT, 25).setBaseOffset(0);
	public static final Field ENABLEKNOCKSPECTROGRAM = Field.create("ENABLEKNOCKSPECTROGRAM", 1476, FieldType.BIT, 26).setBaseOffset(0);
	public static final Field ENABLEKNOCKSPECTROGRAMFILTER = Field.create("ENABLEKNOCKSPECTROGRAMFILTER", 1476, FieldType.BIT, 27).setBaseOffset(0);
	public static final Field AFTERCRANKINGIACTAPERDURATION = Field.create("AFTERCRANKINGIACTAPERDURATION", 1480, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IACBYTPSTAPER = Field.create("IACBYTPSTAPER", 1482, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ACCELEROMETERCSPIN = Field.create("ACCELEROMETERCSPIN", 1484, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field COASTINGFUELCUTVSSLOW = Field.create("COASTINGFUELCUTVSSLOW", 1486, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field COASTINGFUELCUTVSSHIGH = Field.create("COASTINGFUELCUTVSSHIGH", 1487, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TPSACCELENRICHMENTTHRESHOLD = Field.create("TPSACCELENRICHMENTTHRESHOLD", 1488, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXSPEEDSENSORINPUTPIN1 = Field.create("AUXSPEEDSENSORINPUTPIN1", 1492, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXSPEEDSENSORINPUTPIN2 = Field.create("AUXSPEEDSENSORINPUTPIN2", 1494, FieldType.INT16, brain_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TOTALGEARSCOUNT = Field.create("TOTALGEARSCOUNT", 1496, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONTIMINGMODE = Field.create("INJECTIONTIMINGMODE", 1497, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final String[] debug_mode_e = {"INVALID", "TPS acceleration enrichment", "INVALID", "Stepper Idle Control", "Engine Load accl enrich", "Trigger Counters", "Soft Spark Cut", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "SD card", "sr5", "Knock", "INVALID", "Electronic Throttle", "Executor", "Bench Test / TS commands", "INVALID", "Analog inputs #1", "INSTANT_RPM", "INVALID", "Status", "INVALID", "INVALID", "MAP", "Metrics", "INVALID", "Ion Sense", "TLE8888", "Analog inputs #2", "Dwell Metric", "INVALID", "INVALID", "Boost Control", "INVALID", "INVALID", "ETB Autotune", "Composite Log", "INVALID", "INVALID", "INVALID", "Dyno_View", "Logic_Analyzer", "INVALID", "TCU", "Lua"};
	public static final Field DEBUGMODE = Field.create("DEBUGMODE", 1498, FieldType.INT8, debug_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field FAN1EXTRAIDLE = Field.create("FAN1EXTRAIDLE", 1499, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field UARTCONSOLESERIALSPEED = Field.create("UARTCONSOLESERIALSPEED", 1500, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TPSDECELENLEANMENTTHRESHOLD = Field.create("TPSDECELENLEANMENTTHRESHOLD", 1504, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TPSDECELENLEANMENTMULTIPLIER = Field.create("TPSDECELENLEANMENTMULTIPLIER", 1508, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TORQUEREDUCTIONIGNITIONRETARD = Field.create("TORQUEREDUCTIONIGNITIONRETARD", 1512, FieldType.FLOAT).setBaseOffset(0);
	public static final Field THROTTLEPEDALSECONDARYUPVOLTAGE = Field.create("THROTTLEPEDALSECONDARYUPVOLTAGE", 1516, FieldType.FLOAT).setBaseOffset(0);
	public static final Field THROTTLEPEDALSECONDARYWOTVOLTAGE = Field.create("THROTTLEPEDALSECONDARYWOTVOLTAGE", 1520, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] can_baudrate_e = {"50kbps", "83.33kbps", "100kbps", "125kbps", "250kbps", "500kbps", "1Mbps"};
	public static final Field CANBAUDRATE = Field.create("CANBAUDRATE", 1524, FieldType.INT8, can_baudrate_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEOVERRIDEMODE = Field.create("VEOVERRIDEMODE", 1525, FieldType.INT8, ve_override_e).setScale(1.0).setBaseOffset(0);
	public static final Field CAN2BAUDRATE = Field.create("CAN2BAUDRATE", 1526, FieldType.INT8, can_baudrate_e).setScale(1.0).setBaseOffset(0);
	public static final String[] load_override_e = {"None", "MAP", "TPS", "Acc Pedal", "Cyl Filling %"};
	public static final Field AFROVERRIDEMODE = Field.create("AFROVERRIDEMODE", 1527, FieldType.INT8, load_override_e).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_HPFP_I_PEAK = Field.create("MC33_HPFP_I_PEAK", 1528, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field MC33_HPFP_I_HOLD = Field.create("MC33_HPFP_I_HOLD", 1529, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field MC33_HPFP_I_HOLD_OFF = Field.create("MC33_HPFP_I_HOLD_OFF", 1530, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_HPFP_MAX_HOLD = Field.create("MC33_HPFP_MAX_HOLD", 1531, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERDCINVERTEDPINS = Field.create("STEPPERDCINVERTEDPINS", 1532, FieldType.BIT, 0).setBaseOffset(0);
	public static final Field CANOPENBLT = Field.create("CANOPENBLT", 1532, FieldType.BIT, 1).setBaseOffset(0);
	public static final Field CAN2OPENBLT = Field.create("CAN2OPENBLT", 1532, FieldType.BIT, 2).setBaseOffset(0);
	public static final Field INJECTORFLOWASMASSFLOW = Field.create("INJECTORFLOWASMASSFLOW", 1532, FieldType.BIT, 3).setBaseOffset(0);
	public static final Field BOARDUSECANTERMINATOR = Field.create("BOARDUSECANTERMINATOR", 1532, FieldType.BIT, 4).setBaseOffset(0);
	public static final Field KLINEDOHONDASEND = Field.create("KLINEDOHONDASEND", 1532, FieldType.BIT, 5).setBaseOffset(0);
	public static final Field CAN1LISTENMODE = Field.create("CAN1LISTENMODE", 1532, FieldType.BIT, 6).setBaseOffset(0);
	public static final Field CAN2LISTENMODE = Field.create("CAN2LISTENMODE", 1532, FieldType.BIT, 7).setBaseOffset(0);
	public static final Field CAMDECODER2JZPOSITION = Field.create("CAMDECODER2JZPOSITION", 1536, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33810MAXDWELLTIMER = Field.create("MC33810MAXDWELLTIMER", 1537, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BENCHTESTONTIME = Field.create("BENCHTESTONTIME", 1538, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONRESTORETPS = Field.create("LAMBDAPROTECTIONRESTORETPS", 1540, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDAPROTECTIONRESTORELOAD = Field.create("LAMBDAPROTECTIONRESTORELOAD", 1541, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field LAUNCHACTIVATEPINMODE = Field.create("LAUNCHACTIVATEPINMODE", 1542, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1543 = Field.create("ALIGNMENTFILL_AT_1543", 1543, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CAN2TXPIN = Field.create("CAN2TXPIN", 1544, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CAN2RXPIN = Field.create("CAN2RXPIN", 1546, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field STARTERCONTROLPINMODE = Field.create("STARTERCONTROLPINMODE", 1548, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field WASTEGATEPOSITIONSENSOR = Field.create("WASTEGATEPOSITIONSENSOR", 1549, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNOVERRIDEMODE = Field.create("IGNOVERRIDEMODE", 1550, FieldType.INT8, load_override_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORPRESSURETYPE = Field.create("INJECTORPRESSURETYPE", 1551, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPVALVEPIN = Field.create("HPFPVALVEPIN", 1552, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPVALVEPINMODE = Field.create("HPFPVALVEPINMODE", 1554, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1555 = Field.create("ALIGNMENTFILL_AT_1555", 1555, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCUTPRESSURE = Field.create("BOOSTCUTPRESSURE", 1556, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCHARGEBINS = Field.create("TCHARGEBINS", 1560, FieldType.INT8).setScale(5.0).setBaseOffset(0);
	public static final Field TCHARGEVALUES = Field.create("TCHARGEVALUES", 1576, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field FIXEDTIMING = Field.create("FIXEDTIMING", 1592, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAPLOWVALUEVOLTAGE = Field.create("MAPLOWVALUEVOLTAGE", 1596, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAPHIGHVALUEVOLTAGE = Field.create("MAPHIGHVALUEVOLTAGE", 1600, FieldType.FLOAT).setBaseOffset(0);
	public static final Field EGOVALUESHIFT = Field.create("EGOVALUESHIFT", 1604, FieldType.FLOAT).setBaseOffset(0);
	public static final Field VVTPINS1 = Field.create("VVTPINS1", 1608, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field VVTPINS2 = Field.create("VVTPINS2", 1610, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field VVTPINS3 = Field.create("VVTPINS3", 1612, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field VVTPINS4 = Field.create("VVTPINS4", 1614, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field CRANKINGIACPOSITION = Field.create("CRANKINGIACPOSITION", 1616, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TCHARGEMINRPMMINTPS = Field.create("TCHARGEMINRPMMINTPS", 1620, FieldType.INT8).setScale(0.005).setBaseOffset(0);
	public static final Field TCHARGEMINRPMMAXTPS = Field.create("TCHARGEMINRPMMAXTPS", 1621, FieldType.INT8).setScale(0.005).setBaseOffset(0);
	public static final Field TCHARGEMAXRPMMINTPS = Field.create("TCHARGEMAXRPMMINTPS", 1622, FieldType.INT8).setScale(0.005).setBaseOffset(0);
	public static final Field TCHARGEMAXRPMMAXTPS = Field.create("TCHARGEMAXRPMMAXTPS", 1623, FieldType.INT8).setScale(0.005).setBaseOffset(0);
	public static final Field VVTOUTPUTFREQUENCY = Field.create("VVTOUTPUTFREQUENCY", 1624, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MINIMUMIGNITIONTIMING = Field.create("MINIMUMIGNITIONTIMING", 1626, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXIMUMIGNITIONTIMING = Field.create("MAXIMUMIGNITIONTIMING", 1627, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORPWMFREQUENCY = Field.create("ALTERNATORPWMFREQUENCY", 1628, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final String[] vvt_mode_e = {"Inactive", "Single Tooth", "Toyota 3 Tooth", "Miata NB2", "Mitsu 4G69", "Bosch Quick Start", "4/1", "ST 170", "Ford Barra 3+1", "Nissan VQ", "Honda K Intake", "Nissan MR18", "Mitsu 3A92", "Sync by MAP", "Mitsu 6G75", "Mazda Skyactiv", "Honda K Exhaust", "Mitsubishi 4G92/93/94", "Mitsubishi 4G63", "Ford Coyote", "Mitsu 6G72", "Honda 600", "Mazda L", "Dev", "HR12DDR In", "vvt25", "vvt26"};
	public static final Field VVTMODE1 = Field.create("VVTMODE1", 1632, FieldType.INT8, vvt_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field VVTMODE2 = Field.create("VVTMODE2", 1633, FieldType.INT8, vvt_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field FAN2EXTRAIDLE = Field.create("FAN2EXTRAIDLE", 1634, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field PRIMINGDELAY = Field.create("PRIMINGDELAY", 1635, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS1 = Field.create("AUXANALOGINPUTS1", 1636, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS2 = Field.create("AUXANALOGINPUTS2", 1637, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS3 = Field.create("AUXANALOGINPUTS3", 1638, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS4 = Field.create("AUXANALOGINPUTS4", 1639, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS5 = Field.create("AUXANALOGINPUTS5", 1640, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS6 = Field.create("AUXANALOGINPUTS6", 1641, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS7 = Field.create("AUXANALOGINPUTS7", 1642, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXANALOGINPUTS8 = Field.create("AUXANALOGINPUTS8", 1643, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS1 = Field.create("TRAILINGCOILPINS1", 1644, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS2 = Field.create("TRAILINGCOILPINS2", 1646, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS3 = Field.create("TRAILINGCOILPINS3", 1648, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS4 = Field.create("TRAILINGCOILPINS4", 1650, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS5 = Field.create("TRAILINGCOILPINS5", 1652, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS6 = Field.create("TRAILINGCOILPINS6", 1654, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS7 = Field.create("TRAILINGCOILPINS7", 1656, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS8 = Field.create("TRAILINGCOILPINS8", 1658, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS9 = Field.create("TRAILINGCOILPINS9", 1660, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS10 = Field.create("TRAILINGCOILPINS10", 1662, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS11 = Field.create("TRAILINGCOILPINS11", 1664, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRAILINGCOILPINS12 = Field.create("TRAILINGCOILPINS12", 1666, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TLE8888MODE = Field.create("TLE8888MODE", 1668, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACCELEROMETERCSPINMODE = Field.create("ACCELEROMETERCSPINMODE", 1669, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORCOMPENSATIONMODE = Field.create("INJECTORCOMPENSATIONMODE", 1670, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FAN2PINMODE = Field.create("FAN2PINMODE", 1671, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field FUELREFERENCEPRESSURE = Field.create("FUELREFERENCEPRESSURE", 1672, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_TEMPC_1 = Field.create("AUXTEMPSENSOR1_TEMPC_1", 1676, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_TEMPC_2 = Field.create("AUXTEMPSENSOR1_TEMPC_2", 1680, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_TEMPC_3 = Field.create("AUXTEMPSENSOR1_TEMPC_3", 1684, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_RESISTANCE_1 = Field.create("AUXTEMPSENSOR1_RESISTANCE_1", 1688, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_RESISTANCE_2 = Field.create("AUXTEMPSENSOR1_RESISTANCE_2", 1692, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_RESISTANCE_3 = Field.create("AUXTEMPSENSOR1_RESISTANCE_3", 1696, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_BIAS_RESISTOR = Field.create("AUXTEMPSENSOR1_BIAS_RESISTOR", 1700, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_ADCCHANNEL = Field.create("AUXTEMPSENSOR1_ADCCHANNEL", 1704, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR1_ALIGNMENTFILL_AT_29 = Field.create("AUXTEMPSENSOR1_ALIGNMENTFILL_AT_29", 1705, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_TEMPC_1 = Field.create("AUXTEMPSENSOR2_TEMPC_1", 1708, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_TEMPC_2 = Field.create("AUXTEMPSENSOR2_TEMPC_2", 1712, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_TEMPC_3 = Field.create("AUXTEMPSENSOR2_TEMPC_3", 1716, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_RESISTANCE_1 = Field.create("AUXTEMPSENSOR2_RESISTANCE_1", 1720, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_RESISTANCE_2 = Field.create("AUXTEMPSENSOR2_RESISTANCE_2", 1724, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_RESISTANCE_3 = Field.create("AUXTEMPSENSOR2_RESISTANCE_3", 1728, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_BIAS_RESISTOR = Field.create("AUXTEMPSENSOR2_BIAS_RESISTOR", 1732, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_ADCCHANNEL = Field.create("AUXTEMPSENSOR2_ADCCHANNEL", 1736, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXTEMPSENSOR2_ALIGNMENTFILL_AT_29 = Field.create("AUXTEMPSENSOR2_ALIGNMENTFILL_AT_29", 1737, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field KNOCKSAMPLINGDURATION = Field.create("KNOCKSAMPLINGDURATION", 1740, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBFREQ = Field.create("ETBFREQ", 1742, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBWASTEGATEPID_PFACTOR = Field.create("ETBWASTEGATEPID_PFACTOR", 1744, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETBWASTEGATEPID_IFACTOR = Field.create("ETBWASTEGATEPID_IFACTOR", 1748, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETBWASTEGATEPID_DFACTOR = Field.create("ETBWASTEGATEPID_DFACTOR", 1752, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETBWASTEGATEPID_OFFSET = Field.create("ETBWASTEGATEPID_OFFSET", 1756, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBWASTEGATEPID_PERIODMS = Field.create("ETBWASTEGATEPID_PERIODMS", 1758, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBWASTEGATEPID_MINVALUE = Field.create("ETBWASTEGATEPID_MINVALUE", 1760, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBWASTEGATEPID_MAXVALUE = Field.create("ETBWASTEGATEPID_MAXVALUE", 1762, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final String[] stepper_num_micro_steps_e = {"Full-Step (Default)", "INVALID", "Half-Step", "INVALID", "1/4 Micro-Step", "INVALID", "INVALID", "INVALID", "1/8 Micro-Step"};
	public static final Field STEPPERNUMMICROSTEPS = Field.create("STEPPERNUMMICROSTEPS", 1764, FieldType.INT8, stepper_num_micro_steps_e).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERMINDUTYCYCLE = Field.create("STEPPERMINDUTYCYCLE", 1765, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERMAXDUTYCYCLE = Field.create("STEPPERMAXDUTYCYCLE", 1766, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SDCARDSPIDEVICE = Field.create("SDCARDSPIDEVICE", 1767, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER1 = Field.create("TIMING_OFFSET_CYLINDER1", 1768, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER2 = Field.create("TIMING_OFFSET_CYLINDER2", 1772, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER3 = Field.create("TIMING_OFFSET_CYLINDER3", 1776, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER4 = Field.create("TIMING_OFFSET_CYLINDER4", 1780, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER5 = Field.create("TIMING_OFFSET_CYLINDER5", 1784, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER6 = Field.create("TIMING_OFFSET_CYLINDER6", 1788, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER7 = Field.create("TIMING_OFFSET_CYLINDER7", 1792, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER8 = Field.create("TIMING_OFFSET_CYLINDER8", 1796, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER9 = Field.create("TIMING_OFFSET_CYLINDER9", 1800, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER10 = Field.create("TIMING_OFFSET_CYLINDER10", 1804, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER11 = Field.create("TIMING_OFFSET_CYLINDER11", 1808, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TIMING_OFFSET_CYLINDER12 = Field.create("TIMING_OFFSET_CYLINDER12", 1812, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLEPIDACTIVATIONTIME = Field.create("IDLEPIDACTIVATIONTIME", 1816, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] pin_mode_e = {"default", "INVALID", "INVALID", "INVALID", "opendrain", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLUP", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLDOWN"};
	public static final Field SPI1SCKMODE = Field.create("SPI1SCKMODE", 1820, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI1MOSIMODE = Field.create("SPI1MOSIMODE", 1821, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI1MISOMODE = Field.create("SPI1MISOMODE", 1822, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI2SCKMODE = Field.create("SPI2SCKMODE", 1823, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI2MOSIMODE = Field.create("SPI2MOSIMODE", 1824, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI2MISOMODE = Field.create("SPI2MISOMODE", 1825, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI3SCKMODE = Field.create("SPI3SCKMODE", 1826, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI3MOSIMODE = Field.create("SPI3MOSIMODE", 1827, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI3MISOMODE = Field.create("SPI3MISOMODE", 1828, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERENABLEPINMODE = Field.create("STEPPERENABLEPINMODE", 1829, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field MC33816_RSTB = Field.create("MC33816_RSTB", 1830, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field MC33816_DRIVEN = Field.create("MC33816_DRIVEN", 1832, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field BRAKEPEDALPIN = Field.create("BRAKEPEDALPIN", 1834, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID1_PFACTOR = Field.create("AUXPID1_PFACTOR", 1836, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXPID1_IFACTOR = Field.create("AUXPID1_IFACTOR", 1840, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXPID1_DFACTOR = Field.create("AUXPID1_DFACTOR", 1844, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXPID1_OFFSET = Field.create("AUXPID1_OFFSET", 1848, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID1_PERIODMS = Field.create("AUXPID1_PERIODMS", 1850, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID1_MINVALUE = Field.create("AUXPID1_MINVALUE", 1852, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID1_MAXVALUE = Field.create("AUXPID1_MAXVALUE", 1854, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID2_PFACTOR = Field.create("AUXPID2_PFACTOR", 1856, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXPID2_IFACTOR = Field.create("AUXPID2_IFACTOR", 1860, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXPID2_DFACTOR = Field.create("AUXPID2_DFACTOR", 1864, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXPID2_OFFSET = Field.create("AUXPID2_OFFSET", 1868, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID2_PERIODMS = Field.create("AUXPID2_PERIODMS", 1870, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID2_MINVALUE = Field.create("AUXPID2_MINVALUE", 1872, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXPID2_MAXVALUE = Field.create("AUXPID2_MAXVALUE", 1874, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL1 = Field.create("INJECTORCORRECTIONPOLYNOMIAL1", 1876, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL2 = Field.create("INJECTORCORRECTIONPOLYNOMIAL2", 1880, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL3 = Field.create("INJECTORCORRECTIONPOLYNOMIAL3", 1884, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL4 = Field.create("INJECTORCORRECTIONPOLYNOMIAL4", 1888, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL5 = Field.create("INJECTORCORRECTIONPOLYNOMIAL5", 1892, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL6 = Field.create("INJECTORCORRECTIONPOLYNOMIAL6", 1896, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL7 = Field.create("INJECTORCORRECTIONPOLYNOMIAL7", 1900, FieldType.FLOAT).setBaseOffset(0);
	public static final Field INJECTORCORRECTIONPOLYNOMIAL8 = Field.create("INJECTORCORRECTIONPOLYNOMIAL8", 1904, FieldType.FLOAT).setBaseOffset(0);
	public static final Field PRIMEBINS = Field.create("PRIMEBINS", 1908, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field OILPRESSURE_HWCHANNEL = Field.create("OILPRESSURE_HWCHANNEL", 1916, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field OILPRESSURE_ALIGNMENTFILL_AT_1 = Field.create("OILPRESSURE_ALIGNMENTFILL_AT_1", 1917, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field OILPRESSURE_V1 = Field.create("OILPRESSURE_V1", 1920, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILPRESSURE_VALUE1 = Field.create("OILPRESSURE_VALUE1", 1924, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILPRESSURE_V2 = Field.create("OILPRESSURE_V2", 1928, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILPRESSURE_VALUE2 = Field.create("OILPRESSURE_VALUE2", 1932, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ACCELEROMETERSPIDEVICE = Field.create("ACCELEROMETERSPIDEVICE", 1936, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1937 = Field.create("ALIGNMENTFILL_AT_1937", 1937, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FAN2PIN = Field.create("FAN2PIN", 1938, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field FAN2ONTEMPERATURE = Field.create("FAN2ONTEMPERATURE", 1940, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FAN2OFFTEMPERATURE = Field.create("FAN2OFFTEMPERATURE", 1941, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPERENABLEPIN = Field.create("STEPPERENABLEPIN", 1942, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TLE8888_CS = Field.create("TLE8888_CS", 1944, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field TLE8888_CSPINMODE = Field.create("TLE8888_CSPINMODE", 1946, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1947 = Field.create("ALIGNMENTFILL_AT_1947", 1947, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33816_CS = Field.create("MC33816_CS", 1948, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1950 = Field.create("ALIGNMENTFILL_AT_1950", 1950, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXFREQUENCYFILTER = Field.create("AUXFREQUENCYFILTER", 1952, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] sent_input_pin_e = {"NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"};
	public static final Field SENTINPUTPINS1 = Field.create("SENTINPUTPINS1", 1956, FieldType.INT16, sent_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field COASTINGFUELCUTRPMHIGH = Field.create("COASTINGFUELCUTRPMHIGH", 1958, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field COASTINGFUELCUTRPMLOW = Field.create("COASTINGFUELCUTRPMLOW", 1960, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field COASTINGFUELCUTTPS = Field.create("COASTINGFUELCUTTPS", 1962, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field COASTINGFUELCUTCLT = Field.create("COASTINGFUELCUTCLT", 1964, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field PIDEXTRAFORLOWRPM = Field.create("PIDEXTRAFORLOWRPM", 1966, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field COASTINGFUELCUTMAP = Field.create("COASTINGFUELCUTMAP", 1968, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_1970 = Field.create("ALIGNMENTFILL_AT_1970", 1970, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HIGHPRESSUREFUEL_HWCHANNEL = Field.create("HIGHPRESSUREFUEL_HWCHANNEL", 1972, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field HIGHPRESSUREFUEL_ALIGNMENTFILL_AT_1 = Field.create("HIGHPRESSUREFUEL_ALIGNMENTFILL_AT_1", 1973, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HIGHPRESSUREFUEL_V1 = Field.create("HIGHPRESSUREFUEL_V1", 1976, FieldType.FLOAT).setBaseOffset(0);
	public static final Field HIGHPRESSUREFUEL_VALUE1 = Field.create("HIGHPRESSUREFUEL_VALUE1", 1980, FieldType.FLOAT).setBaseOffset(0);
	public static final Field HIGHPRESSUREFUEL_V2 = Field.create("HIGHPRESSUREFUEL_V2", 1984, FieldType.FLOAT).setBaseOffset(0);
	public static final Field HIGHPRESSUREFUEL_VALUE2 = Field.create("HIGHPRESSUREFUEL_VALUE2", 1988, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LOWPRESSUREFUEL_HWCHANNEL = Field.create("LOWPRESSUREFUEL_HWCHANNEL", 1992, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field LOWPRESSUREFUEL_ALIGNMENTFILL_AT_1 = Field.create("LOWPRESSUREFUEL_ALIGNMENTFILL_AT_1", 1993, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field LOWPRESSUREFUEL_V1 = Field.create("LOWPRESSUREFUEL_V1", 1996, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LOWPRESSUREFUEL_VALUE1 = Field.create("LOWPRESSUREFUEL_VALUE1", 2000, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LOWPRESSUREFUEL_V2 = Field.create("LOWPRESSUREFUEL_V2", 2004, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LOWPRESSUREFUEL_VALUE2 = Field.create("LOWPRESSUREFUEL_VALUE2", 2008, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVENAME1 = Field.create("SCRIPTCURVENAME1", 2012, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTCURVENAME2 = Field.create("SCRIPTCURVENAME2", 2028, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTCURVENAME3 = Field.create("SCRIPTCURVENAME3", 2044, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTCURVENAME4 = Field.create("SCRIPTCURVENAME4", 2060, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTCURVENAME5 = Field.create("SCRIPTCURVENAME5", 2076, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTCURVENAME6 = Field.create("SCRIPTCURVENAME6", 2092, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLENAME1 = Field.create("SCRIPTTABLENAME1", 2108, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLENAME2 = Field.create("SCRIPTTABLENAME2", 2124, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLENAME3 = Field.create("SCRIPTTABLENAME3", 2140, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLENAME4 = Field.create("SCRIPTTABLENAME4", 2156, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME1 = Field.create("SCRIPTSETTINGNAME1", 2172, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME2 = Field.create("SCRIPTSETTINGNAME2", 2188, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME3 = Field.create("SCRIPTSETTINGNAME3", 2204, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME4 = Field.create("SCRIPTSETTINGNAME4", 2220, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME5 = Field.create("SCRIPTSETTINGNAME5", 2236, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME6 = Field.create("SCRIPTSETTINGNAME6", 2252, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME7 = Field.create("SCRIPTSETTINGNAME7", 2268, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTSETTINGNAME8 = Field.create("SCRIPTSETTINGNAME8", 2284, 16, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field TCHARGEAIRCOEFMIN = Field.create("TCHARGEAIRCOEFMIN", 2300, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCHARGEAIRCOEFMAX = Field.create("TCHARGEAIRCOEFMAX", 2304, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCHARGEAIRFLOWMAX = Field.create("TCHARGEAIRFLOWMAX", 2308, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCHARGEAIRINCRLIMIT = Field.create("TCHARGEAIRINCRLIMIT", 2312, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCHARGEAIRDECRLIMIT = Field.create("TCHARGEAIRDECRLIMIT", 2316, FieldType.FLOAT).setBaseOffset(0);
	public static final Field HIP9011GAIN = Field.create("HIP9011GAIN", 2320, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETB_ITERMMIN = Field.create("ETB_ITERMMIN", 2324, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETB_ITERMMAX = Field.create("ETB_ITERMMAX", 2326, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLETIMINGPID_PFACTOR = Field.create("IDLETIMINGPID_PFACTOR", 2328, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLETIMINGPID_IFACTOR = Field.create("IDLETIMINGPID_IFACTOR", 2332, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLETIMINGPID_DFACTOR = Field.create("IDLETIMINGPID_DFACTOR", 2336, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLETIMINGPID_OFFSET = Field.create("IDLETIMINGPID_OFFSET", 2340, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLETIMINGPID_PERIODMS = Field.create("IDLETIMINGPID_PERIODMS", 2342, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLETIMINGPID_MINVALUE = Field.create("IDLETIMINGPID_MINVALUE", 2344, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLETIMINGPID_MAXVALUE = Field.create("IDLETIMINGPID_MAXVALUE", 2346, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IDLETIMINGSOFTENTRYTIME = Field.create("IDLETIMINGSOFTENTRYTIME", 2348, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TORQUEREDUCTIONTRIGGERPINMODE = Field.create("TORQUEREDUCTIONTRIGGERPINMODE", 2352, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final String[] torqueReductionActivationMode_e = {"Torque Reduction Button", "Launch Button", "Clutch Down Switch", "Clutch Up Switch"};
	public static final Field TORQUEREDUCTIONACTIVATIONMODE = Field.create("TORQUEREDUCTIONACTIVATIONMODE", 2353, FieldType.INT8, torqueReductionActivationMode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TPSACCELFRACTIONPERIOD = Field.create("TPSACCELFRACTIONPERIOD", 2354, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPSACCELFRACTIONDIVISOR = Field.create("TPSACCELFRACTIONDIVISOR", 2356, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TLE8888SPIDEVICE = Field.create("TLE8888SPIDEVICE", 2360, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33816SPIDEVICE = Field.create("MC33816SPIDEVICE", 2361, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLERPMPID_ITERMMIN = Field.create("IDLERPMPID_ITERMMIN", 2362, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TLE6240SPIDEVICE = Field.create("TLE6240SPIDEVICE", 2364, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field STOICHRATIOPRIMARY = Field.create("STOICHRATIOPRIMARY", 2365, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field IDLERPMPID_ITERMMAX = Field.create("IDLERPMPID_ITERMMAX", 2366, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBIDLETHROTTLERANGE = Field.create("ETBIDLETHROTTLERANGE", 2368, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT1 = Field.create("CYLINDERBANKSELECT1", 2372, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT2 = Field.create("CYLINDERBANKSELECT2", 2373, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT3 = Field.create("CYLINDERBANKSELECT3", 2374, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT4 = Field.create("CYLINDERBANKSELECT4", 2375, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT5 = Field.create("CYLINDERBANKSELECT5", 2376, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT6 = Field.create("CYLINDERBANKSELECT6", 2377, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT7 = Field.create("CYLINDERBANKSELECT7", 2378, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT8 = Field.create("CYLINDERBANKSELECT8", 2379, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT9 = Field.create("CYLINDERBANKSELECT9", 2380, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT10 = Field.create("CYLINDERBANKSELECT10", 2381, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT11 = Field.create("CYLINDERBANKSELECT11", 2382, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CYLINDERBANKSELECT12 = Field.create("CYLINDERBANKSELECT12", 2383, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field PRIMEVALUES = Field.create("PRIMEVALUES", 2384, FieldType.INT8).setScale(5.0).setBaseOffset(0);
	public static final Field TRIGGERCOMPCENTERVOLT = Field.create("TRIGGERCOMPCENTERVOLT", 2392, FieldType.INT8).setScale(0.02).setBaseOffset(0);
	public static final Field TRIGGERCOMPHYSTMIN = Field.create("TRIGGERCOMPHYSTMIN", 2393, FieldType.INT8).setScale(0.02).setBaseOffset(0);
	public static final Field TRIGGERCOMPHYSTMAX = Field.create("TRIGGERCOMPHYSTMAX", 2394, FieldType.INT8).setScale(0.02).setBaseOffset(0);
	public static final Field TRIGGERCOMPSENSORSATRPM = Field.create("TRIGGERCOMPSENSORSATRPM", 2395, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field TRACTIONCONTROLSLIPBINS = Field.create("TRACTIONCONTROLSLIPBINS", 2396, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TRACTIONCONTROLSPEEDBINS = Field.create("TRACTIONCONTROLSPEEDBINS", 2408, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DISABLEFAN1ATSPEED = Field.create("DISABLEFAN1ATSPEED", 2414, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DISABLEFAN2ATSPEED = Field.create("DISABLEFAN2ATSPEED", 2415, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CANVSSNBCTYPE = Field.create("CANVSSNBCTYPE", 2416, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_2418 = Field.create("ALIGNMENTFILL_AT_2418", 2418, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_PIN = Field.create("GPPWM1_PIN", 2420, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_DUTYIFERROR = Field.create("GPPWM1_DUTYIFERROR", 2422, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_ALIGNMENTFILL_AT_3 = Field.create("GPPWM1_ALIGNMENTFILL_AT_3", 2423, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_PWMFREQUENCY = Field.create("GPPWM1_PWMFREQUENCY", 2424, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_ONABOVEDUTY = Field.create("GPPWM1_ONABOVEDUTY", 2426, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_OFFBELOWDUTY = Field.create("GPPWM1_OFFBELOWDUTY", 2427, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_LOADAXIS = Field.create("GPPWM1_LOADAXIS", 2428, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_RPMAXIS = Field.create("GPPWM1_RPMAXIS", 2429, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_LOADBINS = Field.create("GPPWM1_LOADBINS", 2430, FieldType.INT16).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM1_RPMBINS = Field.create("GPPWM1_RPMBINS", 2446, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM1_TABLE = Field.create("GPPWM1_TABLE", 2462, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM1_ALIGNMENTFILL_AT_106 = Field.create("GPPWM1_ALIGNMENTFILL_AT_106", 2526, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_PIN = Field.create("GPPWM2_PIN", 2528, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_DUTYIFERROR = Field.create("GPPWM2_DUTYIFERROR", 2530, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_ALIGNMENTFILL_AT_3 = Field.create("GPPWM2_ALIGNMENTFILL_AT_3", 2531, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_PWMFREQUENCY = Field.create("GPPWM2_PWMFREQUENCY", 2532, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_ONABOVEDUTY = Field.create("GPPWM2_ONABOVEDUTY", 2534, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_OFFBELOWDUTY = Field.create("GPPWM2_OFFBELOWDUTY", 2535, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_LOADAXIS = Field.create("GPPWM2_LOADAXIS", 2536, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_RPMAXIS = Field.create("GPPWM2_RPMAXIS", 2537, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_LOADBINS = Field.create("GPPWM2_LOADBINS", 2538, FieldType.INT16).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM2_RPMBINS = Field.create("GPPWM2_RPMBINS", 2554, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM2_TABLE = Field.create("GPPWM2_TABLE", 2570, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM2_ALIGNMENTFILL_AT_106 = Field.create("GPPWM2_ALIGNMENTFILL_AT_106", 2634, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_PIN = Field.create("GPPWM3_PIN", 2636, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_DUTYIFERROR = Field.create("GPPWM3_DUTYIFERROR", 2638, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_ALIGNMENTFILL_AT_3 = Field.create("GPPWM3_ALIGNMENTFILL_AT_3", 2639, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_PWMFREQUENCY = Field.create("GPPWM3_PWMFREQUENCY", 2640, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_ONABOVEDUTY = Field.create("GPPWM3_ONABOVEDUTY", 2642, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_OFFBELOWDUTY = Field.create("GPPWM3_OFFBELOWDUTY", 2643, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_LOADAXIS = Field.create("GPPWM3_LOADAXIS", 2644, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_RPMAXIS = Field.create("GPPWM3_RPMAXIS", 2645, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_LOADBINS = Field.create("GPPWM3_LOADBINS", 2646, FieldType.INT16).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM3_RPMBINS = Field.create("GPPWM3_RPMBINS", 2662, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM3_TABLE = Field.create("GPPWM3_TABLE", 2678, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM3_ALIGNMENTFILL_AT_106 = Field.create("GPPWM3_ALIGNMENTFILL_AT_106", 2742, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_PIN = Field.create("GPPWM4_PIN", 2744, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_DUTYIFERROR = Field.create("GPPWM4_DUTYIFERROR", 2746, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_ALIGNMENTFILL_AT_3 = Field.create("GPPWM4_ALIGNMENTFILL_AT_3", 2747, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_PWMFREQUENCY = Field.create("GPPWM4_PWMFREQUENCY", 2748, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_ONABOVEDUTY = Field.create("GPPWM4_ONABOVEDUTY", 2750, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_OFFBELOWDUTY = Field.create("GPPWM4_OFFBELOWDUTY", 2751, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_LOADAXIS = Field.create("GPPWM4_LOADAXIS", 2752, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_RPMAXIS = Field.create("GPPWM4_RPMAXIS", 2753, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_LOADBINS = Field.create("GPPWM4_LOADBINS", 2754, FieldType.INT16).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM4_RPMBINS = Field.create("GPPWM4_RPMBINS", 2770, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GPPWM4_TABLE = Field.create("GPPWM4_TABLE", 2786, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GPPWM4_ALIGNMENTFILL_AT_106 = Field.create("GPPWM4_ALIGNMENTFILL_AT_106", 2850, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_I_BOOST = Field.create("MC33_I_BOOST", 2852, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_I_PEAK = Field.create("MC33_I_PEAK", 2854, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_I_HOLD = Field.create("MC33_I_HOLD", 2856, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_T_MAX_BOOST = Field.create("MC33_T_MAX_BOOST", 2858, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_T_PEAK_OFF = Field.create("MC33_T_PEAK_OFF", 2860, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_T_PEAK_TOT = Field.create("MC33_T_PEAK_TOT", 2862, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_T_BYPASS = Field.create("MC33_T_BYPASS", 2864, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_T_HOLD_OFF = Field.create("MC33_T_HOLD_OFF", 2866, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MC33_T_HOLD_TOT = Field.create("MC33_T_HOLD_TOT", 2868, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TCUUPSHIFTBUTTONPINMODE = Field.create("TCUUPSHIFTBUTTONPINMODE", 2870, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCUDOWNSHIFTBUTTONPINMODE = Field.create("TCUDOWNSHIFTBUTTONPINMODE", 2871, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ACSWITCHMODE = Field.create("ACSWITCHMODE", 2872, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID_MODE1 = Field.create("TCU_SOLENOID_MODE1", 2873, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID_MODE2 = Field.create("TCU_SOLENOID_MODE2", 2874, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID_MODE3 = Field.create("TCU_SOLENOID_MODE3", 2875, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID_MODE4 = Field.create("TCU_SOLENOID_MODE4", 2876, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID_MODE5 = Field.create("TCU_SOLENOID_MODE5", 2877, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SOLENOID_MODE6 = Field.create("TCU_SOLENOID_MODE6", 2878, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_2879 = Field.create("ALIGNMENTFILL_AT_2879", 2879, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM1 = Field.create("TRIGGERGAPOVERRIDEFROM1", 2880, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM2 = Field.create("TRIGGERGAPOVERRIDEFROM2", 2884, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM3 = Field.create("TRIGGERGAPOVERRIDEFROM3", 2888, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM4 = Field.create("TRIGGERGAPOVERRIDEFROM4", 2892, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM5 = Field.create("TRIGGERGAPOVERRIDEFROM5", 2896, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM6 = Field.create("TRIGGERGAPOVERRIDEFROM6", 2900, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM7 = Field.create("TRIGGERGAPOVERRIDEFROM7", 2904, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM8 = Field.create("TRIGGERGAPOVERRIDEFROM8", 2908, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM9 = Field.create("TRIGGERGAPOVERRIDEFROM9", 2912, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM10 = Field.create("TRIGGERGAPOVERRIDEFROM10", 2916, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM11 = Field.create("TRIGGERGAPOVERRIDEFROM11", 2920, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM12 = Field.create("TRIGGERGAPOVERRIDEFROM12", 2924, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM13 = Field.create("TRIGGERGAPOVERRIDEFROM13", 2928, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM14 = Field.create("TRIGGERGAPOVERRIDEFROM14", 2932, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM15 = Field.create("TRIGGERGAPOVERRIDEFROM15", 2936, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM16 = Field.create("TRIGGERGAPOVERRIDEFROM16", 2940, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM17 = Field.create("TRIGGERGAPOVERRIDEFROM17", 2944, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDEFROM18 = Field.create("TRIGGERGAPOVERRIDEFROM18", 2948, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO1 = Field.create("TRIGGERGAPOVERRIDETO1", 2952, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO2 = Field.create("TRIGGERGAPOVERRIDETO2", 2956, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO3 = Field.create("TRIGGERGAPOVERRIDETO3", 2960, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO4 = Field.create("TRIGGERGAPOVERRIDETO4", 2964, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO5 = Field.create("TRIGGERGAPOVERRIDETO5", 2968, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO6 = Field.create("TRIGGERGAPOVERRIDETO6", 2972, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO7 = Field.create("TRIGGERGAPOVERRIDETO7", 2976, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO8 = Field.create("TRIGGERGAPOVERRIDETO8", 2980, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO9 = Field.create("TRIGGERGAPOVERRIDETO9", 2984, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO10 = Field.create("TRIGGERGAPOVERRIDETO10", 2988, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO11 = Field.create("TRIGGERGAPOVERRIDETO11", 2992, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO12 = Field.create("TRIGGERGAPOVERRIDETO12", 2996, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO13 = Field.create("TRIGGERGAPOVERRIDETO13", 3000, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO14 = Field.create("TRIGGERGAPOVERRIDETO14", 3004, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO15 = Field.create("TRIGGERGAPOVERRIDETO15", 3008, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO16 = Field.create("TRIGGERGAPOVERRIDETO16", 3012, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO17 = Field.create("TRIGGERGAPOVERRIDETO17", 3016, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERGAPOVERRIDETO18 = Field.create("TRIGGERGAPOVERRIDETO18", 3020, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAXCAMPHASERESOLVERPM = Field.create("MAXCAMPHASERESOLVERPM", 3024, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field DFCODELAY = Field.create("DFCODELAY", 3025, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ACDELAY = Field.create("ACDELAY", 3026, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final String[] tChargeMode_e = {"RPM+TPS (Default)", "Air Mass Interpolation", "Table"};
	public static final Field TCHARGEMODE = Field.create("TCHARGEMODE", 3027, FieldType.INT8, tChargeMode_e).setScale(1.0).setBaseOffset(0);
	public static final Field FORDINJECTORSMALLPULSEBREAKPOINT = Field.create("FORDINJECTORSMALLPULSEBREAKPOINT", 3028, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field ETBJAMDETECTTHRESHOLD = Field.create("ETBJAMDETECTTHRESHOLD", 3030, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPCAMLOBES = Field.create("HPFPCAMLOBES", 3031, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final String[] hpfp_cam_e = {"NONE", "Intake 1", "Exhaust 1", "Intake 2", "Exhaust 2"};
	public static final Field HPFPCAM = Field.create("HPFPCAM", 3032, FieldType.INT8, hpfp_cam_e).setScale(1.0).setBaseOffset(0);
	public static final Field ACLOWRPMLIMIT = Field.create("ACLOWRPMLIMIT", 3033, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field HPFPMINANGLE = Field.create("HPFPMINANGLE", 3034, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3035 = Field.create("ALIGNMENTFILL_AT_3035", 3035, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPPUMPVOLUME = Field.create("HPFPPUMPVOLUME", 3036, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field HPFPACTIVATIONANGLE = Field.create("HPFPACTIVATIONANGLE", 3038, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ISSFILTERRECIPROCAL = Field.create("ISSFILTERRECIPROCAL", 3039, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPPIDP = Field.create("HPFPPIDP", 3040, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field HPFPPIDI = Field.create("HPFPPIDI", 3042, FieldType.INT16).setScale(1.0E-5).setBaseOffset(0);
	public static final Field HPFPTARGETDECAY = Field.create("HPFPTARGETDECAY", 3044, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPER_RAW_OUTPUT1 = Field.create("STEPPER_RAW_OUTPUT1", 3046, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPER_RAW_OUTPUT2 = Field.create("STEPPER_RAW_OUTPUT2", 3048, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPER_RAW_OUTPUT3 = Field.create("STEPPER_RAW_OUTPUT3", 3050, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field STEPPER_RAW_OUTPUT4 = Field.create("STEPPER_RAW_OUTPUT4", 3052, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field GEARRATIO1 = Field.create("GEARRATIO1", 3054, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO2 = Field.create("GEARRATIO2", 3056, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO3 = Field.create("GEARRATIO3", 3058, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO4 = Field.create("GEARRATIO4", 3060, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO5 = Field.create("GEARRATIO5", 3062, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO6 = Field.create("GEARRATIO6", 3064, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO7 = Field.create("GEARRATIO7", 3066, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO8 = Field.create("GEARRATIO8", 3068, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO9 = Field.create("GEARRATIO9", 3070, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field GEARRATIO10 = Field.create("GEARRATIO10", 3072, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field VVTACTIVATIONDELAYMS = Field.create("VVTACTIVATIONDELAYMS", 3074, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TORQUETABLE = Field.create("TORQUETABLE", 3076, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field TORQUERPMBINS = Field.create("TORQUERPMBINS", 3112, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TORQUELOADBINS = Field.create("TORQUELOADBINS", 3124, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field GEARCONTROLLERMODE = Field.create("GEARCONTROLLERMODE", 3136, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TRANSMISSIONCONTROLLERMODE = Field.create("TRANSMISSIONCONTROLLERMODE", 3137, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACRDISABLEPHASE = Field.create("ACRDISABLEPHASE", 3138, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR1_HWCHANNEL = Field.create("AUXLINEAR1_HWCHANNEL", 3140, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR1_ALIGNMENTFILL_AT_1 = Field.create("AUXLINEAR1_ALIGNMENTFILL_AT_1", 3141, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR1_V1 = Field.create("AUXLINEAR1_V1", 3144, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR1_VALUE1 = Field.create("AUXLINEAR1_VALUE1", 3148, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR1_V2 = Field.create("AUXLINEAR1_V2", 3152, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR1_VALUE2 = Field.create("AUXLINEAR1_VALUE2", 3156, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR2_HWCHANNEL = Field.create("AUXLINEAR2_HWCHANNEL", 3160, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR2_ALIGNMENTFILL_AT_1 = Field.create("AUXLINEAR2_ALIGNMENTFILL_AT_1", 3161, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR2_V1 = Field.create("AUXLINEAR2_V1", 3164, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR2_VALUE1 = Field.create("AUXLINEAR2_VALUE1", 3168, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR2_V2 = Field.create("AUXLINEAR2_V2", 3172, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR2_VALUE2 = Field.create("AUXLINEAR2_VALUE2", 3176, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_TCC_ONOFF_SOLENOID = Field.create("TCU_TCC_ONOFF_SOLENOID", 3180, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_TCC_ONOFF_SOLENOID_MODE = Field.create("TCU_TCC_ONOFF_SOLENOID_MODE", 3182, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3183 = Field.create("ALIGNMENTFILL_AT_3183", 3183, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_TCC_PWM_SOLENOID = Field.create("TCU_TCC_PWM_SOLENOID", 3184, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_TCC_PWM_SOLENOID_MODE = Field.create("TCU_TCC_PWM_SOLENOID_MODE", 3186, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3187 = Field.create("ALIGNMENTFILL_AT_3187", 3187, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_TCC_PWM_SOLENOID_FREQ = Field.create("TCU_TCC_PWM_SOLENOID_FREQ", 3188, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PC_SOLENOID_PIN = Field.create("TCU_PC_SOLENOID_PIN", 3190, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PC_SOLENOID_PIN_MODE = Field.create("TCU_PC_SOLENOID_PIN_MODE", 3192, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3193 = Field.create("ALIGNMENTFILL_AT_3193", 3193, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PC_SOLENOID_FREQ = Field.create("TCU_PC_SOLENOID_FREQ", 3194, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_32_SOLENOID_PIN = Field.create("TCU_32_SOLENOID_PIN", 3196, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_32_SOLENOID_PIN_MODE = Field.create("TCU_32_SOLENOID_PIN_MODE", 3198, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3199 = Field.create("ALIGNMENTFILL_AT_3199", 3199, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_32_SOLENOID_FREQ = Field.create("TCU_32_SOLENOID_FREQ", 3200, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ACRPIN2 = Field.create("ACRPIN2", 3202, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field ETBMINIMUMPOSITION = Field.create("ETBMINIMUMPOSITION", 3204, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3205 = Field.create("ALIGNMENTFILL_AT_3205", 3205, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TUNEHIDINGKEY = Field.create("TUNEHIDINGKEY", 3206, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VINNUMBER = Field.create("VINNUMBER", 3208, 17, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3225 = Field.create("ALIGNMENTFILL_AT_3225", 3225, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HIGHSPEEDOFFSETS = Field.create("HIGHSPEEDOFFSETS", 3226, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final String[] fuel_pressure_sensor_mode_e = {"Absolute", "Gauge", "Differential", "INVALID"};
	public static final Field FUELPRESSURESENSORMODE = Field.create("FUELPRESSURESENSORMODE", 3290, FieldType.INT8, fuel_pressure_sensor_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3291 = Field.create("ALIGNMENTFILL_AT_3291", 3291, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS1 = Field.create("LUADIGITALINPUTPINS1", 3292, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS2 = Field.create("LUADIGITALINPUTPINS2", 3294, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS3 = Field.create("LUADIGITALINPUTPINS3", 3296, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS4 = Field.create("LUADIGITALINPUTPINS4", 3298, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS5 = Field.create("LUADIGITALINPUTPINS5", 3300, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS6 = Field.create("LUADIGITALINPUTPINS6", 3302, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS7 = Field.create("LUADIGITALINPUTPINS7", 3304, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINS8 = Field.create("LUADIGITALINPUTPINS8", 3306, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALSMINRPM = Field.create("ALSMINRPM", 3308, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSMAXRPM = Field.create("ALSMAXRPM", 3310, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSMAXDURATION = Field.create("ALSMAXDURATION", 3312, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSMINCLT = Field.create("ALSMINCLT", 3314, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALSMAXCLT = Field.create("ALSMAXCLT", 3315, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALSMINTIMEBETWEEN = Field.create("ALSMINTIMEBETWEEN", 3316, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALSETBPOSITION = Field.create("ALSETBPOSITION", 3317, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACRELAYALTERNATORDUTYADDER = Field.create("ACRELAYALTERNATORDUTYADDER", 3318, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final String[] SentEtbType = {"None", "GM type 1", "Ford type 1", "Custom"};
	public static final Field SENTETBTYPE = Field.create("SENTETBTYPE", 3319, FieldType.INT8, SentEtbType).setScale(1.0).setBaseOffset(0);
	public static final Field CUSTOMSENTTPSMIN = Field.create("CUSTOMSENTTPSMIN", 3320, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3322 = Field.create("ALIGNMENTFILL_AT_3322", 3322, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALSIDLEADD = Field.create("ALSIDLEADD", 3324, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field ALSETBADD = Field.create("ALSETBADD", 3328, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field ALSSKIPRATIO = Field.create("ALSSKIPRATIO", 3332, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ACPRESSUREENABLEHYST = Field.create("ACPRESSUREENABLEHYST", 3336, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field ALSACTIVATEPINMODE = Field.create("ALSACTIVATEPINMODE", 3337, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TPSSECONDARYMAXIMUM = Field.create("TPSSECONDARYMAXIMUM", 3338, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field PPSSECONDARYMAXIMUM = Field.create("PPSSECONDARYMAXIMUM", 3339, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES1 = Field.create("LUADIGITALINPUTPINMODES1", 3340, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES2 = Field.create("LUADIGITALINPUTPINMODES2", 3341, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES3 = Field.create("LUADIGITALINPUTPINMODES3", 3342, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES4 = Field.create("LUADIGITALINPUTPINMODES4", 3343, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES5 = Field.create("LUADIGITALINPUTPINMODES5", 3344, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES6 = Field.create("LUADIGITALINPUTPINMODES6", 3345, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES7 = Field.create("LUADIGITALINPUTPINMODES7", 3346, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field LUADIGITALINPUTPINMODES8 = Field.create("LUADIGITALINPUTPINMODES8", 3347, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field CUSTOMSENTTPSMAX = Field.create("CUSTOMSENTTPSMAX", 3348, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field KLINEBAUDRATE = Field.create("KLINEBAUDRATE", 3350, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final String[] CanGpioType = {"None", "DRT protocol", "MS protocol"};
	public static final Field CANGPIOTYPE = Field.create("CANGPIOTYPE", 3352, FieldType.INT8, CanGpioType).setScale(1.0).setBaseOffset(0);
	public static final String[] UiMode = {"Full", "INVALID", "Tuning"};
	public static final Field UIMODE = Field.create("UIMODE", 3353, FieldType.INT8, UiMode).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPPEAKPOS = Field.create("HPFPPEAKPOS", 3354, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field KLINEPERIODUS = Field.create("KLINEPERIODUS", 3356, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field RPMSOFTLIMITWINDOWSIZE = Field.create("RPMSOFTLIMITWINDOWSIZE", 3358, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field RPMSOFTLIMITTIMINGRETARD = Field.create("RPMSOFTLIMITTIMINGRETARD", 3359, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field RPMSOFTLIMITFUELADDED = Field.create("RPMSOFTLIMITFUELADDED", 3360, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field RPMHARDLIMITHYST = Field.create("RPMHARDLIMITHYST", 3361, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field BENCHTESTOFFTIME = Field.create("BENCHTESTOFFTIME", 3362, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCUTPRESSUREHYST = Field.create("BOOSTCUTPRESSUREHYST", 3364, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field GEARBASEDOPENLOOPBOOSTADDER = Field.create("GEARBASEDOPENLOOPBOOSTADDER", 3365, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3375 = Field.create("ALIGNMENTFILL_AT_3375", 3375, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BENCHTESTCOUNT = Field.create("BENCHTESTCOUNT", 3376, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field IACBYTPSHOLDTIME = Field.create("IACBYTPSHOLDTIME", 3380, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field IACBYTPSDECAYTIME = Field.create("IACBYTPSDECAYTIME", 3381, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field TCU_RANGEINPUT1 = Field.create("TCU_RANGEINPUT1", 3382, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUT2 = Field.create("TCU_RANGEINPUT2", 3384, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUT3 = Field.create("TCU_RANGEINPUT3", 3386, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUT4 = Field.create("TCU_RANGEINPUT4", 3388, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUT5 = Field.create("TCU_RANGEINPUT5", 3390, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUT6 = Field.create("TCU_RANGEINPUT6", 3392, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUTMODE1 = Field.create("TCU_RANGEINPUTMODE1", 3394, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUTMODE2 = Field.create("TCU_RANGEINPUTMODE2", 3395, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUTMODE3 = Field.create("TCU_RANGEINPUTMODE3", 3396, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUTMODE4 = Field.create("TCU_RANGEINPUTMODE4", 3397, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUTMODE5 = Field.create("TCU_RANGEINPUTMODE5", 3398, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEINPUTMODE6 = Field.create("TCU_RANGEINPUTMODE6", 3399, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field CANVSSSCALING = Field.create("CANVSSSCALING", 3400, FieldType.INT16).setScale(1.0E-4).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3402 = Field.create("ALIGNMENTFILL_AT_3402", 3402, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_TEMPC_1 = Field.create("OILTEMPSENSOR_TEMPC_1", 3404, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_TEMPC_2 = Field.create("OILTEMPSENSOR_TEMPC_2", 3408, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_TEMPC_3 = Field.create("OILTEMPSENSOR_TEMPC_3", 3412, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_RESISTANCE_1 = Field.create("OILTEMPSENSOR_RESISTANCE_1", 3416, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_RESISTANCE_2 = Field.create("OILTEMPSENSOR_RESISTANCE_2", 3420, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_RESISTANCE_3 = Field.create("OILTEMPSENSOR_RESISTANCE_3", 3424, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_BIAS_RESISTOR = Field.create("OILTEMPSENSOR_BIAS_RESISTOR", 3428, FieldType.FLOAT).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_ADCCHANNEL = Field.create("OILTEMPSENSOR_ADCCHANNEL", 3432, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field OILTEMPSENSOR_ALIGNMENTFILL_AT_29 = Field.create("OILTEMPSENSOR_ALIGNMENTFILL_AT_29", 3433, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_TEMPC_1 = Field.create("FUELTEMPSENSOR_TEMPC_1", 3436, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_TEMPC_2 = Field.create("FUELTEMPSENSOR_TEMPC_2", 3440, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_TEMPC_3 = Field.create("FUELTEMPSENSOR_TEMPC_3", 3444, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_RESISTANCE_1 = Field.create("FUELTEMPSENSOR_RESISTANCE_1", 3448, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_RESISTANCE_2 = Field.create("FUELTEMPSENSOR_RESISTANCE_2", 3452, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_RESISTANCE_3 = Field.create("FUELTEMPSENSOR_RESISTANCE_3", 3456, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_BIAS_RESISTOR = Field.create("FUELTEMPSENSOR_BIAS_RESISTOR", 3460, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_ADCCHANNEL = Field.create("FUELTEMPSENSOR_ADCCHANNEL", 3464, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field FUELTEMPSENSOR_ALIGNMENTFILL_AT_29 = Field.create("FUELTEMPSENSOR_ALIGNMENTFILL_AT_29", 3465, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_TEMPC_1 = Field.create("AMBIENTTEMPSENSOR_TEMPC_1", 3468, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_TEMPC_2 = Field.create("AMBIENTTEMPSENSOR_TEMPC_2", 3472, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_TEMPC_3 = Field.create("AMBIENTTEMPSENSOR_TEMPC_3", 3476, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_RESISTANCE_1 = Field.create("AMBIENTTEMPSENSOR_RESISTANCE_1", 3480, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_RESISTANCE_2 = Field.create("AMBIENTTEMPSENSOR_RESISTANCE_2", 3484, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_RESISTANCE_3 = Field.create("AMBIENTTEMPSENSOR_RESISTANCE_3", 3488, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_BIAS_RESISTOR = Field.create("AMBIENTTEMPSENSOR_BIAS_RESISTOR", 3492, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_ADCCHANNEL = Field.create("AMBIENTTEMPSENSOR_ADCCHANNEL", 3496, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AMBIENTTEMPSENSOR_ALIGNMENTFILL_AT_29 = Field.create("AMBIENTTEMPSENSOR_ALIGNMENTFILL_AT_29", 3497, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_TEMPC_1 = Field.create("COMPRESSORDISCHARGETEMPERATURE_TEMPC_1", 3500, FieldType.FLOAT).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_TEMPC_2 = Field.create("COMPRESSORDISCHARGETEMPERATURE_TEMPC_2", 3504, FieldType.FLOAT).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_TEMPC_3 = Field.create("COMPRESSORDISCHARGETEMPERATURE_TEMPC_3", 3508, FieldType.FLOAT).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_1 = Field.create("COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_1", 3512, FieldType.FLOAT).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_2 = Field.create("COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_2", 3516, FieldType.FLOAT).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_3 = Field.create("COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_3", 3520, FieldType.FLOAT).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_BIAS_RESISTOR = Field.create("COMPRESSORDISCHARGETEMPERATURE_BIAS_RESISTOR", 3524, FieldType.FLOAT).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_ADCCHANNEL = Field.create("COMPRESSORDISCHARGETEMPERATURE_ADCCHANNEL", 3528, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGETEMPERATURE_ALIGNMENTFILL_AT_29 = Field.create("COMPRESSORDISCHARGETEMPERATURE_ALIGNMENTFILL_AT_29", 3529, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field THROTTLEINLETPRESSURECHANNEL = Field.create("THROTTLEINLETPRESSURECHANNEL", 3532, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field COMPRESSORDISCHARGEPRESSURECHANNEL = Field.create("COMPRESSORDISCHARGEPRESSURECHANNEL", 3533, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field DACOUTPUTPINS1 = Field.create("DACOUTPUTPINS1", 3534, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field DACOUTPUTPINS2 = Field.create("DACOUTPUTPINS2", 3536, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPEEDOMETEROUTPUTPIN = Field.create("SPEEDOMETEROUTPUTPIN", 3538, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPEEDOMETERPULSEPERKM = Field.create("SPEEDOMETERPULSEPERKM", 3540, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SIMULATORCAMPOSITION1 = Field.create("SIMULATORCAMPOSITION1", 3542, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SIMULATORCAMPOSITION2 = Field.create("SIMULATORCAMPOSITION2", 3543, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SIMULATORCAMPOSITION3 = Field.create("SIMULATORCAMPOSITION3", 3544, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SIMULATORCAMPOSITION4 = Field.create("SIMULATORCAMPOSITION4", 3545, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNKEYADCCHANNEL = Field.create("IGNKEYADCCHANNEL", 3546, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI6MISOMODE = Field.create("SPI6MISOMODE", 3547, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDEFROM1 = Field.create("TRIGGERVVTGAPOVERRIDEFROM1", 3548, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDEFROM2 = Field.create("TRIGGERVVTGAPOVERRIDEFROM2", 3552, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDEFROM3 = Field.create("TRIGGERVVTGAPOVERRIDEFROM3", 3556, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDEFROM4 = Field.create("TRIGGERVVTGAPOVERRIDEFROM4", 3560, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDETO1 = Field.create("TRIGGERVVTGAPOVERRIDETO1", 3564, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDETO2 = Field.create("TRIGGERVVTGAPOVERRIDETO2", 3568, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDETO3 = Field.create("TRIGGERVVTGAPOVERRIDETO3", 3572, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRIGGERVVTGAPOVERRIDETO4 = Field.create("TRIGGERVVTGAPOVERRIDETO4", 3576, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRACTIONCONTROLETBDROP = Field.create("TRACTIONCONTROLETBDROP", 3580, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXINJECTORDUTYINSTANT = Field.create("MAXINJECTORDUTYINSTANT", 3616, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXINJECTORDUTYSUSTAINED = Field.create("MAXINJECTORDUTYSUSTAINED", 3617, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXINJECTORDUTYSUSTAINEDTIMEOUT = Field.create("MAXINJECTORDUTYSUSTAINEDTIMEOUT", 3618, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3619 = Field.create("ALIGNMENTFILL_AT_3619", 3619, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE21 = Field.create("INJECTIONPINSSTAGE21", 3620, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE22 = Field.create("INJECTIONPINSSTAGE22", 3622, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE23 = Field.create("INJECTIONPINSSTAGE23", 3624, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE24 = Field.create("INJECTIONPINSSTAGE24", 3626, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE25 = Field.create("INJECTIONPINSSTAGE25", 3628, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE26 = Field.create("INJECTIONPINSSTAGE26", 3630, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE27 = Field.create("INJECTIONPINSSTAGE27", 3632, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE28 = Field.create("INJECTIONPINSSTAGE28", 3634, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE29 = Field.create("INJECTIONPINSSTAGE29", 3636, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE210 = Field.create("INJECTIONPINSSTAGE210", 3638, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE211 = Field.create("INJECTIONPINSSTAGE211", 3640, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTIONPINSSTAGE212 = Field.create("INJECTIONPINSSTAGE212", 3642, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field TRACTIONCONTROLTIMINGDROP = Field.create("TRACTIONCONTROLTIMINGDROP", 3644, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TRACTIONCONTROLIGNITIONSKIP = Field.create("TRACTIONCONTROLIGNITIONSKIP", 3680, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXSPEED1MULTIPLIER = Field.create("AUXSPEED1MULTIPLIER", 3716, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BRAKEMEANEFFECTIVEPRESSUREDIFFERENTIAL = Field.create("BRAKEMEANEFFECTIVEPRESSUREDIFFERENTIAL", 3720, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SPI4MOSIPIN = Field.create("SPI4MOSIPIN", 3724, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI4MISOPIN = Field.create("SPI4MISOPIN", 3726, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI4SCKPIN = Field.create("SPI4SCKPIN", 3728, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI5MOSIPIN = Field.create("SPI5MOSIPIN", 3730, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI5MISOPIN = Field.create("SPI5MISOPIN", 3732, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI5SCKPIN = Field.create("SPI5SCKPIN", 3734, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI6MOSIPIN = Field.create("SPI6MOSIPIN", 3736, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI6MISOPIN = Field.create("SPI6MISOPIN", 3738, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI6SCKPIN = Field.create("SPI6SCKPIN", 3740, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field SPI4SCKMODE = Field.create("SPI4SCKMODE", 3742, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI4MOSIMODE = Field.create("SPI4MOSIMODE", 3743, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI4MISOMODE = Field.create("SPI4MISOMODE", 3744, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI5SCKMODE = Field.create("SPI5SCKMODE", 3745, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI5MOSIMODE = Field.create("SPI5MOSIMODE", 3746, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI5MISOMODE = Field.create("SPI5MISOMODE", 3747, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI6SCKMODE = Field.create("SPI6SCKMODE", 3748, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field SPI6MOSIMODE = Field.create("SPI6MOSIMODE", 3749, FieldType.INT8, pin_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3750 = Field.create("ALIGNMENTFILL_AT_3750", 3750, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TUNERSTUDIOSERIALSPEED = Field.create("TUNERSTUDIOSERIALSPEED", 3752, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field CAMSIMULATORPIN = Field.create("CAMSIMULATORPIN", 3756, FieldType.INT16, Gpio).setScale(1.0).setBaseOffset(0);
	public static final Field CAMSIMULATORPINMODE = Field.create("CAMSIMULATORPINMODE", 3758, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3759 = Field.create("ALIGNMENTFILL_AT_3759", 3759, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ANOTHERCITEST = Field.create("ANOTHERCITEST", 3760, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field DEVICE_UID1 = Field.create("DEVICE_UID1", 3764, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field DEVICE_UID2 = Field.create("DEVICE_UID2", 3768, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field DEVICE_UID3 = Field.create("DEVICE_UID3", 3772, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEANALOGINPUT1 = Field.create("TCU_RANGEANALOGINPUT1", 3776, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEANALOGINPUT2 = Field.create("TCU_RANGEANALOGINPUT2", 3777, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEANALOGINPUT3 = Field.create("TCU_RANGEANALOGINPUT3", 3778, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEANALOGINPUT4 = Field.create("TCU_RANGEANALOGINPUT4", 3779, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEANALOGINPUT5 = Field.create("TCU_RANGEANALOGINPUT5", 3780, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEANALOGINPUT6 = Field.create("TCU_RANGEANALOGINPUT6", 3781, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3782 = Field.create("ALIGNMENTFILL_AT_3782", 3782, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGESENSORBIASRESISTOR = Field.create("TCU_RANGESENSORBIASRESISTOR", 3784, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] MsIoBoxId = {"Off", "ID1 (0x200)", "ID2 (0x220)", "ID3 (0x240)"};
	public static final Field MSIOBOX0_ID = Field.create("MSIOBOX0_ID", 3788, FieldType.INT8, MsIoBoxId).setScale(1.0).setBaseOffset(0);
	public static final String[] MsIoBoxVss = {"Off", "VR speed in (1, 2)", "Hall speed in (3, 4)", "All (1, 2, 3, 4)"};
	public static final Field MSIOBOX0_VSS = Field.create("MSIOBOX0_VSS", 3789, FieldType.INT8, MsIoBoxVss).setScale(1.0).setBaseOffset(0);
	public static final Field MSIOBOX0_ALIGNMENTFILL_AT_2 = Field.create("MSIOBOX0_ALIGNMENTFILL_AT_2", 3790, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MC33810NOMI = Field.create("MC33810NOMI", 3792, FieldType.INT8).setScale(0.25).setBaseOffset(0);
	public static final Field MC33810MAXI = Field.create("MC33810MAXI", 3793, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3794 = Field.create("ALIGNMENTFILL_AT_3794", 3794, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACPRESSURE_HWCHANNEL = Field.create("ACPRESSURE_HWCHANNEL", 3796, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field ACPRESSURE_ALIGNMENTFILL_AT_1 = Field.create("ACPRESSURE_ALIGNMENTFILL_AT_1", 3797, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ACPRESSURE_V1 = Field.create("ACPRESSURE_V1", 3800, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ACPRESSURE_VALUE1 = Field.create("ACPRESSURE_VALUE1", 3804, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ACPRESSURE_V2 = Field.create("ACPRESSURE_V2", 3808, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ACPRESSURE_VALUE2 = Field.create("ACPRESSURE_VALUE2", 3812, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MINACPRESSURE = Field.create("MINACPRESSURE", 3816, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MAXACPRESSURE = Field.create("MAXACPRESSURE", 3818, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field MINIMUMOILPRESSURETIMEOUT = Field.create("MINIMUMOILPRESSURETIMEOUT", 3820, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3821 = Field.create("ALIGNMENTFILL_AT_3821", 3821, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR3_HWCHANNEL = Field.create("AUXLINEAR3_HWCHANNEL", 3824, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR3_ALIGNMENTFILL_AT_1 = Field.create("AUXLINEAR3_ALIGNMENTFILL_AT_1", 3825, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR3_V1 = Field.create("AUXLINEAR3_V1", 3828, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR3_VALUE1 = Field.create("AUXLINEAR3_VALUE1", 3832, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR3_V2 = Field.create("AUXLINEAR3_V2", 3836, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR3_VALUE2 = Field.create("AUXLINEAR3_VALUE2", 3840, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR4_HWCHANNEL = Field.create("AUXLINEAR4_HWCHANNEL", 3844, FieldType.INT8, adc_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR4_ALIGNMENTFILL_AT_1 = Field.create("AUXLINEAR4_ALIGNMENTFILL_AT_1", 3845, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field AUXLINEAR4_V1 = Field.create("AUXLINEAR4_V1", 3848, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR4_VALUE1 = Field.create("AUXLINEAR4_VALUE1", 3852, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR4_V2 = Field.create("AUXLINEAR4_V2", 3856, FieldType.FLOAT).setBaseOffset(0);
	public static final Field AUXLINEAR4_VALUE2 = Field.create("AUXLINEAR4_VALUE2", 3860, FieldType.FLOAT).setBaseOffset(0);
	public static final Field KNOCKSUPPRESSMINTPS = Field.create("KNOCKSUPPRESSMINTPS", 3864, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field KNOCKFUELTRIMAGGRESSION = Field.create("KNOCKFUELTRIMAGGRESSION", 3865, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field KNOCKFUELTRIMREAPPLYRATE = Field.create("KNOCKFUELTRIMREAPPLYRATE", 3866, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field KNOCKFUELTRIM = Field.create("KNOCKFUELTRIM", 3867, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field KNOCKSPECTRUMSENSITIVITY = Field.create("KNOCKSPECTRUMSENSITIVITY", 3868, FieldType.FLOAT).setBaseOffset(0);
	public static final Field KNOCKFREQUENCY = Field.create("KNOCKFREQUENCY", 3872, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SECONDARYINJECTORCOMPENSATIONMODE = Field.create("SECONDARYINJECTORCOMPENSATIONMODE", 3876, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3877 = Field.create("ALIGNMENTFILL_AT_3877", 3877, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SECONDARYINJECTORFUELREFERENCEPRESSURE = Field.create("SECONDARYINJECTORFUELREFERENCEPRESSURE", 3880, FieldType.FLOAT).setBaseOffset(0);
	public static final String[] SentInput = {"None", "SENT input 1", "SENT input 2", "SENT input 3", "SENT input 4", "SENT input 5", "SENT input 6", "SENT input 7"};
	public static final Field ETBSENTINPUT = Field.create("ETBSENTINPUT", 3884, FieldType.INT8, SentInput).setScale(1.0).setBaseOffset(0);
	public static final Field FUELHIGHPRESSURESENTINPUT = Field.create("FUELHIGHPRESSURESENTINPUT", 3885, FieldType.INT8, SentInput).setScale(1.0).setBaseOffset(0);
	public static final String[] SentFuelHighPressureType = {"None", "GM type", "Custom"};
	public static final Field FUELHIGHPRESSURESENTTYPE = Field.create("FUELHIGHPRESSURESENTTYPE", 3886, FieldType.INT8, SentFuelHighPressureType).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3887 = Field.create("ALIGNMENTFILL_AT_3887", 3887, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSCONTROLENABLED = Field.create("NITROUSCONTROLENABLED", 3888, FieldType.BIT, 0).setBaseOffset(0);
	public static final String[] nitrous_arming_method_e = {"Digital Switch Input", "Lua Gauge"};
	public static final Field NITROUSCONTROLARMINGMETHOD = Field.create("NITROUSCONTROLARMINGMETHOD", 3892, FieldType.INT8, nitrous_arming_method_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3893 = Field.create("ALIGNMENTFILL_AT_3893", 3893, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSCONTROLTRIGGERPIN = Field.create("NITROUSCONTROLTRIGGERPIN", 3894, FieldType.INT16, switch_input_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSCONTROLTRIGGERPINMODE = Field.create("NITROUSCONTROLTRIGGERPINMODE", 3896, FieldType.INT8, pin_input_mode_e).setScale(1.0).setBaseOffset(0);
	public static final String[] lua_gauge_e = {"Lua Gauge 1", "Lua Gauge 2", "Lua Gauge 3", "Lua Gauge 4", "Lua Gauge 5", "Lua Gauge 6", "Lua Gauge 7", "Lua Gauge 8"};
	public static final Field NITROUSLUAGAUGE = Field.create("NITROUSLUAGAUGE", 3897, FieldType.INT8, lua_gauge_e).setScale(1.0).setBaseOffset(0);
	public static final String[] lua_gauge_meaning_e = {"Lower Bound", "Upper Bound"};
	public static final Field NITROUSLUAGAUGEMEANING = Field.create("NITROUSLUAGAUGEMEANING", 3898, FieldType.INT8, lua_gauge_meaning_e).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3899 = Field.create("ALIGNMENTFILL_AT_3899", 3899, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSLUAGAUGEARMINGVALUE = Field.create("NITROUSLUAGAUGEARMINGVALUE", 3900, FieldType.FLOAT).setBaseOffset(0);
	public static final Field NITROUSMINIMUMTPS = Field.create("NITROUSMINIMUMTPS", 3904, FieldType.INT).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSMINIMUMCLT = Field.create("NITROUSMINIMUMCLT", 3908, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3909 = Field.create("ALIGNMENTFILL_AT_3909", 3909, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSMAXIMUMMAP = Field.create("NITROUSMAXIMUMMAP", 3910, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSMAXIMUMAFR = Field.create("NITROUSMAXIMUMAFR", 3912, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3913 = Field.create("ALIGNMENTFILL_AT_3913", 3913, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSACTIVATIONRPM = Field.create("NITROUSACTIVATIONRPM", 3914, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSDEACTIVATIONRPM = Field.create("NITROUSDEACTIVATIONRPM", 3916, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSDEACTIVATIONRPMWINDOW = Field.create("NITROUSDEACTIVATIONRPMWINDOW", 3918, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DFCORETARDDEG = Field.create("DFCORETARDDEG", 3920, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DFCORETARDRAMPINTIME = Field.create("DFCORETARDRAMPINTIME", 3921, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field NITROUSRELAYPIN = Field.create("NITROUSRELAYPIN", 3922, FieldType.INT16, output_pin_e).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSRELAYPINMODE = Field.create("NITROUSRELAYPINMODE", 3924, FieldType.INT8, pin_output_mode_e).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSFUELADDERPERCENT = Field.create("NITROUSFUELADDERPERCENT", 3925, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3926 = Field.create("ALIGNMENTFILL_AT_3926", 3926, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field NITROUSIGNITIONRETARD = Field.create("NITROUSIGNITIONRETARD", 3928, FieldType.FLOAT).setBaseOffset(0);
	public static final Field NITROUSMINIMUMVEHICLESPEED = Field.create("NITROUSMINIMUMVEHICLESPEED", 3932, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_3934 = Field.create("ALIGNMENTFILL_AT_3934", 3934, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field FUELLEVELAVERAGINGALPHA = Field.create("FUELLEVELAVERAGINGALPHA", 3936, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELLEVELUPDATEPERIODSEC = Field.create("FUELLEVELUPDATEPERIODSEC", 3940, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELLEVELLOWTHRESHOLDVOLTAGE = Field.create("FUELLEVELLOWTHRESHOLDVOLTAGE", 3944, FieldType.FLOAT).setBaseOffset(0);
	public static final Field FUELLEVELHIGHTHRESHOLDVOLTAGE = Field.create("FUELLEVELHIGHTHRESHOLDVOLTAGE", 3948, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAPEXPAVERAGEAFR = Field.create("MAPEXPAVERAGEAFR", 3952, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SPARKHARDWARELATENCYCORRECTION = Field.create("SPARKHARDWARELATENCYCORRECTION", 3956, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXOILPRESSURETIMEOUT = Field.create("MAXOILPRESSURETIMEOUT", 3957, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_4010 = Field.create("ALIGNMENTFILL_AT_4010", 4010, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field POSTCRANKINGFACTOR = Field.create("POSTCRANKINGFACTOR", 4012, FieldType.FLOAT).setBaseOffset(0);
	public static final Field POSTCRANKINGDURATIONBINS = Field.create("POSTCRANKINGDURATIONBINS", 4156, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field POSTCRANKINGCLTBINS = Field.create("POSTCRANKINGCLTBINS", 4168, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ETBBIASBINS = Field.create("ETBBIASBINS", 4180, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ETBBIASVALUES = Field.create("ETBBIASVALUES", 4212, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IACPIDMULTTABLE = Field.create("IACPIDMULTTABLE", 4244, FieldType.INT8).setScale(0.05).setBaseOffset(0);
	public static final Field IACPIDMULTLOADBINS = Field.create("IACPIDMULTLOADBINS", 4308, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IACPIDMULTRPMBINS = Field.create("IACPIDMULTRPMBINS", 4316, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field SPARKDWELLRPMBINS = Field.create("SPARKDWELLRPMBINS", 4324, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SPARKDWELLVALUES = Field.create("SPARKDWELLVALUES", 4340, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field CLTIDLERPMBINS = Field.create("CLTIDLERPMBINS", 4356, FieldType.INT8).setScale(2.0).setBaseOffset(0);
	public static final Field CLTIDLERPM = Field.create("CLTIDLERPM", 4372, FieldType.INT8).setScale(20.0).setBaseOffset(0);
	public static final Field IGNITIONCLTCORRTABLE = Field.create("IGNITIONCLTCORRTABLE", 4388, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNITIONCLTCORRTEMPBINS = Field.create("IGNITIONCLTCORRTEMPBINS", 4438, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONCLTCORRLOADBINS = Field.create("IGNITIONCLTCORRLOADBINS", 4443, FieldType.INT8).setScale(5.0).setBaseOffset(0);
	public static final Field SCRIPTCURVE1BINS = Field.create("SCRIPTCURVE1BINS", 4448, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE1 = Field.create("SCRIPTCURVE1", 4512, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE2BINS = Field.create("SCRIPTCURVE2BINS", 4576, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE2 = Field.create("SCRIPTCURVE2", 4640, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE3BINS = Field.create("SCRIPTCURVE3BINS", 4704, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE3 = Field.create("SCRIPTCURVE3", 4736, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE4BINS = Field.create("SCRIPTCURVE4BINS", 4768, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE4 = Field.create("SCRIPTCURVE4", 4800, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE5BINS = Field.create("SCRIPTCURVE5BINS", 4832, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE5 = Field.create("SCRIPTCURVE5", 4864, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE6BINS = Field.create("SCRIPTCURVE6BINS", 4896, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTCURVE6 = Field.create("SCRIPTCURVE6", 4928, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BAROCORRPRESSUREBINS = Field.create("BAROCORRPRESSUREBINS", 4960, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BAROCORRRPMBINS = Field.create("BAROCORRRPMBINS", 4976, FieldType.FLOAT).setBaseOffset(0);
	public static final Field BAROCORRTABLE = Field.create("BAROCORRTABLE", 4992, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKINGTPSCOEF = Field.create("CRANKINGTPSCOEF", 5056, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKINGTPSBINS = Field.create("CRANKINGTPSBINS", 5088, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKINGADVANCEBINS = Field.create("CRANKINGADVANCEBINS", 5120, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CRANKINGADVANCE = Field.create("CRANKINGADVANCE", 5128, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field IACCOASTINGRPMBINS = Field.create("IACCOASTINGRPMBINS", 5136, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field IACCOASTING = Field.create("IACCOASTING", 5152, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTTABLEOPENLOOP = Field.create("BOOSTTABLEOPENLOOP", 5168, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTRPMBINS = Field.create("BOOSTRPMBINS", 5232, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPLOADBINS = Field.create("BOOSTOPENLOOPLOADBINS", 5240, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTTABLECLOSEDLOOP = Field.create("BOOSTTABLECLOSEDLOOP", 5256, FieldType.INT8).setScale(2.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPLOADBINS = Field.create("BOOSTCLOSEDLOOPLOADBINS", 5320, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field PEDALTOTPSTABLE = Field.create("PEDALTOTPSTABLE", 5336, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field PEDALTOTPSPEDALBINS = Field.create("PEDALTOTPSPEDALBINS", 5400, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field PEDALTOTPSRPMBINS = Field.create("PEDALTOTPSRPMBINS", 5408, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field CLTCRANKINGCORRBINS = Field.create("CLTCRANKINGCORRBINS", 5416, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLTCRANKINGCORR = Field.create("CLTCRANKINGCORR", 5448, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLEADVANCEBINS = Field.create("IDLEADVANCEBINS", 5480, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field IDLEADVANCE = Field.create("IDLEADVANCE", 5488, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IDLEVERPMBINS = Field.create("IDLEVERPMBINS", 5520, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field IDLEVELOADBINS = Field.create("IDLEVELOADBINS", 5524, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IDLEVETABLE = Field.create("IDLEVETABLE", 5528, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field LUASCRIPT = Field.create("LUASCRIPT", 5560, 8000, FieldType.STRING).setScale(1.0).setBaseOffset(0);
	public static final Field CLTFUELCORRBINS = Field.create("CLTFUELCORRBINS", 13560, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLTFUELCORR = Field.create("CLTFUELCORR", 13624, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IATFUELCORRBINS = Field.create("IATFUELCORRBINS", 13688, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IATFUELCORR = Field.create("IATFUELCORR", 13752, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKINGFUELCOEF = Field.create("CRANKINGFUELCOEF", 13816, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKINGFUELBINS = Field.create("CRANKINGFUELBINS", 13848, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKINGCYCLEBINS = Field.create("CRANKINGCYCLEBINS", 13880, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CRANKINGCYCLEFUELCLTBINS = Field.create("CRANKINGCYCLEFUELCLTBINS", 13912, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CRANKINGCYCLEFUELCOEF = Field.create("CRANKINGCYCLEFUELCOEF", 13920, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLTIDLECORRBINS = Field.create("CLTIDLECORRBINS", 14048, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLTIDLECORR = Field.create("CLTIDLECORR", 14112, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAFDECODING = Field.create("MAFDECODING", 14176, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MAFDECODINGBINS = Field.create("MAFDECODINGBINS", 14304, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IGNITIONIATCORRTABLE = Field.create("IGNITIONIATCORRTABLE", 14432, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNITIONIATCORRTEMPBINS = Field.create("IGNITIONIATCORRTEMPBINS", 14560, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONIATCORRLOADBINS = Field.create("IGNITIONIATCORRLOADBINS", 14568, FieldType.INT8).setScale(5.0).setBaseOffset(0);
	public static final Field INJECTIONPHASE = Field.create("INJECTIONPHASE", 14576, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field INJPHASELOADBINS = Field.create("INJPHASELOADBINS", 15088, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field INJPHASERPMBINS = Field.create("INJPHASERPMBINS", 15120, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TCUSOLENOIDTABLE = Field.create("TCUSOLENOIDTABLE", 15152, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAPESTIMATETABLE = Field.create("MAPESTIMATETABLE", 15212, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field MAPESTIMATETPSBINS = Field.create("MAPESTIMATETPSBINS", 15724, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field MAPESTIMATERPMBINS = Field.create("MAPESTIMATERPMBINS", 15756, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTABLE1 = Field.create("VVTTABLE1", 15788, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTABLE1LOADBINS = Field.create("VVTTABLE1LOADBINS", 15852, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTABLE1RPMBINS = Field.create("VVTTABLE1RPMBINS", 15868, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTABLE2 = Field.create("VVTTABLE2", 15884, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTABLE2LOADBINS = Field.create("VVTTABLE2LOADBINS", 15948, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VVTTABLE2RPMBINS = Field.create("VVTTABLE2RPMBINS", 15964, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONTABLE = Field.create("IGNITIONTABLE", 15980, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNITIONLOADBINS = Field.create("IGNITIONLOADBINS", 16492, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNITIONRPMBINS = Field.create("IGNITIONRPMBINS", 16524, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VETABLE = Field.create("VETABLE", 16556, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VELOADBINS = Field.create("VELOADBINS", 17068, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VERPMBINS = Field.create("VERPMBINS", 17100, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDATABLE = Field.create("LAMBDATABLE", 17132, FieldType.INT8).setScale(0.006802721088435374).setBaseOffset(0);
	public static final Field LAMBDALOADBINS = Field.create("LAMBDALOADBINS", 17388, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDARPMBINS = Field.create("LAMBDARPMBINS", 17420, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TPSTPSACCELTABLE = Field.create("TPSTPSACCELTABLE", 17452, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TPSTPSACCELFROMRPMBINS = Field.create("TPSTPSACCELFROMRPMBINS", 17708, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TPSTPSACCELTORPMBINS = Field.create("TPSTPSACCELTORPMBINS", 17740, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTTABLE1 = Field.create("SCRIPTTABLE1", 17772, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTTABLE1LOADBINS = Field.create("SCRIPTTABLE1LOADBINS", 18028, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE1RPMBINS = Field.create("SCRIPTTABLE1RPMBINS", 18044, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE2 = Field.create("SCRIPTTABLE2", 18060, FieldType.FLOAT).setBaseOffset(0);
	public static final Field SCRIPTTABLE2LOADBINS = Field.create("SCRIPTTABLE2LOADBINS", 18316, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE2RPMBINS = Field.create("SCRIPTTABLE2RPMBINS", 18332, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE3 = Field.create("SCRIPTTABLE3", 18348, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE3LOADBINS = Field.create("SCRIPTTABLE3LOADBINS", 18412, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE3RPMBINS = Field.create("SCRIPTTABLE3RPMBINS", 18428, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE4 = Field.create("SCRIPTTABLE4", 18444, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE4LOADBINS = Field.create("SCRIPTTABLE4LOADBINS", 18524, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field SCRIPTTABLE4RPMBINS = Field.create("SCRIPTTABLE4RPMBINS", 18540, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNTRIMLOADBINS = Field.create("IGNTRIMLOADBINS", 18560, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNTRIMRPMBINS = Field.create("IGNTRIMRPMBINS", 18568, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNTRIMS1_TABLE = Field.create("IGNTRIMS1_TABLE", 18576, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS2_TABLE = Field.create("IGNTRIMS2_TABLE", 18592, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS3_TABLE = Field.create("IGNTRIMS3_TABLE", 18608, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS4_TABLE = Field.create("IGNTRIMS4_TABLE", 18624, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS5_TABLE = Field.create("IGNTRIMS5_TABLE", 18640, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS6_TABLE = Field.create("IGNTRIMS6_TABLE", 18656, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS7_TABLE = Field.create("IGNTRIMS7_TABLE", 18672, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS8_TABLE = Field.create("IGNTRIMS8_TABLE", 18688, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS9_TABLE = Field.create("IGNTRIMS9_TABLE", 18704, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS10_TABLE = Field.create("IGNTRIMS10_TABLE", 18720, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS11_TABLE = Field.create("IGNTRIMS11_TABLE", 18736, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field IGNTRIMS12_TABLE = Field.create("IGNTRIMS12_TABLE", 18752, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMLOADBINS = Field.create("FUELTRIMLOADBINS", 18768, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field FUELTRIMRPMBINS = Field.create("FUELTRIMRPMBINS", 18776, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field FUELTRIMS1_TABLE = Field.create("FUELTRIMS1_TABLE", 18784, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS2_TABLE = Field.create("FUELTRIMS2_TABLE", 18800, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS3_TABLE = Field.create("FUELTRIMS3_TABLE", 18816, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS4_TABLE = Field.create("FUELTRIMS4_TABLE", 18832, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS5_TABLE = Field.create("FUELTRIMS5_TABLE", 18848, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS6_TABLE = Field.create("FUELTRIMS6_TABLE", 18864, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS7_TABLE = Field.create("FUELTRIMS7_TABLE", 18880, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS8_TABLE = Field.create("FUELTRIMS8_TABLE", 18896, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS9_TABLE = Field.create("FUELTRIMS9_TABLE", 18912, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS10_TABLE = Field.create("FUELTRIMS10_TABLE", 18928, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS11_TABLE = Field.create("FUELTRIMS11_TABLE", 18944, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field FUELTRIMS12_TABLE = Field.create("FUELTRIMS12_TABLE", 18960, FieldType.INT8).setScale(0.2).setBaseOffset(0);
	public static final Field CRANKINGFUELCOEFE100 = Field.create("CRANKINGFUELCOEFE100", 18976, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field TCU_PCAIRMASSBINS = Field.create("TCU_PCAIRMASSBINS", 18992, FieldType.INT8).setScale(5.0).setBaseOffset(0);
	public static final Field TCU_PCVALSR = Field.create("TCU_PCVALSR", 19000, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALSN = Field.create("TCU_PCVALSN", 19008, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS1 = Field.create("TCU_PCVALS1", 19016, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS2 = Field.create("TCU_PCVALS2", 19024, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS3 = Field.create("TCU_PCVALS3", 19032, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS4 = Field.create("TCU_PCVALS4", 19040, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS12 = Field.create("TCU_PCVALS12", 19048, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS23 = Field.create("TCU_PCVALS23", 19056, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS34 = Field.create("TCU_PCVALS34", 19064, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS21 = Field.create("TCU_PCVALS21", 19072, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS32 = Field.create("TCU_PCVALS32", 19080, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_PCVALS43 = Field.create("TCU_PCVALS43", 19088, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_TCCTPSBINS = Field.create("TCU_TCCTPSBINS", 19096, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_TCCLOCKSPEED = Field.create("TCU_TCCLOCKSPEED", 19104, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_TCCUNLOCKSPEED = Field.create("TCU_TCCUNLOCKSPEED", 19112, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_32SPEEDBINS = Field.create("TCU_32SPEEDBINS", 19120, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_32VALS = Field.create("TCU_32VALS", 19128, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field THROTTLE2TRIMTABLE = Field.create("THROTTLE2TRIMTABLE", 19136, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field THROTTLE2TRIMTPSBINS = Field.create("THROTTLE2TRIMTPSBINS", 19172, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field THROTTLE2TRIMRPMBINS = Field.create("THROTTLE2TRIMRPMBINS", 19178, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field MAXKNOCKRETARDTABLE = Field.create("MAXKNOCKRETARDTABLE", 19184, FieldType.INT8).setScale(0.25).setBaseOffset(0);
	public static final Field MAXKNOCKRETARDLOADBINS = Field.create("MAXKNOCKRETARDLOADBINS", 19220, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field MAXKNOCKRETARDRPMBINS = Field.create("MAXKNOCKRETARDRPMBINS", 19226, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field ALSTIMINGRETARDTABLE = Field.create("ALSTIMINGRETARDTABLE", 19232, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field ALSIGNRETARDLOADBINS = Field.create("ALSIGNRETARDLOADBINS", 19264, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSIGNRETARDRPMBINS = Field.create("ALSIGNRETARDRPMBINS", 19272, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSFUELADJUSTMENT = Field.create("ALSFUELADJUSTMENT", 19280, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field ALSFUELADJUSTMENTLOADBINS = Field.create("ALSFUELADJUSTMENTLOADBINS", 19312, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSFUELADJUSTMENTRPMBINS = Field.create("ALSFUELADJUSTMENTRPMBINS", 19320, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSIGNSKIPTABLE = Field.create("ALSIGNSKIPTABLE", 19328, FieldType.INT16).setScale(10.0).setBaseOffset(0);
	public static final Field ALSIGNSKIPLOADBINS = Field.create("ALSIGNSKIPLOADBINS", 19360, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALSIGNSKIPRPMBINS = Field.create("ALSIGNSKIPRPMBINS", 19368, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS1_TABLE = Field.create("IGNBLENDS1_TABLE", 19376, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS1_LOADBINS = Field.create("IGNBLENDS1_LOADBINS", 19504, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS1_RPMBINS = Field.create("IGNBLENDS1_RPMBINS", 19520, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS1_BLENDPARAMETER = Field.create("IGNBLENDS1_BLENDPARAMETER", 19536, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS1_YAXISOVERRIDE = Field.create("IGNBLENDS1_YAXISOVERRIDE", 19537, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS1_BLENDBINS = Field.create("IGNBLENDS1_BLENDBINS", 19538, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS1_BLENDVALUES = Field.create("IGNBLENDS1_BLENDVALUES", 19554, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field IGNBLENDS1_ALIGNMENTFILL_AT_186 = Field.create("IGNBLENDS1_ALIGNMENTFILL_AT_186", 19562, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS2_TABLE = Field.create("IGNBLENDS2_TABLE", 19564, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS2_LOADBINS = Field.create("IGNBLENDS2_LOADBINS", 19692, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS2_RPMBINS = Field.create("IGNBLENDS2_RPMBINS", 19708, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS2_BLENDPARAMETER = Field.create("IGNBLENDS2_BLENDPARAMETER", 19724, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS2_YAXISOVERRIDE = Field.create("IGNBLENDS2_YAXISOVERRIDE", 19725, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS2_BLENDBINS = Field.create("IGNBLENDS2_BLENDBINS", 19726, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS2_BLENDVALUES = Field.create("IGNBLENDS2_BLENDVALUES", 19742, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field IGNBLENDS2_ALIGNMENTFILL_AT_186 = Field.create("IGNBLENDS2_ALIGNMENTFILL_AT_186", 19750, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS3_TABLE = Field.create("IGNBLENDS3_TABLE", 19752, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS3_LOADBINS = Field.create("IGNBLENDS3_LOADBINS", 19880, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS3_RPMBINS = Field.create("IGNBLENDS3_RPMBINS", 19896, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS3_BLENDPARAMETER = Field.create("IGNBLENDS3_BLENDPARAMETER", 19912, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS3_YAXISOVERRIDE = Field.create("IGNBLENDS3_YAXISOVERRIDE", 19913, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS3_BLENDBINS = Field.create("IGNBLENDS3_BLENDBINS", 19914, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS3_BLENDVALUES = Field.create("IGNBLENDS3_BLENDVALUES", 19930, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field IGNBLENDS3_ALIGNMENTFILL_AT_186 = Field.create("IGNBLENDS3_ALIGNMENTFILL_AT_186", 19938, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS4_TABLE = Field.create("IGNBLENDS4_TABLE", 19940, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS4_LOADBINS = Field.create("IGNBLENDS4_LOADBINS", 20068, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS4_RPMBINS = Field.create("IGNBLENDS4_RPMBINS", 20084, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS4_BLENDPARAMETER = Field.create("IGNBLENDS4_BLENDPARAMETER", 20100, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS4_YAXISOVERRIDE = Field.create("IGNBLENDS4_YAXISOVERRIDE", 20101, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field IGNBLENDS4_BLENDBINS = Field.create("IGNBLENDS4_BLENDBINS", 20102, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field IGNBLENDS4_BLENDVALUES = Field.create("IGNBLENDS4_BLENDVALUES", 20118, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field IGNBLENDS4_ALIGNMENTFILL_AT_186 = Field.create("IGNBLENDS4_ALIGNMENTFILL_AT_186", 20126, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS1_TABLE = Field.create("VEBLENDS1_TABLE", 20128, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS1_LOADBINS = Field.create("VEBLENDS1_LOADBINS", 20256, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS1_RPMBINS = Field.create("VEBLENDS1_RPMBINS", 20272, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS1_BLENDPARAMETER = Field.create("VEBLENDS1_BLENDPARAMETER", 20288, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS1_YAXISOVERRIDE = Field.create("VEBLENDS1_YAXISOVERRIDE", 20289, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS1_BLENDBINS = Field.create("VEBLENDS1_BLENDBINS", 20290, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS1_BLENDVALUES = Field.create("VEBLENDS1_BLENDVALUES", 20306, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDS1_ALIGNMENTFILL_AT_186 = Field.create("VEBLENDS1_ALIGNMENTFILL_AT_186", 20314, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS2_TABLE = Field.create("VEBLENDS2_TABLE", 20316, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS2_LOADBINS = Field.create("VEBLENDS2_LOADBINS", 20444, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS2_RPMBINS = Field.create("VEBLENDS2_RPMBINS", 20460, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS2_BLENDPARAMETER = Field.create("VEBLENDS2_BLENDPARAMETER", 20476, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS2_YAXISOVERRIDE = Field.create("VEBLENDS2_YAXISOVERRIDE", 20477, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS2_BLENDBINS = Field.create("VEBLENDS2_BLENDBINS", 20478, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS2_BLENDVALUES = Field.create("VEBLENDS2_BLENDVALUES", 20494, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDS2_ALIGNMENTFILL_AT_186 = Field.create("VEBLENDS2_ALIGNMENTFILL_AT_186", 20502, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS3_TABLE = Field.create("VEBLENDS3_TABLE", 20504, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS3_LOADBINS = Field.create("VEBLENDS3_LOADBINS", 20632, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS3_RPMBINS = Field.create("VEBLENDS3_RPMBINS", 20648, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS3_BLENDPARAMETER = Field.create("VEBLENDS3_BLENDPARAMETER", 20664, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS3_YAXISOVERRIDE = Field.create("VEBLENDS3_YAXISOVERRIDE", 20665, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS3_BLENDBINS = Field.create("VEBLENDS3_BLENDBINS", 20666, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS3_BLENDVALUES = Field.create("VEBLENDS3_BLENDVALUES", 20682, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDS3_ALIGNMENTFILL_AT_186 = Field.create("VEBLENDS3_ALIGNMENTFILL_AT_186", 20690, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS4_TABLE = Field.create("VEBLENDS4_TABLE", 20692, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS4_LOADBINS = Field.create("VEBLENDS4_LOADBINS", 20820, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS4_RPMBINS = Field.create("VEBLENDS4_RPMBINS", 20836, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS4_BLENDPARAMETER = Field.create("VEBLENDS4_BLENDPARAMETER", 20852, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS4_YAXISOVERRIDE = Field.create("VEBLENDS4_YAXISOVERRIDE", 20853, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field VEBLENDS4_BLENDBINS = Field.create("VEBLENDS4_BLENDBINS", 20854, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field VEBLENDS4_BLENDVALUES = Field.create("VEBLENDS4_BLENDVALUES", 20870, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field VEBLENDS4_ALIGNMENTFILL_AT_186 = Field.create("VEBLENDS4_ALIGNMENTFILL_AT_186", 20878, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field THROTTLEESTIMATEEFFECTIVEAREABINS = Field.create("THROTTLEESTIMATEEFFECTIVEAREABINS", 20880, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field THROTTLEESTIMATEEFFECTIVEAREAVALUES = Field.create("THROTTLEESTIMATEEFFECTIVEAREAVALUES", 20904, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_TABLE = Field.create("BOOSTOPENLOOPBLENDS1_TABLE", 20928, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_LOADBINS = Field.create("BOOSTOPENLOOPBLENDS1_LOADBINS", 21056, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_RPMBINS = Field.create("BOOSTOPENLOOPBLENDS1_RPMBINS", 21072, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_BLENDPARAMETER = Field.create("BOOSTOPENLOOPBLENDS1_BLENDPARAMETER", 21088, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_YAXISOVERRIDE = Field.create("BOOSTOPENLOOPBLENDS1_YAXISOVERRIDE", 21089, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_BLENDBINS = Field.create("BOOSTOPENLOOPBLENDS1_BLENDBINS", 21090, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_BLENDVALUES = Field.create("BOOSTOPENLOOPBLENDS1_BLENDVALUES", 21106, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS1_ALIGNMENTFILL_AT_186 = Field.create("BOOSTOPENLOOPBLENDS1_ALIGNMENTFILL_AT_186", 21114, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_TABLE = Field.create("BOOSTOPENLOOPBLENDS2_TABLE", 21116, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_LOADBINS = Field.create("BOOSTOPENLOOPBLENDS2_LOADBINS", 21244, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_RPMBINS = Field.create("BOOSTOPENLOOPBLENDS2_RPMBINS", 21260, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_BLENDPARAMETER = Field.create("BOOSTOPENLOOPBLENDS2_BLENDPARAMETER", 21276, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_YAXISOVERRIDE = Field.create("BOOSTOPENLOOPBLENDS2_YAXISOVERRIDE", 21277, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_BLENDBINS = Field.create("BOOSTOPENLOOPBLENDS2_BLENDBINS", 21278, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_BLENDVALUES = Field.create("BOOSTOPENLOOPBLENDS2_BLENDVALUES", 21294, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTOPENLOOPBLENDS2_ALIGNMENTFILL_AT_186 = Field.create("BOOSTOPENLOOPBLENDS2_ALIGNMENTFILL_AT_186", 21302, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_TABLE = Field.create("BOOSTCLOSEDLOOPBLENDS1_TABLE", 21304, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_LOADBINS = Field.create("BOOSTCLOSEDLOOPBLENDS1_LOADBINS", 21432, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_RPMBINS = Field.create("BOOSTCLOSEDLOOPBLENDS1_RPMBINS", 21448, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_BLENDPARAMETER = Field.create("BOOSTCLOSEDLOOPBLENDS1_BLENDPARAMETER", 21464, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_YAXISOVERRIDE = Field.create("BOOSTCLOSEDLOOPBLENDS1_YAXISOVERRIDE", 21465, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_BLENDBINS = Field.create("BOOSTCLOSEDLOOPBLENDS1_BLENDBINS", 21466, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_BLENDVALUES = Field.create("BOOSTCLOSEDLOOPBLENDS1_BLENDVALUES", 21482, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS1_ALIGNMENTFILL_AT_186 = Field.create("BOOSTCLOSEDLOOPBLENDS1_ALIGNMENTFILL_AT_186", 21490, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_TABLE = Field.create("BOOSTCLOSEDLOOPBLENDS2_TABLE", 21492, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_LOADBINS = Field.create("BOOSTCLOSEDLOOPBLENDS2_LOADBINS", 21620, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_RPMBINS = Field.create("BOOSTCLOSEDLOOPBLENDS2_RPMBINS", 21636, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_BLENDPARAMETER = Field.create("BOOSTCLOSEDLOOPBLENDS2_BLENDPARAMETER", 21652, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_YAXISOVERRIDE = Field.create("BOOSTCLOSEDLOOPBLENDS2_YAXISOVERRIDE", 21653, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_BLENDBINS = Field.create("BOOSTCLOSEDLOOPBLENDS2_BLENDBINS", 21654, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_BLENDVALUES = Field.create("BOOSTCLOSEDLOOPBLENDS2_BLENDVALUES", 21670, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field BOOSTCLOSEDLOOPBLENDS2_ALIGNMENTFILL_AT_186 = Field.create("BOOSTCLOSEDLOOPBLENDS2_ALIGNMENTFILL_AT_186", 21678, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_RANGEP = Field.create("TCU_RANGEP", 21680, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGER = Field.create("TCU_RANGER", 21704, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGEN = Field.create("TCU_RANGEN", 21728, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGED = Field.create("TCU_RANGED", 21752, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGEM = Field.create("TCU_RANGEM", 21776, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGEM3 = Field.create("TCU_RANGEM3", 21800, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGEM2 = Field.create("TCU_RANGEM2", 21824, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGEM1 = Field.create("TCU_RANGEM1", 21848, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGEPLUS = Field.create("TCU_RANGEPLUS", 21872, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGEMINUS = Field.create("TCU_RANGEMINUS", 21896, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TCU_RANGELOW = Field.create("TCU_RANGELOW", 21920, FieldType.FLOAT).setBaseOffset(0);
	public static final Field LAMBDAMAXDEVIATIONTABLE = Field.create("LAMBDAMAXDEVIATIONTABLE", 21944, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field LAMBDAMAXDEVIATIONLOADBINS = Field.create("LAMBDAMAXDEVIATIONLOADBINS", 21960, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field LAMBDAMAXDEVIATIONRPMBINS = Field.create("LAMBDAMAXDEVIATIONRPMBINS", 21968, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORSTAGINGTABLE = Field.create("INJECTORSTAGINGTABLE", 21976, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORSTAGINGLOADBINS = Field.create("INJECTORSTAGINGLOADBINS", 22012, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field INJECTORSTAGINGRPMBINS = Field.create("INJECTORSTAGINGRPMBINS", 22024, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field WWCLTBINS = Field.create("WWCLTBINS", 22036, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field WWTAUCLTVALUES = Field.create("WWTAUCLTVALUES", 22044, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field WWBETACLTVALUES = Field.create("WWBETACLTVALUES", 22052, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field WWMAPBINS = Field.create("WWMAPBINS", 22060, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field WWTAUMAPVALUES = Field.create("WWTAUMAPVALUES", 22068, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field WWBETAMAPVALUES = Field.create("WWBETAMAPVALUES", 22076, FieldType.INT8).setScale(0.01).setBaseOffset(0);
	public static final Field HPFPLOBEPROFILEQUANTITYBINS = Field.create("HPFPLOBEPROFILEQUANTITYBINS", 22084, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field HPFPLOBEPROFILEANGLE = Field.create("HPFPLOBEPROFILEANGLE", 22100, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field HPFPDEADTIMEVOLTSBINS = Field.create("HPFPDEADTIMEVOLTSBINS", 22116, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPDEADTIMEMS = Field.create("HPFPDEADTIMEMS", 22124, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field HPFPTARGET = Field.create("HPFPTARGET", 22140, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPTARGETLOADBINS = Field.create("HPFPTARGETLOADBINS", 22340, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field HPFPTARGETRPMBINS = Field.create("HPFPTARGETRPMBINS", 22360, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field HPFPCOMPENSATION = Field.create("HPFPCOMPENSATION", 22370, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field HPFPCOMPENSATIONLOADBINS = Field.create("HPFPCOMPENSATIONLOADBINS", 22470, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field HPFPCOMPENSATIONRPMBINS = Field.create("HPFPCOMPENSATIONRPMBINS", 22490, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field HPFPFUELMASSCOMPENSATION = Field.create("HPFPFUELMASSCOMPENSATION", 22500, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field HPFPFUELMASSCOMPENSATIONFUELMASS = Field.create("HPFPFUELMASSCOMPENSATIONFUELMASS", 22628, FieldType.INT16).setScale(0.01).setBaseOffset(0);
	public static final Field HPFPFUELMASSCOMPENSATIONFUELPRESSURE = Field.create("HPFPFUELMASSCOMPENSATIONFUELPRESSURE", 22644, FieldType.INT).setScale(0.1).setBaseOffset(0);
	public static final Field KNOCKNOISERPMBINS = Field.create("KNOCKNOISERPMBINS", 22676, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field KNOCKBASENOISE = Field.create("KNOCKBASENOISE", 22708, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field TPSTSPCORRVALUESBINS = Field.create("TPSTSPCORRVALUESBINS", 22724, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field TPSTSPCORRVALUES = Field.create("TPSTSPCORRVALUES", 22728, FieldType.INT8).setScale(0.02).setBaseOffset(0);
	public static final Field CLTREVLIMITRPMBINS = Field.create("CLTREVLIMITRPMBINS", 22732, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field CLTREVLIMITRPM = Field.create("CLTREVLIMITRPM", 22736, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field FUELLEVELBINS = Field.create("FUELLEVELBINS", 22744, FieldType.INT16).setScale(0.001).setBaseOffset(0);
	public static final Field FUELLEVELVALUES = Field.create("FUELLEVELVALUES", 22760, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DWELLVOLTAGECORRVOLTBINS = Field.create("DWELLVOLTAGECORRVOLTBINS", 22768, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field DWELLVOLTAGECORRVALUES = Field.create("DWELLVOLTAGECORRVALUES", 22776, FieldType.INT8).setScale(0.02).setBaseOffset(0);
	public static final Field TCU_SHIFTTPSBINS = Field.create("TCU_SHIFTTPSBINS", 22784, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SHIFTSPEED12 = Field.create("TCU_SHIFTSPEED12", 22792, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SHIFTSPEED23 = Field.create("TCU_SHIFTSPEED23", 22800, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SHIFTSPEED34 = Field.create("TCU_SHIFTSPEED34", 22808, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SHIFTSPEED21 = Field.create("TCU_SHIFTSPEED21", 22816, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SHIFTSPEED32 = Field.create("TCU_SHIFTSPEED32", 22824, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SHIFTSPEED43 = Field.create("TCU_SHIFTSPEED43", 22832, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TCU_SHIFTTIME = Field.create("TCU_SHIFTTIME", 22840, FieldType.FLOAT).setBaseOffset(0);
	public static final Field ALTERNATORVOLTAGETARGETTABLE = Field.create("ALTERNATORVOLTAGETARGETTABLE", 22844, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field ALTERNATORVOLTAGETARGETLOADBINS = Field.create("ALTERNATORVOLTAGETARGETLOADBINS", 22876, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field ALTERNATORVOLTAGETARGETRPMBINS = Field.create("ALTERNATORVOLTAGETARGETRPMBINS", 22884, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field CLTBOOSTCORRBINS = Field.create("CLTBOOSTCORRBINS", 22892, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLTBOOSTCORR = Field.create("CLTBOOSTCORR", 22912, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IATBOOSTCORRBINS = Field.create("IATBOOSTCORRBINS", 22932, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IATBOOSTCORR = Field.create("IATBOOSTCORR", 22952, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLTBOOSTADDERBINS = Field.create("CLTBOOSTADDERBINS", 22972, FieldType.FLOAT).setBaseOffset(0);
	public static final Field CLTBOOSTADDER = Field.create("CLTBOOSTADDER", 22992, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IATBOOSTADDERBINS = Field.create("IATBOOSTADDERBINS", 23012, FieldType.FLOAT).setBaseOffset(0);
	public static final Field IATBOOSTADDER = Field.create("IATBOOSTADDER", 23032, FieldType.FLOAT).setBaseOffset(0);
	public static final Field MINIMUMOILPRESSUREBINS = Field.create("MINIMUMOILPRESSUREBINS", 23052, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field MINIMUMOILPRESSUREVALUES = Field.create("MINIMUMOILPRESSUREVALUES", 23060, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_TABLE = Field.create("TARGETAFRBLENDS1_TABLE", 23068, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_LOADBINS = Field.create("TARGETAFRBLENDS1_LOADBINS", 23196, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_RPMBINS = Field.create("TARGETAFRBLENDS1_RPMBINS", 23212, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_BLENDPARAMETER = Field.create("TARGETAFRBLENDS1_BLENDPARAMETER", 23228, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_YAXISOVERRIDE = Field.create("TARGETAFRBLENDS1_YAXISOVERRIDE", 23229, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_BLENDBINS = Field.create("TARGETAFRBLENDS1_BLENDBINS", 23230, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_BLENDVALUES = Field.create("TARGETAFRBLENDS1_BLENDVALUES", 23246, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS1_ALIGNMENTFILL_AT_186 = Field.create("TARGETAFRBLENDS1_ALIGNMENTFILL_AT_186", 23254, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_TABLE = Field.create("TARGETAFRBLENDS2_TABLE", 23256, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_LOADBINS = Field.create("TARGETAFRBLENDS2_LOADBINS", 23384, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_RPMBINS = Field.create("TARGETAFRBLENDS2_RPMBINS", 23400, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_BLENDPARAMETER = Field.create("TARGETAFRBLENDS2_BLENDPARAMETER", 23416, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_YAXISOVERRIDE = Field.create("TARGETAFRBLENDS2_YAXISOVERRIDE", 23417, FieldType.INT8, gppwm_channel_e).setScale(1.0).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_BLENDBINS = Field.create("TARGETAFRBLENDS2_BLENDBINS", 23418, FieldType.INT16).setScale(0.1).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_BLENDVALUES = Field.create("TARGETAFRBLENDS2_BLENDVALUES", 23434, FieldType.INT8).setScale(0.5).setBaseOffset(0);
	public static final Field TARGETAFRBLENDS2_ALIGNMENTFILL_AT_186 = Field.create("TARGETAFRBLENDS2_ALIGNMENTFILL_AT_186", 23442, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field TRIMRPMBINS = Field.create("TRIMRPMBINS", 23444, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field TRIMLOADBINS = Field.create("TRIMLOADBINS", 23452, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DYNORPMSTEP = Field.create("DYNORPMSTEP", 23468, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOSAETEMPERATUREC = Field.create("DYNOSAETEMPERATUREC", 23469, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOSAERELATIVEHUMIDITY = Field.create("DYNOSAERELATIVEHUMIDITY", 23470, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field ALIGNMENTFILL_AT_23471 = Field.create("ALIGNMENTFILL_AT_23471", 23471, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOSAEBARO = Field.create("DYNOSAEBARO", 23472, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DYNOCARWHEELDIAINCH = Field.create("DYNOCARWHEELDIAINCH", 23476, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOCARWHEELASPECTRATIO = Field.create("DYNOCARWHEELASPECTRATIO", 23477, FieldType.INT8).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOCARWHEELTIREWIDTHMM = Field.create("DYNOCARWHEELTIREWIDTHMM", 23478, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOCARGEARPRIMARYREDUCTION = Field.create("DYNOCARGEARPRIMARYREDUCTION", 23480, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DYNOCARGEARRATIO = Field.create("DYNOCARGEARRATIO", 23484, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DYNOCARGEARFINALDRIVE = Field.create("DYNOCARGEARFINALDRIVE", 23488, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DYNOCARCARMASSKG = Field.create("DYNOCARCARMASSKG", 23492, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOCARCARGOMASSKG = Field.create("DYNOCARCARGOMASSKG", 23494, FieldType.INT16).setScale(1.0).setBaseOffset(0);
	public static final Field DYNOCARCOEFFOFDRAG = Field.create("DYNOCARCOEFFOFDRAG", 23496, FieldType.FLOAT).setBaseOffset(0);
	public static final Field DYNOCARFRONTALAREAM2 = Field.create("DYNOCARFRONTALAREAM2", 23500, FieldType.FLOAT).setBaseOffset(0);
	public static final Field TRAILINGSPARKTABLE = Field.create("TRAILINGSPARKTABLE", 23504, FieldType.INT8).setScale(0.1).setBaseOffset(0);
	public static final Field TRAILINGSPARKRPMBINS = Field.create("TRAILINGSPARKRPMBINS", 23520, FieldType.INT8).setScale(50.0).setBaseOffset(0);
	public static final Field TRAILINGSPARKLOADBINS = Field.create("TRAILINGSPARKLOADBINS", 23524, FieldType.INT8).setScale(5.0).setBaseOffset(0);
	public static final Field MAXIMUMOILPRESSUREBINS = Field.create("MAXIMUMOILPRESSUREBINS", 23528, FieldType.INT8).setScale(100.0).setBaseOffset(0);
	public static final Field MAXIMUMOILPRESSUREVALUES = Field.create("MAXIMUMOILPRESSUREVALUES", 23532, FieldType.INT8).setScale(10.0).setBaseOffset(0);
	public static final Field[] VALUES = {
	ENGINETYPE,
	STARTBUTTONSUPPRESSONSTARTUPMS,
	SENSORSNIFFERRPMTHRESHOLD,
	LAUNCHRPM,
	RPMHARDLIMIT,
	ENGINESNIFFERRPMTHRESHOLD,
	MULTISPARKMAXRPM,
	MAXACRPM,
	MAXACTPS,
	MAXACCLT,
	COMPRESSIONRATIO,
	WASTEGATEPOSITIONMIN,
	WASTEGATEPOSITIONMAX,
	IDLEPOSITIONMIN,
	IDLEPOSITIONMAX,
	MAINRELAYPIN,
	SDCARDCSPIN,
	CANTXPIN,
	CANRXPIN,
	TORQUEREDUCTIONTRIGGERPIN,
	LAUNCHFUELADDERPERCENT,
	ETBJAMTIMEOUT,
	TACHOUTPUTPIN,
	TACHOUTPUTPINMODE,
	ACIDLEEXTRAOFFSET,
	MULTISPARKMAXSPARKINGANGLE,
	MULTISPARKMAXEXTRASPARKCOUNT,
	VVTCONTROLMINRPM,
	INJECTOR_FLOW,
	INJECTOR_BATTLAGCORRBATTBINS,
	INJECTOR_BATTLAGCORRPRESSBINS,
	INJECTOR_BATTLAGCORRTABLE,
	INJECTORSECONDARY_FLOW,
	INJECTORSECONDARY_BATTLAGCORRBATTBINS,
	INJECTORSECONDARY_BATTLAGCORRPRESSBINS,
	INJECTORSECONDARY_BATTLAGCORRTABLE,
	ISFORCEDINDUCTION,
	USEFORDREDUNDANTTPS,
	ENABLEKLINE,
	OVERRIDETRIGGERGAPS,
	ENABLEFAN1WITHAC,
	ENABLEFAN2WITHAC,
	DISABLEFAN1WHENSTOPPED,
	DISABLEFAN2WHENSTOPPED,
	ENABLETRAILINGSPARKS,
	ETB_USE_TWO_WIRES,
	ISDOUBLESOLENOIDIDLE,
	USEEEPROM,
	USECICPIDFORIDLE,
	USETLE8888_CRANKING_HACK,
	KICKSTARTCRANKING,
	USESEPARATEIDLETABLESFORCRANKINGTAPER,
	LAUNCHCONTROLENABLED,
	DONOTFILTERTRIGGEREDGENOISE,
	ANTILAGENABLED,
	USERUNNINGMATHFORCRANKING,
	DISPLAYLOGICLEVELSINENGINESNIFFER,
	USETLE8888_STEPPER,
	USESCRIPTTABLEFORCANSNIFFINGFILTERING,
	VERBOSECAN,
	ARTIFICIALTESTMISFIRE,
	USEFORDREDUNDANTPPS,
	CLTSENSORPULLDOWN,
	IATSENSORPULLDOWN,
	ALLOWIDENTICALPPS,
	OVERRIDEVVTTRIGGERGAPS,
	USESPIIMU,
	ENABLESTAGEDINJECTION,
	TPSMIN,
	TPSMAX,
	TPSERRORDETECTIONTOOLOW,
	TPSERRORDETECTIONTOOHIGH,
	CRANKING_BASEFUEL,
	CRANKING_RPM,
	CRANKING_ALIGNMENTFILL_AT_6,
	IGNITIONDWELLFORCRANKINGMS,
	ETBREVLIMITSTART,
	ETBREVLIMITRANGE,
	MAP_SAMPLINGANGLEBINS,
	MAP_SAMPLINGANGLE,
	MAP_SAMPLINGWINDOWBINS,
	MAP_SAMPLINGWINDOW,
	MAP_SENSOR_LOWVALUE,
	MAP_SENSOR_HIGHVALUE,
	MAP_SENSOR_TYPE,
	MAP_SENSOR_HWCHANNEL,
	MAP_SENSOR_ALIGNMENTFILL_AT_10,
	CLT_TEMPC_1,
	CLT_TEMPC_2,
	CLT_TEMPC_3,
	CLT_RESISTANCE_1,
	CLT_RESISTANCE_2,
	CLT_RESISTANCE_3,
	CLT_BIAS_RESISTOR,
	CLT_ADCCHANNEL,
	CLT_ALIGNMENTFILL_AT_29,
	IAT_TEMPC_1,
	IAT_TEMPC_2,
	IAT_TEMPC_3,
	IAT_RESISTANCE_1,
	IAT_RESISTANCE_2,
	IAT_RESISTANCE_3,
	IAT_BIAS_RESISTOR,
	IAT_ADCCHANNEL,
	IAT_ALIGNMENTFILL_AT_29,
	LAUNCHTIMINGRETARD,
	HIP9011PRESCALER,
	TUNINGDETECTOR,
	ALTERNATOR_ITERMMIN,
	ALTERNATOR_ITERMMAX,
	DISPLACEMENT,
	TRIGGERSIMULATORRPM,
	CYLINDERSCOUNT,
	FIRINGORDER,
	JUSTATEMPTEST,
	MAPSYNCTHRESHOLD,
	TORQUEREDUCTIONIGNITIONCUT,
	CYLINDERBORE,
	FUELALGORITHM,
	ALSMAXTPS,
	CRANKINGINJECTIONMODE,
	INJECTIONMODE,
	BOOSTCONTROLMINRPM,
	BOOSTCONTROLMINTPS,
	ALIGNMENTFILL_AT_435,
	BOOSTCONTROLMINMAP,
	ALIGNMENTFILL_AT_438,
	CRANKINGTIMINGANGLE,
	IGNITIONMODE,
	GAPTRACKINGLENGTHOVERRIDE,
	MAXIDLEVSS,
	CAMDECODER2JZPRECISION,
	MINOILPRESSUREAFTERSTART,
	TIMINGMODE,
	CANNBCTYPE,
	FIXEDMODETIMING,
	GLOBALTRIGGERANGLEOFFSET,
	ANALOGINPUTDIVIDERCOEFFICIENT,
	VBATTDIVIDERCOEFF,
	FANONTEMPERATURE,
	FANOFFTEMPERATURE,
	ACRPIN,
	ALIGNMENTFILL_AT_478,
	DRIVEWHEELREVPERKM,
	CANSLEEPPERIODMS,
	BYFIRMWAREVERSION,
	TPS1_1ADCCHANNEL,
	VBATTADCCHANNEL,
	FUELLEVELSENSOR,
	TPS2_1ADCCHANNEL,
	IDLE_DERIVATIVEFILTERLOSS,
	TRIGGER_TYPE,
	TRIGGER_CUSTOMTOTALTOOTHCOUNT,
	TRIGGER_CUSTOMSKIPPEDTOOTHCOUNT,
	AIRBYRPMTAPER,
	HIP9011SPIDEVICE,
	BOOSTCONTROLSAFEDUTYCYCLE,
	MAFADCCHANNEL,
	ACRREVOLUTIONS,
	CALIBRATIONBIRTHDAY,
	ADCVCC,
	MAPCAMDETECTIONANGLEPOSITION,
	CAMINPUTS1,
	CAMINPUTS2,
	CAMINPUTS3,
	CAMINPUTS4,
	AFR_HWCHANNEL,
	AFR_HWCHANNEL2,
	AFR_ALIGNMENTFILL_AT_2,
	AFR_V1,
	AFR_VALUE1,
	AFR_V2,
	AFR_VALUE2,
	THROTTLEPEDALPOSITIONADCCHANNEL,
	ETBSPLIT,
	TLE6240_CS,
	TLE6240_CSPINMODE,
	MC33810_CSPINMODE,
	ALIGNMENTFILL_AT_566,
	BAROSENSOR_LOWVALUE,
	BAROSENSOR_HIGHVALUE,
	BAROSENSOR_TYPE,
	BAROSENSOR_HWCHANNEL,
	BAROSENSOR_ALIGNMENTFILL_AT_10,
	IDLE_SOLENOIDFREQUENCY,
	IDLE_SOLENOIDPIN,
	IDLE_STEPPERDIRECTIONPIN,
	IDLE_STEPPERSTEPPIN,
	IDLE_SOLENOIDPINMODE,
	IDLE_ALIGNMENTFILL_AT_11,
	KNOCKRETARDAGGRESSION,
	KNOCKRETARDREAPPLYRATE,
	ENGINESYNCCAM,
	SDCARDCSPINMODE,
	VSSGEARRATIO,
	VSSFILTERRECIPROCAL,
	VSSTOOTHCOUNT,
	IDLEVEOVERRIDEMODE,
	ALIGNMENTFILL_AT_601,
	L9779_CS,
	INJECTIONPINS1,
	INJECTIONPINS2,
	INJECTIONPINS3,
	INJECTIONPINS4,
	INJECTIONPINS5,
	INJECTIONPINS6,
	INJECTIONPINS7,
	INJECTIONPINS8,
	INJECTIONPINS9,
	INJECTIONPINS10,
	INJECTIONPINS11,
	INJECTIONPINS12,
	IGNITIONPINS1,
	IGNITIONPINS2,
	IGNITIONPINS3,
	IGNITIONPINS4,
	IGNITIONPINS5,
	IGNITIONPINS6,
	IGNITIONPINS7,
	IGNITIONPINS8,
	IGNITIONPINS9,
	IGNITIONPINS10,
	IGNITIONPINS11,
	IGNITIONPINS12,
	INJECTIONPINMODE,
	IGNITIONPINMODE,
	FUELPUMPPIN,
	FUELPUMPPINMODE,
	GAPVVTTRACKINGLENGTHOVERRIDE,
	MALFUNCTIONINDICATORPIN,
	MALFUNCTIONINDICATORPINMODE,
	FANPINMODE,
	FANPIN,
	CLUTCHDOWNPIN,
	ALTERNATORCONTROLPIN,
	ALTERNATORCONTROLPINMODE,
	CLUTCHDOWNPINMODE,
	DIGITALPOTENTIOMETERCHIPSELECT1,
	DIGITALPOTENTIOMETERCHIPSELECT2,
	DIGITALPOTENTIOMETERCHIPSELECT3,
	DIGITALPOTENTIOMETERCHIPSELECT4,
	ELECTRONICTHROTTLEPIN1MODE,
	MAX31855SPIDEVICE,
	DEBUGTRIGGERSYNC,
	DIGITALPOTENTIOMETERSPIDEVICE,
	BRAKEPEDALPINMODE,
	MC33972_CS,
	MC33972_CSPINMODE,
	AUXFASTSENSOR1_ADCCHANNEL,
	TPS1_2ADCCHANNEL,
	TPS2_2ADCCHANNEL,
	THROTTLEPEDALPOSITIONSECONDADCCHANNEL,
	AFR_TYPE,
	MC33810_CS1,
	ALIGNMENTFILL_AT_694,
	IDLE_ANTIWINDUPFREQ,
	TRIGGERINPUTPINS1,
	TRIGGERINPUTPINS2,
	MC33_T_MIN_BOOST,
	HIP9011CSPINMODE,
	ALIGNMENTFILL_AT_707,
	FINALGEARRATIO,
	TCUINPUTSPEEDSENSORPIN,
	TCUINPUTSPEEDSENSORTEETH,
	ALIGNMENTFILL_AT_713,
	TRIGGERSIMULATORPINS1,
	TRIGGERSIMULATORPINS2,
	FORDINJECTORSMALLPULSESLOPE,
	TRIGGERSIMULATORPINMODES1,
	TRIGGERSIMULATORPINMODES2,
	MAF2ADCCHANNEL,
	ALIGNMENTFILL_AT_723,
	O2HEATERPIN,
	O2HEATERPINMODETODO,
	LAMBDAPROTECTIONMINRPM,
	LAMBDAPROTECTIONMINLOAD,
	ALIGNMENTFILL_AT_729,
	IS_ENABLED_SPI_1,
	IS_ENABLED_SPI_2,
	IS_ENABLED_SPI_3,
	ISSDCARDENABLED,
	RUSEFIVERBOSE29B,
	RETHROWHARDFAULT,
	ISHIP9011ENABLED,
	REQUIREFOOTONBRAKETOCRANK,
	VERBOSEQUAD,
	USESTEPPERIDLE,
	ENABLEDSTEP1LIMITER,
	USETPICADVANCEDMODE,
	LAMBDAPROTECTIONENABLE,
	VERBOSETLE8888,
	ENABLEVERBOSECANTX,
	EXTERNALRUSEFIGDIMODULE,
	FLIPWBOCHANNELS,
	MEASUREMAPONLYINONECYLINDER,
	STEPPERFORCEPARKINGEVERYRESTART,
	ISFASTERENGINESPINUPENABLED,
	COASTINGFUELCUTENABLED,
	USEIACTABLEFORCOASTING,
	USENOISELESSTRIGGERDECODER,
	USEIDLETIMINGPIDCONTROL,
	DISABLEETBWHENENGINESTOPPED,
	IS_ENABLED_SPI_4,
	PAUSEETBCONTROL,
	TPSTPSPERCENTMODE,
	VERBOSEKLINE,
	IDLEINCREMENTALPIDCIC,
	ENABLEAEMXSERIES,
	MODELEDFLOWIDLE,
	LOGICANALYZERPINS1,
	LOGICANALYZERPINS2,
	LOGICANALYZERPINS3,
	LOGICANALYZERPINS4,
	MAINRELAYPINMODE,
	ALIGNMENTFILL_AT_745,
	HIP9011CSPIN,
	HIP9011INTHOLDPIN,
	HIP9011INTHOLDPINMODE,
	ALIGNMENTFILL_AT_751,
	VERBOSECANBASEADDRESS,
	MC33_HVOLT,
	ALIGNMENTFILL_AT_757,
	MINIMUMBOOSTCLOSEDLOOPMAP,
	INITIALIGNITIONCUTPERCENT,
	FINALIGNITIONCUTPERCENTBEFORELAUNCH,
	BOOSTOPENLOOPYAXIS,
	L9779SPIDEVICE,
	IMUTYPE,
	ALIGNMENTFILL_AT_765,
	IDLEPIDRPMUPPERLIMIT,
	APPLYNONLINEARBELOWPULSE,
	LPS25BAROSENSORSCL,
	LPS25BAROSENSORSDA,
	VEHICLESPEEDSENSORINPUTPIN,
	CLUTCHUPPIN,
	INJECTORNONLINEARMODE,
	CLUTCHUPPINMODE,
	MAX31855_CS1,
	MAX31855_CS2,
	MAX31855_CS3,
	MAX31855_CS4,
	MAX31855_CS5,
	MAX31855_CS6,
	MAX31855_CS7,
	MAX31855_CS8,
	FLEXSENSORPIN,
	TORQUEREDUCTIONARMINGRPM,
	STEPPERDIRECTIONPINMODE,
	MC33972SPIDEVICE,
	STOICHRATIOSECONDARY,
	ETBMAXIMUMPOSITION,
	SDCARDLOGFREQUENCY,
	IDLEPOSITIONCHANNEL,
	ALIGNMENTFILL_AT_807,
	LAUNCHCORRECTIONSENDRPM,
	STARTERRELAYDISABLEPIN,
	STARTERRELAYDISABLEPINMODE,
	ALIGNMENTFILL_AT_813,
	SECONDSOLENOIDPIN,
	STARTSTOPBUTTONPIN,
	LAMBDAPROTECTIONRESTORERPM,
	ACRELAYPINMODE,
	MAPMINBUFFERLENGTH,
	IDLEPIDDEACTIVATIONTPSTHRESHOLD,
	STEPPERPARKINGEXTRASTEPS,
	TPS1SECONDARYMIN,
	TPS1SECONDARYMAX,
	ANTILAGRPMTRESHOLD,
	STARTCRANKINGDURATION,
	TRIGGERERRORPIN,
	TRIGGERERRORPINMODE,
	ALIGNMENTFILL_AT_839,
	ACRELAYPIN,
	LAMBDAPROTECTIONMINTPS,
	LAMBDAPROTECTIONTIMEOUT,
	SCRIPTSETTING1,
	SCRIPTSETTING2,
	SCRIPTSETTING3,
	SCRIPTSETTING4,
	SCRIPTSETTING5,
	SCRIPTSETTING6,
	SCRIPTSETTING7,
	SCRIPTSETTING8,
	SPI1MOSIPIN,
	SPI1MISOPIN,
	SPI1SCKPIN,
	SPI2MOSIPIN,
	SPI2MISOPIN,
	SPI2SCKPIN,
	SPI3MOSIPIN,
	SPI3MISOPIN,
	SPI3SCKPIN,
	CDMINPUTPIN,
	CONSOLEUARTDEVICE,
	SENSORCHARTMODE,
	MAFSENSORTYPE,
	ALIGNMENTFILL_AT_899,
	IDLERETURNTARGETRAMP,
	USEHBRIDGESTODRIVEIDLESTEPPER,
	MULTISPARKENABLE,
	ENABLELAUNCHRETARD,
	CANINPUTBCM,
	CONSUMEOBDSENSORS,
	ENABLECANVSS,
	SUPPRESSWBOWORKAROUND7048,
	STFTIGNOREERRORMAGNITUDE,
	VVTBOOLEANFORVERYSPECIALCASES,
	ENABLESOFTWAREKNOCK,
	VERBOSEVVTDECODING,
	INVERTCAMVVTSIGNAL,
	ALPHANUSEIAT,
	KNOCKBANKCYL1,
	KNOCKBANKCYL2,
	KNOCKBANKCYL3,
	KNOCKBANKCYL4,
	KNOCKBANKCYL5,
	KNOCKBANKCYL6,
	KNOCKBANKCYL7,
	KNOCKBANKCYL8,
	KNOCKBANKCYL9,
	KNOCKBANKCYL10,
	KNOCKBANKCYL11,
	KNOCKBANKCYL12,
	TCUENABLED,
	CANBROADCASTUSECHANNELTWO,
	USERAWOUTPUTTODRIVEIDLESTEPPER,
	VERBOSECAN2,
	ETBIO1_DIRECTIONPIN1,
	ETBIO1_DIRECTIONPIN2,
	ETBIO1_CONTROLPIN,
	ETBIO1_DISABLEPIN,
	ETBIO2_DIRECTIONPIN1,
	ETBIO2_DIRECTIONPIN2,
	ETBIO2_CONTROLPIN,
	ETBIO2_DISABLEPIN,
	BOOSTCONTROLPIN,
	BOOSTCONTROLPINMODE,
	ALIGNMENTFILL_AT_923,
	ALSACTIVATEPIN,
	LAUNCHACTIVATEPIN,
	BOOSTPID_PFACTOR,
	BOOSTPID_IFACTOR,
	BOOSTPID_DFACTOR,
	BOOSTPID_OFFSET,
	BOOSTPID_PERIODMS,
	BOOSTPID_MINVALUE,
	BOOSTPID_MAXVALUE,
	BOOSTTYPE,
	IGNITIONKEYDIGITALPINMODE,
	IGNITIONKEYDIGITALPIN,
	BOOSTPWMFREQUENCY,
	LAUNCHACTIVATIONMODE,
	ANTILAGACTIVATIONMODE,
	TPSACCELLOOKBACK,
	NOFUELTRIMAFTERDFCOTIME,
	LAUNCHSPEEDTHRESHOLD,
	LAUNCHRPMWINDOW,
	TRIGGEREVENTSTIMEOUTMS,
	MAPEXPAVERAGEALPHA,
	MAGICNUMBERAVAILABLEFORDEVTRICKS,
	TURBOCHARGERFILTER,
	LAUNCHTPSTHRESHOLD,
	LAUNCHACTIVATEDELAY,
	STFT_MAXIDLEREGIONRPM,
	STFT_ALIGNMENTFILL_AT_1,
	STFT_MAXOVERRUNLOAD,
	STFT_MINPOWERLOAD,
	STFT_DEADBAND,
	STFT_MINCLT,
	STFT_MINAFR,
	STFT_MAXAFR,
	STFT_STARTUPDELAY,
	STFT_ALIGNMENTFILL_AT_11,
	STFT_CELLCFGS1_MAXADD,
	STFT_CELLCFGS1_MAXREMOVE,
	STFT_CELLCFGS1_TIMECONSTANT,
	STFT_CELLCFGS2_MAXADD,
	STFT_CELLCFGS2_MAXREMOVE,
	STFT_CELLCFGS2_TIMECONSTANT,
	STFT_CELLCFGS3_MAXADD,
	STFT_CELLCFGS3_MAXREMOVE,
	STFT_CELLCFGS3_TIMECONSTANT,
	STFT_CELLCFGS4_MAXADD,
	STFT_CELLCFGS4_MAXREMOVE,
	STFT_CELLCFGS4_TIMECONSTANT,
	STEPPERDCIO1_DIRECTIONPIN1,
	STEPPERDCIO1_DIRECTIONPIN2,
	STEPPERDCIO1_CONTROLPIN,
	STEPPERDCIO1_DISABLEPIN,
	STEPPERDCIO2_DIRECTIONPIN1,
	STEPPERDCIO2_DIRECTIONPIN2,
	STEPPERDCIO2_CONTROLPIN,
	STEPPERDCIO2_DISABLEPIN,
	ENGINEMAKE,
	ENGINECODE,
	VEHICLENAME,
	TCU_SOLENOID1,
	TCU_SOLENOID2,
	TCU_SOLENOID3,
	TCU_SOLENOID4,
	TCU_SOLENOID5,
	TCU_SOLENOID6,
	ETBFUNCTIONS1,
	ETBFUNCTIONS2,
	DRV8860SPIDEVICE,
	ALIGNMENTFILL_AT_1147,
	DRV8860_CS,
	DRV8860_CSPINMODE,
	ALIGNMENTFILL_AT_1151,
	DRV8860_MISO,
	LUAOUTPUTPINS1,
	LUAOUTPUTPINS2,
	LUAOUTPUTPINS3,
	LUAOUTPUTPINS4,
	LUAOUTPUTPINS5,
	LUAOUTPUTPINS6,
	LUAOUTPUTPINS7,
	LUAOUTPUTPINS8,
	ALIGNMENTFILL_AT_1170,
	VVTOFFSETS1,
	VVTOFFSETS2,
	VVTOFFSETS3,
	VVTOFFSETS4,
	VRTHRESHOLD1_RPMBINS,
	VRTHRESHOLD1_VALUES,
	VRTHRESHOLD1_PIN,
	VRTHRESHOLD1_ALIGNMENTFILL_AT_14,
	VRTHRESHOLD2_RPMBINS,
	VRTHRESHOLD2_VALUES,
	VRTHRESHOLD2_PIN,
	VRTHRESHOLD2_ALIGNMENTFILL_AT_14,
	GPPWMNOTE1,
	GPPWMNOTE2,
	GPPWMNOTE3,
	GPPWMNOTE4,
	TPS2SECONDARYMIN,
	TPS2SECONDARYMAX,
	WIDEBANDONSECONDBUS,
	FUELCLOSEDLOOPCORRECTIONENABLED,
	ALWAYSWRITESDCARD,
	KNOCKDETECTIONUSEDOUBLEFREQUENCY,
	YESUNDERSTANDLOCKING,
	SILENTTRIGGERERROR,
	USELINEARCLTSENSOR,
	CANREADENABLED,
	CANWRITEENABLED,
	USELINEARIATSENSOR,
	ENABLEOILPRESSUREPROTECT,
	TACHPULSEDURATIONASDUTYCYCLE,
	ISALTERNATORCONTROLENABLED,
	INVERTPRIMARYTRIGGERSIGNAL,
	INVERTSECONDARYTRIGGERSIGNAL,
	CUTFUELONHARDLIMIT,
	CUTSPARKONHARDLIMIT,
	LAUNCHFUELCUTENABLE,
	LAUNCHSPARKCUTENABLE,
	TORQUEREDUCTIONENABLED,
	LIMITTORQUEREDUCTIONTIME,
	VERBOSEISOTP,
	ENGINESNIFFERFOCUSONINPUTS,
	TWOSTROKE,
	SKIPPEDWHEELONCAM,
	HIPOUTPUTCHANNEL,
	ALIGNMENTFILL_AT_1293,
	ACSWITCH,
	VREFADCCHANNEL,
	ETBNEUTRALPOSITION,
	IDLEMODE,
	ALIGNMENTFILL_AT_1299,
	ISINJECTIONENABLED,
	ISIGNITIONENABLED,
	ISCYLINDERCLEANUPENABLED,
	COMPLEXWALLMODEL,
	ALWAYSINSTANTRPM,
	ISMAPAVERAGINGENABLED,
	USESEPARATEADVANCEFORIDLE,
	ISWAVEANALYZERENABLED,
	USESEPARATEVEFORIDLE,
	VERBOSETRIGGERSYNCHDETAILS,
	HONDAK,
	TWOWIREBATCHIGNITION,
	USEFIXEDBAROCORRFROMMAP,
	USESEPARATEADVANCEFORCRANKING,
	USEADVANCECORRECTIONSFORCRANKING,
	FLEXCRANKING,
	USEIACPIDMULTTABLE,
	ISBOOSTCONTROLENABLED,
	LAUNCHSMOOTHRETARD,
	ISPHASESYNCREQUIREDFORIGNITION,
	USECLTBASEDRPMLIMIT,
	FORCEO2HEATING,
	INVERTVVTCONTROLINTAKE,
	INVERTVVTCONTROLEXHAUST,
	USEBIQUADONAUXSPEEDSENSORS,
	SDTRIGGERLOG,
	STEPPER_DC_USE_TWO_WIRES,
	WATCHOUTFORLINEARTIME,
	ENGINECHARTSIZE,
	TURBOSPEEDSENSORMULTIPLIER,
	CAMINPUTSDEBUG1,
	CAMINPUTSDEBUG2,
	CAMINPUTSDEBUG3,
	CAMINPUTSDEBUG4,
	ACIDLERPMTARGET,
	WARNINGPERIOD,
	KNOCKDETECTIONWINDOWSTART,
	IDLESTEPPERREACTIONTIME,
	IDLESTEPPERTOTALSTEPS,
	TORQUEREDUCTIONARMINGAPP,
	TACHPULSEDURACTIONMS,
	WWAETAU,
	ALTERNATORCONTROL_PFACTOR,
	ALTERNATORCONTROL_IFACTOR,
	ALTERNATORCONTROL_DFACTOR,
	ALTERNATORCONTROL_OFFSET,
	ALTERNATORCONTROL_PERIODMS,
	ALTERNATORCONTROL_MINVALUE,
	ALTERNATORCONTROL_MAXVALUE,
	ETB_PFACTOR,
	ETB_IFACTOR,
	ETB_DFACTOR,
	ETB_OFFSET,
	ETB_PERIODMS,
	ETB_MINVALUE,
	ETB_MAXVALUE,
	TRIGGERINPUTDEBUGPINS1,
	TRIGGERINPUTDEBUGPINS2,
	AIRTAPERRPMRANGE,
	TURBOSPEEDSENSORINPUTPIN,
	TPS2MIN,
	TPS2MAX,
	STARTERCONTROLPIN,
	STARTSTOPBUTTONMODE,
	ALIGNMENTFILL_AT_1403,
	MC33816_FLAG0,
	TACHPULSEPERREV,
	MAPERRORDETECTIONTOOLOW,
	MAPERRORDETECTIONTOOHIGH,
	MULTISPARKSPARKDURATION,
	MULTISPARKDWELL,
	IDLERPMPID_PFACTOR,
	IDLERPMPID_IFACTOR,
	IDLERPMPID_DFACTOR,
	IDLERPMPID_OFFSET,
	IDLERPMPID_PERIODMS,
	IDLERPMPID_MINVALUE,
	IDLERPMPID_MAXVALUE,
	WWAEBETA,
	BINARYSERIALTXPIN,
	BINARYSERIALRXPIN,
	AUXVALVES1,
	AUXVALVES2,
	TCUUPSHIFTBUTTONPIN,
	TCUDOWNSHIFTBUTTONPIN,
	THROTTLEPEDALUPVOLTAGE,
	THROTTLEPEDALWOTVOLTAGE,
	STARTUPFUELPUMPDURATION,
	IDLEPIDRPMDEADZONE,
	IDLEMAXIMUMAIRMASS,
	ALIGNMENTFILL_AT_1469,
	TORQUEREDUCTIONTIME,
	MC33810DISABLERECOVERYMODE,
	MC33810GPGD0MODE,
	MC33810GPGD1MODE,
	MC33810GPGD2MODE,
	MC33810GPGD3MODE,
	ENABLEEXTENDEDCANBROADCAST,
	LUACANRXWORKAROUND,
	FLEXSENSORINVERTED,
	USEHARDSKIPINTRACTION,
	USEAUXSPEEDFORSLIPRATIO,
	USEVSSASSECONDWHEELSPEED,
	IS_ENABLED_SPI_5,
	IS_ENABLED_SPI_6,
	ENABLEAEMXSERIESEGT,
	STARTREQUESTPININVERTED,
	TCU_RANGESENSORPULLDOWN,
	DEVBIT01,
	DEVBIT0,
	DEVBIT1,
	DEVBIT2,
	DEVBIT3,
	DEVBIT4,
	DEVBIT5,
	DEVBIT6,
	DEVBIT7,
	INVERTEXHAUSTCAMVVTSIGNAL,
	ENABLEKNOCKSPECTROGRAM,
	ENABLEKNOCKSPECTROGRAMFILTER,
	AFTERCRANKINGIACTAPERDURATION,
	IACBYTPSTAPER,
	ACCELEROMETERCSPIN,
	COASTINGFUELCUTVSSLOW,
	COASTINGFUELCUTVSSHIGH,
	TPSACCELENRICHMENTTHRESHOLD,
	AUXSPEEDSENSORINPUTPIN1,
	AUXSPEEDSENSORINPUTPIN2,
	TOTALGEARSCOUNT,
	INJECTIONTIMINGMODE,
	DEBUGMODE,
	FAN1EXTRAIDLE,
	UARTCONSOLESERIALSPEED,
	TPSDECELENLEANMENTTHRESHOLD,
	TPSDECELENLEANMENTMULTIPLIER,
	TORQUEREDUCTIONIGNITIONRETARD,
	THROTTLEPEDALSECONDARYUPVOLTAGE,
	THROTTLEPEDALSECONDARYWOTVOLTAGE,
	CANBAUDRATE,
	VEOVERRIDEMODE,
	CAN2BAUDRATE,
	AFROVERRIDEMODE,
	MC33_HPFP_I_PEAK,
	MC33_HPFP_I_HOLD,
	MC33_HPFP_I_HOLD_OFF,
	MC33_HPFP_MAX_HOLD,
	STEPPERDCINVERTEDPINS,
	CANOPENBLT,
	CAN2OPENBLT,
	INJECTORFLOWASMASSFLOW,
	BOARDUSECANTERMINATOR,
	KLINEDOHONDASEND,
	CAN1LISTENMODE,
	CAN2LISTENMODE,
	CAMDECODER2JZPOSITION,
	MC33810MAXDWELLTIMER,
	BENCHTESTONTIME,
	LAMBDAPROTECTIONRESTORETPS,
	LAMBDAPROTECTIONRESTORELOAD,
	LAUNCHACTIVATEPINMODE,
	ALIGNMENTFILL_AT_1543,
	CAN2TXPIN,
	CAN2RXPIN,
	STARTERCONTROLPINMODE,
	WASTEGATEPOSITIONSENSOR,
	IGNOVERRIDEMODE,
	INJECTORPRESSURETYPE,
	HPFPVALVEPIN,
	HPFPVALVEPINMODE,
	ALIGNMENTFILL_AT_1555,
	BOOSTCUTPRESSURE,
	TCHARGEBINS,
	TCHARGEVALUES,
	FIXEDTIMING,
	MAPLOWVALUEVOLTAGE,
	MAPHIGHVALUEVOLTAGE,
	EGOVALUESHIFT,
	VVTPINS1,
	VVTPINS2,
	VVTPINS3,
	VVTPINS4,
	CRANKINGIACPOSITION,
	TCHARGEMINRPMMINTPS,
	TCHARGEMINRPMMAXTPS,
	TCHARGEMAXRPMMINTPS,
	TCHARGEMAXRPMMAXTPS,
	VVTOUTPUTFREQUENCY,
	MINIMUMIGNITIONTIMING,
	MAXIMUMIGNITIONTIMING,
	ALTERNATORPWMFREQUENCY,
	VVTMODE1,
	VVTMODE2,
	FAN2EXTRAIDLE,
	PRIMINGDELAY,
	AUXANALOGINPUTS1,
	AUXANALOGINPUTS2,
	AUXANALOGINPUTS3,
	AUXANALOGINPUTS4,
	AUXANALOGINPUTS5,
	AUXANALOGINPUTS6,
	AUXANALOGINPUTS7,
	AUXANALOGINPUTS8,
	TRAILINGCOILPINS1,
	TRAILINGCOILPINS2,
	TRAILINGCOILPINS3,
	TRAILINGCOILPINS4,
	TRAILINGCOILPINS5,
	TRAILINGCOILPINS6,
	TRAILINGCOILPINS7,
	TRAILINGCOILPINS8,
	TRAILINGCOILPINS9,
	TRAILINGCOILPINS10,
	TRAILINGCOILPINS11,
	TRAILINGCOILPINS12,
	TLE8888MODE,
	ACCELEROMETERCSPINMODE,
	INJECTORCOMPENSATIONMODE,
	FAN2PINMODE,
	FUELREFERENCEPRESSURE,
	AUXTEMPSENSOR1_TEMPC_1,
	AUXTEMPSENSOR1_TEMPC_2,
	AUXTEMPSENSOR1_TEMPC_3,
	AUXTEMPSENSOR1_RESISTANCE_1,
	AUXTEMPSENSOR1_RESISTANCE_2,
	AUXTEMPSENSOR1_RESISTANCE_3,
	AUXTEMPSENSOR1_BIAS_RESISTOR,
	AUXTEMPSENSOR1_ADCCHANNEL,
	AUXTEMPSENSOR1_ALIGNMENTFILL_AT_29,
	AUXTEMPSENSOR2_TEMPC_1,
	AUXTEMPSENSOR2_TEMPC_2,
	AUXTEMPSENSOR2_TEMPC_3,
	AUXTEMPSENSOR2_RESISTANCE_1,
	AUXTEMPSENSOR2_RESISTANCE_2,
	AUXTEMPSENSOR2_RESISTANCE_3,
	AUXTEMPSENSOR2_BIAS_RESISTOR,
	AUXTEMPSENSOR2_ADCCHANNEL,
	AUXTEMPSENSOR2_ALIGNMENTFILL_AT_29,
	KNOCKSAMPLINGDURATION,
	ETBFREQ,
	ETBWASTEGATEPID_PFACTOR,
	ETBWASTEGATEPID_IFACTOR,
	ETBWASTEGATEPID_DFACTOR,
	ETBWASTEGATEPID_OFFSET,
	ETBWASTEGATEPID_PERIODMS,
	ETBWASTEGATEPID_MINVALUE,
	ETBWASTEGATEPID_MAXVALUE,
	STEPPERNUMMICROSTEPS,
	STEPPERMINDUTYCYCLE,
	STEPPERMAXDUTYCYCLE,
	SDCARDSPIDEVICE,
	TIMING_OFFSET_CYLINDER1,
	TIMING_OFFSET_CYLINDER2,
	TIMING_OFFSET_CYLINDER3,
	TIMING_OFFSET_CYLINDER4,
	TIMING_OFFSET_CYLINDER5,
	TIMING_OFFSET_CYLINDER6,
	TIMING_OFFSET_CYLINDER7,
	TIMING_OFFSET_CYLINDER8,
	TIMING_OFFSET_CYLINDER9,
	TIMING_OFFSET_CYLINDER10,
	TIMING_OFFSET_CYLINDER11,
	TIMING_OFFSET_CYLINDER12,
	IDLEPIDACTIVATIONTIME,
	SPI1SCKMODE,
	SPI1MOSIMODE,
	SPI1MISOMODE,
	SPI2SCKMODE,
	SPI2MOSIMODE,
	SPI2MISOMODE,
	SPI3SCKMODE,
	SPI3MOSIMODE,
	SPI3MISOMODE,
	STEPPERENABLEPINMODE,
	MC33816_RSTB,
	MC33816_DRIVEN,
	BRAKEPEDALPIN,
	AUXPID1_PFACTOR,
	AUXPID1_IFACTOR,
	AUXPID1_DFACTOR,
	AUXPID1_OFFSET,
	AUXPID1_PERIODMS,
	AUXPID1_MINVALUE,
	AUXPID1_MAXVALUE,
	AUXPID2_PFACTOR,
	AUXPID2_IFACTOR,
	AUXPID2_DFACTOR,
	AUXPID2_OFFSET,
	AUXPID2_PERIODMS,
	AUXPID2_MINVALUE,
	AUXPID2_MAXVALUE,
	INJECTORCORRECTIONPOLYNOMIAL1,
	INJECTORCORRECTIONPOLYNOMIAL2,
	INJECTORCORRECTIONPOLYNOMIAL3,
	INJECTORCORRECTIONPOLYNOMIAL4,
	INJECTORCORRECTIONPOLYNOMIAL5,
	INJECTORCORRECTIONPOLYNOMIAL6,
	INJECTORCORRECTIONPOLYNOMIAL7,
	INJECTORCORRECTIONPOLYNOMIAL8,
	PRIMEBINS,
	OILPRESSURE_HWCHANNEL,
	OILPRESSURE_ALIGNMENTFILL_AT_1,
	OILPRESSURE_V1,
	OILPRESSURE_VALUE1,
	OILPRESSURE_V2,
	OILPRESSURE_VALUE2,
	ACCELEROMETERSPIDEVICE,
	ALIGNMENTFILL_AT_1937,
	FAN2PIN,
	FAN2ONTEMPERATURE,
	FAN2OFFTEMPERATURE,
	STEPPERENABLEPIN,
	TLE8888_CS,
	TLE8888_CSPINMODE,
	ALIGNMENTFILL_AT_1947,
	MC33816_CS,
	ALIGNMENTFILL_AT_1950,
	AUXFREQUENCYFILTER,
	SENTINPUTPINS1,
	COASTINGFUELCUTRPMHIGH,
	COASTINGFUELCUTRPMLOW,
	COASTINGFUELCUTTPS,
	COASTINGFUELCUTCLT,
	PIDEXTRAFORLOWRPM,
	COASTINGFUELCUTMAP,
	ALIGNMENTFILL_AT_1970,
	HIGHPRESSUREFUEL_HWCHANNEL,
	HIGHPRESSUREFUEL_ALIGNMENTFILL_AT_1,
	HIGHPRESSUREFUEL_V1,
	HIGHPRESSUREFUEL_VALUE1,
	HIGHPRESSUREFUEL_V2,
	HIGHPRESSUREFUEL_VALUE2,
	LOWPRESSUREFUEL_HWCHANNEL,
	LOWPRESSUREFUEL_ALIGNMENTFILL_AT_1,
	LOWPRESSUREFUEL_V1,
	LOWPRESSUREFUEL_VALUE1,
	LOWPRESSUREFUEL_V2,
	LOWPRESSUREFUEL_VALUE2,
	SCRIPTCURVENAME1,
	SCRIPTCURVENAME2,
	SCRIPTCURVENAME3,
	SCRIPTCURVENAME4,
	SCRIPTCURVENAME5,
	SCRIPTCURVENAME6,
	SCRIPTTABLENAME1,
	SCRIPTTABLENAME2,
	SCRIPTTABLENAME3,
	SCRIPTTABLENAME4,
	SCRIPTSETTINGNAME1,
	SCRIPTSETTINGNAME2,
	SCRIPTSETTINGNAME3,
	SCRIPTSETTINGNAME4,
	SCRIPTSETTINGNAME5,
	SCRIPTSETTINGNAME6,
	SCRIPTSETTINGNAME7,
	SCRIPTSETTINGNAME8,
	TCHARGEAIRCOEFMIN,
	TCHARGEAIRCOEFMAX,
	TCHARGEAIRFLOWMAX,
	TCHARGEAIRINCRLIMIT,
	TCHARGEAIRDECRLIMIT,
	HIP9011GAIN,
	ETB_ITERMMIN,
	ETB_ITERMMAX,
	IDLETIMINGPID_PFACTOR,
	IDLETIMINGPID_IFACTOR,
	IDLETIMINGPID_DFACTOR,
	IDLETIMINGPID_OFFSET,
	IDLETIMINGPID_PERIODMS,
	IDLETIMINGPID_MINVALUE,
	IDLETIMINGPID_MAXVALUE,
	IDLETIMINGSOFTENTRYTIME,
	TORQUEREDUCTIONTRIGGERPINMODE,
	TORQUEREDUCTIONACTIVATIONMODE,
	TPSACCELFRACTIONPERIOD,
	TPSACCELFRACTIONDIVISOR,
	TLE8888SPIDEVICE,
	MC33816SPIDEVICE,
	IDLERPMPID_ITERMMIN,
	TLE6240SPIDEVICE,
	STOICHRATIOPRIMARY,
	IDLERPMPID_ITERMMAX,
	ETBIDLETHROTTLERANGE,
	CYLINDERBANKSELECT1,
	CYLINDERBANKSELECT2,
	CYLINDERBANKSELECT3,
	CYLINDERBANKSELECT4,
	CYLINDERBANKSELECT5,
	CYLINDERBANKSELECT6,
	CYLINDERBANKSELECT7,
	CYLINDERBANKSELECT8,
	CYLINDERBANKSELECT9,
	CYLINDERBANKSELECT10,
	CYLINDERBANKSELECT11,
	CYLINDERBANKSELECT12,
	PRIMEVALUES,
	TRIGGERCOMPCENTERVOLT,
	TRIGGERCOMPHYSTMIN,
	TRIGGERCOMPHYSTMAX,
	TRIGGERCOMPSENSORSATRPM,
	TRACTIONCONTROLSLIPBINS,
	TRACTIONCONTROLSPEEDBINS,
	DISABLEFAN1ATSPEED,
	DISABLEFAN2ATSPEED,
	CANVSSNBCTYPE,
	ALIGNMENTFILL_AT_2418,
	GPPWM1_PIN,
	GPPWM1_DUTYIFERROR,
	GPPWM1_ALIGNMENTFILL_AT_3,
	GPPWM1_PWMFREQUENCY,
	GPPWM1_ONABOVEDUTY,
	GPPWM1_OFFBELOWDUTY,
	GPPWM1_LOADAXIS,
	GPPWM1_RPMAXIS,
	GPPWM1_LOADBINS,
	GPPWM1_RPMBINS,
	GPPWM1_TABLE,
	GPPWM1_ALIGNMENTFILL_AT_106,
	GPPWM2_PIN,
	GPPWM2_DUTYIFERROR,
	GPPWM2_ALIGNMENTFILL_AT_3,
	GPPWM2_PWMFREQUENCY,
	GPPWM2_ONABOVEDUTY,
	GPPWM2_OFFBELOWDUTY,
	GPPWM2_LOADAXIS,
	GPPWM2_RPMAXIS,
	GPPWM2_LOADBINS,
	GPPWM2_RPMBINS,
	GPPWM2_TABLE,
	GPPWM2_ALIGNMENTFILL_AT_106,
	GPPWM3_PIN,
	GPPWM3_DUTYIFERROR,
	GPPWM3_ALIGNMENTFILL_AT_3,
	GPPWM3_PWMFREQUENCY,
	GPPWM3_ONABOVEDUTY,
	GPPWM3_OFFBELOWDUTY,
	GPPWM3_LOADAXIS,
	GPPWM3_RPMAXIS,
	GPPWM3_LOADBINS,
	GPPWM3_RPMBINS,
	GPPWM3_TABLE,
	GPPWM3_ALIGNMENTFILL_AT_106,
	GPPWM4_PIN,
	GPPWM4_DUTYIFERROR,
	GPPWM4_ALIGNMENTFILL_AT_3,
	GPPWM4_PWMFREQUENCY,
	GPPWM4_ONABOVEDUTY,
	GPPWM4_OFFBELOWDUTY,
	GPPWM4_LOADAXIS,
	GPPWM4_RPMAXIS,
	GPPWM4_LOADBINS,
	GPPWM4_RPMBINS,
	GPPWM4_TABLE,
	GPPWM4_ALIGNMENTFILL_AT_106,
	MC33_I_BOOST,
	MC33_I_PEAK,
	MC33_I_HOLD,
	MC33_T_MAX_BOOST,
	MC33_T_PEAK_OFF,
	MC33_T_PEAK_TOT,
	MC33_T_BYPASS,
	MC33_T_HOLD_OFF,
	MC33_T_HOLD_TOT,
	TCUUPSHIFTBUTTONPINMODE,
	TCUDOWNSHIFTBUTTONPINMODE,
	ACSWITCHMODE,
	TCU_SOLENOID_MODE1,
	TCU_SOLENOID_MODE2,
	TCU_SOLENOID_MODE3,
	TCU_SOLENOID_MODE4,
	TCU_SOLENOID_MODE5,
	TCU_SOLENOID_MODE6,
	ALIGNMENTFILL_AT_2879,
	TRIGGERGAPOVERRIDEFROM1,
	TRIGGERGAPOVERRIDEFROM2,
	TRIGGERGAPOVERRIDEFROM3,
	TRIGGERGAPOVERRIDEFROM4,
	TRIGGERGAPOVERRIDEFROM5,
	TRIGGERGAPOVERRIDEFROM6,
	TRIGGERGAPOVERRIDEFROM7,
	TRIGGERGAPOVERRIDEFROM8,
	TRIGGERGAPOVERRIDEFROM9,
	TRIGGERGAPOVERRIDEFROM10,
	TRIGGERGAPOVERRIDEFROM11,
	TRIGGERGAPOVERRIDEFROM12,
	TRIGGERGAPOVERRIDEFROM13,
	TRIGGERGAPOVERRIDEFROM14,
	TRIGGERGAPOVERRIDEFROM15,
	TRIGGERGAPOVERRIDEFROM16,
	TRIGGERGAPOVERRIDEFROM17,
	TRIGGERGAPOVERRIDEFROM18,
	TRIGGERGAPOVERRIDETO1,
	TRIGGERGAPOVERRIDETO2,
	TRIGGERGAPOVERRIDETO3,
	TRIGGERGAPOVERRIDETO4,
	TRIGGERGAPOVERRIDETO5,
	TRIGGERGAPOVERRIDETO6,
	TRIGGERGAPOVERRIDETO7,
	TRIGGERGAPOVERRIDETO8,
	TRIGGERGAPOVERRIDETO9,
	TRIGGERGAPOVERRIDETO10,
	TRIGGERGAPOVERRIDETO11,
	TRIGGERGAPOVERRIDETO12,
	TRIGGERGAPOVERRIDETO13,
	TRIGGERGAPOVERRIDETO14,
	TRIGGERGAPOVERRIDETO15,
	TRIGGERGAPOVERRIDETO16,
	TRIGGERGAPOVERRIDETO17,
	TRIGGERGAPOVERRIDETO18,
	MAXCAMPHASERESOLVERPM,
	DFCODELAY,
	ACDELAY,
	TCHARGEMODE,
	FORDINJECTORSMALLPULSEBREAKPOINT,
	ETBJAMDETECTTHRESHOLD,
	HPFPCAMLOBES,
	HPFPCAM,
	ACLOWRPMLIMIT,
	HPFPMINANGLE,
	ALIGNMENTFILL_AT_3035,
	HPFPPUMPVOLUME,
	HPFPACTIVATIONANGLE,
	ISSFILTERRECIPROCAL,
	HPFPPIDP,
	HPFPPIDI,
	HPFPTARGETDECAY,
	STEPPER_RAW_OUTPUT1,
	STEPPER_RAW_OUTPUT2,
	STEPPER_RAW_OUTPUT3,
	STEPPER_RAW_OUTPUT4,
	GEARRATIO1,
	GEARRATIO2,
	GEARRATIO3,
	GEARRATIO4,
	GEARRATIO5,
	GEARRATIO6,
	GEARRATIO7,
	GEARRATIO8,
	GEARRATIO9,
	GEARRATIO10,
	VVTACTIVATIONDELAYMS,
	TORQUETABLE,
	TORQUERPMBINS,
	TORQUELOADBINS,
	GEARCONTROLLERMODE,
	TRANSMISSIONCONTROLLERMODE,
	ACRDISABLEPHASE,
	AUXLINEAR1_HWCHANNEL,
	AUXLINEAR1_ALIGNMENTFILL_AT_1,
	AUXLINEAR1_V1,
	AUXLINEAR1_VALUE1,
	AUXLINEAR1_V2,
	AUXLINEAR1_VALUE2,
	AUXLINEAR2_HWCHANNEL,
	AUXLINEAR2_ALIGNMENTFILL_AT_1,
	AUXLINEAR2_V1,
	AUXLINEAR2_VALUE1,
	AUXLINEAR2_V2,
	AUXLINEAR2_VALUE2,
	TCU_TCC_ONOFF_SOLENOID,
	TCU_TCC_ONOFF_SOLENOID_MODE,
	ALIGNMENTFILL_AT_3183,
	TCU_TCC_PWM_SOLENOID,
	TCU_TCC_PWM_SOLENOID_MODE,
	ALIGNMENTFILL_AT_3187,
	TCU_TCC_PWM_SOLENOID_FREQ,
	TCU_PC_SOLENOID_PIN,
	TCU_PC_SOLENOID_PIN_MODE,
	ALIGNMENTFILL_AT_3193,
	TCU_PC_SOLENOID_FREQ,
	TCU_32_SOLENOID_PIN,
	TCU_32_SOLENOID_PIN_MODE,
	ALIGNMENTFILL_AT_3199,
	TCU_32_SOLENOID_FREQ,
	ACRPIN2,
	ETBMINIMUMPOSITION,
	ALIGNMENTFILL_AT_3205,
	TUNEHIDINGKEY,
	VINNUMBER,
	ALIGNMENTFILL_AT_3225,
	HIGHSPEEDOFFSETS,
	FUELPRESSURESENSORMODE,
	ALIGNMENTFILL_AT_3291,
	LUADIGITALINPUTPINS1,
	LUADIGITALINPUTPINS2,
	LUADIGITALINPUTPINS3,
	LUADIGITALINPUTPINS4,
	LUADIGITALINPUTPINS5,
	LUADIGITALINPUTPINS6,
	LUADIGITALINPUTPINS7,
	LUADIGITALINPUTPINS8,
	ALSMINRPM,
	ALSMAXRPM,
	ALSMAXDURATION,
	ALSMINCLT,
	ALSMAXCLT,
	ALSMINTIMEBETWEEN,
	ALSETBPOSITION,
	ACRELAYALTERNATORDUTYADDER,
	SENTETBTYPE,
	CUSTOMSENTTPSMIN,
	ALIGNMENTFILL_AT_3322,
	ALSIDLEADD,
	ALSETBADD,
	ALSSKIPRATIO,
	ACPRESSUREENABLEHYST,
	ALSACTIVATEPINMODE,
	TPSSECONDARYMAXIMUM,
	PPSSECONDARYMAXIMUM,
	LUADIGITALINPUTPINMODES1,
	LUADIGITALINPUTPINMODES2,
	LUADIGITALINPUTPINMODES3,
	LUADIGITALINPUTPINMODES4,
	LUADIGITALINPUTPINMODES5,
	LUADIGITALINPUTPINMODES6,
	LUADIGITALINPUTPINMODES7,
	LUADIGITALINPUTPINMODES8,
	CUSTOMSENTTPSMAX,
	KLINEBAUDRATE,
	CANGPIOTYPE,
	UIMODE,
	HPFPPEAKPOS,
	KLINEPERIODUS,
	RPMSOFTLIMITWINDOWSIZE,
	RPMSOFTLIMITTIMINGRETARD,
	RPMSOFTLIMITFUELADDED,
	RPMHARDLIMITHYST,
	BENCHTESTOFFTIME,
	BOOSTCUTPRESSUREHYST,
	GEARBASEDOPENLOOPBOOSTADDER,
	ALIGNMENTFILL_AT_3375,
	BENCHTESTCOUNT,
	IACBYTPSHOLDTIME,
	IACBYTPSDECAYTIME,
	TCU_RANGEINPUT1,
	TCU_RANGEINPUT2,
	TCU_RANGEINPUT3,
	TCU_RANGEINPUT4,
	TCU_RANGEINPUT5,
	TCU_RANGEINPUT6,
	TCU_RANGEINPUTMODE1,
	TCU_RANGEINPUTMODE2,
	TCU_RANGEINPUTMODE3,
	TCU_RANGEINPUTMODE4,
	TCU_RANGEINPUTMODE5,
	TCU_RANGEINPUTMODE6,
	CANVSSSCALING,
	ALIGNMENTFILL_AT_3402,
	OILTEMPSENSOR_TEMPC_1,
	OILTEMPSENSOR_TEMPC_2,
	OILTEMPSENSOR_TEMPC_3,
	OILTEMPSENSOR_RESISTANCE_1,
	OILTEMPSENSOR_RESISTANCE_2,
	OILTEMPSENSOR_RESISTANCE_3,
	OILTEMPSENSOR_BIAS_RESISTOR,
	OILTEMPSENSOR_ADCCHANNEL,
	OILTEMPSENSOR_ALIGNMENTFILL_AT_29,
	FUELTEMPSENSOR_TEMPC_1,
	FUELTEMPSENSOR_TEMPC_2,
	FUELTEMPSENSOR_TEMPC_3,
	FUELTEMPSENSOR_RESISTANCE_1,
	FUELTEMPSENSOR_RESISTANCE_2,
	FUELTEMPSENSOR_RESISTANCE_3,
	FUELTEMPSENSOR_BIAS_RESISTOR,
	FUELTEMPSENSOR_ADCCHANNEL,
	FUELTEMPSENSOR_ALIGNMENTFILL_AT_29,
	AMBIENTTEMPSENSOR_TEMPC_1,
	AMBIENTTEMPSENSOR_TEMPC_2,
	AMBIENTTEMPSENSOR_TEMPC_3,
	AMBIENTTEMPSENSOR_RESISTANCE_1,
	AMBIENTTEMPSENSOR_RESISTANCE_2,
	AMBIENTTEMPSENSOR_RESISTANCE_3,
	AMBIENTTEMPSENSOR_BIAS_RESISTOR,
	AMBIENTTEMPSENSOR_ADCCHANNEL,
	AMBIENTTEMPSENSOR_ALIGNMENTFILL_AT_29,
	COMPRESSORDISCHARGETEMPERATURE_TEMPC_1,
	COMPRESSORDISCHARGETEMPERATURE_TEMPC_2,
	COMPRESSORDISCHARGETEMPERATURE_TEMPC_3,
	COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_1,
	COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_2,
	COMPRESSORDISCHARGETEMPERATURE_RESISTANCE_3,
	COMPRESSORDISCHARGETEMPERATURE_BIAS_RESISTOR,
	COMPRESSORDISCHARGETEMPERATURE_ADCCHANNEL,
	COMPRESSORDISCHARGETEMPERATURE_ALIGNMENTFILL_AT_29,
	THROTTLEINLETPRESSURECHANNEL,
	COMPRESSORDISCHARGEPRESSURECHANNEL,
	DACOUTPUTPINS1,
	DACOUTPUTPINS2,
	SPEEDOMETEROUTPUTPIN,
	SPEEDOMETERPULSEPERKM,
	SIMULATORCAMPOSITION1,
	SIMULATORCAMPOSITION2,
	SIMULATORCAMPOSITION3,
	SIMULATORCAMPOSITION4,
	IGNKEYADCCHANNEL,
	SPI6MISOMODE,
	TRIGGERVVTGAPOVERRIDEFROM1,
	TRIGGERVVTGAPOVERRIDEFROM2,
	TRIGGERVVTGAPOVERRIDEFROM3,
	TRIGGERVVTGAPOVERRIDEFROM4,
	TRIGGERVVTGAPOVERRIDETO1,
	TRIGGERVVTGAPOVERRIDETO2,
	TRIGGERVVTGAPOVERRIDETO3,
	TRIGGERVVTGAPOVERRIDETO4,
	TRACTIONCONTROLETBDROP,
	MAXINJECTORDUTYINSTANT,
	MAXINJECTORDUTYSUSTAINED,
	MAXINJECTORDUTYSUSTAINEDTIMEOUT,
	ALIGNMENTFILL_AT_3619,
	INJECTIONPINSSTAGE21,
	INJECTIONPINSSTAGE22,
	INJECTIONPINSSTAGE23,
	INJECTIONPINSSTAGE24,
	INJECTIONPINSSTAGE25,
	INJECTIONPINSSTAGE26,
	INJECTIONPINSSTAGE27,
	INJECTIONPINSSTAGE28,
	INJECTIONPINSSTAGE29,
	INJECTIONPINSSTAGE210,
	INJECTIONPINSSTAGE211,
	INJECTIONPINSSTAGE212,
	TRACTIONCONTROLTIMINGDROP,
	TRACTIONCONTROLIGNITIONSKIP,
	AUXSPEED1MULTIPLIER,
	BRAKEMEANEFFECTIVEPRESSUREDIFFERENTIAL,
	SPI4MOSIPIN,
	SPI4MISOPIN,
	SPI4SCKPIN,
	SPI5MOSIPIN,
	SPI5MISOPIN,
	SPI5SCKPIN,
	SPI6MOSIPIN,
	SPI6MISOPIN,
	SPI6SCKPIN,
	SPI4SCKMODE,
	SPI4MOSIMODE,
	SPI4MISOMODE,
	SPI5SCKMODE,
	SPI5MOSIMODE,
	SPI5MISOMODE,
	SPI6SCKMODE,
	SPI6MOSIMODE,
	ALIGNMENTFILL_AT_3750,
	TUNERSTUDIOSERIALSPEED,
	CAMSIMULATORPIN,
	CAMSIMULATORPINMODE,
	ALIGNMENTFILL_AT_3759,
	ANOTHERCITEST,
	DEVICE_UID1,
	DEVICE_UID2,
	DEVICE_UID3,
	TCU_RANGEANALOGINPUT1,
	TCU_RANGEANALOGINPUT2,
	TCU_RANGEANALOGINPUT3,
	TCU_RANGEANALOGINPUT4,
	TCU_RANGEANALOGINPUT5,
	TCU_RANGEANALOGINPUT6,
	ALIGNMENTFILL_AT_3782,
	TCU_RANGESENSORBIASRESISTOR,
	MSIOBOX0_ID,
	MSIOBOX0_VSS,
	MSIOBOX0_ALIGNMENTFILL_AT_2,
	MC33810NOMI,
	MC33810MAXI,
	ALIGNMENTFILL_AT_3794,
	ACPRESSURE_HWCHANNEL,
	ACPRESSURE_ALIGNMENTFILL_AT_1,
	ACPRESSURE_V1,
	ACPRESSURE_VALUE1,
	ACPRESSURE_V2,
	ACPRESSURE_VALUE2,
	MINACPRESSURE,
	MAXACPRESSURE,
	MINIMUMOILPRESSURETIMEOUT,
	ALIGNMENTFILL_AT_3821,
	AUXLINEAR3_HWCHANNEL,
	AUXLINEAR3_ALIGNMENTFILL_AT_1,
	AUXLINEAR3_V1,
	AUXLINEAR3_VALUE1,
	AUXLINEAR3_V2,
	AUXLINEAR3_VALUE2,
	AUXLINEAR4_HWCHANNEL,
	AUXLINEAR4_ALIGNMENTFILL_AT_1,
	AUXLINEAR4_V1,
	AUXLINEAR4_VALUE1,
	AUXLINEAR4_V2,
	AUXLINEAR4_VALUE2,
	KNOCKSUPPRESSMINTPS,
	KNOCKFUELTRIMAGGRESSION,
	KNOCKFUELTRIMREAPPLYRATE,
	KNOCKFUELTRIM,
	KNOCKSPECTRUMSENSITIVITY,
	KNOCKFREQUENCY,
	SECONDARYINJECTORCOMPENSATIONMODE,
	ALIGNMENTFILL_AT_3877,
	SECONDARYINJECTORFUELREFERENCEPRESSURE,
	ETBSENTINPUT,
	FUELHIGHPRESSURESENTINPUT,
	FUELHIGHPRESSURESENTTYPE,
	ALIGNMENTFILL_AT_3887,
	NITROUSCONTROLENABLED,
	NITROUSCONTROLARMINGMETHOD,
	ALIGNMENTFILL_AT_3893,
	NITROUSCONTROLTRIGGERPIN,
	NITROUSCONTROLTRIGGERPINMODE,
	NITROUSLUAGAUGE,
	NITROUSLUAGAUGEMEANING,
	ALIGNMENTFILL_AT_3899,
	NITROUSLUAGAUGEARMINGVALUE,
	NITROUSMINIMUMTPS,
	NITROUSMINIMUMCLT,
	ALIGNMENTFILL_AT_3909,
	NITROUSMAXIMUMMAP,
	NITROUSMAXIMUMAFR,
	ALIGNMENTFILL_AT_3913,
	NITROUSACTIVATIONRPM,
	NITROUSDEACTIVATIONRPM,
	NITROUSDEACTIVATIONRPMWINDOW,
	DFCORETARDDEG,
	DFCORETARDRAMPINTIME,
	NITROUSRELAYPIN,
	NITROUSRELAYPINMODE,
	NITROUSFUELADDERPERCENT,
	ALIGNMENTFILL_AT_3926,
	NITROUSIGNITIONRETARD,
	NITROUSMINIMUMVEHICLESPEED,
	ALIGNMENTFILL_AT_3934,
	FUELLEVELAVERAGINGALPHA,
	FUELLEVELUPDATEPERIODSEC,
	FUELLEVELLOWTHRESHOLDVOLTAGE,
	FUELLEVELHIGHTHRESHOLDVOLTAGE,
	MAPEXPAVERAGEAFR,
	SPARKHARDWARELATENCYCORRECTION,
	MAXOILPRESSURETIMEOUT,
	ALIGNMENTFILL_AT_4010,
	POSTCRANKINGFACTOR,
	POSTCRANKINGDURATIONBINS,
	POSTCRANKINGCLTBINS,
	ETBBIASBINS,
	ETBBIASVALUES,
	IACPIDMULTTABLE,
	IACPIDMULTLOADBINS,
	IACPIDMULTRPMBINS,
	SPARKDWELLRPMBINS,
	SPARKDWELLVALUES,
	CLTIDLERPMBINS,
	CLTIDLERPM,
	IGNITIONCLTCORRTABLE,
	IGNITIONCLTCORRTEMPBINS,
	IGNITIONCLTCORRLOADBINS,
	SCRIPTCURVE1BINS,
	SCRIPTCURVE1,
	SCRIPTCURVE2BINS,
	SCRIPTCURVE2,
	SCRIPTCURVE3BINS,
	SCRIPTCURVE3,
	SCRIPTCURVE4BINS,
	SCRIPTCURVE4,
	SCRIPTCURVE5BINS,
	SCRIPTCURVE5,
	SCRIPTCURVE6BINS,
	SCRIPTCURVE6,
	BAROCORRPRESSUREBINS,
	BAROCORRRPMBINS,
	BAROCORRTABLE,
	CRANKINGTPSCOEF,
	CRANKINGTPSBINS,
	CRANKINGADVANCEBINS,
	CRANKINGADVANCE,
	IACCOASTINGRPMBINS,
	IACCOASTING,
	BOOSTTABLEOPENLOOP,
	BOOSTRPMBINS,
	BOOSTOPENLOOPLOADBINS,
	BOOSTTABLECLOSEDLOOP,
	BOOSTCLOSEDLOOPLOADBINS,
	PEDALTOTPSTABLE,
	PEDALTOTPSPEDALBINS,
	PEDALTOTPSRPMBINS,
	CLTCRANKINGCORRBINS,
	CLTCRANKINGCORR,
	IDLEADVANCEBINS,
	IDLEADVANCE,
	IDLEVERPMBINS,
	IDLEVELOADBINS,
	IDLEVETABLE,
	LUASCRIPT,
	CLTFUELCORRBINS,
	CLTFUELCORR,
	IATFUELCORRBINS,
	IATFUELCORR,
	CRANKINGFUELCOEF,
	CRANKINGFUELBINS,
	CRANKINGCYCLEBINS,
	CRANKINGCYCLEFUELCLTBINS,
	CRANKINGCYCLEFUELCOEF,
	CLTIDLECORRBINS,
	CLTIDLECORR,
	MAFDECODING,
	MAFDECODINGBINS,
	IGNITIONIATCORRTABLE,
	IGNITIONIATCORRTEMPBINS,
	IGNITIONIATCORRLOADBINS,
	INJECTIONPHASE,
	INJPHASELOADBINS,
	INJPHASERPMBINS,
	TCUSOLENOIDTABLE,
	MAPESTIMATETABLE,
	MAPESTIMATETPSBINS,
	MAPESTIMATERPMBINS,
	VVTTABLE1,
	VVTTABLE1LOADBINS,
	VVTTABLE1RPMBINS,
	VVTTABLE2,
	VVTTABLE2LOADBINS,
	VVTTABLE2RPMBINS,
	IGNITIONTABLE,
	IGNITIONLOADBINS,
	IGNITIONRPMBINS,
	VETABLE,
	VELOADBINS,
	VERPMBINS,
	LAMBDATABLE,
	LAMBDALOADBINS,
	LAMBDARPMBINS,
	TPSTPSACCELTABLE,
	TPSTPSACCELFROMRPMBINS,
	TPSTPSACCELTORPMBINS,
	SCRIPTTABLE1,
	SCRIPTTABLE1LOADBINS,
	SCRIPTTABLE1RPMBINS,
	SCRIPTTABLE2,
	SCRIPTTABLE2LOADBINS,
	SCRIPTTABLE2RPMBINS,
	SCRIPTTABLE3,
	SCRIPTTABLE3LOADBINS,
	SCRIPTTABLE3RPMBINS,
	SCRIPTTABLE4,
	SCRIPTTABLE4LOADBINS,
	SCRIPTTABLE4RPMBINS,
	IGNTRIMLOADBINS,
	IGNTRIMRPMBINS,
	IGNTRIMS1_TABLE,
	IGNTRIMS2_TABLE,
	IGNTRIMS3_TABLE,
	IGNTRIMS4_TABLE,
	IGNTRIMS5_TABLE,
	IGNTRIMS6_TABLE,
	IGNTRIMS7_TABLE,
	IGNTRIMS8_TABLE,
	IGNTRIMS9_TABLE,
	IGNTRIMS10_TABLE,
	IGNTRIMS11_TABLE,
	IGNTRIMS12_TABLE,
	FUELTRIMLOADBINS,
	FUELTRIMRPMBINS,
	FUELTRIMS1_TABLE,
	FUELTRIMS2_TABLE,
	FUELTRIMS3_TABLE,
	FUELTRIMS4_TABLE,
	FUELTRIMS5_TABLE,
	FUELTRIMS6_TABLE,
	FUELTRIMS7_TABLE,
	FUELTRIMS8_TABLE,
	FUELTRIMS9_TABLE,
	FUELTRIMS10_TABLE,
	FUELTRIMS11_TABLE,
	FUELTRIMS12_TABLE,
	CRANKINGFUELCOEFE100,
	TCU_PCAIRMASSBINS,
	TCU_PCVALSR,
	TCU_PCVALSN,
	TCU_PCVALS1,
	TCU_PCVALS2,
	TCU_PCVALS3,
	TCU_PCVALS4,
	TCU_PCVALS12,
	TCU_PCVALS23,
	TCU_PCVALS34,
	TCU_PCVALS21,
	TCU_PCVALS32,
	TCU_PCVALS43,
	TCU_TCCTPSBINS,
	TCU_TCCLOCKSPEED,
	TCU_TCCUNLOCKSPEED,
	TCU_32SPEEDBINS,
	TCU_32VALS,
	THROTTLE2TRIMTABLE,
	THROTTLE2TRIMTPSBINS,
	THROTTLE2TRIMRPMBINS,
	MAXKNOCKRETARDTABLE,
	MAXKNOCKRETARDLOADBINS,
	MAXKNOCKRETARDRPMBINS,
	ALSTIMINGRETARDTABLE,
	ALSIGNRETARDLOADBINS,
	ALSIGNRETARDRPMBINS,
	ALSFUELADJUSTMENT,
	ALSFUELADJUSTMENTLOADBINS,
	ALSFUELADJUSTMENTRPMBINS,
	ALSIGNSKIPTABLE,
	ALSIGNSKIPLOADBINS,
	ALSIGNSKIPRPMBINS,
	IGNBLENDS1_TABLE,
	IGNBLENDS1_LOADBINS,
	IGNBLENDS1_RPMBINS,
	IGNBLENDS1_BLENDPARAMETER,
	IGNBLENDS1_YAXISOVERRIDE,
	IGNBLENDS1_BLENDBINS,
	IGNBLENDS1_BLENDVALUES,
	IGNBLENDS1_ALIGNMENTFILL_AT_186,
	IGNBLENDS2_TABLE,
	IGNBLENDS2_LOADBINS,
	IGNBLENDS2_RPMBINS,
	IGNBLENDS2_BLENDPARAMETER,
	IGNBLENDS2_YAXISOVERRIDE,
	IGNBLENDS2_BLENDBINS,
	IGNBLENDS2_BLENDVALUES,
	IGNBLENDS2_ALIGNMENTFILL_AT_186,
	IGNBLENDS3_TABLE,
	IGNBLENDS3_LOADBINS,
	IGNBLENDS3_RPMBINS,
	IGNBLENDS3_BLENDPARAMETER,
	IGNBLENDS3_YAXISOVERRIDE,
	IGNBLENDS3_BLENDBINS,
	IGNBLENDS3_BLENDVALUES,
	IGNBLENDS3_ALIGNMENTFILL_AT_186,
	IGNBLENDS4_TABLE,
	IGNBLENDS4_LOADBINS,
	IGNBLENDS4_RPMBINS,
	IGNBLENDS4_BLENDPARAMETER,
	IGNBLENDS4_YAXISOVERRIDE,
	IGNBLENDS4_BLENDBINS,
	IGNBLENDS4_BLENDVALUES,
	IGNBLENDS4_ALIGNMENTFILL_AT_186,
	VEBLENDS1_TABLE,
	VEBLENDS1_LOADBINS,
	VEBLENDS1_RPMBINS,
	VEBLENDS1_BLENDPARAMETER,
	VEBLENDS1_YAXISOVERRIDE,
	VEBLENDS1_BLENDBINS,
	VEBLENDS1_BLENDVALUES,
	VEBLENDS1_ALIGNMENTFILL_AT_186,
	VEBLENDS2_TABLE,
	VEBLENDS2_LOADBINS,
	VEBLENDS2_RPMBINS,
	VEBLENDS2_BLENDPARAMETER,
	VEBLENDS2_YAXISOVERRIDE,
	VEBLENDS2_BLENDBINS,
	VEBLENDS2_BLENDVALUES,
	VEBLENDS2_ALIGNMENTFILL_AT_186,
	VEBLENDS3_TABLE,
	VEBLENDS3_LOADBINS,
	VEBLENDS3_RPMBINS,
	VEBLENDS3_BLENDPARAMETER,
	VEBLENDS3_YAXISOVERRIDE,
	VEBLENDS3_BLENDBINS,
	VEBLENDS3_BLENDVALUES,
	VEBLENDS3_ALIGNMENTFILL_AT_186,
	VEBLENDS4_TABLE,
	VEBLENDS4_LOADBINS,
	VEBLENDS4_RPMBINS,
	VEBLENDS4_BLENDPARAMETER,
	VEBLENDS4_YAXISOVERRIDE,
	VEBLENDS4_BLENDBINS,
	VEBLENDS4_BLENDVALUES,
	VEBLENDS4_ALIGNMENTFILL_AT_186,
	THROTTLEESTIMATEEFFECTIVEAREABINS,
	THROTTLEESTIMATEEFFECTIVEAREAVALUES,
	BOOSTOPENLOOPBLENDS1_TABLE,
	BOOSTOPENLOOPBLENDS1_LOADBINS,
	BOOSTOPENLOOPBLENDS1_RPMBINS,
	BOOSTOPENLOOPBLENDS1_BLENDPARAMETER,
	BOOSTOPENLOOPBLENDS1_YAXISOVERRIDE,
	BOOSTOPENLOOPBLENDS1_BLENDBINS,
	BOOSTOPENLOOPBLENDS1_BLENDVALUES,
	BOOSTOPENLOOPBLENDS1_ALIGNMENTFILL_AT_186,
	BOOSTOPENLOOPBLENDS2_TABLE,
	BOOSTOPENLOOPBLENDS2_LOADBINS,
	BOOSTOPENLOOPBLENDS2_RPMBINS,
	BOOSTOPENLOOPBLENDS2_BLENDPARAMETER,
	BOOSTOPENLOOPBLENDS2_YAXISOVERRIDE,
	BOOSTOPENLOOPBLENDS2_BLENDBINS,
	BOOSTOPENLOOPBLENDS2_BLENDVALUES,
	BOOSTOPENLOOPBLENDS2_ALIGNMENTFILL_AT_186,
	BOOSTCLOSEDLOOPBLENDS1_TABLE,
	BOOSTCLOSEDLOOPBLENDS1_LOADBINS,
	BOOSTCLOSEDLOOPBLENDS1_RPMBINS,
	BOOSTCLOSEDLOOPBLENDS1_BLENDPARAMETER,
	BOOSTCLOSEDLOOPBLENDS1_YAXISOVERRIDE,
	BOOSTCLOSEDLOOPBLENDS1_BLENDBINS,
	BOOSTCLOSEDLOOPBLENDS1_BLENDVALUES,
	BOOSTCLOSEDLOOPBLENDS1_ALIGNMENTFILL_AT_186,
	BOOSTCLOSEDLOOPBLENDS2_TABLE,
	BOOSTCLOSEDLOOPBLENDS2_LOADBINS,
	BOOSTCLOSEDLOOPBLENDS2_RPMBINS,
	BOOSTCLOSEDLOOPBLENDS2_BLENDPARAMETER,
	BOOSTCLOSEDLOOPBLENDS2_YAXISOVERRIDE,
	BOOSTCLOSEDLOOPBLENDS2_BLENDBINS,
	BOOSTCLOSEDLOOPBLENDS2_BLENDVALUES,
	BOOSTCLOSEDLOOPBLENDS2_ALIGNMENTFILL_AT_186,
	TCU_RANGEP,
	TCU_RANGER,
	TCU_RANGEN,
	TCU_RANGED,
	TCU_RANGEM,
	TCU_RANGEM3,
	TCU_RANGEM2,
	TCU_RANGEM1,
	TCU_RANGEPLUS,
	TCU_RANGEMINUS,
	TCU_RANGELOW,
	LAMBDAMAXDEVIATIONTABLE,
	LAMBDAMAXDEVIATIONLOADBINS,
	LAMBDAMAXDEVIATIONRPMBINS,
	INJECTORSTAGINGTABLE,
	INJECTORSTAGINGLOADBINS,
	INJECTORSTAGINGRPMBINS,
	WWCLTBINS,
	WWTAUCLTVALUES,
	WWBETACLTVALUES,
	WWMAPBINS,
	WWTAUMAPVALUES,
	WWBETAMAPVALUES,
	HPFPLOBEPROFILEQUANTITYBINS,
	HPFPLOBEPROFILEANGLE,
	HPFPDEADTIMEVOLTSBINS,
	HPFPDEADTIMEMS,
	HPFPTARGET,
	HPFPTARGETLOADBINS,
	HPFPTARGETRPMBINS,
	HPFPCOMPENSATION,
	HPFPCOMPENSATIONLOADBINS,
	HPFPCOMPENSATIONRPMBINS,
	HPFPFUELMASSCOMPENSATION,
	HPFPFUELMASSCOMPENSATIONFUELMASS,
	HPFPFUELMASSCOMPENSATIONFUELPRESSURE,
	KNOCKNOISERPMBINS,
	KNOCKBASENOISE,
	TPSTSPCORRVALUESBINS,
	TPSTSPCORRVALUES,
	CLTREVLIMITRPMBINS,
	CLTREVLIMITRPM,
	FUELLEVELBINS,
	FUELLEVELVALUES,
	DWELLVOLTAGECORRVOLTBINS,
	DWELLVOLTAGECORRVALUES,
	TCU_SHIFTTPSBINS,
	TCU_SHIFTSPEED12,
	TCU_SHIFTSPEED23,
	TCU_SHIFTSPEED34,
	TCU_SHIFTSPEED21,
	TCU_SHIFTSPEED32,
	TCU_SHIFTSPEED43,
	TCU_SHIFTTIME,
	ALTERNATORVOLTAGETARGETTABLE,
	ALTERNATORVOLTAGETARGETLOADBINS,
	ALTERNATORVOLTAGETARGETRPMBINS,
	CLTBOOSTCORRBINS,
	CLTBOOSTCORR,
	IATBOOSTCORRBINS,
	IATBOOSTCORR,
	CLTBOOSTADDERBINS,
	CLTBOOSTADDER,
	IATBOOSTADDERBINS,
	IATBOOSTADDER,
	MINIMUMOILPRESSUREBINS,
	MINIMUMOILPRESSUREVALUES,
	TARGETAFRBLENDS1_TABLE,
	TARGETAFRBLENDS1_LOADBINS,
	TARGETAFRBLENDS1_RPMBINS,
	TARGETAFRBLENDS1_BLENDPARAMETER,
	TARGETAFRBLENDS1_YAXISOVERRIDE,
	TARGETAFRBLENDS1_BLENDBINS,
	TARGETAFRBLENDS1_BLENDVALUES,
	TARGETAFRBLENDS1_ALIGNMENTFILL_AT_186,
	TARGETAFRBLENDS2_TABLE,
	TARGETAFRBLENDS2_LOADBINS,
	TARGETAFRBLENDS2_RPMBINS,
	TARGETAFRBLENDS2_BLENDPARAMETER,
	TARGETAFRBLENDS2_YAXISOVERRIDE,
	TARGETAFRBLENDS2_BLENDBINS,
	TARGETAFRBLENDS2_BLENDVALUES,
	TARGETAFRBLENDS2_ALIGNMENTFILL_AT_186,
	TRIMRPMBINS,
	TRIMLOADBINS,
	DYNORPMSTEP,
	DYNOSAETEMPERATUREC,
	DYNOSAERELATIVEHUMIDITY,
	ALIGNMENTFILL_AT_23471,
	DYNOSAEBARO,
	DYNOCARWHEELDIAINCH,
	DYNOCARWHEELASPECTRATIO,
	DYNOCARWHEELTIREWIDTHMM,
	DYNOCARGEARPRIMARYREDUCTION,
	DYNOCARGEARRATIO,
	DYNOCARGEARFINALDRIVE,
	DYNOCARCARMASSKG,
	DYNOCARCARGOMASSKG,
	DYNOCARCOEFFOFDRAG,
	DYNOCARFRONTALAREAM2,
	TRAILINGSPARKTABLE,
	TRAILINGSPARKRPMBINS,
	TRAILINGSPARKLOADBINS,
	MAXIMUMOILPRESSUREBINS,
	MAXIMUMOILPRESSUREVALUES,
	};
}
