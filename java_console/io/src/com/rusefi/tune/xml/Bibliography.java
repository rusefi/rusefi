package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class Bibliography {
    @XmlAttribute
    public String getAuthor() {
        return "rusEFI";
    }

    @XmlAttribute
    public String getTuneComment() {
        return "comments";
    }

    @XmlAttribute
    public String getWriteDate() {
        return "date";
    }
}
