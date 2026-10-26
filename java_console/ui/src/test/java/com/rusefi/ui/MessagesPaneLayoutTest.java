package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.CommandBox;
import com.rusefi.ui.widgets.JoinedButtons;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class MessagesPaneLayoutTest {
    @Test
    void toolbarQuickCommandsAndCommandBoxFollowTheConsoleStyle() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            MessagesPane pane = new MessagesPane(GaugesPanelLayoutTest.createContext(), new Node());
            List<Component> all = UiUtils.getAllComponents(pane.getContent());

            JTextArea area = (JTextArea) all.stream().filter(c -> c instanceof JTextArea).findFirst().orElseThrow(AssertionError::new);
            assertTrue(area.getParent() instanceof CommandBox, "The multi-line command input sits in the shared command box");
            CommandBox box = (CommandBox) area.getParent();
            assertEquals(JoinedButtons.HEIGHT, box.getPreferredSize().height, "One line: as tall as the other controls");
            area.setText("help\nsensorinfo\npins");
            assertTrue(box.getPreferredSize().height > JoinedButtons.HEIGHT, "More lines grow the box");

            assertTrue(button(all, "clear").getParent() instanceof JoinedButtons);
            assertSame(button(all, "clear").getParent(), button(all, "pause").getParent());
            assertTrue(labelTexts(all).contains(RecentCommands.TITLE));
            assertTrue(button(all, RecentCommands.RESET).getParent() instanceof JoinedButtons);
        });
    }

    private static JButton button(List<Component> all, String text) {
        return (JButton) all.stream()
            .filter(c -> c instanceof JButton && text.equalsIgnoreCase(((JButton) c).getText()))
            .findFirst().orElseThrow(() -> new AssertionError("No button " + text));
    }

    private static List<String> labelTexts(List<Component> all) {
        List<String> texts = new java.util.ArrayList<>();
        for (Component c : all) {
            if (c instanceof JLabel) {
                texts.add(((JLabel) c).getText());
            }
        }
        return texts;
    }
}
