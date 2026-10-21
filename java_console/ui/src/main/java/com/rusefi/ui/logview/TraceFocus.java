package com.rusefi.ui.logview;

import java.util.ArrayList;
import java.util.List;
import java.util.function.IntPredicate;

/**
 * The trace the user is looking at: the one under the pointer in a legend or the Stats table, otherwise
 * the pinned one. The focused trace is drawn on top of faded traces with markers at its visible min and
 * max. One trace is pinned at a time.
 */
final class TraceFocus {
    static final int NONE = -1;

    private final List<Runnable> listeners = new ArrayList<>();
    private int hovered = NONE;
    private int pinned = NONE;

    void addListener(Runnable listener) {
        listeners.add(listener);
    }

    private void changed() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    /** The hovered field, else the pinned one, else {@link #NONE}. */
    int focused() {
        return hovered != NONE ? hovered : pinned;
    }

    int hovered() {
        return hovered;
    }

    int pinned() {
        return pinned;
    }

    void hover(int field) {
        if (hovered != field) {
            hovered = field;
            changed();
        }
    }

    void clearHover() {
        hover(NONE);
    }

    /** Pins the field, or unpins it when it is already pinned. */
    void togglePin(int field) {
        pinned = pinned == field ? NONE : field;
        changed();
    }

    /** Forgets fields that are no longer plotted. */
    void retain(IntPredicate plotted) {
        boolean change = false;
        if (hovered != NONE && !plotted.test(hovered)) {
            hovered = NONE;
            change = true;
        }
        if (pinned != NONE && !plotted.test(pinned)) {
            pinned = NONE;
            change = true;
        }
        if (change) {
            changed();
        }
    }
}
