package com.rusefi;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

/** The application appearance remains available before an ECU is connected. */
class StartupAppearanceMenuTest {
    @Test
    void startupWindowOffersTheSharedAppearanceChoices() throws Exception {
        AtomicReference<JMenuBar> result = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> result.set(StartupFrame.createStartupMenuBar(null)));

        JMenuBar menuBar = result.get();
        assertEquals(1, menuBar.getMenuCount());
        JMenu fileMenu = menuBar.getMenu(0);
        assertEquals("File", fileMenu.getText());

        JMenu appearanceMenu = (JMenu) fileMenu.getItem(0);
        assertNotNull(appearanceMenu);
        assertEquals("Appearance", appearanceMenu.getText());
        assertEquals("Light", appearanceMenu.getItem(0).getText());
        assertEquals("Dark", appearanceMenu.getItem(1).getText());
        assertEquals("Follow system", appearanceMenu.getItem(2).getText());
    }
}
