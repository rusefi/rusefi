package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.tune.SettingHelpCard;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

public class HelpModeChoiceTest {
    @AfterEach
    public void restoreDefault() {
        SettingHelpCard.setMode(SettingHelpCard.Mode.CARDS);
    }

    @Test
    public void cardsByDefaultAndNoChoiceWithoutSettings() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JComponent row = TuningPane.withHelpChoices(new JPanel(), new Node(), () -> { });
            assertTrue(segment(row, "Cards").isSelected());
            assertFalse(segment(row, "Descriptions").isSelected());
            assertFalse(segment(row, "Off").isSelected());
            assertEquals(SettingHelpCard.Mode.CARDS, SettingHelpCard.getMode());

            JPanel toolbar = new JPanel();
            assertSame(toolbar, TuningPane.withHelpChoices(toolbar, null, () -> { }));
        });
    }

    @Test
    public void savedChoiceAppliesAndAnUnknownOneFallsBackToCards() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            config.setProperty(TuningPane.HELP_MODE, "DESCRIPTIONS");
            JComponent row = TuningPane.withHelpChoices(new JPanel(), config, () -> { });
            assertTrue(segment(row, "Descriptions").isSelected());
            assertEquals(SettingHelpCard.Mode.DESCRIPTIONS, SettingHelpCard.getMode());

            config.setProperty(TuningPane.HELP_MODE, "something else");
            TuningPane.withHelpChoices(new JPanel(), config, () -> { });
            assertEquals(SettingHelpCard.Mode.CARDS, SettingHelpCard.getMode());
        });
    }

    /** One way at a time; dialogs are redrawn only when descriptions turn on or off. */
    @Test
    public void choosingAModeSavesItAndRedrawsOnlyForDescriptions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            int[] rebuilds = new int[1];
            JComponent row = TuningPane.withHelpChoices(new JPanel(), config, () -> rebuilds[0]++);

            segment(row, "Descriptions").doClick(0);
            assertEquals(SettingHelpCard.Mode.DESCRIPTIONS, SettingHelpCard.getMode());
            assertEquals("DESCRIPTIONS", config.getProperty(TuningPane.HELP_MODE, null));
            assertFalse(segment(row, "Cards").isSelected(), "cards and descriptions never both");
            assertEquals(1, rebuilds[0]);

            segment(row, "Off").doClick(0);
            assertEquals(SettingHelpCard.Mode.OFF, SettingHelpCard.getMode());
            assertEquals(2, rebuilds[0], "descriptions removed");

            segment(row, "Cards").doClick(0);
            assertEquals(SettingHelpCard.Mode.CARDS, SettingHelpCard.getMode());
            assertEquals("CARDS", config.getProperty(TuningPane.HELP_MODE, null));
            assertEquals(2, rebuilds[0], "no descriptions involved, nothing to redraw");
        });
    }

    private static JToggleButton segment(Container container, String text) {
        JToggleButton found = (JToggleButton) find(container, "helpMode" + text);
        assertNotNull(found, text);
        return found;
    }

    private static Component find(Container container, String name) {
        for (Component component : container.getComponents()) {
            if (name.equals(component.getName())) {
                return component;
            }
            if (component instanceof Container) {
                Component nested = find((Container) component, name);
                if (nested != null) {
                    return nested;
                }
            }
        }
        return null;
    }
}
