package com.rusefi.core;

import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.TsOutputs;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteBuffer;
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
    RPMValue(GAUGE_NAME_RPM, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, TsOutputs.RPMVALUE, 1, 0, 8000, "RPM"),
//    rpmAcceleration("dRPM", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 6, 1.0, 0.0, 5.0, "RPM/s"),
//    speedToRpmRatio("Gearbox Ratio", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 8, 0.01, 0.0, 0.0, "value"),
    vehicleSpeedKph("Vehicle Speed", SensorCategory.SENSOR_INPUTS, FieldType.UINT8, TsOutputs.VEHICLESPEEDKPH, 1.0, 0.0, 0.0, "kph"),

    // Temperatures
    INT_TEMP(GAUGE_NAME_ECU_TEMPERATURE, SensorCategory.OPERATIONS, FieldType.INT8, TsOutputs.INTERNALMCUTEMPERATURE, 1, 0, 5, "C"),
    // throttle, pedal
    TPS(GAUGE_NAME_TPS, SensorCategory.SENSOR_INPUTS, FieldType.INT16, TsOutputs.TPSVALUE, 1.0 / PACK_MULT_PERCENT, 0, 100, "%"), // throttle position sensor

    // air flow/mass measurement
    MAF(GAUGE_NAME_MAF, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, TsOutputs.MAFMEASURED, 1.0 / PACK_MULT_MASS_FLOW, 0, 5, "Volts"),
    MAP(GAUGE_NAME_MAP, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, TsOutputs.MAPVALUE, 1.0 / PACK_MULT_PRESSURE, 20, 300, "kPa"),

    VBATT(GAUGE_NAME_VBAT, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, TsOutputs.VBATT, 1.0 / PACK_MULT_VOLTAGE, 4, 18, "Volts"),
    // Mode, firmware, protocol, run time
    TIME_SECONDS(GAUGE_NAME_TIME, SensorCategory.OPERATIONS, FieldType.INT, TsOutputs.SECONDS, 1, 0, 5, ""),

    // Errors
    totalTriggerErrorCounter(GAUGE_NAME_TRG_ERR, SensorCategory.STATUS, FieldType.INT, TsOutputs.TOTALTRIGGERERRORCOUNTER, 0, 5),
    lastErrorCode(GAUGE_NAME_WARNING_LAST, SensorCategory.STATUS, FieldType.INT, TsOutputs.LASTERRORCODE, 0, 5),

	// Debug
	debugIntField1(GAUGE_NAME_DEBUG_I1, SensorCategory.DEBUG, FieldType.INT, TsOutputs.DEBUGINTFIELD1, 0, 5),

    // Raw sensors
    rawClt("raw CLT", SensorCategory.SENSOR_INPUTS, FieldType.INT16, TsOutputs.RAWCLT, 1.0 / PACK_MULT_VOLTAGE, 0, 5, "volts"),
    rawIat("raw IAT", SensorCategory.SENSOR_INPUTS, FieldType.INT16, TsOutputs.RAWIAT, 1.0 / PACK_MULT_VOLTAGE, 0, 5, "volts"),
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

    static {
        Set<String> NAMES = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

        for (Sensor s : Sensor.values()) {
            if (!NAMES.add(s.name))
                throw new IllegalArgumentException("Unique name expected " + s.name);
        }
    }

    Sensor(String name, SensorCategory category, FieldType type, Field field, double scale, double minValue, double maxValue, String units) {
        this.name = name == null ? name() : name;
        this.type = type;
        this.offset = field.getTotalOffset();
        this.scale = scale;
        this.category = category;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
    }

    Sensor(String name, SensorCategory category, FieldType type, Field field, double minValue, double maxValue) {
        this(name, category, type, field, 1.0, minValue, maxValue, "n/a");
    }

    Sensor(String name, SensorCategory category, FieldType type, Field field) {
        this(name, category, type, field, 0, 100);
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

    public String getUnit() {
        return units;
    }

    public int getByteSize() {
        switch (getType()) {
            case UINT8:
                return 0;
            case INT8:
                return 1;
            case UINT16:
                return 2;
            case INT16:
                return 3;
            case INT:
                return 4;
            case FLOAT:
                return 7;
            default:
                throw new UnsupportedOperationException("" + getType());
        }
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

    static {
        if (TS_FILE_VERSION_OFFSET != TsOutputs.TSCONFIGVERSION.getOffset())
            throw new IllegalStateException("static assert failed TS_FILE_VERSION_OFFSET");
    }
}
