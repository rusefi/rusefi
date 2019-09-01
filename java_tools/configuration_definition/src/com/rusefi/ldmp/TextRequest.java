package com.rusefi.ldmp;

import java.util.Objects;

/**
 * Static text label
 */
public class TextRequest extends Request {
    private final String value;

    public TextRequest(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }

    public boolean isEol() {
        return "EOL".equalsIgnoreCase(value);
    }

    @Override
    public String toString() {
        return "TextRequest{" +
                "value='" + value + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        TextRequest that = (TextRequest) o;
        return value.equals(that.value);
    }

    @Override
    public int hashCode() {
        return Objects.hash(value);
    }

    @Override
    public String getGeneratedJavaCode() {
        return withSimpleParameter(quoteString(value));
    }
}
