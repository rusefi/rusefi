package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Page {
    // this collection is populated through magic of XML
    public final List<Constant> constant = new ArrayList<>();
    public final List<PcVariable> pcVariable = new ArrayList<>();

    private final transient Map<String, Constant> asMap = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    private Integer number;
    private Integer size;

    public Page() {
    }

    public Page(Integer number, Integer size) {
        this.number = number;
        this.size = size;
    }

    @XmlAttribute
    public Integer getNumber() {
        return number;
    }

    public void setNumber(Integer number) {
        this.number = number;
    }

    @XmlAttribute
    public Integer getSize() {
        return size;
    }

    public void setSize(Integer size) {
        this.size = size;
    }

    public Constant findParameter(String name) {
        for (Constant parameter : constant) {
            if (parameter.getName().equals(name)) {
                return parameter;
            }
        }
        return null;
    }

    @Override
    public String toString() {
        return "Page{" +
                "constant.size=" + constant.size() +
                ", number=" + number +
                ", size=" + size +
                '}';
    }

    public Map<String, Constant> getConstantsAsMap() {
        if (asMap.isEmpty()) {
            for (Constant constant : this.constant) {
                asMap.put(constant.getName(), constant);
            }
        }
        return asMap;
    }
}
