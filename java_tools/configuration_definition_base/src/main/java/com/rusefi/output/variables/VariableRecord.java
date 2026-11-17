package com.rusefi.output.variables;

public class VariableRecord {
    public final String type;
    private final String humanName;
    private final String fullName;
    private final String conditional;
    private final int offset;
    private final int bitOffset;
    /**
     * True when the C++ storage is a scaled_channel with a non-unity scale: such a field is assigned
     * through scaled_channel's float constructor (which rounds and scales), not by an integer store.
     */
    public final boolean isFloatScaled;

    public VariableRecord(String humanName, String fullName, String type, String conditional, int offset, int bitOffset, boolean isFloatScaled) {
        this.humanName = humanName;
        this.fullName = fullName;
        this.type = type;
        this.conditional = conditional;
        this.offset = offset;
        this.bitOffset = bitOffset;
        this.isFloatScaled = isFloatScaled;
    }

    public VariableRecord(String humanName, String fullName, String type, String conditional, int offset, int bitOffset) {
        this(humanName, fullName, type, conditional, offset, bitOffset, false);
    }

    public VariableRecord(String humanName, String fullName, String type, String conditional, int offset) {
        this(humanName, fullName, type, conditional, offset, 0);
    }

    public VariableRecord(String humanName, String fullName, String type, String conditional) {
        this(humanName, fullName, type, conditional, -1);
    }

    public String getUserName() {
        return humanName;
    }

    public String getFullName() {
        return fullName;
    }

    public String getConditional() {
        return conditional;
    }

    public int getOffset() {
        return offset;
    }

    public int getBitOffset() {
        return bitOffset;
    }

    @Override
    public String toString() {
        return "VariableRecord{" +
            "type='" + type + '\'' +
            ", humanName='" + humanName + '\'' +
            ", fullName='" + fullName + '\'' +
            ", conditional='" + conditional + '\'' +
            '}';
    }
}
