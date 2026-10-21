package com.rusefi.ui.basic;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.ui.FlatStylingSupport;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

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
            assertEquals(new Color(0xeef0f3), UIManager.getColor("Console.Readout.background"));
            assertEquals(new Color(0xffffff), UIManager.getColor("Console.Segmented.selectedBackground"));
            // log viewer plot palette, including a #rrggbbaa colour
            assertEquals(new Color(0x2563eb), UIManager.getColor("Console.LogChart.light.trace1"));
            assertEquals(new Color(0x33, 0x41, 0x55, 0x99), UIManager.getColor("Console.LogChart.light.cursor"));
            // [style] classes are stored as maps, so a broken entry shows up as missing
            assertNotNull(UIManager.get("[style]Button.primary"), "Launch Wizard style class");
            // a clicked button keeps focus: the primary style must stay blue with white text then,
            // and FlatLaf rejects unknown style keys when the style is applied
            JButton primary = new JButton("Open log");
            primary.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
            primary.updateUI();
            FlatStylingSupport.StyleableUI ui = (FlatStylingSupport.StyleableUI) primary.getUI();
            for (String state : new String[]{"focused", "default.focused"}) {
                assertEquals(new Color(0x2563eb), ui.getStyleableValue(primary, state + "Background"), state);
                assertEquals(Color.WHITE, ui.getStyleableValue(primary, state + "Foreground"), state);
            }
            // view toggles in toolbars (Log Viewer Stats) turn a light accent tint when on, not grey
            JToggleButton viewToggle = new JToggleButton("Stats");
            viewToggle.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
            viewToggle.putClientProperty(FlatClientProperties.STYLE_CLASS, "viewToggle");
            viewToggle.updateUI();
            FlatStylingSupport.StyleableUI toggleUi = (FlatStylingSupport.StyleableUI) viewToggle.getUI();
            Color selected = (Color) toggleUi.getStyleableValue(viewToggle, "toolbar.selectedBackground");
            assertTrue(selected.getBlue() > selected.getRed() && selected.getRed() > 200, "light blue tint: " + selected);
            assertEquals(new Color(0x1d4ed8), toggleUi.getStyleableValue(viewToggle, "toolbar.selectedForeground"));
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
