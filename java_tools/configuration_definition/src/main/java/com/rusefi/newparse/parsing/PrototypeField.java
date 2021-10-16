package com.rusefi.newparse.parsing;

public abstract class PrototypeField implements Field {
    public final String name;

    protected PrototypeField(String name) {
        this.name = name;
    }
}
