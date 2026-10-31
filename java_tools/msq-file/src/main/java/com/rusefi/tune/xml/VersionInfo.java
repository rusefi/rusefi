package com.rusefi.tune.xml;

import jakarta.xml.bind.annotation.XmlAttribute;
import java.util.Objects;

public class VersionInfo {
    private String firmwareInfo;
    private String signature;
    private int nPages = 1;

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
        return Integer.toString(nPages);
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

    public void setNPages(String nPages) {
        this.nPages = Integer.parseInt(nPages);
    }

    public void setPageCount(int nPages) {
        this.nPages = nPages;
    }

    public void validate() {
        Objects.requireNonNull(signature, "signature");
    }
}
