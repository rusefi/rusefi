package com.rusefi.core.net;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class JarFileUtilTest {
    @Test
    void resolvesCanonicalAndPendingConsolePrefixes() {
        assertEquals("rusefi", JarFileUtil.getJarFileNamePrefix("rusefi_console.jar"));
        assertEquals("rusefi", JarFileUtil.getJarFileNamePrefix("rusefi_console_pending.jar"));
        assertEquals("custom_label", JarFileUtil.getJarFileNamePrefix("custom_label_console.jar"));
        assertEquals("custom_label", JarFileUtil.getJarFileNamePrefix("custom_label_console_pending.jar"));
    }
}
