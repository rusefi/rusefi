package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlValue;

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
}
