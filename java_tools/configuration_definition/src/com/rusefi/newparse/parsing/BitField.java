package com.rusefi.newparse.parsing;

public class BitField {
    public final String name;
    public final String comment;

    public BitField(String name, String comment) {
        this.name = name;
        this.comment = comment;
    }

    @Override
    public String toString() {
        return "BitField: " + this.name;
    }
}
