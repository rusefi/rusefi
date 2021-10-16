package com.rusefi.newparse.parsing;

public class ArrayTypedef extends Typedef {
    public final FieldOptions options;
    public final Type type;
    public final int[] length;

    public ArrayTypedef(String name, int[] length, Type type, FieldOptions options) {
        super(name);

        this.length = length;
        this.type = type;
        this.options = options;
    }
}
