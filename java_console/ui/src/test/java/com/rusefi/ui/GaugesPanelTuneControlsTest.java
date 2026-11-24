package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.tune.TuningToolbarWidget;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.ui.GaugesPanelLayoutTest.createContext;
import static org.junit.jupiter.api.Assertions.*;

/** [tag:tune_controls_position] Burn / Discard / Undo / Redo keep the Tuning tab's place on Gauges. */
class GaugesPanelTuneControlsTest {
    @Test
    void burnSitsWhereTheTuningTabHasItAndNeverMoves() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), new Node());
            try {
                JComponent content = panel.getContent();
                Point placeholder = burnLocation(content);
                assertEquals(new Point(TuningToolbarWidget.CONTROLS_LEFT, TuningToolbarWidget.CONTROLS_TOP),
                    placeholder);
                assertFalse(findButton(content, "Burn to ECU").isEnabled(), "No tune to burn yet");

                panel.setTuneControls(TuningToolbarWidget.createPlaceholderControls());
                assertEquals(placeholder, burnLocation(content), "Live controls replace the stand-ins in place");

                panel.setFreeMode(true);
                panel.getFreeArea().setEditing(true);
                assertEquals(placeholder, burnLocation(content), "Arranging gauges does not move them");
            } finally {
                panel.destroy();
            }
        });
    }

    private static Point burnLocation(JComponent content) {
        content.setSize(1900, 900);
        layOutTree(content);
        JButton burn = findButton(content, "Burn to ECU");
        assertNotNull(burn);
        assertTrue(burn.isVisible());
        return SwingUtilities.convertPoint(burn, 0, 0, content);
    }

    private static void layOutTree(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) {
                layOutTree((Container) child);
            }
        }
    }

    private static JButton findButton(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
