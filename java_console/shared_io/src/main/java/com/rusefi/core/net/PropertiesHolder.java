package com.rusefi.core.net;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;

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
            loadRemoteOverrides(props);
            return props;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private static void loadRemoteOverrides(Properties local) {
        String apiBaseUrl = System.getProperty("rusefi.api.url",   local.getProperty("api_base_url", "")).trim();
        String apiToken   = System.getProperty("rusefi.api.token", local.getProperty("api_token",   "")).trim();
        if (apiBaseUrl.isEmpty()) return;

        // 1. Global console properties — always applied
        merge(local, RemotePropertiesLoader.load(apiBaseUrl, apiToken, "universal"));

        // 2. Board-specific properties overlay on top (board wins over global)
        String boardTarget = BundleUtil.getBundleTarget();
        if (boardTarget != null && !boardTarget.isBlank()
                && !"universal".equalsIgnoreCase(boardTarget)
                && !"unknown".equalsIgnoreCase(boardTarget)) {
            merge(local, RemotePropertiesLoader.load(apiBaseUrl, apiToken, boardTarget));
        }
    }

    private static void merge(Properties base, Properties overrides) {
        for (String key : overrides.stringPropertyNames()) {
            base.setProperty(key, overrides.getProperty(key));
        }
    }

    public static String getBaseUrl() {
        String propertyFileValue = INSTANCE.getProperties().getProperty("auto_update_root_url");
        log.info("auto_update_root_url=" + propertyFileValue);
        String result = System.getProperty("RE_UPDATE_URL", propertyFileValue);
        log.info("auto_update_root_url getProperty=" + propertyFileValue);
        return result;
    }

    public static String getPinoutBaseUrl() {
        String propertyFileValue = INSTANCE.getProperties().getProperty("pinout_base_url");
        log.info("pinout_base_url=" + propertyFileValue);
        return System.getProperty("RE_PINOUT_URL", propertyFileValue);
    }

    public static String getPinoutMetaName() {
        String propertyFileValue = INSTANCE.getProperties().getProperty("pinout_meta_name", "boards_meta.yaml");
        log.info("pinout_meta_name=" + propertyFileValue);
        return System.getProperty("RE_PINOUT_META", propertyFileValue);
    }

    public static boolean getBoolean(String propertyName, Properties properties) {
        String flag = properties.getProperty(propertyName);
        return Boolean.TRUE.toString().equalsIgnoreCase(flag);
    }

}
