package com.rusefi.config;

/**
 * todo: make this file completely auto-generated
 */

public class Fields {
    public static final Field SENSORCHARTFREQUENCY = new Field(@@sensorChartFrequency_offset@@, FieldType.INT);
    public static final Field SENSORCHARTMODE = new Field(@@sensorChartMode_offset@@, FieldType.INT, @@sensor_chart_e_enum@@);
    public static final Field GLOBALFUELCORRECTION = new Field(@@globalFuelCorrection_offset@@, FieldType.FLOAT);
    public static final Field ENGINECHARTSIZE = new Field(@@engineChartSize_offset@@, FieldType.INT);
    public static final Field ISENGINECHARTENABLED = new Field(@@isEngineChartEnabled_offset@@, FieldType.BIT, 5);
    public static final Field TRIGGER_TYPE = new Field(@@trigger_type_offset@@, FieldType.INT, @@trigger_type_e_enum@@);
    public static final Field ALGORITHM = new Field(@@algorithm_offset@@, FieldType.INT, @@algo_enum@@);
    
    public static final Field GLOBALTRIGGERANGLEOFFSET = new Field(@@globalTriggerAngleOffset_offset@@, FieldType.FLOAT);
    
    public static final Field TRIGGERINPUTPINS1 = new Field(@@triggerInputPins1_offset@@, FieldType.INT, @@brain_input_pin_e_enum@@);
    public static final Field TRIGGERINPUTPINS2 = new Field(@@triggerInputPins2_offset@@, FieldType.INT, @@brain_input_pin_e_enum@@);
    public static final Field TRIGGERINPUTPINS3 = new Field(@@triggerInputPins3_offset@@, FieldType.INT, @@brain_input_pin_e_enum@@);
    public static final Field INJECTIONPINS1 = new Field(@@injectionPins1_offset@@, FieldType.INT, @@brain_pin_e_enum@@);
}