package com.rusefi.core;

import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Fields;
import org.jetbrains.annotations.Nullable;

import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Collection;
import java.util.Comparator;
import java.util.Set;
import java.util.TreeSet;

import static com.rusefi.config.generated.Fields.*;

/**
 * @author Andrey Belomutskiy
 * 2/11/13
 * @see Fields#TS_OUTPUT_SIZE
 */
public enum Sensor {
    /**
     * Please note that these enum names are used to make 'set_mock_XXX_voltage' commands
     */

    // RPM, vss
    RPM(GAUGE_NAME_RPM, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 4, 1, 0, 8000, "RPM"),
    SPEED2RPM("SpeedToRpm", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 6, 1.0 / PACK_MULT_PERCENT, 0, 5, "RPM/kph"),
    VSS(GAUGE_NAME_VVS, SensorCategory.OPERATIONS, FieldType.UINT8, 8, 1, 0, 150, "kph"),

    // Temperatures
    INT_TEMP(GAUGE_NAME_CPU_TEMP, SensorCategory.OPERATIONS, FieldType.INT8, 11, 1, 0, 5, "C"),
    CLT(GAUGE_NAME_CLT, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 12, 1.0 / PACK_MULT_TEMPERATURE, -40, 150, "deg C"),
    IAT(GAUGE_NAME_IAT, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 14, 1.0 / PACK_MULT_TEMPERATURE, -40, 150, "deg C"),
    AuxT1("AuxT1", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 16, 1.0 / PACK_MULT_TEMPERATURE, -40, 150, "deg C"),
    AuxT2("AuxT2", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 18, 1.0 / PACK_MULT_TEMPERATURE, -40, 150, "deg C"),

    // throttle, pedal
    TPS(GAUGE_NAME_TPS, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 20, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"), // throttle position sensor
    PPS(GAUGE_NAME_THROTTLE_PEDAL, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 22, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"), // pedal position sensor

    // air flow/mass measurement
    MAF(GAUGE_NAME_MAF, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 26, 1.0 / PACK_MULT_VOLTAGE, 0, 5, "Volts"),
    MAP(GAUGE_NAME_MAP, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 30, 1.0 / PACK_MULT_PRESSURE, 20, 300, "kPa"),

    Lambda(GAUGE_NAME_LAMBDA, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 34, 1.0 / PACK_MULT_LAMBDA, 0.65, 1.2, "lambda"),

    VBATT(GAUGE_NAME_VBAT, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 38, 1.0 / PACK_MULT_VOLTAGE, 4, 18, "Volts"),
    oilPressure(GAUGE_NAME_OIL_PRESSURE, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 40, 1.0 / PACK_MULT_PRESSURE, 0, 5, "X"),
    vvtPositionB1I(GAUGE_NAME_VVT_B1I, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 42, 1.0 / PACK_MULT_ANGLE, 0, 5, "deg"),

    // fuel math
    CHARGE_AIR_MASS("airmass", SensorCategory.OPERATIONS, FieldType.UINT16, 44, 0.001, 0, 3, "g/cyl"),
    crankingFuel(GAUGE_NAME_FUEL_CRANKING, SensorCategory.FUEL, FieldType.UINT16, 46, 1.0 / PACK_MULT_MS, 0, 30, "ms"),
    TARGET_AFR(GAUGE_NAME_TARGET_AFR, SensorCategory.OPERATIONS, FieldType.INT16, 48, 1.0 / PACK_MULT_AFR, 10, 20, "afr"),
    baseFuel(Fields.GAUGE_NAME_FUEL_BASE, SensorCategory.FUEL, FieldType.UINT16, 50, 1.0 / PACK_MULT_MS, 0, 30, "ms"),
    runningFuel(GAUGE_NAME_FUEL_RUNNING, SensorCategory.FUEL, FieldType.UINT16, 52, 1.0 / PACK_MULT_MS, 0, 15, "ms"),
    actualLastInjection(GAUGE_NAME_FUEL_LAST_INJECTION, SensorCategory.FUEL, FieldType.UINT16, 54, 1.0 / PACK_MULT_MS, 0, 30, "ms"),
    injectorDutyCycle(Fields.GAUGE_NAME_FUEL_INJ_DUTY, SensorCategory.FUEL, FieldType.UINT8, 56, 0.5, 0, 100, "%"),
    veValue(GAUGE_NAME_FUEL_VE, SensorCategory.FUEL, FieldType.FLOAT, 57, 0.5, 0, 100, "%"),
    tCharge(GAUGE_NAME_TCHARGE, SensorCategory.FUEL, FieldType.FLOAT, 60, 1.0 / PACK_MULT_TEMPERATURE, 30, 140, "C"),

    // Corrections
    injectorLagMs(GAUGE_NAME_INJECTOR_LAG, SensorCategory.FUEL, FieldType.UINT16, 62, 1.0 / PACK_MULT_MS, 0, 15, "ms"),
    iatCorrection(GAUGE_NAME_FUEL_IAT_CORR, SensorCategory.FUEL, FieldType.INT16, 64, 1.0 / PACK_MULT_PERCENT, 0, 5, "ratio"),
    cltCorrection(GAUGE_NAME_FUEL_CLT_CORR, SensorCategory.FUEL, FieldType.INT16, 66, 1.0 / PACK_MULT_PERCENT, 0, 5, "ratio"),
    fuelPidCorrection("Fuel PID", SensorCategory.FUEL, FieldType.INT16, 70, 1.0 / PACK_MULT_MS, -2, 2, "ms"),

    // Wall model AE
    wallFuelAmount(GAUGE_NAME_FUEL_WALL_AMOUNT, SensorCategory.FUEL, FieldType.UINT16, 72, 1.0 / PACK_MULT_MS, 0, 20, "ms"),
    wallFuelCorrection(GAUGE_NAME_FUEL_WALL_CORRECTION, SensorCategory.FUEL, FieldType.INT16, 74, 0.001, -5, 5, "ms"),

    // TPS/load AE
    engineLoadAccelDelta("load accel delta", SensorCategory.FUEL, FieldType.INT16, 76, 1.0 / PACK_MULT_PERCENT, -5, 5, "ratio"),
    deltaTps(Fields.GAUGE_NAME_FUEL_TPS_ROC, SensorCategory.FUEL, FieldType.INT16, 78, 1.0 / PACK_MULT_PERCENT, -100, 100, "%"),
    tpsAccelFuel(Fields.GAUGE_NAME_FUEL_TPS_EXTRA, SensorCategory.FUEL, FieldType.INT16, 82, 1.0 / PACK_MULT_MS, 0, 200, "ms"),

    // Ignition
    ignitionAdvance("ignition timing", SensorCategory.OPERATIONS, FieldType.INT16, 84, 1.0 / PACK_MULT_ANGLE, 30, 140, "deg"),
    DWELL(Fields.GAUGE_COIL_DWELL_TIME, SensorCategory.OPERATIONS, FieldType.UINT16, 86, 1.0 / PACK_MULT_MS, 1, 10, "ms"),
    coilDutyCycle(Fields.GAUGE_NAME_DWELL_DUTY, SensorCategory.OPERATIONS, FieldType.UINT16, 88, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"),

    // Idle & ETB
    idlePosition("Idle Position", SensorCategory.OPERATIONS, FieldType.INT16, 90, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"),
    etbTarget(GAUGE_NAME_ETB_TARGET, SensorCategory.OTHERS, FieldType.INT16, 92, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"),
    etb1DutyCycle(GAUGE_NAME_ETB_DUTY, SensorCategory.OTHERS, FieldType.INT16, 94, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"),
    etb1Error(GAUGE_NAME_ETB_ERROR, SensorCategory.OTHERS, FieldType.INT16, 96, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"),

    // Fuel system

    // Knock
    knockLevel(GAUGE_NAME_KNOCK_LEVEL, SensorCategory.DEBUG, FieldType.FLOAT, 108, 0, 5),

    // Mode, firmware, protocol, run time
    TIME_SECONDS(GAUGE_NAME_TIME, SensorCategory.OPERATIONS, FieldType.INT, 112, 1, 0, 5, ""),
    engineMode("mode", SensorCategory.OPERATIONS, FieldType.INT, 116, 0, 5),
    FIRMWARE_VERSION(GAUGE_NAME_VERSION, SensorCategory.OPERATIONS, FieldType.INT, 120, 1, 0, 100, "version_f"),
    TS_CONFIG_VERSION(".ini version", SensorCategory.OPERATIONS, FieldType.INT, 124),

    engineMakeCodeNameCrc16("engine crc16", SensorCategory.STATUS, FieldType.UINT16, 138, 0, 5),
    // Errors
    totalTriggerErrorCounter(GAUGE_NAME_TRG_ERR, SensorCategory.STATUS, FieldType.INT, 140, 0, 5),
    lastErrorCode(GAUGE_NAME_WARNING_LAST, SensorCategory.STATUS, FieldType.INT, 150, 0, 5),

    // Debug
    debugFloatField1(GAUGE_NAME_DEBUG_F1, SensorCategory.DEBUG, FieldType.FLOAT, 168, 0, 5),
    debugFloatField2(GAUGE_NAME_DEBUG_F2, SensorCategory.DEBUG, FieldType.FLOAT, 172, 0, 5),
    debugFloatField3(GAUGE_NAME_DEBUG_F3, SensorCategory.DEBUG, FieldType.FLOAT, 176, 0, 5),
    debugFloatField4(GAUGE_NAME_DEBUG_F4, SensorCategory.DEBUG, FieldType.FLOAT, 180, 0, 5),
    debugFloatField5(GAUGE_NAME_DEBUG_F5, SensorCategory.DEBUG, FieldType.FLOAT, 184, 0, 5),
    debugFloatField6(GAUGE_NAME_DEBUG_F6, SensorCategory.DEBUG, FieldType.FLOAT, 188, 0, 5),
    debugFloatField7(GAUGE_NAME_DEBUG_F7, SensorCategory.DEBUG, FieldType.FLOAT, 192, 0, 5),
    debugIntField1(GAUGE_NAME_DEBUG_I1, SensorCategory.DEBUG, FieldType.INT, 196, 0, 5),
    debugIntField2(GAUGE_NAME_DEBUG_I2, SensorCategory.DEBUG, FieldType.INT, 200, 0, 5),
    debugIntField3(GAUGE_NAME_DEBUG_I3, SensorCategory.DEBUG, FieldType.INT, 204, 0, 5),
    debugIntField4("debug i4", SensorCategory.DEBUG, FieldType.INT16, 208, 0, 5),
    debugIntField5("debug i5", SensorCategory.DEBUG, FieldType.INT16, 210, 0, 5),
    accelerationX(GAUGE_NAME_ACCEL_X, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 212, 1.0 / PACK_MULT_PERCENT, -3, 3, "G"),
    accelerationY(GAUGE_NAME_ACCEL_Y, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 214, 1.0 / PACK_MULT_PERCENT, -3, 3, "G"),

    tuneCrc16("tune crc16", SensorCategory.STATUS, FieldType.UINT16, 244, 0, 5),

    // Raw sensors
    rawClt("raw CLT", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 238, 1.0 / PACK_MULT_VOLTAGE, 0, 5, "volts"),
    rawIat("raw IAT", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 240, 1.0 / PACK_MULT_VOLTAGE, 0, 5, "volts"),

    tcuDesiredGear(GAUGE_NAME_DESIRED_GEAR, SensorCategory.SENSOR_INPUTS, FieldType.INT8, 262, 1.0, 0, 100, "gear"),
    flexPercent(GAUGE_NAME_FLEX, SensorCategory.SENSOR_INPUTS, FieldType.INT8, 263, 1.0 / 2, 0, 100, "%"),

    wastegatePosition(GAUGE_NAME_WG_POSITION, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 268, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"),
    idlePositionSensor(GAUGE_NAME_IDLE_POSITION, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 270, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"),

    lowFuelPressure(GAUGE_NAME_FUEL_PRESSURE_LOW, SensorCategory.OPERATIONS, FieldType.INT16, 276, 1.0 / PACK_MULT_PRESSURE, 10, 20, "afr"),
    highFuelPressure(GAUGE_NAME_FUEL_PRESSURE_HIGH, SensorCategory.OPERATIONS, FieldType.INT16, 278, 1.0 / PACK_MULT_HIGH_PRESSURE, 10, 20, "afr"),


    airFuelRatio(GAUGE_NAME_AFR, SensorCategory.OPERATIONS, FieldType.INT16, 282, 1.0 / PACK_MULT_AFR, 10, 20, "afr"),
    airFuelRatio2(GAUGE_NAME_AFR2, SensorCategory.OPERATIONS, FieldType.INT16, 288, 1.0 / PACK_MULT_AFR, 10, 20, "afr"),

    vvtPositionB1E(GAUGE_NAME_VVT_B1E, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 290, 1.0 / PACK_MULT_ANGLE, 0, 5, "deg"),
    vvtPositionB2I(GAUGE_NAME_VVT_B2I, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 292, 1.0 / PACK_MULT_ANGLE, 0, 5, "deg"),
    vvtPositionB2E(GAUGE_NAME_VVT_B2E, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 294, 1.0 / PACK_MULT_ANGLE, 0, 5, "deg"),

    vvtTargetB1I(GAUGE_NAME_VVT_TARGET_B1I, SensorCategory.SENSOR_INPUTS, FieldType.INT8, 314, 1, -50, 50, "deg"),
    vvtTargetB1E(GAUGE_NAME_VVT_TARGET_B1E, SensorCategory.SENSOR_INPUTS, FieldType.INT8, 315, 1, -50, 50, "deg"),
    vvtTargetB2I(GAUGE_NAME_VVT_TARGET_B2I, SensorCategory.SENSOR_INPUTS, FieldType.INT8, 316, 1, -50, 50, "deg"),
    vvtTargetB2E(GAUGE_NAME_VVT_TARGET_B2E, SensorCategory.SENSOR_INPUTS, FieldType.INT8, 317, 1, -50, 50, "deg"),

    accelerationZ(GAUGE_NAME_ACCEL_Z, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 308, 1.0 / PACK_MULT_PERCENT, -3, 3, "G"),
    accelerationRoll(GAUGE_NAME_ACCEL_ROLL, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 310, 1.0 / PACK_MULT_PERCENT, -30, 30, "deg/s"),
    accelerationYaw(GAUGE_NAME_ACCEL_YAW, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 312, 1.0 / PACK_MULT_PERCENT, -30, 30, "deg/s"),

    // Synthetic (console only) channels
    ETB_CONTROL_QUALITY("ETB metric", SensorCategory.SNIFFING, "", 100),
    ;

    private final String name;
    private final SensorCategory category;
    private final String units;
    private final double minValue;
    private final double maxValue;
    @Nullable
    private final FieldType type;
    private final int offset;
    private final double scale;

    Sensor(String name, SensorCategory category, FieldType type, int offset, double scale, double minValue, double maxValue, String units) {
        this.name = name == null ? name() : name;
        this.type = type;
        this.offset = offset;
        this.scale = scale;
        this.category = category;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
    }

    Sensor(String name, SensorCategory category, FieldType type, int offset, double minValue, double maxValue) {
        this(name, category, type, offset, 1.0, minValue, maxValue, "n/a");
    }

    Sensor(String name, SensorCategory category, FieldType type, int offset) {
        this(name, category, type, offset, 0, 100);
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
        this.name = name;
        this.category = category;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
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

    public double getValueForChannel(ByteBuffer bb) {
        switch (getType()) {
            case FLOAT:
                return bb.getFloat();
            case INT:
                return bb.getInt();
            case UINT16:
                // no cast - we want to discard sign
                return bb.getInt() & 0xFFFF;
            case INT16:
                // cast - we want to retain sign
                return  (short)(bb.getInt() & 0xFFFF);
            case UINT8:
                // no cast - discard sign
                return bb.getInt() & 0xFF;
            case INT8:
                // cast - retain sign
                return (byte)(bb.getInt() & 0xFF);
            default:
                throw new UnsupportedOperationException("type " + getType());
        }
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

    public void writeToLog(DataOutputStream dos, double value) throws IOException {
        switch (type) {
            case INT8:
            case UINT8:
                dos.write((int) value);
                return;
            case FLOAT:
                dos.writeFloat((float) value);
                return;
            case UINT16:
            case INT16:
                dos.writeShort((int) value);
                return;
            case INT:
                dos.writeInt((int) value);
                return;
            default:
                throw new UnsupportedOperationException("Type " + type);
        }
    }

    public String getLogValue(double value) {
        if (scale == 1 && type != null) {
            // only handle sensors without scale, i.e. not packed floats
            switch (type) {
                case UINT16: {
                    int v = ((int) value) & 0xFFFF;
                    return Integer.toString(v);
                }
            }

        }

        return Double.toString(value);
    }
}
