package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import java.util.ArrayList;
import java.util.List;

public class DialogDescription {
    private final String dialogTitle;
    @XmlElementWrapper
    @XmlElement(name = "field")
    public List<FieldDescription> fields = new ArrayList<>();

    public DialogDescription(String dialogTitle) {
        this.dialogTitle = dialogTitle;
    }

    @XmlAttribute
    public String getDialogTitle() {
        return dialogTitle;
    }
}
