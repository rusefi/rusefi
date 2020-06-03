package com.rusefi.ui.storage;

import java.beans.XMLDecoder;
import java.beans.XMLEncoder;
import java.io.*;
import java.util.HashMap;
import java.util.Map;

public class PersistentConfiguration {
    private static final PersistentConfiguration INSTANCE = new PersistentConfiguration();
    private static final String CONFIG_FILE_NAME = "rusefi_console_properties.xml";

    private Map<String, Object> config = new HashMap<>();
    private boolean isLoaded;

    public static PersistentConfiguration getConfig() {
        return INSTANCE;
    }

    private PersistentConfiguration() {
        Runtime.getRuntime().addShutdownHook(new Thread(() -> getConfig().save()));
    }

    @SuppressWarnings("unchecked")
    public void load() {
        try {
            XMLDecoder e = new XMLDecoder(new BufferedInputStream(new FileInputStream(CONFIG_FILE_NAME)));
            config = (Map<String, Object>) e.readObject();
            e.close();
            System.out.println("Got configuration from " + CONFIG_FILE_NAME);
        } catch (Throwable e) {
            System.out.println("Console configuration not found " + CONFIG_FILE_NAME + ", using defaults");
        }
        isLoaded = true;
    }

    /**
     * todo: maybe the shutdown hook is the only place where this method should be invoked?
     */
    public void save() {
        if (!isLoaded) {
            // settings not loaded, nothing to save
            return;
        }
        try {
            XMLEncoder e = new XMLEncoder(new BufferedOutputStream(new FileOutputStream(CONFIG_FILE_NAME)));
            e.writeObject(config);
            e.close();
            System.out.println("Saved settings to " + CONFIG_FILE_NAME);
        } catch (FileNotFoundException e1) {
            System.out.println("Error saving " + CONFIG_FILE_NAME);
        }
    }

    public Node getRoot() {
        if (!isLoaded)
            load();
        return new Node("root", config);
    }
}