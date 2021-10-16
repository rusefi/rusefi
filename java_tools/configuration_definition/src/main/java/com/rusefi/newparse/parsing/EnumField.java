package com.rusefi.newparse.parsing;

public class EnumField extends PrototypeField {
    public final Type type;
    public final String enumType;
    public final int endBit;
    public final String[] values;
    public final FieldOptions options;

    public EnumField(Type type, String enumType, String name, int endBit, String[] values, FieldOptions options) {
        super(name);

        this.type = type;
        this.enumType = enumType;
        this.endBit = endBit;
        this.values = values;
        this.options = options;
    }

    @Override
    public String toString() {
        return "enum " + type.cType + " " + this.name;
    }
}
