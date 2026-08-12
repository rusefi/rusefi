package com.rusefi.core.preferences.storage;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;

import java.beans.XMLDecoder;
import java.beans.XMLEncoder;
import java.io.*;
import java.util.HashMap;
import java.util.Map;

import static com.devexperts.logging.Logging.getLogging;

public class PersistentConfiguration {
    private static final Logging log = getLogging(PersistentConfiguration.class);
    private static final PersistentConfiguration INSTANCE = new PersistentConfiguration();

    private static boolean hookRegistered;

    static {
        new File(FileUtil.RUSEFI_SETTINGS_FOLDER).mkdirs();
    }

    private static final String CONFIG_FILE_NAME = FileUtil.RUSEFI_SETTINGS_FOLDER + "console_properties.xml";

    private Map<String, Object> config = new HashMap<>();
    private boolean isLoaded;

    public static PersistentConfiguration getConfig() {
        return INSTANCE;
    }

    private PersistentConfiguration() {
    }

    /**
     * Does not work under TS plugin looks like TS does a violent System.exit?
     */
    public static synchronized void registerShutdownHook() {
        if (hookRegistered) {
            return;
        }
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            log.info("Shutdown hook...");
            getConfig().save();
            log.info("Shutdown hook!");
        }));
        hookRegistered = true;
    }

    public static boolean getBoolProperty(String name) {
        return getConfig().getRoot().getBoolProperty(name);
    }

    public static boolean getBoolProperty(String name, boolean defaultValue) {
        return getConfig().getRoot().getBoolProperty(name, defaultValue);
    }

    public static void setBoolProperty(String name, boolean value) {
        getConfig().getRoot().setBoolProperty(name, value);
    }

    @SuppressWarnings("unchecked")
    public void load() {
        if (!config.isEmpty()) {
            log.info("Configuration already loaded");
            return;
        }
        try {
            BufferedInputStream inputStream = new BufferedInputStream(new FileInputStream(CONFIG_FILE_NAME));
            /**
             * interesting things are happening while this is executed under TS as a plugin so we need non-default CL parameter
             */
            XMLDecoder e = new XMLDecoder(inputStream, null, e1 -> {
                // empty not null exception listener replaces default exception listener which prints to System.err
            }, PersistentConfiguration.class.getClassLoader());
            config = (Map<String, Object>) e.readObject();
            e.close();
            log.info("Console configuration from " + CONFIG_FILE_NAME);
        } catch (Throwable e) {
            log.info("Console configuration not found " + CONFIG_FILE_NAME + ", using defaults");
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
            log.info("Saved settings to " + CONFIG_FILE_NAME);
        } catch (FileNotFoundException e) {
            log.info("Error saving " + CONFIG_FILE_NAME + e);
        }
    }

    public Node getRoot() {
        if (!isLoaded)
            load();
        return new Node("root", config);
    }
}
