package com.rusefi.ui.livedata;

public interface VariableValueSource {
    VariableValueSource VOID = name -> null;

    Object getValue(String name);
}
