package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.tune.SettingHelpCard;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

public class HelpCardsChoiceTest {
    @AfterEach
    public void restoreDefault() {
        SettingHelpCard.setAutoShow(true);
    }

    @Test
    public void choiceStartsFromTheSavedSettingAndSavesChanges() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            config.setBoolProperty(TuningPane.HELP_CARDS, false);
            JComponent row = TuningPane.withHelpCardsChoice(new JPanel(), config);
            JCheckBox choice = (JCheckBox) find(row, "helpCardsChoice");
            assertNotNull(choice);
            assertFalse(choice.isSelected());
            assertFalse(SettingHelpCard.isAutoShow(), "the saved choice applies at once");

            choice.doClick(0);
            assertTrue(SettingHelpCard.isAutoShow());
            assertTrue(config.getBoolProperty(TuningPane.HELP_CARDS, false));
        });
    }

    @Test
    public void onByDefaultAndNoChoiceWithoutSettings() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JCheckBox choice = (JCheckBox) find(TuningPane.withHelpCardsChoice(new JPanel(), new Node()), "helpCardsChoice");
            assertTrue(choice.isSelected());

            JPanel toolbar = new JPanel();
            assertSame(toolbar, TuningPane.withHelpCardsChoice(toolbar, null));
        });
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
