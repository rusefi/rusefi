package com.rusefi.ldmp;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static com.rusefi.ConfigDefinition.EOL;

public class IfRequest extends Request {
    private final String variable;
    public List<Request> trueBlock = new ArrayList<>();
    public List<Request> falseBlock = new ArrayList<>();

    public IfRequest(String variable) {
        this.variable = variable;
    }

    public IfRequest(String variable, Request[] trueBlock, Request[] falseBlock) {
        this(variable);
        this.trueBlock.addAll(Arrays.asList(trueBlock));
        this.falseBlock.addAll(Arrays.asList(falseBlock));
    }

    @Override
    public String toString() {
        return "IfRequest{" +
                "variable='" + variable + '\'' +
                ", trueBlock=" + trueBlock +
                ", falseBlock=" + falseBlock +
                '}';
    }

    public String getVariable() {
        return variable;
    }

    @Override
    public String getJavaCode() {
        String result = Request.GLOBAL_PREFIX;

        Request.GLOBAL_PREFIX = Request.GLOBAL_PREFIX + "\t";

        result += "new IfRequest(" + quoteString(variable) + "," + EOL +
                Request.GLOBAL_PREFIX + "new Request[]{" + EOL +
                Request.printList(trueBlock) +
                "}," + EOL +
                Request.GLOBAL_PREFIX + "new Request[]{" + EOL +
                Request.printList(falseBlock) +
                "})," + EOL;

        Request.GLOBAL_PREFIX = Request.GLOBAL_PREFIX.substring(1);

        return result;
    }
}
