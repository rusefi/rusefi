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
    TIMING("Timing", "deg", -180, 180),

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
    ADVANCE0("Input Adv #1", "deg", -180, 180, BackgroundColor.BROWN),
    ADVANCE1("Input Adv #2", "deg", -180, 180, BackgroundColor.BROWN),
    ADVANCE2("Input Adv #3", "deg", -180, 180, BackgroundColor.BROWN),
    ADVANCE3("Input Adv #4", "deg", -180, 180, BackgroundColor.BROWN),
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
    INJECTOR_1_DWELL("inj #1"),
    INJECTOR_2_DWELL("inj #2"),


    INJ_1_2_DELTA("inj 1-2 delta"),;

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

    public static double processAdvance(double advance) {
        return advance > 360 ? advance - 720 : advance;
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
