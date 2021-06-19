package com.rusefi.newparse.parsing;

public class Definition {
    public final String name;
    public final String value;
    public final OverwritePolicy overwritePolicy;

    public enum OverwritePolicy {
        NotAllowed,
        Replace,
        IgnoreNew
    }

    public Definition(String name, String value, OverwritePolicy overwritePolicy) {
        this.name = name;
        this.value = value;
        this.overwritePolicy = overwritePolicy;
    }
}
