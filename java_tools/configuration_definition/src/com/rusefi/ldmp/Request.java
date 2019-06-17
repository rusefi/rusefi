package com.rusefi.ldmp;

import static com.rusefi.ConfigDefinition.EOL;

public class Request {
    public String getJavaCode() {
        return withSimpleParameter("");
    }

    protected String withSimpleParameter(String parameter) {
        return "\t\t\tnew " + getClass().getSimpleName() + "(" + parameter + ")," + EOL;
    }
}
