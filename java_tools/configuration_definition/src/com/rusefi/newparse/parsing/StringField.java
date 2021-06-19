package com.rusefi.newparse.parsing;

public class StringField extends PrototypeField{
    public final int size;

    public StringField(String name, int size) {
        super(name);

        this.size = size;
    }
}
