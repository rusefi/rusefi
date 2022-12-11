package com.rusefi.core;

public class Tuple<Z> extends Pair<String, String> {

    public final Z third;

    public Tuple(String first, String second, Z third) {
        super(first, second);
        this.third = third;
    }
}
