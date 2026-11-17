package com.rusefi.ui.logview;

import com.rusefi.ui.basic.UiHelper;
import org.junit.jupiter.api.Test;

import javax.swing.*;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** [tag:console_dark_mode] A new Log Viewer chart starts with the console theme's plot brightness. */
class ChartSettingsThemeTest {
    @Test
    void newChartsStartInTheConsoleThemesBrightness() throws Exception {
        LookAndFeel previous = UIManager.getLookAndFeel();
        boolean previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
        boolean previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
        try {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.configureLaf(true);
                assertTrue(new ChartSettings().isDarkPlot(), "dark console theme");
                UiHelper.unregisterThemeForUnitTest();
                UiHelper.configureLaf(false);
                assertFalse(new ChartSettings().isDarkPlot(), "light console theme");
            });
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
