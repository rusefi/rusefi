package com.rusefi.native_;

/**
 * Native interface allowing java to invoke rusEFI unit_tests library
 */
public class EngineLogic {
    public native String getVersion();

    public native void setSensor(String sensorTypeName, double value);

    public native byte[] getConfiguration();

    public native void setConfiguration(byte[] content, int offset, int size);

    public native void setEngineType(int engineType);

    public native void invokePeriodicCallback();

    public native byte[] getOutputs();
}
