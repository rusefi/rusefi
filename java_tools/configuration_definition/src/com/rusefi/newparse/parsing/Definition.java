package com.rusefi.newparse.parsing;

public class Definition {
    public final String name;
    public final Object value;
    public final OverwritePolicy overwritePolicy;

    public enum OverwritePolicy {
        NotAllowed,
        Replace,
        IgnoreNew
    }

    public Definition(String name, Object value, OverwritePolicy overwritePolicy) {
        this.name = name;
        this.value = value;
        this.overwritePolicy = overwritePolicy;
    }

    public boolean isNumeric() {
        return this.value instanceof Double || this.value instanceof Integer;
    }

    public double asDouble() {
        if (this.value instanceof Double) {
            return ((Double)this.value);
        } else {
            return ((Integer)this.value).doubleValue();
        }
    }

    @Override
    public String toString() {
        return this.value.toString();
    }
}
