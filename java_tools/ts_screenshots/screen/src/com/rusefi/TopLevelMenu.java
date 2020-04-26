package com.rusefi;

import com.opensr5.ini.DialogModel;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import java.util.ArrayList;
import java.util.List;

public class TopLevelMenu {
    private final String title;

    @XmlElementWrapper
    @XmlElement(name = "dialog")
    private List<DialogModel> dialogs = new ArrayList<>();

    public TopLevelMenu(String title) {
        this.title = title;
    }

    @XmlAttribute
    public String getTitle() {
        return title;
    }

    public List<DialogModel> getDialogs() {
        return dialogs;
    }
}
