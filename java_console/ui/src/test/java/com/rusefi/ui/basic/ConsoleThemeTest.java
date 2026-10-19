package com.rusefi.ui.basic;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

/**
 * [tag:console_theme] The console look is defined in com/rusefi/ui/theme/*.properties;
 * a missing or misnamed file silently falls back to stock FlatLaf.
 */
public class ConsoleThemeTest {
    @Test
    public void sharedAndLightThemeFilesAreApplied() throws Exception {
        LookAndFeel previous = UIManager.getLookAndFeel();
        boolean previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
        boolean previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
        try {
            SwingUtilities.invokeAndWait(UiHelper::configureLaf);

            // shared FlatLaf.properties
            assertEquals(10, UIManager.getInt("Button.arc"));
            assertEquals(new Dimension(45, 30), UIManager.getDimension("TitlePane.buttonSize"));
            assertEquals(40, UIManager.getInt("TabbedPane.tabHeight"));
            // light palette from FlatLightLaf.properties
            assertEquals(new Color(0xffffff), UIManager.getColor("MenuBar.background"));
            assertEquals(new Color(0x2563eb), UIManager.getColor("TabbedPane.underlineColor"));
            assertEquals(new Color(0x1d4ed8), UIManager.getColor("TabbedPane.selectedForeground"));
            assertEquals(new Color(0x586273), UIManager.getColor("TabbedPane.foreground"));
            assertEquals(new Color(0xffffff), UIManager.getColor("Console.TabArea.background"));
            assertEquals(new Color(0x16a34a), UIManager.getColor("Console.Status.connected.dot"));
            // [style] classes are stored as maps, so a broken entry shows up as missing
            assertNotNull(UIManager.get("[style]Button.primary"), "Launch Wizard style class");
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.unregisterThemeForUnitTest();
                JFrame.setDefaultLookAndFeelDecorated(previousFrameDecorated);
                JDialog.setDefaultLookAndFeelDecorated(previousDialogDecorated);
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException e) {
                    throw new AssertionError(e);
                }
            });
        }
    }
}
