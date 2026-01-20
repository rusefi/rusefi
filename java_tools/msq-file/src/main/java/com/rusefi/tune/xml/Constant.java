package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlValue;
import java.util.Objects;

public class Constant {
    private String name;
    private String units;
    private String value;
    private String digits;
    private String rows;
    private String cols;

    public Constant() {
    }

    public Constant(String name, String units, String value, String digits) {
        this.name = name;
        this.units = units;
        this.value = value;
        this.digits = digits;
    }

    public Constant(
        final String name,
        final String units,
        final String value,
        final String digits,
        final String rows,
        final String cols
    ) {
        this(name, units, value, digits);
        this.rows = rows;
        this.cols = cols;
    }

    @XmlAttribute
    public String getName() {
        return name;
    }

    @XmlAttribute
    public String getUnits() {
        return units;
    }

    @XmlValue
    public String getValue() {
        return value;
    }

    @XmlAttribute
    public String getDigits() {
        return digits;
    }

    @XmlAttribute
    public String getRows() {
        return rows;
    }

    @XmlAttribute
    public String getCols() {
        return cols;
    }

    public void setCols(String cols) {
        this.cols = cols;
    }

    public void setRows(String rows) {
        this.rows = rows;
    }

    public void setDigits(String digits) {
        this.digits = digits;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setUnits(String units) {
        this.units = units;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public Constant cloneWithValue(final String newValue) {
        return new Constant(getName(), getUnits(), newValue, getDigits(), getRows(), getCols());
    }

    @Override
    public String toString() {
        return "Constant{" +
            "name='" + name + '\'' +
            ", units='" + units + '\'' +
            ", value='" + value + '\'' +
            ", digits='" + digits + '\'' +
            ", rows='" + rows + '\'' +
            ", cols='" + cols + '\'' +
            '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass())
            return false;
        Constant constant = (Constant) o;
        return equalsWithoutUnits(o) &&
            Objects.equals(getUnits(), constant.getUnits());
    }

    public boolean equalsWithoutUnits(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Constant constant = (Constant) o;
        return Objects.equals(getName(), constant.getName())
            && Objects.equals(getValue(), constant.getValue()) && Objects.equals(getDigits(), constant.getDigits())
            && Objects.equals(getRows(), constant.getRows()) && Objects.equals(getCols(), constant.getCols());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getName(), getUnits(), getValue(), getDigits(), getRows(), getCols());
    }
}
