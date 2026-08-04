package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;

import java.io.File;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * The launch-time updater must detect that it is running from the console JAR (some white-label
 * exe launchers are configured that way) and take the staged pending-JAR update path instead of
 * overwriting the running JAR in place, see #10000.
 * <p>
 * The console JAR name is white-label aware ({@link Autoupdate#consoleJarFileName()}), so tests
 * compare against the resolved name rather than assuming {@code rusefi_console.jar}.
 */
class ConsoleJarDetectionTest {
    private final String consoleJar = Autoupdate.consoleJarFileName();

    @Test
    void nullMeansClassDirectoryNotConsoleJar() {
        assertFalse(Autoupdate.isConsoleJarFileName(null));
    }

    @Test
    void consoleJarIsDetectedRegardlessOfDirectory() {
        assertTrue(Autoupdate.isConsoleJarFileName(consoleJar));
        assertTrue(Autoupdate.isConsoleJarFileName(
            new File(new File("Program Files", "console"), consoleJar).getAbsolutePath()));
    }

    @Test
    void pendingConsoleJarCountsAsConsoleJar() {
        assertTrue(Autoupdate.isConsoleJarFileName(
            new File("console", Autoupdate.PENDING_CONSOLE_JAR).getPath()));
    }

    @Test
    void updaterJarIsNotConsoleJar() {
        assertFalse(Autoupdate.isConsoleJarFileName(new File("console", "rusefi_autoupdate.jar").getPath()));
        assertFalse(Autoupdate.isConsoleJarFileName(new File("console", "some_other.jar").getPath()));
    }
}
