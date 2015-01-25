package com.rusefi.ui.storage;

import java.util.HashMap;
import java.util.Map;

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

    public void setProperty(String key, String value) {
        config.put(key, value);
    }

    public void setProperty(String key, int value) {
        config.put(key, "" + value);
    }

    public String getProperty(String key, String s) {
        String value = (String) config.get(key);
        return value == null ? s : value;
    }

    public String getPrefix() {
        return prefix;
    }

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
}