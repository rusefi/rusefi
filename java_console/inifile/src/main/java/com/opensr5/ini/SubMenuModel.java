package com.opensr5.ini;

public class SubMenuModel implements MenuItem {
    private final String key;
    private final String name;

    public SubMenuModel(String key, String name) {
        this.key = key;
        this.name = name;
    }

    public String getKey() {
        return key;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() {
        return "SubMenuModel{" +
                "key='" + key + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}
