package com.rusefi.ldmp;

import java.util.Objects;

public class FieldRequest extends Request {
    private final String field;

    public FieldRequest(String field) {
        this.field = field;
    }

    public String getField() {
        return field;
    }

    @Override
    public String toString() {
        return "FieldRequest{" +
                "field='" + field + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        FieldRequest that = (FieldRequest) o;
        return field.equals(that.field);
    }

    @Override
    public int hashCode() {
        return Objects.hash(field);
    }

    @Override
    public String getJavaCode() {
        return withSimpleParameter("\"" + field + "\"");
    }
}
