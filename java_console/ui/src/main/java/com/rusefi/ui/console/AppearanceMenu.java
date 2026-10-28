package com.rusefi.ui.console;

import com.rusefi.ui.basic.ConsoleTheme;

import javax.swing.*;
import java.awt.*;
import java.util.function.Consumer;

/**
 * File > Appearance: Light, Dark or Follow system. The choice is saved and applied on the next start, because
 * the console builds its colours once [tag:console_dark_mode].
 */
final class AppearanceMenu {
    static final String TITLE = "Appearance";
    static final String RESTART_NOTE = "The new appearance applies the next time the console starts.";

    private AppearanceMenu() {
    }

    static JMenu create(Component parent) {
        return create(ConsoleTheme.saved(), ConsoleTheme::save,
            () -> JOptionPane.showMessageDialog(parent, RESTART_NOTE, TITLE, JOptionPane.INFORMATION_MESSAGE));
    }

    /** Test seam: the current choice, where a new one is saved, and how the restart note is shown. */
    static JMenu create(ConsoleTheme.Choice current, Consumer<ConsoleTheme.Choice> save, Runnable restartNote) {
        JMenu menu = new JMenu(TITLE);
        menu.setToolTipText("Light or dark console colours; applies on the next start");
        ButtonGroup group = new ButtonGroup();
        ConsoleTheme.Choice[] selected = {current};
        for (ConsoleTheme.Choice choice : ConsoleTheme.Choice.values()) {
            JRadioButtonMenuItem item = new JRadioButtonMenuItem(choice.label, choice == current);
            if (choice == ConsoleTheme.Choice.SYSTEM) {
                item.setToolTipText("Light or dark like the operating system when the console starts");
            }
            item.addActionListener(e -> {
                if (choice != selected[0]) {
                    selected[0] = choice;
                    save.accept(choice);
                    restartNote.run();
                }
            });
            group.add(item);
            menu.add(item);
        }
        return menu;
    }
}
