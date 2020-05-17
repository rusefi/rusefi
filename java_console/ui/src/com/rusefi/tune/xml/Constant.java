package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class Constant {
    private final String name;
    private final String units;

    public Constant(String name, String units) {
        this.name = name;
        this.units = units;
    }

    @XmlAttribute
    public String getName() {
        return name;
    }

    @XmlAttribute
    public String getUnits() {
        return units;
    }
}
