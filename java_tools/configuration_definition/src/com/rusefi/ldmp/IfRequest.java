package com.rusefi.ldmp;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static com.rusefi.ConfigDefinition.EOL;

public class IfRequest extends Request implements FieldReference {
    private final String stateContext;
    private final String variable;
    public List<Request> trueBlock = new ArrayList<>();
    public List<Request> falseBlock = new ArrayList<>();

    public IfRequest(String stateContext, String variable) {
        this.stateContext = stateContext;
        this.variable = variable;
    }

    public IfRequest(String stateContext, String variable, Request[] trueBlock, Request[] falseBlock) {
        this(stateContext, variable);
        this.trueBlock.addAll(Arrays.asList(trueBlock));
        this.falseBlock.addAll(Arrays.asList(falseBlock));
    }

    @Override
    public String getStateContext() {
        return stateContext;
    }

    @Override
    public String toString() {
        return "IfRequest{" +
                "variable='" + variable + '\'' +
                ", trueBlock=" + trueBlock +
                ", falseBlock=" + falseBlock +
                '}';
    }

    @Override
    public String getField() {
        return variable;
    }

    @Override
    public String getGeneratedJavaCode() {
        String result = Request.GLOBAL_PREFIX;

        Request.GLOBAL_PREFIX = Request.GLOBAL_PREFIX + "\t";

        result += "new IfRequest("
                + quoteString(stateContext)
                + ", "
                + quoteString(variable)
                + "," + EOL +
                Request.GLOBAL_PREFIX + "new Request[]{" + EOL +
                Request.getGeneratedJavaCode(trueBlock) +
                "}," + EOL +
                Request.GLOBAL_PREFIX + "new Request[]{" + EOL +
                Request.getGeneratedJavaCode(falseBlock) +
                "})," + EOL;

        Request.GLOBAL_PREFIX = Request.GLOBAL_PREFIX.substring(1);

        return result;
    }
}
