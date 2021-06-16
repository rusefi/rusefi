package com.rusefi.newparse.parsing;

public class ScalarField extends PrototypeField {
    public final Type type;
    public final FieldOptions options;

    public ScalarField(Type type, String name, FieldOptions options) {
        super(name);

        this.type = type;
        this.options = options;
    }

    @Override
    public String toString() {
        return type.cType + " " + name;
    }
}
