package com.rusefi.native_;

/**
 * Native interface allowing java to invoke rusEFI unit_tests library
 *
 * See com_rusefi_native__EngineLogic.h
 * See native_impl.cpp
 */
public class EngineLogic {
    public static native String getVersion();

    // huh? I am having trouble making this method static?
    public static native void resetTest();

    public native void setSensor(String sensorTypeName, double value);

    public native byte[] getConfiguration();

    public native void setConfiguration(byte[] content, int offset, int size);

    public native void setEngineType(int engineType);

    public native void initTps();

    public native void invokePeriodicCallback();

    public native void invokeEtbCycle();

    public native void burnRequest();

    public native byte[] getOutputs();
}
