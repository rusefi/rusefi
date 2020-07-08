package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlValue;

public class Constant {
    private String name;
    private String units;
    private String value;

    public Constant() {
    }

    public Constant(String name, String units, String value) {
        this.name = name;
        this.units = units;
        this.value = value;
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
