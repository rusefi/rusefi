package com.rusefi.xml;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import java.util.ArrayList;
import java.util.List;

public class DialogDescription {
    @XmlElementWrapper
    @XmlElement(name = "field")
    public List<FieldDescription> fields = new ArrayList<>();
}
