package com.rusefi.ui.engine;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/** Space pauses and resumes the Engine Sniffer, a one-handed key while cranking. */
class EngineSnifferSpacePauseTest {
    private static final KeyStroke SPACE = KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, 0);

    @Test
    void spaceTogglesPauseAndFocusedButtonsDoNotTakeIt() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            EngineSnifferPanel panel = new EngineSnifferPanel(new UIContext(), new Node());
            JPanel content = panel.getPanel();
            List<Component> all = UiUtils.getAllComponents(content);
            JButton pause = button(all, "pause");

            Object key = content.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).get(SPACE);
            assertEquals(EngineSnifferPanel.PAUSE_ACTION, key, "Space works anywhere on the tab");
            Action action = content.getActionMap().get(key);
            assertTrue(action.isEnabled(), "no text field has focus");

            action.actionPerformed(new ActionEvent(content, ActionEvent.ACTION_PERFORMED, "space"));
            assertEquals("resume", pause.getText().toLowerCase());
            action.actionPerformed(new ActionEvent(content, ActionEvent.ACTION_PERFORMED, "space"));
            assertEquals("pause", pause.getText().toLowerCase());

            for (Component c : all) {
                if (c instanceof AbstractButton) {
                    assertEquals("none", ((AbstractButton) c).getInputMap(JComponent.WHEN_FOCUSED).get(SPACE),
                        "a focused " + ((AbstractButton) c).getText() + " button leaves Space to pause");
                }
            }
            assertTrue(pause.getToolTipText().contains("Space"));
        });
    }

    private static JButton button(List<Component> all, String text) {
        return (JButton) all.stream()
            .filter(c -> c instanceof JButton && text.equalsIgnoreCase(((JButton) c).getText()))
            .findFirst().orElseThrow(() -> new AssertionError("No button " + text));
    }
}
