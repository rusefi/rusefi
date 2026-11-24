package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import java.net.URL;
import java.net.URLClassLoader;

import static org.junit.jupiter.api.Assertions.*;

class TopLevelMenuSandboxTest {
    @Test
    void ideaClassesOnlyClasspathFindsSourceFixtures() throws Exception {
        URL classes = TopLevelMenuSandbox.class.getProtectionDomain().getCodeSource().getLocation();
        try (URLClassLoader loader = new URLClassLoader(new URL[]{classes}, null)) {
            assertNull(loader.getResource("26october.ini"));
            assertNull(loader.getResource("26october.msq"));
            for (String resource : new String[]{"/26october.ini", "/26october.msq"}) {
                URL fixture = TopLevelMenuSandbox.findResource(resource, loader, classes);
                try (java.io.InputStream stream = fixture.openStream()) {
                    assertTrue(stream.read() >= 0);
                }
            }
            assertThrows(java.io.IOException.class,
                () -> TopLevelMenuSandbox.findResource("/missing-fixture.ini", loader, classes));
        }
    }
}
