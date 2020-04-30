package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class FieldDescription {

    private final String uiName;
    private final String fieldName;
    private final String fileName;

    public FieldDescription(String uiName, String fieldName, String fileName) {
        this.uiName = uiName;
        this.fieldName = fieldName;
        this.fileName = fileName;
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
}
