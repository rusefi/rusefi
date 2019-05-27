package com.rusefi.ldmp;

import java.util.ArrayList;
import java.util.List;

public class IfRequest extends Request {
    private final String variable;
    public List<Request> trueBlock = new ArrayList<>();
    public List<Request> falseBlock = new ArrayList<>();

    public IfRequest(String variable) {
        this.variable = variable;
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
}
