package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class FieldDescription {

    private final String uiName;
    private final String fieldName;
    private final String fileName;
    private final String tooltip;

    public FieldDescription(String uiName, String fieldName, String fileName, String tooltip) {
        this.uiName = uiName;
        this.fieldName = fieldName;
        this.fileName = fileName;
        this.tooltip = tooltip == null ? "" : tooltip;
    }

    @XmlAttribute
    public String getUiName() {
        return uiName;
    }

    @XmlAttribute
    public String getFieldName() {
        return fieldName;
    }

    @XmlAttribute
    public String getFileName() {
        return fileName;
    }

    @XmlAttribute
    public String getTooltip() {
        return tooltip;
    }
}
