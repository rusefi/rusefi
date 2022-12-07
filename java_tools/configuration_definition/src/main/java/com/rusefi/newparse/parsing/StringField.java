package com.rusefi.newparse.parsing;

public class StringField extends PrototypeField {
    public final int size;
    public final String comment;

    public StringField(String name, int size, String comment) {
        super(name);

        this.size = size;
        this.comment = comment;
    }
}
