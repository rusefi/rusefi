package com.rusefi.enum_reader;

import com.rusefi.VariableRegistry;

import java.util.Objects;

public class Value implements Comparable<Value> {
    private final String name;
    private final String value;

    public Value(String name, String value) {
        this.name = name;
        this.value = value;
    }

    public String getName() {
        return name;
    }

    public String getValue() {
        return value;
    }

    public int getIntValue() {
        return Integer.parseInt(value);
    }

    @Override
    public int compareTo(Value o) {
        return name.compareTo(o.name);
    }

    @Override
    public String toString() {
        return "Value{" +
                "name='" + name + '\'' +
                ", value='" + value + '\'' +
                '}';
    }

    public int getIntValueMaybeResolve(VariableRegistry registry) {
        try {
            return getIntValue();
        } catch (NumberFormatException e) {
            String resolvedValue = registry.get(value);
            Objects.requireNonNull(resolvedValue, value);
            return Integer.parseInt(resolvedValue);
        }
    }
}
