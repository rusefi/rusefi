package com.rusefi.ui.widgets;

import javax.swing.JPopupMenu;
import java.awt.Component;
import java.awt.event.MouseEvent;
import java.util.function.Consumer;

/** Shows gauge context menus; one place, so tests can see which mouse events open one. */
public final class GaugePopups {
    private static Consumer<JPopupMenu> shownForUnitTest;

    private GaugePopups() {
    }

    public static void show(JPopupMenu menu, MouseEvent event) {
        show(menu, event.getComponent(), event.getX(), event.getY());
    }

    public static void show(JPopupMenu menu, Component invoker, int x, int y) {
        if (shownForUnitTest != null) {
            // A headless test cannot show a popup; report it instead.
            shownForUnitTest.accept(menu);
            return;
        }
        menu.show(invoker, x, y);
    }

    /** @param listener receives each menu instead of showing it; null restores normal behavior */
    public static void setShownForUnitTest(Consumer<JPopupMenu> listener) {
        shownForUnitTest = listener;
    }
}
