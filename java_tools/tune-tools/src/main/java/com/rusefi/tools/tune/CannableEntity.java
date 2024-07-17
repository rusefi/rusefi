package com.rusefi.tools.tune;

public interface CannableEntity {
    String getCsourceMethod(String reference, String methodNamePrefix, String name);

    String getCinvokeMethod(String methodNamePrefix);

    String getName();
}
