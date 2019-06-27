package com.rusefi.ldmp;

import java.util.List;

import static com.rusefi.ConfigDefinition.EOL;

public abstract class Request {
    static String GLOBAL_PREFIX = "\t\t\t";

    static String printList(List<Request> r) {
        StringBuilder java = new StringBuilder();
        for (Request request : r)
            java.append(request.getJavaCode());
        return java.toString();
    }

    public abstract String getJavaCode();

    protected String withSimpleParameter(String parameter) {
        return GLOBAL_PREFIX + "new " + getClass().getSimpleName() + "(" + parameter + ")," + EOL;
    }

    protected String quoteString(String variable) {
        return "\"" + variable + "\"";
    }
}
