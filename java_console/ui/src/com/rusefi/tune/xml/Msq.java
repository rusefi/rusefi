package com.rusefi.tune.xml;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class Msq {
    public static final String outputXmlFileName = "output.msq";

    private final Page page = new Page();

    private final VersionInfo versionInfo;

    public Msq() {
        versionInfo = new VersionInfo("rusEFI+2020");
    }

    public void loadConstant(IniFileModel ini, String key, ConfigurationImage image) {
        IniField field = ini.allIniFields.get(key);
        String value = field.getValue(image);
        getPage().constant.add(new Constant(field.getName(), field.getUnits(), value));
    }

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
        return versionInfo;
    }

    @XmlElement
    public Page getPage() {
        return page;
    }
}
