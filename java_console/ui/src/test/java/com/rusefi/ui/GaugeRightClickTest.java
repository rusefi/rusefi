package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.GaugePopups;
import com.rusefi.ui.widgets.JPanelWithListener;
import com.rusefi.ui.widgets.SensorGauge;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Gauge and live graph menus used to open from mouseClicked, which AWT sends only when the button
 * is released on the exact pixel where it was pressed. A right-click that moved by one pixel,
 * common with a normal hand or a touchpad, opened no menu, so right-click "sometimes" failed.
 * They now follow the platform popup trigger: on release (Windows) or on press (macOS, Linux).
 */
class GaugeRightClickTest {
    @Test
    void gaugeMenuOpensEvenWhenTheMouseMovesDuringTheClick() throws Throwable {
        List<JPopupMenu> shown = new ArrayList<>();
        GaugePopups.setShownForUnitTest(shown::add);
        try {
            SwingUtilities.invokeAndWait(() -> {
                JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
                SensorGauge.createGaugeBody(GaugesPanelLayoutTest.createContext(), "RPMGauge", wrapper, name -> { }, null);
                try {
                    Component gauge = wrapper.getComponent(0);
                    assertOneMenuPerRightClick(gauge, shown);
                } finally {
                    wrapper.removeAllChildrenAndListeners();
                }
            });
        } finally {
            GaugePopups.setShownForUnitTest(null);
        }
    }

    @Test
    void liveGraphMenuOpensEvenWhenTheMouseMovesDuringTheClick() throws Throwable {
        List<JPopupMenu> shown = new ArrayList<>();
        GaugePopups.setShownForUnitTest(shown::add);
        try {
            SwingUtilities.invokeAndWait(() -> {
                SensorLiveGraph graph = new SensorLiveGraph(GaugesPanelLayoutTest.createContext(), new Node(), "RPMGauge", null);
                try {
                    graph.setActive(false);
                    assertOneMenuPerRightClick(graph, shown);
                } finally {
                    graph.destroy();
                }
            });
        } finally {
            GaugePopups.setShownForUnitTest(null);
        }
    }

    private static void assertOneMenuPerRightClick(Component target, List<JPopupMenu> shown) {
        steadyRightClick(target);
        assertEquals(1, shown.size(), "A steady right-click opens the menu once, not again on mouseClicked");
        wobblyRightClick(target);
        assertEquals(2, shown.size(), "A right-click that moves 1 px still opens the menu");
        pressTriggeredRightClick(target);
        assertEquals(3, shown.size(), "Platforms that trigger on press open it too, once");
    }

    /** What AWT sends for a right-click released on the pressed pixel (popup trigger on release, as on Windows). */
    private static void steadyRightClick(Component target) {
        send(target, MouseEvent.MOUSE_PRESSED, 50, 50, InputEvent.BUTTON3_DOWN_MASK, false);
        send(target, MouseEvent.MOUSE_RELEASED, 50, 50, 0, true);
        send(target, MouseEvent.MOUSE_CLICKED, 50, 50, 0, false);
    }

    /** Released one pixel away: AWT sends no MOUSE_CLICKED. */
    private static void wobblyRightClick(Component target) {
        send(target, MouseEvent.MOUSE_PRESSED, 50, 50, InputEvent.BUTTON3_DOWN_MASK, false);
        send(target, MouseEvent.MOUSE_RELEASED, 51, 50, 0, true);
    }

    /** macOS and Linux report the popup trigger on press; moved here too, so no MOUSE_CLICKED. */
    private static void pressTriggeredRightClick(Component target) {
        send(target, MouseEvent.MOUSE_PRESSED, 50, 50, InputEvent.BUTTON3_DOWN_MASK, true);
        send(target, MouseEvent.MOUSE_RELEASED, 52, 51, 0, false);
    }

    private static void send(Component target, int id, int x, int y, int modifiers, boolean popupTrigger) {
        target.dispatchEvent(new MouseEvent(target, id, System.currentTimeMillis(), modifiers, x, y, 1,
            popupTrigger, MouseEvent.BUTTON3));
    }
}
