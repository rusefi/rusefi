package com.opensr5.ini;

public class SubMenuModel implements MenuItem {
    private final String key;
    private final String name;
    private final String enableExpression;
    private final String visibleExpression;

    public SubMenuModel(String key, String name) {
        this(key, name, null, null);
    }

    public SubMenuModel(String key, String name, String enableExpression, String visibleExpression) {
        this.key = key;
        this.name = name;
        this.enableExpression = enableExpression;
        this.visibleExpression = visibleExpression;
    }

    public String getKey() {
        return key;
    }

    public String getName() {
        return name;
    }

    public String getEnableExpression() {
        return enableExpression;
    }

    public String getVisibleExpression() {
        return visibleExpression;
    }

    @Override
    public String toString() {
        return "SubMenuModel{" +
                "key='" + key + '\'' +
                ", name='" + name + '\'' +
                ", enableExpression='" + enableExpression + '\'' +
                ", visibleExpression='" + visibleExpression + '\'' +
                '}';
    }
}
