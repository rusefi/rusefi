package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;
import java.util.Objects;

public class VersionInfo {
    private String firmwareInfo;
    private String signature;

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
        return signature;
    }

    public void setFirmwareInfo(String firmwareInfo) {
        this.firmwareInfo = firmwareInfo;
    }

    public void setSignature(String signature) {
        this.signature = signature;
    }

    public void validate() {
        Objects.requireNonNull(signature, "signature");
    }
}
