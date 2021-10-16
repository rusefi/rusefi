package com.rusefi.newparse.parsing;

public class StringTypedef extends Typedef {
    public final int size;

    public StringTypedef(String name, int size) {
        super(name);

        this.size = size;
    }
}

