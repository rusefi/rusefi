package com.rusefi.tune.xml;

import com.rusefi.config.generated.Fields;

import javax.xml.bind.annotation.XmlAttribute;

public class VersionInfo {
    private String firmwareInfo;

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
    public String getFirmwareInfo() {
        return firmwareInfo;
    }

    @XmlAttribute
    public String getNPages() {
        return "1";
    }

    @XmlAttribute
    public String getSignature() {
        return Fields.TS_SIGNATURE;
    }
}
