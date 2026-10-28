package com.rusefi.ui.console;

import com.rusefi.ui.basic.ConsoleTheme;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

/** [tag:console_dark_mode] File > Appearance shows the saved choice and saves a new one once. */
class AppearanceMenuTest {
    @Test
    void showsTheSavedChoiceAndSavesAChange() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<ConsoleTheme.Choice> saved = new ArrayList<>();
            AtomicInteger notes = new AtomicInteger();
            JMenu menu = AppearanceMenu.create(ConsoleTheme.Choice.LIGHT, saved::add, notes::incrementAndGet);

            assertEquals(3, menu.getItemCount());
            assertTrue(item(menu, "Light").isSelected());
            assertFalse(item(menu, "Dark").isSelected());

            item(menu, "Light").doClick();
            assertTrue(saved.isEmpty(), "Picking the current appearance saves nothing");

            item(menu, "Dark").doClick();
            assertEquals(ConsoleTheme.Choice.DARK, saved.get(saved.size() - 1));
            assertEquals(1, notes.get(), "The restart note is shown once");
            assertTrue(item(menu, "Dark").isSelected());

            item(menu, "Follow system").doClick();
            assertEquals(ConsoleTheme.Choice.SYSTEM, saved.get(saved.size() - 1));
        });
    }

    private static JMenuItem item(JMenu menu, String text) {
        for (int i = 0; i < menu.getItemCount(); i++) {
            if (text.equals(menu.getItem(i).getText())) {
                return menu.getItem(i);
            }
        }
        throw new AssertionError("No item " + text);
    }
}
