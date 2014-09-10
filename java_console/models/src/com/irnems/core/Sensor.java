package com.irnems.core;

import eu.hansolo.steelseries.tools.BackgroundColor;

/**
 * @author Andrey Belomutskiy
 *         2/11/13
 */
public enum Sensor {
    RPM("RPM"),
    MAP("MAP"),
    MAP_RAW("MAP_RAW"),
    BARO("Baro"),
    TIMING("Timing"),

    /**
     * Please note that these enum names are used to make 'set_mock_XXX_voltage' commands
     */
    CLT("Coolant", "temperature, C", -40, 300),
    IAT("Intake Air", "temperature, C", -40, 150),
    AFR("A/F ratio", "", 0, 20),
    MAF("MAF", "Volts", 4),
    TPS("throttle", "%", 100),

    TRG_0_DUTY("trg0 duty", "%", 100),
    TRG_1_DUTY("trg1 duty", "%", 100),

//    COOLANT_WIDTH("c w", "", 30),
//    INTAKE_AIR_WIDTH("air w", "", 30),

    TABLE_SPARK("table spark", "ms", -40, 40),
    VREF("VRef", "Volts", 6),
    VBATT("VBatt", "Volts", 18),
    VREF_WIDTH("VRef w", "", 1),
    DWELL0("Input dwl #1", "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL1("Input dwl #2", "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL2("Input dwl #3", "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL3("Input dwl #4", "ms", 0, 30, BackgroundColor.BEIGE),
    DWELL0_SD("Input d #1", "std dev", 100),


    TOTAL_DWELL0("Input tdwl #1", "ms", 0, 30, BackgroundColor.BEIGE),
    TOTAL_DWELL1("Input tdwl #2", "ms", 0, 30, BackgroundColor.BEIGE),
    TOTAL_DWELL2("Input tdwl #3", "ms", 0, 30, BackgroundColor.BEIGE),
    TOTAL_DWELL3("Input tdwl #4", "ms", 0, 30, BackgroundColor.BEIGE),
    ADVANCE0("Input Adv #1", "dg", -40, 40, BackgroundColor.BROWN),
    ADVANCE1("Input Adv #2", "dg", -40, 40, BackgroundColor.BROWN),
    ADVANCE2("Input Adv #3", "dg", -40, 40, BackgroundColor.BROWN),
    ADVANCE3("Input Adv #4", "dg", -40, 40, BackgroundColor.BROWN),
    PERIOD0("Period", "dg", 0, 400),
    DUTY0("Duty0", "%", 0, 100, BackgroundColor.RED),
    DUTY1("Duty1", "%", 0, 100, BackgroundColor.RED),
    FUEL("Fuel", "ms", 0, 30),
    FUEL_BASE("Fuel Base", "ms", 0, 30),
    FUEL_IAT("F IAT", "", 0, 10),
    FUEL_CLT("F CLT", "", 0, 10),
    FUEL_LAG("F Lag", "", 0, 30),

    IDLE_SWITCH("idle switch"),

    DEFAULT_FUEL("map fuel", "ms", 0, 40),
    T_CHARGE("T Charge", "f", 0, 200),

    CHARTSIZE("CHARTSIZE"),
    CHART_STATUS("CHART_STATUS"),
    ADC_STATUS("ADC_STATUS"),

    ADC_FAST("ADC_FAST", "b", 4000),
    ADC_FAST_AVG("ADC_FAST_AVG", "b", 4000),
    ;

    private final String name;
    private final String units;
    private final double minValue;
    private final double maxValue;
    private final BackgroundColor color;

    Sensor(String name) {
        this(name, "", 255);
    }

    Sensor(String name, String units, double maxValue) {
        this(name, units, 0, maxValue);
    }

    Sensor(String name, String units, double minValue, double maxValue) {
        this(name, units, minValue, maxValue, BackgroundColor.LIGHT_GRAY);
    }

    Sensor(String name, String units, double minValue, double maxValue, BackgroundColor color) {
        this.name = name;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
        this.color = color;
    }

    public String getName() {
        return name;
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
}
