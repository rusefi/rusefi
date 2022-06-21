package com.rusefi.sensor_logs;

public class BinaryLogEntry {
    private final String name;
    private final String unit;
    private final int byteSize;

    public BinaryLogEntry(String name, String unit, int byteSize) {
        this.name = name;
        this.unit = unit;
        this.byteSize = byteSize;
    }

    public String getName() {
        return name;
    }

    public String getUnit() {
        return unit;
    }

    public int getByteSize() {
        return byteSize;
    }
}
