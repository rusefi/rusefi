package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import java.util.ArrayList;
import java.util.List;

public class TopLevelMenuModel {
    private final String title;
    private final String imageName;

    @XmlElementWrapper
    @XmlElement(name = "dialog")
    private List<DialogModel> dialogs = new ArrayList<>();

    public TopLevelMenuModel(String title, String imageName) {
        this.title = title;
        this.imageName = imageName;
    }

    @XmlAttribute
    public String getTitle() {
        return title;
    }

    @XmlAttribute
    public String getImageName() {
        return imageName;
    }

    public List<DialogModel> getDialogs() {
        return dialogs;
    }
}
