package com.opensr5.ini;

import java.util.ArrayList;
import java.util.List;

public class GroupMenuModel implements MenuItem {
    private final String name;
    private final List<MenuItem> items = new ArrayList<>();

    public GroupMenuModel(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public List<MenuItem> getItems() {
        return items;
    }

    @Override
    public String toString() {
        return "GroupMenuModel{" +
                "name='" + name + '\'' +
                ", items=" + items.size() +
                '}';
    }
}
