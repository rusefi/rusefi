package com.rusefi.core;

import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.TsOutputs;
import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;
import java.util.Set;
import java.util.TreeSet;

import static com.rusefi.config.generated.Fields.*;

public enum Sensor {
    // RPM, vss
    RPMValue(GAUGE_NAME_RPM, FieldType.UINT16, TsOutputs.RPMVALUE, 1, "RPM"),
    vehicleSpeedKph("Vehicle Speed", FieldType.UINT8, TsOutputs.VEHICLESPEEDKPH, 1, "kph"),

    // Temperatures
    INT_TEMP(GAUGE_NAME_ECU_TEMPERATURE, FieldType.INT8, TsOutputs.INTERNALMCUTEMPERATURE, 1, "C"),
    // throttle, pedal
    TPS(GAUGE_NAME_TPS, FieldType.INT16, TsOutputs.TPSVALUE, 1.0 / PACK_MULT_PERCENT, "%"), // throttle position sensor

    // air flow/mass measurement
    MAF(GAUGE_NAME_MAF, FieldType.UINT16, TsOutputs.MAFMEASURED, 1.0 / PACK_MULT_MASS_FLOW, "Volts"),
    MAP(GAUGE_NAME_MAP, FieldType.UINT16, TsOutputs.MAPVALUE, 1.0 / PACK_MULT_PRESSURE, "kPa"),

    VBATT(GAUGE_NAME_VBAT, FieldType.UINT16, TsOutputs.VBATT, 1.0 / PACK_MULT_VOLTAGE, "Volts"),
    // Mode, firmware, protocol, run time
    TIME_SECONDS(GAUGE_NAME_TIME, FieldType.INT, TsOutputs.SECONDS, 1, ""),

    // Errors
    totalTriggerErrorCounter(GAUGE_NAME_TRG_ERR, FieldType.INT, TsOutputs.TOTALTRIGGERERRORCOUNTER),

    // Debug
    debugIntField1(GAUGE_NAME_DEBUG_I1, FieldType.INT, TsOutputs.DEBUGINTFIELD1),

    // Raw sensors
    rawClt("raw CLT", FieldType.INT16, TsOutputs.RAWCLT, 1.0 / PACK_MULT_VOLTAGE, "volts"),
    rawIat("raw IAT", FieldType.INT16, TsOutputs.RAWIAT, 1.0 / PACK_MULT_VOLTAGE, "volts"),
    ;

    private final String name;
    private final String units;

    @NotNull
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

    Sensor(String name, FieldType type, Field field, double scale, String units) {
        this.name = name == null ? name() : name;
        this.type = type;
        this.offset = field.getTotalOffset();
        this.scale = scale;
        this.units = units;
    }

    Sensor(String name, FieldType type, Field field) {
        this(name, type, field, 1.0, "n/a");
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
