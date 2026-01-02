package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class FieldModel {

    private String uiName;
    private String fieldName;
    private String imageName;
    private String tooltip;
    private Integer x;
    private Integer y;

    public FieldModel() {
    }

    public FieldModel(String uiName, String fieldName, String fileName, String tooltip, Integer x, Integer y) {
        this.uiName = uiName;
        this.fieldName = fieldName;
        this.imageName = fileName;
        this.tooltip = tooltip == null ? "" : tooltip;
        this.x = x;
        this.y = y;
    }

    @XmlAttribute
    public String getUiName() {
        return uiName;
    }

    public void setUiName(String uiName) {
        this.uiName = uiName;
    }

    @XmlAttribute
    public String getFieldName() {
        return fieldName;
    }

    public void setFieldName(String fieldName) {
        this.fieldName = fieldName;
    }

    @XmlAttribute
    public String getImageName() {
        return imageName;
    }

    public void setImageName(String imageName) {
        this.imageName = imageName;
    }

    @XmlAttribute
    public String getTooltip() {
        return tooltip;
    }

    public void setTooltip(String tooltip) {
        this.tooltip = tooltip;
    }

    @XmlAttribute
    public Integer getX() {
        return x;
    }

    public void setX(Integer x) {
        this.x = x;
    }

    @XmlAttribute
    public Integer getY() {
        return y;
    }

    public void setY(Integer y) {
        this.y = y;
    }

    @Override
    public String toString() {
        return "FieldModel{" +
                "uiName='" + uiName + '\'' +
                ", fieldName='" + fieldName + '\'' +
                ", imageName='" + imageName + '\'' +
                ", tooltip='" + tooltip + '\'' +
                '}';
    }
}
