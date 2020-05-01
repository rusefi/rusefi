package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import java.util.ArrayList;
import java.util.List;

public class DialogModel {
    private final String dialogTitle;
    private final String imageName;
    @XmlElementWrapper
    @XmlElement(name = "field")
    public List<FieldModel> fields = new ArrayList<>();

    public DialogModel(String dialogTitle, String imageName) {
        this.dialogTitle = dialogTitle;
        this.imageName = imageName;
    }

    @XmlAttribute
    public String getImageName() {
        return imageName;
    }

    @XmlAttribute
    public String getDialogTitle() {
        return dialogTitle;
    }
}
