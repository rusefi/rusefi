package com.rusefi.core;

import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Fields;
import eu.hansolo.steelseries.tools.BackgroundColor;
import org.jetbrains.annotations.Nullable;

import java.util.Collection;
import java.util.Comparator;
import java.util.Set;
import java.util.TreeSet;

import static com.rusefi.config.generated.Fields.*;

/**
 * @author Andrey Belomutskiy
 * 2/11/13
 */
public enum Sensor {
    /**
     * Please note that these enum names are used to make 'set_mock_XXX_voltage' commands
     */

    // RPM, vss
    RPM(SensorCategory.SENSOR_INPUTS, FieldType.INT, 4, BackgroundColor.RED, 0, 8000),
    SPEED2RPM(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 6, BackgroundColor.MUD, 0, 5),
    VSS(SensorCategory.OPERATIONS, FieldType.FLOAT, 8, BackgroundColor.BLUE),

    // Temperatures
    INT_TEMP(SensorCategory.OPERATIONS, FieldType.FLOAT, 10, BackgroundColor.MUD, 0, 5),
    CLT("Coolant", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 12, BackgroundColor.MUD, -40, 300),
    IAT(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 14, BackgroundColor.WHITE, -40, 150, "C"),

    // throttle, pedal
    TPS(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 20, BackgroundColor.MUD, 0, 100, "%"), // throttle position sensor
    PPS("Throttle Pedal", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 22, BackgroundColor.MUD, 0, 100, "%"), // pedal position sensor

    // air flow/mass measurement
    MAF("MAF", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, /*offset */ 26, BackgroundColor.MUD, 0, 5),
    MAP("MAP", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, /*offset */ 28, BackgroundColor.MUD, 20, 300),

    AFR("A/F ratio", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, /*offset */ 34, BackgroundColor.MUD, 0, 20),

    VBATT("VBatt", SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 38, BackgroundColor.BEIGE, 4, 18, "Volts"),
    vvtPosition(SensorCategory.SENSOR_INPUTS, FieldType.FLOAT, 42, BackgroundColor.MUD, 0, 5),

    // fuel math
    CHARGE_AIR_MASS(SensorCategory.OPERATIONS, FieldType.FLOAT, 44, BackgroundColor.MUD),
    crankingFuel(GAUGE_NAME_FUEL_CRANKING, SensorCategory.FUEL, FieldType.FLOAT, 46, BackgroundColor.MUD, 0, 30, "ms"),
    TARGET_AFR(SensorCategory.OPERATIONS, FieldType.FLOAT, 48, BackgroundColor.MUD),
    baseFuel(Fields.GAUGE_NAME_FUEL_BASE, SensorCategory.FUEL, FieldType.FLOAT, 50, BackgroundColor.MUD, 0, 30, "ms"),
    runningFuel(GAUGE_NAME_FUEL_RUNNING, SensorCategory.FUEL, FieldType.FLOAT, 52, BackgroundColor.MUD, 0, 15, "ms"),
    actualLastInjection(GAUGE_NAME_FUEL_LAST_INJECTION, SensorCategory.FUEL, FieldType.FLOAT, /*offset */ 54, BackgroundColor.MUD, 0, 30, "ms"),
    injectorDutyCycle(Fields.GAUGE_NAME_FUEL_INJ_DUTY, SensorCategory.FUEL, FieldType.FLOAT, 56, BackgroundColor.MUD),
    veValue(GAUGE_NAME_FUEL_VE, SensorCategory.FUEL, FieldType.FLOAT, 57, BackgroundColor.MUD),
    tCharge(GAUGE_NAME_TCHARGE, SensorCategory.FUEL, FieldType.FLOAT, 60, BackgroundColor.MUD, 30, 140),

    // Corrections
    injectorLagMs(GAUGE_NAME_INJECTOR_LAG, SensorCategory.FUEL, FieldType.FLOAT, 62, BackgroundColor.MUD, 0, 15, "ms"),
    iatCorrection(GAUGE_NAME_FUEL_IAT_CORR, SensorCategory.FUEL, FieldType.FLOAT, 64, BackgroundColor.MUD, 0, 5),
    cltCorrection(GAUGE_NAME_FUEL_CLT_CORR, SensorCategory.FUEL, FieldType.FLOAT, 66, BackgroundColor.MUD, 0, 5),
    fuelPidCorrection(SensorCategory.FUEL, FieldType.FLOAT, 70, BackgroundColor.MUD),

    // Wall model AE
    wallFuelAmount(GAUGE_NAME_FUEL_WALL_AMOUNT, SensorCategory.FUEL, FieldType.FLOAT, 72, BackgroundColor.MUD),
    wallFuelCorrection(GAUGE_NAME_FUEL_WALL_CORRECTION, SensorCategory.FUEL, FieldType.FLOAT, 74, BackgroundColor.MUD),

    // TPS/load AE
    engineLoadAccelDelta(SensorCategory.FUEL, FieldType.FLOAT, 76, BackgroundColor.MUD),
    deltaTps(Fields.GAUGE_NAME_FUEL_TPS_ROC, SensorCategory.FUEL, FieldType.FLOAT, 78, BackgroundColor.MUD, -100, 100, "%"),
    tpsAccelFuel(Fields.GAUGE_NAME_FUEL_TPS_EXTRA, SensorCategory.FUEL, FieldType.FLOAT, 82, BackgroundColor.MUD, 0, 200, "ms"),


    // Ignition
    ignitionAdvance(SensorCategory.OPERATIONS, FieldType.FLOAT, 84, BackgroundColor.MUD, 30, 140),
    DWELL(Fields.GAUGE_COIL_DWELL_TIME, SensorCategory.OPERATIONS, FieldType.FLOAT, 86, BackgroundColor.MUD, 1, 10),
    coilDutyCycle(Fields.GAUGE_NAME_DWELL_DUTY, SensorCategory.OPERATIONS, FieldType.FLOAT, 88, BackgroundColor.MUD),

    // Idle & ETB
    idlePosition(SensorCategory.OPERATIONS, FieldType.FLOAT, 90, BackgroundColor.MUD),
    etbTarget(GAUGE_NAME_ETB_TARGET, SensorCategory.OTHERS, FieldType.FLOAT, 92, BackgroundColor.MUD),
    etb1DutyCycle(GAUGE_NAME_ETB_DUTY, SensorCategory.OTHERS, FieldType.FLOAT, 94, BackgroundColor.MUD),
    etb1Error(GAUGE_NAME_ETB_ERROR, SensorCategory.OTHERS, FieldType.FLOAT, 96, BackgroundColor.MUD),

    // Fuel system

    // Knock

    // Mode, firmware, protocol, run time
    TIME_SECONDS(SensorCategory.OPERATIONS, FieldType.INT, 112, BackgroundColor.MUD, 0, 5),
    engineMode(SensorCategory.OPERATIONS, FieldType.INT, 116, BackgroundColor.MUD, 0, 5),
    FIRMWARE_VERSION(SensorCategory.OPERATIONS, FieldType.INT, 120, BackgroundColor.BLUE),


    // Errors
    errorCodeCounter(SensorCategory.STATUS, FieldType.INT, 136, BackgroundColor.MUD, 0, 5),
    lastErrorCode(SensorCategory.STATUS, FieldType.INT, 138, BackgroundColor.MUD, 0, 5),

    // Debug
    debugFloatField1(GAUGE_NAME_DEBUG_F1, SensorCategory.DEBUG, FieldType.FLOAT, 156, BackgroundColor.MUD, 0, 5),
    debugFloatField2(GAUGE_NAME_DEBUG_F2, SensorCategory.DEBUG, FieldType.FLOAT, 160, BackgroundColor.MUD, 0, 5),
    debugFloatField3(GAUGE_NAME_DEBUG_F3, SensorCategory.DEBUG, FieldType.FLOAT, 164, BackgroundColor.MUD, 0, 5),
    debugFloatField4(GAUGE_NAME_DEBUG_F4, SensorCategory.DEBUG, FieldType.FLOAT, 168, BackgroundColor.MUD, 0, 5),
    debugFloatField5(GAUGE_NAME_DEBUG_F5, SensorCategory.DEBUG, FieldType.FLOAT, 172, BackgroundColor.MUD, 0, 5),
    debugFloatField6(GAUGE_NAME_DEBUG_F6, SensorCategory.DEBUG, FieldType.FLOAT, 176, BackgroundColor.MUD, 0, 5),
    debugFloatField7(GAUGE_NAME_DEBUG_F7, SensorCategory.DEBUG, FieldType.FLOAT, 180, BackgroundColor.MUD, 0, 5),
    debugIntField1(GAUGE_NAME_DEBUG_I1, SensorCategory.DEBUG, FieldType.INT, 184, BackgroundColor.MUD, 0, 5),
    debugIntField2(GAUGE_NAME_DEBUG_I2, SensorCategory.DEBUG, FieldType.INT, 188, BackgroundColor.MUD, 0, 5),
    debugIntField3(GAUGE_NAME_DEBUG_I3, SensorCategory.DEBUG, FieldType.INT, 192, BackgroundColor.MUD, 0, 5),
    debugIntField4("debug i4", SensorCategory.DEBUG, FieldType.INT16, 196, BackgroundColor.MUD, 0, 5),
    debugIntField5("debug i5", SensorCategory.DEBUG, FieldType.INT16, 198, BackgroundColor.MUD, 0, 5),

    // Synthetic (console only) channels
    ETB_CONTROL_QUALITY("ETB metric", SensorCategory.SNIFFING, "", 100),
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

    public static Sensor find(String value) {
        for (Sensor s : values())
            if (s.name.equals(value) || s.name().equals(value))
                return s;
        throw new IllegalStateException("Sensor not found: " + value);
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
