package com.rusefi.enum_reader;

public class Value implements Comparable<Value> {
    private final String name;

    public Value(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    @Override
    public int compareTo(Value o) {
        return name.compareTo(o.name);
    }
}
