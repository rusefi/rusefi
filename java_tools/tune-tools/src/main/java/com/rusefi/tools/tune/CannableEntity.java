package com.rusefi.tools.tune;

public interface CannableEntity {
    String getCsourceMethod(String reference, String methodNamePrefix);

    String getCinvokeMethod(String methodNamePrefix);
}
