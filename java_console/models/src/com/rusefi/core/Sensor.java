package com.rusefi.core;

import com.rusefi.config.FieldType;
import com.rusefi.config.Fields;
import eu.hansolo.steelseries.tools.BackgroundColor;
import org.jetbrains.annotations.Nullable;

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
    MAP("MAP", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, /*offset */ 40, BackgroundColor.MUD, 20, 300),

    /**
     * Please note that these enum names are used to make 'set_mock_XXX_voltage' commands
     */
    CLT("Coolant", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, /*offset */ 4, BackgroundColor.MUD, -40, 300),
    AFR("A/F ratio", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, /*offset */ 20, BackgroundColor.MUD, 0, 20),
    MAF("MAF", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, /*offset */ 16, BackgroundColor.MUD, 0, 5),

//    knockCount("Knock", SensorCategory.SENSOR_INPUTS, "count", 30),
//    KnockValue("Knock level", SensorCategory.SENSOR_INPUTS, "v", 6),

//    ENGINE_LOAD("Engine Load", SensorCategory.SENSOR_INPUTS, "x", 300),


//    MAFR_CFM("MAFR_CFM", SensorCategory.SENSOR_INPUTS, "cub f/m", 800),


//    COOLANT_WIDTH("c w", "", 30),
//    INTAKE_AIR_WIDTH("air w", "", 30),

//    VREF("VRef", SensorCategory.SENSOR_INPUTS, "Volts", 6),
    VBATT("VBatt", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 28, BackgroundColor.BEIGE, 4, 18, "Volts"),

    ETB_CONTROL_QUALITY("ETB metric", SensorCategory.SNIFFING, "", 100),

    IAT(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 8, BackgroundColor.WHITE, -40, 150, "C"),
    TPS(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 12, BackgroundColor.MUD, 0, 100, "%"), // throttle position sensor
    crankingFuel(GAUGE_NAME_FUEL_CRANKING, SensorCategory.FUEL, FieldType.FLOAT, 44, BackgroundColor.MUD, 0, 30, "ms"),
    baseFuel(Fields.GAUGE_NAME_FUEL_BASE, SensorCategory.FUEL, FieldType.FLOAT, 48, BackgroundColor.MUD, 0, 30, "ms"),
    tCharge(GAUGE_NAME_TCHARGE, SensorCategory.FUEL, FieldType.FLOAT, 52, BackgroundColor.MUD, 30, 140),
    // todo: unify with TIMING
    ignitionAdvance(SensorCategory.OPERATIONS, FieldType.FLOAT, 56, BackgroundColor.MUD, 30, 140),
    DWELL(Fields.GAUGE_COIL_DWELL_TIME, SensorCategory.OPERATIONS, FieldType.FLOAT, 60, BackgroundColor.MUD, 1, 10),
    actualLastInjection(GAUGE_NAME_FUEL_LAST_INJECTION, SensorCategory.FUEL, FieldType.FLOAT, /*offset */ 64, BackgroundColor.MUD, 0, 30, "ms"),
    debugFloatField1(GAUGE_NAME_DEBUG_F1, SensorCategory.DEBUG, FieldType.FLOAT, 68, BackgroundColor.MUD, 0, 5),
    VSS(SensorCategory.OPERATIONS, FieldType.FLOAT, 76, BackgroundColor.BLUE),
    FIRMWARE_VERSION(SensorCategory.OPERATIONS, FieldType.INT, 84, BackgroundColor.BLUE),
    veValue(GAUGE_NAME_FUEL_VE, SensorCategory.FUEL, FieldType.FLOAT, 112, BackgroundColor.MUD),

    deltaTps(Fields.GAUGE_NAME_FUEL_TPS_ROC, SensorCategory.FUEL, FieldType.FLOAT, 116, BackgroundColor.MUD, -100, 100, "%"),
    engineLoadAccelDelta(SensorCategory.FUEL, FieldType.FLOAT, 124, BackgroundColor.MUD),
    tpsAccelFuel(Fields.GAUGE_NAME_FUEL_TPS_EXTRA, SensorCategory.FUEL, FieldType.FLOAT, 128, BackgroundColor.MUD, 0, 200, "ms"),
    PPS("Throttle Pedal", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 136, BackgroundColor.MUD, 0, 100, "%"), // pedal position sensor

    injectorDutyCycle(Fields.GAUGE_NAME_FUEL_INJ_DUTY, SensorCategory.FUEL, FieldType.FLOAT, 140, BackgroundColor.MUD),
    wallFuelAmount(GAUGE_NAME_FUEL_WALL_AMOUNT, SensorCategory.FUEL, FieldType.FLOAT, 160, BackgroundColor.MUD),
    iatCorrection(GAUGE_NAME_FUEL_IAT_CORR, SensorCategory.FUEL, FieldType.FLOAT, 164, BackgroundColor.MUD, 0, 5),
    wallFuelCorrection(GAUGE_NAME_FUEL_WALL_CORRECTION, SensorCategory.FUEL, FieldType.FLOAT, 168, BackgroundColor.MUD),
    idlePosition(SensorCategory.OPERATIONS, FieldType.FLOAT, 172, BackgroundColor.MUD),
    TARGET_AFR(SensorCategory.OPERATIONS, FieldType.FLOAT, 176, BackgroundColor.MUD),
    CHARGE_AIR_MASS(SensorCategory.OPERATIONS, FieldType.FLOAT, 180, BackgroundColor.MUD),
    cltCorrection(GAUGE_NAME_FUEL_CLT_CORR, SensorCategory.FUEL, FieldType.FLOAT, 184, BackgroundColor.MUD, 0, 5),
    runningFuel(GAUGE_NAME_FUEL_RUNNING, SensorCategory.FUEL, FieldType.FLOAT, 188, BackgroundColor.MUD, 0, 15, "ms"),
    debugIntField1(GAUGE_NAME_DEBUG_I1, SensorCategory.DEBUG, FieldType.INT, 192, BackgroundColor.MUD, 0, 5),
    injectorLagMs(GAUGE_NAME_INJECTOR_LAG, SensorCategory.FUEL, FieldType.FLOAT, 196, BackgroundColor.MUD, 0, 15, "ms"),

    debugFloatField2(GAUGE_NAME_DEBUG_F2, SensorCategory.DEBUG, FieldType.FLOAT, 200, BackgroundColor.MUD, 0, 5),
    debugFloatField3(GAUGE_NAME_DEBUG_F3, SensorCategory.DEBUG, FieldType.FLOAT, 204, BackgroundColor.MUD, 0, 5),
    debugFloatField4(GAUGE_NAME_DEBUG_F4, SensorCategory.DEBUG, FieldType.FLOAT, 208, BackgroundColor.MUD, 0, 5),
    debugFloatField5(GAUGE_NAME_DEBUG_F5, SensorCategory.DEBUG, FieldType.FLOAT, 212, BackgroundColor.MUD, 0, 5),
    debugIntField2(GAUGE_NAME_DEBUG_I2, SensorCategory.DEBUG, FieldType.INT, 216, BackgroundColor.MUD, 0, 5),
    debugIntField3(GAUGE_NAME_DEBUG_I3, SensorCategory.DEBUG, FieldType.INT, 220, BackgroundColor.MUD, 0, 5),

    errorCodeCounter(SensorCategory.STATUS, FieldType.INT, 236, BackgroundColor.MUD, 0, 5),
    lastErrorCode(SensorCategory.STATUS, FieldType.INT, 240, BackgroundColor.MUD, 0, 5),

    RPM(SensorCategory.SENSOR_INPUTS, FieldType.INT, 0, BackgroundColor.RED, 0, 8000),
    TIME_SECONDS(SensorCategory.OPERATIONS, FieldType.INT, 224, BackgroundColor.MUD, 0, 5),
    SPEED2RPM(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 232, BackgroundColor.MUD, 0, 5),
    INT_TEMP(SensorCategory.OPERATIONS, FieldType.FLOAT, 244, BackgroundColor.MUD, 0, 5),
    vvtPosition(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 248, BackgroundColor.MUD, 0, 5),
    engineMode(SensorCategory.OPERATIONS, FieldType.INT, 252, BackgroundColor.MUD, 0, 5),

    debugFloatField6(GAUGE_NAME_DEBUG_F6, SensorCategory.DEBUG, FieldType.FLOAT, 256, BackgroundColor.MUD, 0, 5),
    debugFloatField7(GAUGE_NAME_DEBUG_F7, SensorCategory.DEBUG, FieldType.FLOAT, 260, BackgroundColor.MUD, 0, 5),
    fuelPidCorrection(SensorCategory.FUEL, FieldType.FLOAT, 268, BackgroundColor.MUD),
    coilDutyCycle(Fields.GAUGE_NAME_DWELL_DUTY, SensorCategory.OPERATIONS, FieldType.FLOAT, 272, BackgroundColor.MUD),

    debugIntField4("debug i4", SensorCategory.DEBUG, FieldType.INT16, 292, BackgroundColor.MUD, 0, 5),
    debugIntField5("debug i5", SensorCategory.DEBUG, FieldType.INT16, 294, BackgroundColor.MUD, 0, 5),

    ;

    private final String name;
    private final SensorCategory category;
    private final String units;
    private final double minValue;
    private final double maxValue;
    private final BackgroundColor color;
    @Nullable
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

    /**
     * This constructor is used for virtual sensors which do not directly come from ECU
     */
    Sensor(String name, SensorCategory category, String units, double maxValue) {
        this(name, category, units, 0, maxValue);
    }

    /**
     * This constructor is used for virtual sensors which do not directly come from ECU
     */
    Sensor(String name, SensorCategory category, String units, double minValue, double maxValue) {
        this(name, category, units, minValue, maxValue, BackgroundColor.LIGHT_GRAY);
    }

    /**
     * This constructor is used for virtual sensors which do not directly come from ECU
     */
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
        return value;
    }
}
