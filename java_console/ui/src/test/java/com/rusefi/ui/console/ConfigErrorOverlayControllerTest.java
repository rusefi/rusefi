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

    /**
     * Issue #10219 "not fixed": the overlay lives in the frame glass pane, which TabbedPanel also
     * owns. TabbedPanel replaces our overlay with its status glass pane right after we show it, so
     * the overlay is no longer on screen even though we still believe it is. Every later poll must
     * re-assert it - a glass-pane change is not a message change, so without the displacement check
     * the controller stays quiet forever and the user sees nothing.
     */
    @Test
    void reassertsOverlayAfterAnotherGlassPaneReplacesIt() {
        List<String> shown = new ArrayList<>();
        // Model the shared frame glass pane: showing our overlay makes it the current glass pane.
        AtomicReference<Object> glassPane = new AtomicReference<>();
        Object ourOverlay = new Object();
        ConfigErrorOverlayController controller = new ConfigErrorOverlayController(
            text -> {
                shown.add(text);
                glassPane.set(ourOverlay);
            },
            () -> glassPane.set(null),
            () -> glassPane.get() != ourOverlay);
        Object session = new Object();

        controller.update(session, "Bad pin", true);
        assertEquals(Arrays.asList("Bad pin"), shown);
        assertSame(ourOverlay, glassPane.get());

        // A poll while our overlay is still on screen must not re-show it.
        controller.update(session, "Bad pin", true);
        assertEquals(Arrays.asList("Bad pin"), shown);

        // TabbedPanel installs its own glass pane, silently displacing ours.
        Object statusGlassPane = new Object();
        glassPane.set(statusGlassPane);

        // The next identical poll must notice the displacement and re-assert the overlay.
        controller.update(session, "Bad pin", true);
        assertEquals(Arrays.asList("Bad pin", "Bad pin"), shown);
        assertSame(ourOverlay, glassPane.get());

        // Once we own the glass pane again, further polls are quiet.
        controller.update(session, "Bad pin", true);
        assertEquals(Arrays.asList("Bad pin", "Bad pin"), shown);
    }

    /** A displaced overlay while another overlay is up (canShow=false) waits, then shows once free. */
    @Test
    void displacedOverlayStillWaitsForCanShow() {
        List<String> shown = new ArrayList<>();
        AtomicReference<Object> glassPane = new AtomicReference<>();
        Object ourOverlay = new Object();
        ConfigErrorOverlayController controller = new ConfigErrorOverlayController(
            text -> {
                shown.add(text);
                glassPane.set(ourOverlay);
            },
            () -> glassPane.set(null),
            () -> glassPane.get() != ourOverlay);
        Object session = new Object();

        controller.update(session, "Bad pin", true);
        glassPane.set(new Object()); // displaced
        controller.update(session, "Bad pin", false); // blocked by another overlay - do not steal
        assertEquals(Arrays.asList("Bad pin"), shown);
        controller.update(session, "Bad pin", true); // free again - re-assert
        assertEquals(Arrays.asList("Bad pin", "Bad pin"), shown);
    }
}
