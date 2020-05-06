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
    RPM("RPM", SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 4, 1, BackgroundColor.RED, 0, 8000, "/min"),
    SPEED2RPM("SpeedToRpm", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 6, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 5, "RPM/kph"),
    VSS("VSS", SensorCategory.OPERATIONS, FieldType.UINT8, 8, 1, BackgroundColor.BLUE, 0, 150, "kph"),

    // Temperatures
    INT_TEMP("MCU Temp", SensorCategory.OPERATIONS, FieldType.INT8, 10, 1, BackgroundColor.MUD, 0, 5, "C"),
    CLT("Coolant", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 12, 1.0 / PACK_MULT_TEMPERATURE,BackgroundColor.MUD, -40, 150, "C"),
    IAT("IAT", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 14, 1.0 / PACK_MULT_TEMPERATURE, BackgroundColor.WHITE, -40, 150, "C"),

    // throttle, pedal
    TPS("TPS", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 20, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 100, "%"), // throttle position sensor
    PPS("Throttle Pedal", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 22, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 100, "%"), // pedal position sensor

    // air flow/mass measurement
    MAF("MAF", SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 26, 1.0 / PACK_MULT_VOLTAGE, BackgroundColor.MUD, 0, 5, "Volts"),
    MAP("MAP", SensorCategory.SENSOR_INPUTS, FieldType.UINT16,  30, 1.0 / PACK_MULT_PRESSURE, BackgroundColor.MUD, 20, 300, "kPa"),

    AFR("A/F ratio", SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 34, 1.0 / PACK_MULT_AFR, BackgroundColor.MUD, 10, 20, "afr"),

    VBATT("VBatt", SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 38, 1.0 / PACK_MULT_VOLTAGE, BackgroundColor.BEIGE, 4, 18, "Volts"),
    oilPressure("Oil Pressure", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 40, 1.0 / PACK_MULT_PRESSURE, BackgroundColor.MUD, 0, 5, "X"),
    vvtPosition("vvt position", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 42, 1.0 / PACK_MULT_ANGLE, BackgroundColor.MUD, 0, 5, "deg"),

    // fuel math
    CHARGE_AIR_MASS("airmass", SensorCategory.OPERATIONS, FieldType.UINT16, 44, 0.001, BackgroundColor.MUD, 0, 3, "g/cyl"),
    crankingFuel(GAUGE_NAME_FUEL_CRANKING, SensorCategory.FUEL, FieldType.UINT16, 46, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 0, 30, "ms"),
    TARGET_AFR("A/F target", SensorCategory.OPERATIONS, FieldType.INT16, 48, 1.0 / PACK_MULT_AFR, BackgroundColor.MUD, 10, 20, "afr"),
    baseFuel(Fields.GAUGE_NAME_FUEL_BASE, SensorCategory.FUEL, FieldType.UINT16, 50, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 0, 30, "ms"),
    runningFuel(GAUGE_NAME_FUEL_RUNNING, SensorCategory.FUEL, FieldType.UINT16, 52, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 0, 15, "ms"),
    actualLastInjection(GAUGE_NAME_FUEL_LAST_INJECTION, SensorCategory.FUEL, FieldType.UINT16, 54, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 0, 30, "ms"),
    injectorDutyCycle(Fields.GAUGE_NAME_FUEL_INJ_DUTY, SensorCategory.FUEL, FieldType.UINT8, 56, 0.5, BackgroundColor.MUD, 0, 100, "%"),
    veValue(GAUGE_NAME_FUEL_VE, SensorCategory.FUEL, FieldType.FLOAT, 57, 0.5, BackgroundColor.MUD, 0, 100, "%"),
    tCharge(GAUGE_NAME_TCHARGE, SensorCategory.FUEL, FieldType.FLOAT, 60, 1.0 / PACK_MULT_TEMPERATURE, BackgroundColor.MUD, 30, 140, "C"),

    // Corrections
    injectorLagMs(GAUGE_NAME_INJECTOR_LAG, SensorCategory.FUEL, FieldType.UINT16, 62, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 0, 15, "ms"),
    iatCorrection(GAUGE_NAME_FUEL_IAT_CORR, SensorCategory.FUEL, FieldType.INT16, 64, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 5, "ratio"),
    cltCorrection(GAUGE_NAME_FUEL_CLT_CORR, SensorCategory.FUEL, FieldType.INT16, 66, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 5, "ratio"),
    fuelPidCorrection("Fuel PID", SensorCategory.FUEL, FieldType.INT16, 70,  1.0 / PACK_MULT_MS, BackgroundColor.MUD, -2, 2, "ms"),

    // Wall model AE
    wallFuelAmount(GAUGE_NAME_FUEL_WALL_AMOUNT, SensorCategory.FUEL, FieldType.UINT16, 72, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 0, 20, "ms"),
    wallFuelCorrection(GAUGE_NAME_FUEL_WALL_CORRECTION, SensorCategory.FUEL, FieldType.INT16, 74, 0.001, BackgroundColor.MUD, -5, 5, "ms"),

    // TPS/load AE
    engineLoadAccelDelta("load accel delta", SensorCategory.FUEL, FieldType.INT16, 76, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, -5, 5, "ratio"),
    deltaTps(Fields.GAUGE_NAME_FUEL_TPS_ROC, SensorCategory.FUEL, FieldType.INT16, 78, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, -100, 100, "%"),
    tpsAccelFuel(Fields.GAUGE_NAME_FUEL_TPS_EXTRA, SensorCategory.FUEL, FieldType.INT16, 82, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 0, 200, "ms"),

    // Ignition
    ignitionAdvance("ignition timing", SensorCategory.OPERATIONS, FieldType.INT16, 84, 1.0 / PACK_MULT_ANGLE, BackgroundColor.MUD, 30, 140, "deg"),
    DWELL(Fields.GAUGE_COIL_DWELL_TIME, SensorCategory.OPERATIONS, FieldType.UINT16, 86, 1.0 / PACK_MULT_MS, BackgroundColor.MUD, 1, 10, "ms"),
    coilDutyCycle(Fields.GAUGE_NAME_DWELL_DUTY, SensorCategory.OPERATIONS, FieldType.UINT16, 88, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 100, "%"),

    // Idle & ETB
    idlePosition("Idle Position", SensorCategory.OPERATIONS, FieldType.INT16, 90, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 100, "%"),
    etbTarget(GAUGE_NAME_ETB_TARGET, SensorCategory.OTHERS, FieldType.INT16, 92, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 100, "%"),
    etb1DutyCycle(GAUGE_NAME_ETB_DUTY, SensorCategory.OTHERS, FieldType.INT16, 94, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 100, "%"),
    etb1Error(GAUGE_NAME_ETB_ERROR, SensorCategory.OTHERS, FieldType.INT16, 96, 1.0 / PACK_MULT_PERCENT, BackgroundColor.MUD, 0, 100, "%"),

    // Fuel system

    // Knock

    // Mode, firmware, protocol, run time
    TIME_SECONDS("uptime", SensorCategory.OPERATIONS, FieldType.INT, 112, BackgroundColor.MUD, 0, 5),
    engineMode("mode", SensorCategory.OPERATIONS, FieldType.INT, 116, BackgroundColor.MUD, 0, 5),
    FIRMWARE_VERSION("FW version", SensorCategory.OPERATIONS, FieldType.INT, 120, BackgroundColor.BLUE),
    TS_CONFIG_VERSION(".ini version", SensorCategory.OPERATIONS, FieldType.INT, 124, BackgroundColor.BLUE),

    // Errors
    totalTriggerErrorCounter("trigger total error counter", SensorCategory.STATUS, FieldType.INT, 128, BackgroundColor.MUD, 0, 5),
    orderingErrorCounter("trigger order error counter", SensorCategory.STATUS, FieldType.INT, 132, BackgroundColor.MUD, 0, 5),
    errorCodeCounter("error counter", SensorCategory.STATUS, FieldType.INT, 136, BackgroundColor.MUD, 0, 5),
    lastErrorCode("last error", SensorCategory.STATUS, FieldType.INT, 138, BackgroundColor.MUD, 0, 5),

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
    private final double scale;

    Sensor(String name, SensorCategory category, FieldType type, int offset, double scale, BackgroundColor color, double minValue, double maxValue, String units) {
        this.name = name == null ? name() : name;
        this.type = type;
        this.offset = offset;
        this.scale = scale;
        this.category = category;
        this.color = color;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
    }

    Sensor(String name, SensorCategory category, FieldType type, int offset, BackgroundColor color, double minValue, double maxValue) {
        this(name, category, type, offset, 1.0, color, minValue, maxValue, "n/a");
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
        this.scale = 1.0;
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

    public double getScale() {
        return scale;
    }

    public FieldType getType() {
        return type;
    }

    public double translateValue(double value) {
        return value;
    }
}
