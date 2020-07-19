package com.rusefi.ts_plugin.util;

import com.rusefi.ts_plugin.PluginEntry;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.net.URL;
import java.util.jar.Attributes;
import java.util.jar.Manifest;

public class ManifestHelper {
    private static final String BUILT_DATE = "Built-Date";
    private static final String BUILT_TIMESTAMP = "Built-Timestamp";

    @NotNull
    public static String getVersion() {
        return getAttribute(BUILT_DATE);
    }

    @NotNull
    public static String getBuildTimestamp() {
        return getAttribute(BUILT_TIMESTAMP);
    }

    @NotNull
    private static String getAttribute(String attributeName) {
        // all this magic below to make sure we are reading manifest of the *our* jar file not TS main jar file
        Class clazz = PluginEntry.class;
        String className = clazz.getSimpleName() + ".class";
        String classPath = clazz.getResource(className).toString();
        if (!classPath.startsWith("jar")) {
            // Class not from JAR
            return "Local Run";
        }
        String manifestPath = classPath.substring(0, classPath.lastIndexOf("!") + 1) +
                "/META-INF/MANIFEST.MF";
        try {
            Manifest manifest = new Manifest(new URL(manifestPath).openStream());
            Attributes attributes = manifest.getMainAttributes();

            String result = attributes.getValue(attributeName);
            System.out.println(BUILT_DATE + " " + result);
            return result == null ? "Unknown version" : result;
        } catch (IOException e) {
            e.printStackTrace();
            return "Unknown version";
        }
    }
}
