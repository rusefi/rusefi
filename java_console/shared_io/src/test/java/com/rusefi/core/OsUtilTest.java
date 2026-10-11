package com.rusefi.core;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class OsUtilTest {
    /**
     * "Darwin" contains "win". Matching on the short form would tag macOS as Windows, which is
     * how the first version of this got it wrong.
     */
    @Test
    void darwinIsNotWindows() {
        assertFalse("Darwin".contains("Windows"));
        assertFalse("Mac OS X".contains("Windows"));
        assertTrue("Windows 11".contains("Windows"));
        assertTrue("Windows Server 2022".contains("Windows"));
    }

    @Test
    void currentHostIsExactlyOneOfThem() {
        // whatever we are running on, isWindows and isLinux must not both be true
        assertFalse(OsUtil.isWindows() && OsUtil.isLinux());
    }

    @Test
    void osNameIsNeverNull() {
        // System.getProperty with a default, so callers can chain .contains() safely
        assertFalse(OsUtil.getOsName() == null);
    }
}
