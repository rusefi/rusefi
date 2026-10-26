package com.rusefi.ui.lua;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.JoinedButtons;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class LuaScriptPanelLayoutTest {
    @Test
    void toolbarAndPanesKeepEveryActionInTheConsoleStyle() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            LuaScriptPanel panel = new LuaScriptPanel(new UIContext(), new Node());
            List<Component> all = UiUtils.getAllComponents(panel.getPanel());

            JButton write = button(all, LuaScriptPanel.WRITE);
            JButton burn = button(all, LuaScriptPanel.BURN);
            JButton more = button(all, LuaScriptPanel.MORE);
            assertTrue(write.getParent() instanceof JoinedButtons);
            assertTrue(more.getParent() instanceof JoinedButtons);
            assertEquals("primary", burn.getClientProperty(FlatClientProperties.STYLE_CLASS), "Burn as in the Tuning tab");
            assertEquals(JoinedButtons.HEIGHT, burn.getPreferredSize().height);
            assertNotNull(write.getToolTipText());
            assertNotNull(burn.getToolTipText());

            assertTrue(labels(all).containsAll(Arrays.asList(LuaScriptPanel.SCRIPT, LuaScriptPanel.OUTPUT)));
            assertFalse(labels(all).contains("Command: "), "The command box shows a placeholder instead");

            JButton clear = button(all, "Clear");
            assertTrue(clear.getParent() instanceof JoinedButtons, "Clear and Pause sit in the Output header");
            assertSame(clear.getParent(), button(all, "Pause").getParent());

            List<String> moreItems = new ArrayList<>();
            for (Component c : panel.createMoreMenu().getComponents()) {
                if (c instanceof JMenuItem) {
                    moreItems.add(((JMenuItem) c).getText());
                }
            }
            assertEquals(Arrays.asList("Format Script", "Reset Lua", "Script name not specified", "Select Working Folder"),
                moreItems);
        });
    }

    @Test
    void statusBarShowsCaretPositionAndSize() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            LuaTextEditor editor = new LuaTextEditor(new UIContext());
            assertEquals("Ln 1, Col 1", editor.getLocationLabel().getText());

            editor.showSize(998, 48000);
            assertEquals("998 / 48000 bytes", editor.getSizeLabel().getText());
            java.awt.Color normal = editor.getSizeLabel().getForeground();
            editor.showSize(47000, 48000);
            assertNotEquals(normal, editor.getSizeLabel().getForeground(), "Near the limit the size stands out");
        });
    }

    private static JButton button(List<Component> all, String text) {
        for (Component c : all) {
            if (c instanceof JButton && text.equalsIgnoreCase(((JButton) c).getText())) {
                return (JButton) c;
            }
        }
        throw new AssertionError("No button " + text);
    }

    private static List<String> labels(List<Component> all) {
        List<String> texts = new ArrayList<>();
        for (Component c : all) {
            if (c instanceof JLabel) {
                texts.add(((JLabel) c).getText());
            }
        }
        return texts;
    }
}
