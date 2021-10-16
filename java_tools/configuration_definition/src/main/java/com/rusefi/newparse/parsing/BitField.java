package com.rusefi.newparse.parsing;

public class BitField {
    public final String name;
    public final String comment;
    public final String trueValue;
    public final String falseValue;

    public BitField(String name, String comment, String trueValue, String falseValue) {
        this.name = name;
        this.comment = comment;
        this.trueValue = trueValue;
        this.falseValue = falseValue;
    }

    @Override
    public String toString() {
        return "BitField: " + this.name;
    }
}
