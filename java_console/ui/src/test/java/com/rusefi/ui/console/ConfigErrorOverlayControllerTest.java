package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.awt.Color;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

class ConfigErrorOverlayControllerTest {
    @Test
    void displaysEcuTextAndCloseDoesNotReopenOnEveryPoll() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<MainFrame.FrameOverlay> visible = new AtomicReference<>();
            ConfigErrorOverlayController controller = new ConfigErrorOverlayController(
                text -> visible.set(new MainFrame.FrameOverlay(text, Color.RED,
                    new MainFrame.OverlayAction("Close", KeyEvent.VK_C, () -> visible.set(null)))),
                () -> visible.set(null));
            Object session = new Object();
            controller.update(session, "Pin PA1 is already used\nSelect another pin", true);
            assertEquals("Pin PA1 is already used\nSelect another pin", visible.get().getMessageForUnitTest());
            visible.get().actionForUnitTest(0);
            controller.update(session, "Pin PA1 is already used\nSelect another pin", true);
            assertNull(visible.get());
            controller.update(session, "Another error", true);
            assertEquals("Another error", visible.get().getMessageForUnitTest());
            controller.update(session, null, true);
            assertNull(visible.get());
        });
    }

    @Test
    void recurrenceAndNewConnectionsShowTheSameMessageAgain() {
        List<String> shown = new ArrayList<>();
        ConfigErrorOverlayController controller = new ConfigErrorOverlayController(shown::add, () -> {});
        Object session = new Object();
        controller.update(session, "Bad pin", true);
        controller.update(session, "Bad pin", true);
        controller.update(session, null, true);
        controller.update(session, "Bad pin", true);
        controller.update(null, null, false);
        controller.update(new Object(), "Bad pin", true);
        assertEquals(Arrays.asList("Bad pin", "Bad pin", "Bad pin"), shown);
    }

    @Test
    void waitsForOtherOverlaysAndDropsPendingErrorsOnDisconnect() {
        List<String> shown = new ArrayList<>();
        ConfigErrorOverlayController controller = new ConfigErrorOverlayController(shown::add, () -> {});
        Object session = new Object();
        controller.update(session, "Bad pin", false);
        assertTrue(shown.isEmpty());
        controller.update(session, "Bad pin", true);
        assertEquals(Arrays.asList("Bad pin"), shown);
        controller.update(session, "Pending error", false);
        controller.update(null, null, true);
        assertEquals(Arrays.asList("Bad pin"), shown);
    }
}
