package com.rusefi.newparse.parsing;

public class Definition {
    public final Object value;
    public final OverwritePolicy overwritePolicy;

    public enum OverwritePolicy {
        NotAllowed,
        IgnoreNew
    }

    public Definition(Object value, OverwritePolicy overwritePolicy) {
        this.value = value;
        this.overwritePolicy = overwritePolicy;
    }

    @Override
    public String toString() {
        return value.toString();
    }
}
