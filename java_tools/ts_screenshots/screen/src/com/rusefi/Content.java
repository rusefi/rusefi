package com.rusefi;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import javax.xml.bind.annotation.XmlRootElement;
import java.util.ArrayList;
import java.util.List;

@XmlRootElement
public class Content {
    private List<TopLevelMenu> menus = new ArrayList<>();

    @XmlElementWrapper
    @XmlElement(name = "keyword")
    public List<TopLevelMenu> getTopLevelMenus() {
        return menus;
    }
}
