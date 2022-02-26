package com.rusefi.native_;

/**
 * Native interface allowing java to invoke rusEFI unit_tests library
 */
public class EngineLogic {
    public native String getVersion();

    public native void setSensor(String sensorTypeName, double value);
}
