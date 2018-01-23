package com.rusefi.core;

import com.rusefi.config.FieldType;
import com.rusefi.config.Fields;
import eu.hansolo.steelseries.tools.BackgroundColor;

import java.util.Collection;
import java.util.Comparator;
import java.util.Set;
import java.util.TreeSet;

import static com.rusefi.config.Fields.*;

/**
 * @author Andrey Belomutskiy
 *         2/11/13
 */
public enum Sensor {
    MAP("MAP", SensorCategory.SENSOR_INPUTS),
    MAP_RAW("MAP_RAW", SensorCategory.SENSOR_INPUTS),
    BARO("Baro", SensorCategory.SENSOR_INPUTS),
    // todo: unify with ignitionAdvance
    TIMING("Timing Advance", SensorCategory.OPERATIONS, "deg", -180, 180),

    /**
     * Please note that these enum names are used to make 'set_mock_XXX_voltage' commands
     */
    CLT("Coolant", SensorCategory.SENSOR_INPUTS, "temperature, C", -40, 300),
    AFR("A/F ratio", SensorCategory.SENSOR_INPUTS, "", 0, 20),
    MAF("MAF", SensorCategory.SENSOR_INPUTS, "Volts", 4),
    MAFR("MAFR", SensorCategory.SENSOR_INPUTS, "kg/hr", 4),

    PPS("pedal", SensorCategory.SENSOR_INPUTS, "%", 100), // pedal position sensor
    knockCount("Knock", SensorCategory.SENSOR_INPUTS, "count", 30),
    KnockValue("Knock level", SensorCategory.SENSOR_INPUTS, "v", 6),

    ENGINE_LOAD("Engine Load", SensorCategory.SENSOR_INPUTS, "x", 300),


    MAFR_CFM("MAFR_CFM", SensorCategory.SENSOR_INPUTS, "cub f/m", 800),
    CLT_F("Coolant F", SensorCategory.SENSOR_INPUTS, "temperature, F", -40, 500),
    IAT_F("Intake Air F", SensorCategory.SENSOR_INPUTS, "temperature, F", -40, 350),

    ADC0("adc0", SensorCategory.SENSOR_INPUTS, "%", 100),
    ADC1("adc1", SensorCategory.SENSOR_INPUTS, "%", 100),
    ADC2("adc2", SensorCategory.SENSOR_INPUTS, "%", 100),
    ADC3("adc3", SensorCategory.SENSOR_INPUTS, "%", 100),

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

    ETB_CONTROL_QUALITY("ETB metric", SensorCategory.SNIFFING, "", 100),


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

    IDLE_SWITCH("idle switch", SensorCategory.OTHERS),

    DEFAULT_FUEL("map fuel", SensorCategory.FUEL, "ms", 0, 40),

    CHARTSIZE("CHARTSIZE", SensorCategory.OTHERS),
    CHART_STATUS("CHART_STATUS", SensorCategory.OTHERS),
    ADC_STATUS("ADC_STATUS", SensorCategory.OTHERS),

    ADC_FAST("ADC_FAST", SensorCategory.OTHERS, "b", 4000),
    ADC_FAST_AVG("ADC_FAST_AVG", SensorCategory.OTHERS, "b", 4000),
    INJECTOR_1_DWELL("inj #1", SensorCategory.SNIFFING),
    INJECTOR_2_DWELL("inj #2", SensorCategory.SNIFFING),
    INJECTOR_3_DWELL("inj #3", SensorCategory.SNIFFING),
    INJECTOR_4_DWELL("inj #4", SensorCategory.SNIFFING),

    FUEL("Fuel", SensorCategory.FUEL, "ms", 0, 30),
    FUEL_IAT("F IAT", SensorCategory.FUEL, "", 0, 10),
    FUEL_CLT("F CLT", SensorCategory.FUEL, "", 0, 10),
    FUEL_LAG("F Lag", SensorCategory.FUEL, "", 0, 30),

    IAT(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 8, BackgroundColor.WHITE, -40, 150, "C"),
    TPS(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 12, BackgroundColor.MUD, 0, 100, "%"), // throttle position sensor
    CRANKING_BASE(SensorCategory.FUEL, FieldType.FLOAT, 44, BackgroundColor.MUD, 0, 30, "ms"),
    FUEL_BASE(Fields.GAUGE_NAME_FUEL_BASE, SensorCategory.FUEL, FieldType.FLOAT, 48, BackgroundColor.MUD, 0, 30, "ms"),
    T_CHARGE(SensorCategory.FUEL, FieldType.FLOAT, 52, BackgroundColor.MUD, 30, 140),
    // todo: unify with TIMING
    ignitionAdvance(SensorCategory.OPERATIONS, FieldType.FLOAT, 56, BackgroundColor.MUD, 30, 140),
    DWELL(Fields.GAUGE_COIL_DWELL_TIME, SensorCategory.OPERATIONS, FieldType.FLOAT, 60, BackgroundColor.MUD, 1, 10),
    actualLastInjection(SensorCategory.FUEL, FieldType.FLOAT, /*offset */ 64, BackgroundColor.MUD, 0, 30, "ms"),
    debugFloatField1(GAUGE_NAME_DEBUG_F1, SensorCategory.OPERATIONS, FieldType.FLOAT, 68, BackgroundColor.MUD, 0, 5),
    VSS(SensorCategory.OPERATIONS, FieldType.FLOAT, 76, BackgroundColor.BLUE),
    FIRMWARE_VERSION(SensorCategory.OPERATIONS, FieldType.INT, 84, BackgroundColor.BLUE),
    CURRENT_VE(SensorCategory.FUEL, FieldType.FLOAT, 112, BackgroundColor.MUD),

    deltaTps(SensorCategory.FUEL, FieldType.FLOAT, 116, BackgroundColor.MUD),
    engineLoadAccelDelta(SensorCategory.FUEL, FieldType.FLOAT, 124, BackgroundColor.MUD),
    tpsAccelFuel(Fields.GAUGE_NAME_FUEL_TPS_EXTRA, SensorCategory.FUEL, FieldType.FLOAT, 128, BackgroundColor.MUD),
    injectorDutyCycle(Fields.GAUGE_NAME_FUEL_INJ_DUTY, SensorCategory.OPERATIONS, FieldType.FLOAT, 140, BackgroundColor.MUD),
    wallFuelAmount(SensorCategory.FUEL, FieldType.FLOAT, 160, BackgroundColor.MUD),
    iatCorrection(SensorCategory.FUEL, FieldType.FLOAT, 164, BackgroundColor.MUD, 0, 5),
    wallFuelCorrection(SensorCategory.FUEL, FieldType.FLOAT, 168, BackgroundColor.MUD),
    idlePosition(SensorCategory.OPERATIONS, FieldType.FLOAT, 172, BackgroundColor.MUD),
    TARGET_AFR(SensorCategory.OPERATIONS, FieldType.FLOAT, 176, BackgroundColor.MUD),
    CHARGE_AIR_MASS(SensorCategory.OPERATIONS, FieldType.FLOAT, 180, BackgroundColor.MUD),
    cltCorrection(SensorCategory.OPERATIONS, FieldType.FLOAT, 184, BackgroundColor.MUD, 0, 5),
    runningFuel(SensorCategory.FUEL, FieldType.FLOAT, 188, BackgroundColor.MUD, 0, 15, "ms"),
    debugIntField1(GAUGE_NAME_DEBUG_I1, SensorCategory.OPERATIONS, FieldType.INT, 192, BackgroundColor.MUD, 0, 5),
    injectorLagMs(SensorCategory.FUEL, FieldType.FLOAT, 196, BackgroundColor.MUD, 0, 15, "ms"),

    debugFloatField2(GAUGE_NAME_DEBUG_F2, SensorCategory.OPERATIONS, FieldType.FLOAT, 200, BackgroundColor.MUD, 0, 5),
    debugFloatField3(GAUGE_NAME_DEBUG_F3, SensorCategory.OPERATIONS, FieldType.FLOAT, 204, BackgroundColor.MUD, 0, 5),
    debugFloatField4(GAUGE_NAME_DEBUG_F4, SensorCategory.OPERATIONS, FieldType.FLOAT, 208, BackgroundColor.MUD, 0, 5),
    debugFloatField5(GAUGE_NAME_DEBUG_F5, SensorCategory.OPERATIONS, FieldType.FLOAT, 212, BackgroundColor.MUD, 0, 5),
    debugIntField2(GAUGE_NAME_DEBUG_I2, SensorCategory.OPERATIONS, FieldType.INT, 216, BackgroundColor.MUD, 0, 5),
    debugIntField3(GAUGE_NAME_DEBUG_I3, SensorCategory.OPERATIONS, FieldType.INT, 220, BackgroundColor.MUD, 0, 5),

    errorCodeCounter(SensorCategory.OPERATIONS, FieldType.INT, 236, BackgroundColor.MUD, 0, 5),
    lastErrorCode(SensorCategory.OPERATIONS, FieldType.INT, 240, BackgroundColor.MUD, 0, 5),

    RPM(SensorCategory.SENSOR_INPUTS, FieldType.INT, 0, BackgroundColor.RED, 0, 8000),
    TIME_SECONDS(SensorCategory.OPERATIONS, FieldType.INT, 224, BackgroundColor.MUD, 0, 5),
    SPEED2RPM(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 232, BackgroundColor.MUD, 0, 5),
    INT_TEMP(SensorCategory.OPERATIONS, FieldType.FLOAT, 244, BackgroundColor.MUD, 0, 5),
    vvtPosition(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 248, BackgroundColor.MUD, 0, 5),
    engineMode(SensorCategory.OPERATIONS, FieldType.INT, 252, BackgroundColor.MUD, 0, 5),

    debugFloatField6(GAUGE_NAME_DEBUG_F6, SensorCategory.OPERATIONS, FieldType.FLOAT, 256, BackgroundColor.MUD, 0, 5),
    debugFloatField7(GAUGE_NAME_DEBUG_F7, SensorCategory.OPERATIONS, FieldType.FLOAT, 260, BackgroundColor.MUD, 0, 5),
    coilDutyCycle(Fields.GAUGE_NAME_DWELL_DUTY, SensorCategory.OPERATIONS, FieldType.FLOAT, 272, BackgroundColor.MUD),

    INJ_1_2_DELTA("inj 1-2 delta", SensorCategory.SNIFFING),
    INJ_3_4_DELTA("inj 3-4 delta", SensorCategory.SNIFFING),
    ;

    private final String name;
    private final SensorCategory category;
    private final String units;
    private final double minValue;
    private final double maxValue;
    private final BackgroundColor color;
    private final FieldType type;
    private final int offset;

    Sensor(String name, SensorCategory category, FieldType type, int offset, BackgroundColor color, double minValue, double maxValue, String units) {
        this.name = name == null ? name() : name;
        this.type = type;
        this.offset = offset;
        this.category = category;
        this.color = color;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
    }

    Sensor(SensorCategory category, FieldType type, int offset, BackgroundColor color, double minValue, double maxValue, String units) {
        this(null, category, type, offset, color, minValue, maxValue, units);
    }

    Sensor(String name, SensorCategory category, FieldType type, int offset, BackgroundColor color, double minValue, double maxValue) {
        this(name, category, type, offset, color, minValue, maxValue, "n/a");
    }

    Sensor(SensorCategory category, FieldType type, int offset, BackgroundColor color, double minValue, double maxValue) {
        this(null, category, type, offset, color, minValue, maxValue);
    }

    Sensor(SensorCategory category, FieldType type, int offset, BackgroundColor color) {
        this(null, category, type, offset, color);
    }

    Sensor(String name, SensorCategory category, FieldType type, int offset, BackgroundColor color) {
        this(name, category, type, offset, color, 0, 100);
    }

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
        type = null;
        offset = -1;
    }

    public static Collection<Sensor> getSensorsForCategory(String category) {
        final Set<Sensor> sensors = new TreeSet<>(Comparator.comparing(o -> o.getName().toLowerCase()));

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

    public static Sensor lookup(String gaugeName, Sensor defaultValue) {
        Sensor sensor;
        try {
            sensor = valueOf(Sensor.class, gaugeName);
        } catch (IllegalArgumentException e) {
            sensor = defaultValue;
        }
        return sensor;
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

    public int getOffset() {
        return offset;
    }

    public FieldType getType() {
        return type;
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
