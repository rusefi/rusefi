package com.rusefi.ui.storage;

import com.irnems.FileLog;

import java.beans.XMLDecoder;
import java.beans.XMLEncoder;
import java.io.*;
import java.util.HashMap;
import java.util.Map;

public class PersistentConfiguration {
    private static final PersistentConfiguration INSTANCE = new PersistentConfiguration();
    public static final String CONFIG_FILE_NAME = "rusefi_console_properties.xml";

    private Map<String, Object> config = new HashMap<>();

    public static PersistentConfiguration getConfig() {
        return INSTANCE;
    }

    @SuppressWarnings("unchecked")
    public void load() {
        try {
            XMLDecoder e = new XMLDecoder(new BufferedInputStream(new FileInputStream(CONFIG_FILE_NAME)));
            config = (Map<String, Object>) e.readObject();
            e.close();
        } catch (Throwable e) {
            FileLog.rlog("Error reading from " + CONFIG_FILE_NAME);
        }
    }

    public void save() {
        try {
            XMLEncoder e = new XMLEncoder(new BufferedOutputStream(new FileOutputStream(CONFIG_FILE_NAME)));
            e.writeObject(config);
            e.close();
            System.out.println("Saved to " + CONFIG_FILE_NAME);
        } catch (FileNotFoundException e1) {
            FileLog.rlog("Error saving " + CONFIG_FILE_NAME);
        }
    }

    public Node getRoot() {
        return new Node("root", config);
    }
}