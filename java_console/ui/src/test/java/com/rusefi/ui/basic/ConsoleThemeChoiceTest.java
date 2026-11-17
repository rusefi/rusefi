package com.rusefi.ui.basic;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

/** [tag:console_dark_mode] The saved appearance choice and how it resolves to light or dark. */
public class ConsoleThemeChoiceTest {
    @Test
    public void savedChoiceDefaultsToLightAndToleratesJunk() {
        Node root = new Node();
        assertEquals(ConsoleTheme.Choice.LIGHT, ConsoleTheme.saved(root), "nothing saved: light, as before");
        root.setProperty(ConsoleTheme.KEY, "dark");
        assertEquals(ConsoleTheme.Choice.DARK, ConsoleTheme.saved(root));
        root.setProperty(ConsoleTheme.KEY, "SYSTEM");
        assertEquals(ConsoleTheme.Choice.SYSTEM, ConsoleTheme.saved(root));
        root.setProperty(ConsoleTheme.KEY, "purple");
        assertEquals(ConsoleTheme.Choice.LIGHT, ConsoleTheme.saved(root), "an unknown value falls back to light");
    }

    @Test
    public void followSystemAsksTheOperatingSystemOnlyThen() {
        assertFalse(ConsoleTheme.isDark(ConsoleTheme.Choice.LIGHT, () -> fail("light does not ask")));
        assertTrue(ConsoleTheme.isDark(ConsoleTheme.Choice.DARK, () -> fail("dark does not ask")));
        assertTrue(ConsoleTheme.isDark(ConsoleTheme.Choice.SYSTEM, () -> true));
        assertFalse(ConsoleTheme.isDark(ConsoleTheme.Choice.SYSTEM, () -> false));
    }
}
