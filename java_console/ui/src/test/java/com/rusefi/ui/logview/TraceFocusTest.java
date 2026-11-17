package com.rusefi.ui.logview;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class TraceFocusTest {
    @Test
    void hoverWinsOverThePinWhileItLasts() {
        TraceFocus focus = new TraceFocus();
        assertEquals(TraceFocus.NONE, focus.focused());
        focus.togglePin(3);
        assertEquals(3, focus.focused());
        focus.hover(5);
        assertEquals(5, focus.focused());
        focus.clearHover();
        assertEquals(3, focus.focused(), "back to the pinned trace");
    }

    @Test
    void oneTraceIsPinnedAtATime() {
        TraceFocus focus = new TraceFocus();
        focus.togglePin(3);
        focus.togglePin(4);
        assertEquals(4, focus.pinned());
        focus.togglePin(4);
        assertEquals(TraceFocus.NONE, focus.pinned(), "a second click releases it");
    }

    @Test
    void removedTracesAreForgotten() {
        TraceFocus focus = new TraceFocus();
        focus.togglePin(3);
        focus.hover(5);
        focus.retain(field -> field != 5);
        assertEquals(3, focus.focused());
        focus.retain(field -> false);
        assertEquals(TraceFocus.NONE, focus.focused());
    }

    @Test
    void listenersHearOnlyChanges() {
        TraceFocus focus = new TraceFocus();
        int[] changes = {0};
        focus.addListener(() -> changes[0]++);
        focus.hover(2);
        focus.hover(2);
        focus.clearHover();
        focus.clearHover();
        focus.togglePin(2);
        focus.retain(field -> true);
        assertEquals(3, changes[0]);
    }
}
