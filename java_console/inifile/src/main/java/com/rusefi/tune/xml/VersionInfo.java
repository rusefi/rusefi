package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class VersionInfo {
    private String firmwareInfo;
    private String tsSignature;

    public VersionInfo() {
    }

    public VersionInfo(String firmwareInfo) {
        this.firmwareInfo = firmwareInfo;
    }

    @XmlAttribute
    public String getVersion() {
        return "5.0";
    }

    @XmlAttribute
    public String getNPages() {
        return "1";
    }

    @XmlAttribute
    public String getFirmwareInfo() {
        return firmwareInfo;
    }

    @XmlAttribute
    public String getSignature() {
        return tsSignature;
    }

    public void setFirmwareInfo(String firmwareInfo) {
        this.firmwareInfo = firmwareInfo;
    }

    public void setTsSignature(String tsSignature) {
        this.tsSignature = tsSignature;
    }
}
