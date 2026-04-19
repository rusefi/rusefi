package com.rusefi.core;

import com.rusefi.config.FieldType;
import com.rusefi.sensor_logs.BinaryLogEntry;
import org.jetbrains.annotations.Nullable;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Collection;
import java.util.Comparator;
import java.util.Set;
import java.util.TreeSet;

import static com.rusefi.config.generated.VariableRegistryValues.*;

/**
 * this class is on the way out: most of info should be taken from Gauge meta model
 * @see SensorNames
 * @author Andrey Belomutskiy
 * 2/11/13
 */
@Deprecated
public enum Sensor implements BinaryLogEntry {
    /**
     * Please note that these enum names are used to make 'set_mock_XXX_voltage' commands
     * TODO: we should match this names within the .ini for better consistency on SensorsHolder
     */

    // RPM, vss
    @Deprecated
    RPMGauge(SensorNames.RPMValue, GAUGE_NAME_RPM, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 1, 0, 8000, "RPM"),
//    rpmAcceleration("dRPM", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 6, 1.0, 0.0, 5.0, "RPM/s"),
//    speedToRpmRatio("Gearbox Ratio", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 8, 0.01, 0.0, 0.0, "value"),

    MAPGauge(SensorNames.MAPGauge, GAUGE_NAME_MAP, SensorCategory.SENSOR_INPUTS, FieldType.UINT16, 1.0 / PACK_MULT_PRESSURE, 20, 300, "kPa"),

    // Errors
    totalTriggerErrorCounter("totalTriggerErrorCounter", GAUGE_NAME_TRG_ERR, SensorCategory.STATUS, FieldType.INT, 0, 5),
    lastErrorCode("lastErrorCode", GAUGE_NAME_WARNING_LAST, SensorCategory.STATUS, FieldType.INT, 0, 5),

    // Debug
    debugIntField1("debugIntField1", GAUGE_NAME_DEBUG_I1, SensorCategory.DEBUG, FieldType.INT, 0, 5),
    debugIntField2("debugIntField2", GAUGE_NAME_DEBUG_I2, SensorCategory.DEBUG, FieldType.INT, 0, 5),


    vvtPositionB1I("vvtPositionB1I", GAUGE_NAME_VVT_B1I, SensorCategory.SENSOR_INPUTS, FieldType.INT16, 1.0 / PACK_MULT_ANGLE, 0, 5, "deg"),

    ;

    private final String name;
    private final String nativeName;
    private final SensorCategory category;
    private final String units;
    private final double minValue;
    private final double maxValue;
    @Nullable
    private final FieldType type;
    private final double scale;

    static {
        Set<String> NAMES = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

        for (Sensor s : Sensor.values()) {
            if (!NAMES.add(s.name))
                throw new IllegalArgumentException("Unique name expected " + s.name);
        }
    }

    Sensor(String nativeName, String name, SensorCategory category, FieldType type, double scale, double minValue, double maxValue, String units) {
        this.nativeName = nativeName;
        this.name = name == null ? name() : name;
        this.type = type;
        this.scale = scale;
        this.category = category;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
    }

    Sensor(String nativeName, String name, SensorCategory category, FieldType type, double minValue, double maxValue) {
        this(nativeName, name, category, type, 1.0, minValue, maxValue, "n/a");
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
        this.nativeName = name;
        this.name = name;
        this.category = category;
        this.units = units;
        this.minValue = minValue;
        this.maxValue = maxValue;
        this.scale = 1.0;
        type = null;
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

    @Override
    public String getName() {
        return name;
    }

    public String getNativeName() {
        return nativeName;
    }

    @Override
    public String getUnit() {
        return units;
    }

    @Override
    public int getByteSize() {
        switch (type) {
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
                throw new UnsupportedOperationException("" + type);
        }
    }

    // TODO: this should be a string
    @Override
    public SensorCategory getCategory() {
        return category;
    }

    public double translateValue(double value) {
        return value;
    }

    @Override
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

    private static class Constants {
    }
}
