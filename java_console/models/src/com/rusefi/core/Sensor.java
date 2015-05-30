package com.rusefi.core;

import eu.hansolo.steelseries.tools.BackgroundColor;

import java.util.ArrayList;

/**
 * @author Andrey Belomutskiy
 *         2/11/13
 */
public enum Sensor {
    RPM("RPM", SensorCategory.SENSOR_INPUTS, "rpm", 8000),
    MAP("MAP", SensorCategory.SENSOR_INPUTS),
    MAP_RAW("MAP_RAW", SensorCategory.SENSOR_INPUTS),
    BARO("Baro", SensorCategory.SENSOR_INPUTS),
    TIMING("Timing Advance", SensorCategory.OPERATIONS, "deg", -180, 180),

    /**
     * Please note that these enum names are used to make 'set_mock_XXX_voltage' commands
     */
    CLT("Coolant", SensorCategory.SENSOR_INPUTS, "temperature, C", -40, 300),
    IAT("Intake Air", SensorCategory.SENSOR_INPUTS, "temperature, C", -40, 150),
    AFR("A/F ratio", SensorCategory.SENSOR_INPUTS, "", 0, 20),
    MAF("MAF", SensorCategory.SENSOR_INPUTS, "Volts", 4),
    MAFR("MAFR", SensorCategory.SENSOR_INPUTS, "kg/hr", 4),
    TPS("throttle", SensorCategory.SENSOR_INPUTS, "%", 100),
    PPS("pedal", SensorCategory.SENSOR_INPUTS, "%", 100),
    VSS("Speed", SensorCategory.SENSOR_INPUTS, "kph", 100),

    ENGINE_LOAD("Engine Load", SensorCategory.SENSOR_INPUTS, "x", 300),


    MAFR_CFM("MAFR_CFM", SensorCategory.SENSOR_INPUTS, "cub f/m", 800),
    CLT_F("Coolant F", SensorCategory.SENSOR_INPUTS, "temperature, F", -40, 500),
    IAT_F("Intake Air F", SensorCategory.SENSOR_INPUTS, "temperature, F", -40, 350),

    TRG_0_DUTY("trg0 duty", SensorCategory.SENSOR_INPUTS, "%", 100),
    TRG_1_DUTY("trg1 duty", SensorCategory.SENSOR_INPUTS, "%", 100),

//    COOLANT_WIDTH("c w", "", 30),
//    INTAKE_AIR_WIDTH("air w", "", 30),

    TABLE_SPARK("table spark", SensorCategory.OPERATIONS, "ms", -40, 40),
    VREF("VRef", SensorCategory.SENSOR_INPUTS, "Volts", 6),
    VBATT("VBatt", SensorCategory.SENSOR_INPUTS, "Volts", 18),
    VREF_WIDTH("VRef w", SensorCategory.OTHERS, "", 1),
    DWELL0("Input dwl #1", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL1("Input dwl #2", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL2("Input dwl #3", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL3("Input dwl #4", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL0_SD("Input d #1", SensorCategory.SNIFFING, "std dev", 100),


    TOTAL_DWELL0("Input tdwl #1", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    TOTAL_DWELL1("Input tdwl #2", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    TOTAL_DWELL2("Input tdwl #3", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    TOTAL_DWELL3("Input tdwl #4", SensorCategory.SNIFFING, "ms", 0, 30, BackgroundColor.BEIGE),
    ADVANCE0("Input Adv #1", SensorCategory.SNIFFING, "deg", -180, 180, BackgroundColor.BROWN),
    ADVANCE1("Input Adv #2", SensorCategory.SNIFFING, "deg", -180, 180, BackgroundColor.BROWN),
    ADVANCE2("Input Adv #3", SensorCategory.SNIFFING, "deg", -180, 180, BackgroundColor.BROWN),
    ADVANCE3("Input Adv #4", SensorCategory.SNIFFING, "deg", -180, 180, BackgroundColor.BROWN),
    PERIOD0("Period", SensorCategory.SNIFFING, "dg", 0, 400),
    DUTY0("Duty0", SensorCategory.SNIFFING, "%", 0, 100, BackgroundColor.RED),
    DUTY1("Duty1", SensorCategory.SNIFFING, "%", 0, 100, BackgroundColor.RED),
    FUEL("Fuel", SensorCategory.OPERATIONS, "ms", 0, 30),
    FUEL_BASE("Fuel Base", SensorCategory.OPERATIONS, "ms", 0, 30),
    FUEL_IAT("F IAT", SensorCategory.OPERATIONS, "", 0, 10),
    FUEL_CLT("F CLT", SensorCategory.OPERATIONS, "", 0, 10),
    FUEL_LAG("F Lag", SensorCategory.OPERATIONS, "", 0, 30),

    IDLE_SWITCH("idle switch", SensorCategory.OTHERS),

    DEFAULT_FUEL("map fuel", SensorCategory.OPERATIONS, "ms", 0, 40),
    T_CHARGE("T Charge", SensorCategory.OPERATIONS, "f", 0, 200),

    CHARTSIZE("CHARTSIZE", SensorCategory.OTHERS),
    CHART_STATUS("CHART_STATUS", SensorCategory.OTHERS),
    ADC_STATUS("ADC_STATUS", SensorCategory.OTHERS),

    ADC_FAST("ADC_FAST", SensorCategory.OTHERS, "b", 4000),
    ADC_FAST_AVG("ADC_FAST_AVG", SensorCategory.OTHERS, "b", 4000),
    INJECTOR_1_DWELL("inj #1", SensorCategory.SNIFFING),
    INJECTOR_2_DWELL("inj #2", SensorCategory.SNIFFING),
    INJECTOR_3_DWELL("inj #3", SensorCategory.SNIFFING),
    INJECTOR_4_DWELL("inj #4", SensorCategory.SNIFFING),


    INJ_1_2_DELTA("inj 1-2 delta", SensorCategory.SNIFFING),
    INJ_3_4_DELTA("inj 3-4 delta", SensorCategory.SNIFFING),
    ;

    private final String name;
    private final SensorCategory category;
    private final String units;
    private final double minValue;
    private final double maxValue;
    private final BackgroundColor color;

    Sensor(String name, SensorCategory category) {
        this(name, category, "", 255);
    }

    Sensor(String name, SensorCategory category, String units, double maxValue) {
        this(name, category, units, 0, maxValue);
    }

    Sensor(String name, SensorCategory category, String units, double minValue, double maxValue) {
        this(name, category, units, minValue, maxValue, BackgroundColor.LIGHT_GRAY);
    }

    Sensor(String name, SensorCategory category, String units, double minValue, double maxValue, BackgroundColor color) {
        this.name = name;
        this.category = category;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
        this.color = color;
    }

    public static ArrayList<Sensor> getSensorsForCategory(String category) {
        final ArrayList<Sensor> sensors = new ArrayList<Sensor>();

        for (final Sensor sensor : values()) {
            if (sensor.category.getName().equals(category)) {
                sensors.add(sensor);
            }
        }

        return sensors;
    }

    public static double processAdvance(double advance) {
        return advance > 360 ? advance - 720 : advance;
    }

    public String getName() {
        return name;
    }

    public SensorCategory getCategory() {
        return category;
    }

    public String getUnits() {
        return units;
    }

    public double getMinValue() {
        return minValue;
    }

    public double getMaxValue() {
        return maxValue;
    }

    public BackgroundColor getColor() {
        return color;
    }

    public double translateValue(double value) {
        switch (this) {
            case ADVANCE0:
            case ADVANCE1:
            case ADVANCE2:
            case ADVANCE3:
            case TIMING:
                return processAdvance(value);
            default:
                return value;
        }
    }
}
