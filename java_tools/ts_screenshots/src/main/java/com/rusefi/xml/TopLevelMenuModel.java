package com.rusefi.xml;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import java.util.ArrayList;
import java.util.List;

public class TopLevelMenuModel {
    private String title;
    private String imageName;

    @XmlElementWrapper
    @XmlElement(name = "dialog")
    private List<DialogModel> dialogs = new ArrayList<>();

    public TopLevelMenuModel() {
    }

    public TopLevelMenuModel(String title, String imageName) {
        this.title = title;
        this.imageName = imageName;
    }

    @XmlAttribute
    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    @XmlAttribute
    public String getImageName() {
        return imageName;
    }

    public void setImageName(String imageName) {
        this.imageName = imageName;
    }

    public List<DialogModel> getDialogs() {
        return dialogs;
    }

    @Override
    public String toString() {
        return "TopLevelMenuModel{" +
                "title='" + title + '\'' +
                ", imageName='" + imageName + '\'' +
                ", dialogs=" + dialogs +
                '}';
    }
}
