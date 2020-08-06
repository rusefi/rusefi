package com.rusefi.ldmp;

import java.util.Objects;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class FieldRequest extends Request implements FieldReference  {
    private final String stateContext;
    private final String field;

    public FieldRequest(String stateContext, String field) {
        this.stateContext = stateContext;
        this.field = field;
    }

    @Override
    public String getStateContext() {
        return stateContext;
    }

    @Override
    public String getField() {
        return field;
    }

    @Override
    public String toString() {
        return "FieldRequest{" +
                "stateContext='" + stateContext + '\'' +
                ", field='" + field + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        FieldRequest request = (FieldRequest) o;
        return stateContext.equals(request.stateContext) &&
                field.equals(request.field);
    }

    @Override
    public int hashCode() {
        return Objects.hash(stateContext, field);
    }

    @Override
    public String getGeneratedJavaCode() {
        return GLOBAL_PREFIX + "new " + getClass().getSimpleName() + "("
                + quoteString(stateContext)
                + ", "
                + quoteString(field)
                + ")," + EOL;
    }

}
