package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlValue;

public class Constant {
    private final String name;
    private final String units;
    private final String value;

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

}
