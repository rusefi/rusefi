package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import java.net.URL;
import java.net.URLClassLoader;

import static org.junit.jupiter.api.Assertions.assertNull;

class TopLevelMenuSandboxTest {
    @Test
    void ideaClassesOnlyClasspathOmitsFixtures() throws Exception {
        URL classes = TopLevelMenuSandbox.class.getProtectionDomain().getCodeSource().getLocation();
        try (URLClassLoader loader = new URLClassLoader(new URL[]{classes}, null)) {
            assertNull(loader.getResource("26october.ini"));
            assertNull(loader.getResource("26october.msq"));
        }
    }
}
