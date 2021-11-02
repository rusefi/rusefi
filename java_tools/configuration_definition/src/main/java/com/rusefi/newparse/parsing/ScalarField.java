package com.rusefi.newparse.parsing;

public class ScalarField extends PrototypeField {
    public final Type type;
    public final FieldOptions options;
    public final Boolean autoscale;

    public ScalarField(Type type, String name, FieldOptions options, boolean autoscale) {
        super(name);

        this.type = type;
        this.options = options;
        this.autoscale = autoscale;
    }

    @Override
    public String toString() {
        return type.cType + " " + name;
    }
}
