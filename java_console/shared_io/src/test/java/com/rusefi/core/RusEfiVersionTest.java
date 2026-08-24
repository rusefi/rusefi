package com.rusefi.core;

import org.junit.jupiter.api.Test;

import java.io.File;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * #6836: an installation path containing a space made the console log
 * "Compiled Wed Dec 31 19:00:00 EST 1969" - the percent-encoded jar URL was turned into a file
 * name that does not exist, so lastModified() returned 0.
 */
class RusEfiVersionTest {
    private static final String CLASS_SUFFIX = "!/com/rusefi/core/rusEFIVersion.class";

    @Test
    void percentEncodedPathIsDecoded() {
        File jar = rusEFIVersion.jarFileOf(
            "file:/C:/Program%20Files/Purple%20Updater/console/rusefi_console.jar" + CLASS_SUFFIX
        );

        assertNotNull(jar);
        assertTrue(jar.getPath().contains("Program Files"), jar.getPath());
        assertTrue(jar.getPath().contains("Purple Updater"), jar.getPath());
        assertEquals("rusefi_console.jar", jar.getName());
    }

    @Test
    void plainPathIsUnchanged() {
        File jar = rusEFIVersion.jarFileOf("file:/opt/rusefi/console/rusefi_console.jar" + CLASS_SUFFIX);

        assertNotNull(jar);
        assertEquals("rusefi_console.jar", jar.getName());
        assertTrue(jar.getPath().contains("rusefi"), jar.getPath());
    }

    @Test
    void nameIsNeverLeftPercentEncoded() {
        File jar = rusEFIVersion.jarFileOf("file:/tmp/my%20bundle/rusefi%20console.jar" + CLASS_SUFFIX);

        assertNotNull(jar);
        assertEquals("rusefi console.jar", jar.getName());
    }

    @Test
    void pathWithoutSeparatorIsRejected() {
        assertNull(rusEFIVersion.jarFileOf("file:/opt/rusefi/console/rusefi_console.jar"));
    }

    @Test
    void malformedUrlIsRejected() {
        assertNull(rusEFIVersion.jarFileOf("not a url" + CLASS_SUFFIX));
    }

    @Test
    void relativeUrlIsRejectedRatherThanThrowing() {
        // new File(URI) demands an absolute file: URI - anything else must come back as null
        assertNull(rusEFIVersion.jarFileOf("console/rusefi_console.jar" + CLASS_SUFFIX));
    }

    @Test
    void unknownBuildTimeIsNotRenderedAsTheEpoch() {
        // the class under test is loaded from a directory or a jar that really exists, so this only
        // pins the contract that we never print a 1970-ish date
        String buildTime = rusEFIVersion.classBuildTimeString();

        assertNotNull(buildTime);
        assertTrue(
            !buildTime.contains("1969") && !buildTime.contains("1970"),
            "unexpected epoch build time: " + buildTime
        );
    }
}
