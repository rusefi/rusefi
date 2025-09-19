package com.rusefi.core.net;

import com.devexperts.logging.Logging;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import static com.devexperts.logging.Logging.getLogging;

public enum PropertiesHolder {
    INSTANCE;

    private static final Logging log = getLogging(PropertiesHolder.class);

    private static final String IO_PROPERTIES = "/shared_io.properties";

    // sad: we have failed to completely understand #6777, now we simply launch separate process instead
    private volatile Properties properties;

    public static String getProperty(String key, String defaultValue) {
        return INSTANCE.getProperties().getProperty(key, defaultValue);
    }

    public static String getProperty(String key) {
        return INSTANCE.getProperties().getProperty(key);
    }

    public Properties getProperties() {
        if (properties == null) {
            properties = getPropertiesForReal();
        }
        return properties;
    }

    private static Properties getPropertiesForReal() throws RuntimeException {
        Properties props = new Properties();
        try {
            InputStream stream = ConnectionAndMeta.class.getResourceAsStream(IO_PROPERTIES);
            if (stream == null) {
                if (new File(".").getCanonicalPath().contains("!\\"))
                    throw new IllegalArgumentException("Use folder names without exclamation marks at the end");
                throw new NullPointerException("Error opening resource stream " + IO_PROPERTIES);
            }
            props.load(stream);
            return props;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static String getBaseUrl() {
        String propertyFileValue = INSTANCE.getProperties().getProperty("auto_update_root_url");
        log.info("auto_update_root_url=" + propertyFileValue);
        String result = System.getProperty("RE_UPDATE_URL", propertyFileValue);
        log.info("auto_update_root_url getProperty=" + propertyFileValue);
        return result;
    }

    public static boolean getBoolean(String propertyName, Properties properties) {
        String flag = properties.getProperty(propertyName);
        return Boolean.TRUE.toString().equalsIgnoreCase(flag);
    }

}
