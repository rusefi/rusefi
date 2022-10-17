package com.rusefi.core.preferences.storage;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.HashMap;
import java.util.Map;

/**
 * @see PersistentConfiguration
 */
public class Node {
    private String prefix;
    private Map<String, Object> config = new HashMap<>();

    /**
     * this is needed for XMLEncoder
     */
    public Node() {
    }

    protected Node(String prefix, Map<String, Object> parent) {
        this.prefix = prefix;
        if (parent.containsKey(prefix)) {
            Node existing = (Node) parent.get(prefix);
            config = existing.getConfig();
        } else {
            parent.put(prefix, this);
        }
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    public static boolean isNumeric(String str) {
        try {
            Double.parseDouble(str);
        } catch (NumberFormatException nfe) {
            return false;
        }
        return true;
    }

    @NotNull
    public Node getChild(String name) {
        Node child = (Node) config.get(name);
        if (child == null) {
            child = new Node(name, config);
        }
        return child;
    }

    public int getIntProperty(String key, int defaultValue) {
        try {
            String value = (String) config.get(key);
            return Integer.parseInt(value);
        } catch (Throwable e) {
            return defaultValue;
        }
    }

    public boolean getBoolProperty(String key) {
        return getBoolProperty(key, false);
    }

    public boolean getBoolProperty(String key, boolean defaultValue) {
        String value = (String) config.get(key);
        if (value == null)
            return defaultValue;
        return Boolean.parseBoolean(value);
    }

    public void setProperty(String key, String value) {
        config.put(key, value);
    }

    public void setProperty(String key, int value) {
        config.put(key, "" + value);
    }

    public void setProperty(String key, boolean value) {
        config.put(key, "" + value);
    }

    public void setBoolProperty(String key, boolean b) {
        setProperty(key, "" + b);
    }

    @Nullable
    public String getProperty(String key, @Nullable String defaultValue) {
        String value = (String) config.get(key);
        return value == null ? defaultValue : value;
    }

    @NotNull
    public String getProperty(String key) {
        return getProperty(key, "");
    }

    // this method is used by serializer
    @SuppressWarnings("unused")
    public String getPrefix() {
        return prefix;
    }

    // this method is used by serializer
    @SuppressWarnings("unused")
    public void setPrefix(String prefix) {
        this.prefix = prefix;
    }

    public Map<String, Object> getConfig() {
        return config;
    }

    public void setConfig(Map<String, Object> config) {
        this.config = config;
    }

    @Override
    public String toString() {
        return "Node{" +
                "prefix='" + prefix + '\'' +
                '}';
    }

    public void setProperty(String key, double value) {
        setProperty(key, Double.toString(value));
    }

    public double getDoubleProperty(String key, double defaultValue) {
        String value = getProperty(key);
        if (isNumeric(value)) {
            return Double.parseDouble(value);
        } else {
            return defaultValue;
        }
    }
}