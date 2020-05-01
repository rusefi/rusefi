package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class FieldModel {

    private final String uiName;
    private final String fieldName;
    private final String imageName;
    private final String tooltip;

    public FieldModel(String uiName, String fieldName, String fileName, String tooltip) {
        this.uiName = uiName;
        this.fieldName = fieldName;
        this.imageName = fileName;
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
    public String getImageName() {
        return imageName;
    }

    @XmlAttribute
    public String getTooltip() {
        return tooltip;
    }
}
