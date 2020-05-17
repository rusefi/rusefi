package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class Msq {
    private final Page page = new Page();

    @XmlAttribute
    public String getXmlns() {
        return "http://www.msefi.com/:msq";
    }

    @XmlElement
    public Bibliography getBibliography() {
        return new Bibliography();
    }

    @XmlElement
    public VersionInfo getVersionInfo() {
        return new VersionInfo();
    }

    @XmlElement
    public Page getPage() {
        return page;
    }
}
