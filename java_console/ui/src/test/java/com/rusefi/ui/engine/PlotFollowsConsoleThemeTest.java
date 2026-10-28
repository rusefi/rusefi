package com.rusefi.ui.engine;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.UiHelper;
import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

/**
 * [tag:console_dark_mode] Plots start in the console theme's brightness, and a Light/Dark choice made in one
 * console theme is remembered for that theme only (UiHelper.plotThemeKey).
 */
class PlotFollowsConsoleThemeTest {
    @Test
    void snifferPlotFollowsTheConsoleThemeAndKeepsAChoicePerTheme() throws Exception {
        Node config = new Node();
        LookAndFeel previous = UIManager.getLookAndFeel();
        boolean previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
        boolean previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
        try {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.configureLaf(true);
                EngineSnifferPanel dark = new EngineSnifferPanel(new UIContext(), config);
                assertTrue(dark.isDarkPlotForTests(), "A dark console starts with a dark plot");
                toggle(dark, EngineSnifferPanel.PLOT_LIGHT).doClick();
                assertFalse(config.getBoolProperty(UiHelper.plotThemeKey(EngineSnifferPanel.DARK_PLOT_KEY), true),
                    "The light choice is saved for the dark console theme");
                assertFalse(config.getConfig().containsKey(EngineSnifferPanel.DARK_PLOT_KEY),
                    "The light console theme's choice is left alone");

                UiHelper.unregisterThemeForUnitTest();
                UiHelper.configureLaf(false);
                EngineSnifferPanel light = new EngineSnifferPanel(new UIContext(), config);
                assertFalse(light.isDarkPlotForTests(), "A light console still starts with a light plot");
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

    private static JToggleButton toggle(EngineSnifferPanel panel, String text) {
        for (Component c : UiUtils.getAllComponents(panel.getPanel())) {
            if (c instanceof JToggleButton && text.equals(((JToggleButton) c).getText())) {
                return (JToggleButton) c;
            }
        }
        throw new AssertionError("No toggle " + text);
    }
}
