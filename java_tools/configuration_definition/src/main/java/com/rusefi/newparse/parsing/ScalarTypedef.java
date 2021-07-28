package com.rusefi.newparse.parsing;

public class ScalarTypedef extends Typedef {
    public final FieldOptions options;
    public final Type type;

    public ScalarTypedef(String name, Type type, FieldOptions options) {
        super(name);

        this.options = options;
        this.type = type;
    }
}
