package com.rusefi.core;

import com.rusefi.UiVersion;

import java.io.File;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Date;
import java.util.concurrent.atomic.AtomicReference;

public interface rusEFIVersion extends UiVersion {
    AtomicReference<String> firmwareVersion = new AtomicReference<>("N/A");

    String UNKNOWN_BUILD_TIME = "unknown";

    static long classBuildTimeMillis() {
        Class<?> clazz = rusEFIVersion.class;
        return classBuildTimeMillis(clazz);
    }

    /**
     * Human readable build timestamp for logs.
     * <p>
     * Never renders {@code new Date(0)} - printing "Wed Dec 31 19:00:00 EST 1969" told the reader
     * that the build time is unknown in the least obvious way possible. See #6836.
     */
    static String classBuildTimeString(Class<?> clazz) {
        long millis = classBuildTimeMillis(clazz);
        return millis == 0 ? UNKNOWN_BUILD_TIME : new Date(millis).toString();
    }

    static String classBuildTimeString() {
        return classBuildTimeString(rusEFIVersion.class);
    }

    static long classBuildTimeMillis(Class<?> clazz) {
        URL resource = clazz.getResource(clazz.getSimpleName() + ".class");
        if (resource == null) {
            throw new IllegalStateException("Failed to find class file for class: " +
                    clazz.getName());
        }

        if (resource.getProtocol().equals("file")) {
            try {
                return new File(resource.toURI()).lastModified();
            } catch (URISyntaxException e) {
                return 0;
            }
        } else if (resource.getProtocol().equals("jar")) {
            File jarFile = jarFileOf(resource.getPath());
            return jarFile == null ? 0 : jarFile.lastModified();
        } else {
            throw new IllegalArgumentException("Unhandled url protocol: " +
                    resource.getProtocol() + " for class: " +
                    clazz.getName() + " resource: " + resource);
        }
    }

    /**
     * Extracts the jar file out of the path of a {@code jar:} URL, for example
     * {@code file:/C:/Program%20Files/rusefi/console/rusefi_console.jar!/com/rusefi/core/rusEFIVersion.class}.
     * <p>
     * That path is percent-encoded, so it has to be parsed as a URI rather than by chopping the
     * "file:" prefix off with a substring: a bundle installed under "Program Files" used to yield a
     * literal {@code %20} in the file name, a file which does not exist, and therefore
     * {@code lastModified() == 0}. See #6836.
     *
     * @return the jar file, or null when the path cannot be understood
     */
    static File jarFileOf(String jarUrlPath) {
        int separator = jarUrlPath.indexOf('!');
        if (separator < 0) {
            return null;
        }
        try {
            return new File(new URI(jarUrlPath.substring(0, separator)));
        } catch (URISyntaxException | IllegalArgumentException e) {
            return null;
        }
    }
}
