package com.opensr5.ini;

import java.util.ArrayList;
import java.util.List;

public class MenuModel {
    private final String name;
    private final List<MenuItem> items = new ArrayList<>();

    public MenuModel(String name) {
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
        return "MenuModel{" +
                "name='" + name + '\'' +
                ", items=" + items.size() +
                '}';
    }
}
