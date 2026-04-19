package com.opensr5.ini;

public class IndicatorModel {
    private final String expression;
    private final String offLabel;
    private final String onLabel;
    private final String offBg;
    private final String offFg;
    private final String onBg;
    private final String onFg;

    public IndicatorModel(String expression, String offLabel, String onLabel, String offBg, String offFg, String onBg, String onFg) {
        this.expression = expression;
        this.offLabel = offLabel;
        this.onLabel = onLabel;
        this.offBg = offBg;
        this.offFg = offFg;
        this.onBg = onBg;
        this.onFg = onFg;
    }

    public String getExpression() {
        return expression;
    }

    public String getOffLabel() {
        return offLabel;
    }

    public String getOnLabel() {
        return onLabel;
    }

    public String getOffBg() {
        return offBg;
    }

    public String getOffFg() {
        return offFg;
    }

    public String getOnBg() {
        return onBg;
    }

    public String getOnFg() {
        return onFg;
    }

    @Override
    public String toString() {
        return "IndicatorModel{" +
                "expression='" + expression + '\'' +
                ", offLabel='" + offLabel + '\'' +
                ", onLabel='" + onLabel + '\'' +
                '}';
    }
}
