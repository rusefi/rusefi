package com.rusefi.ldmp;


import java.util.Objects;

import static com.rusefi.ConfigDefinition.EOL;

public class ConfigRequest extends Request {
    //@NotNull
    private final String field;

    public ConfigRequest(String field) {
        Objects.requireNonNull(field);
        this.field = field;
    }

    public String getField() {
        return field;
    }

    @Override
    public String toString() {
        return "ConfigRequest{" +
                "field='" + field + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ConfigRequest that = (ConfigRequest) o;
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
