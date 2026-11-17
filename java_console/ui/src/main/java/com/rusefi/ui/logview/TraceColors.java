package com.rusefi.ui.logview;

import java.util.Arrays;

/**
 * Which fields are plotted and which palette slot each one uses. A field keeps its slot until it is
 * removed, so plotting or removing another field never recolours the existing traces.
 */
final class TraceColors {
    private static final int NONE = -1;
    private final int[] slots;

    TraceColors(int fieldCount) {
        slots = new int[fieldCount];
        Arrays.fill(slots, NONE);
    }

    /** Takes every field's slot from {@code other}, which has the same fields. */
    void copyFrom(TraceColors other) {
        System.arraycopy(other.slots, 0, slots, 0, slots.length);
    }

    int fieldCount() {
        return slots.length;
    }

    boolean isPlotted(int field) {
        return slots[field] != NONE;
    }

    /** Palette slot of a plotted field, or -1. */
    int slot(int field) {
        return slots[field];
    }

    /** Gives the field the least-used slot, the lowest one on ties, so the first traces get distinct colours. */
    void plot(int field) {
        if (isPlotted(field)) {
            return;
        }
        int[] uses = new int[PlotPalette.TRACE_COUNT];
        for (int slot : slots) {
            if (slot != NONE) {
                uses[slot]++;
            }
        }
        int best = 0;
        for (int slot = 1; slot < uses.length; slot++) {
            if (uses[slot] < uses[best]) {
                best = slot;
            }
        }
        slots[field] = best;
    }

    void remove(int field) {
        slots[field] = NONE;
    }

    void toggle(int field) {
        if (isPlotted(field)) {
            remove(field);
        } else {
            plot(field);
        }
    }

    int plottedCount() {
        int count = 0;
        for (int slot : slots) {
            if (slot != NONE) {
                count++;
            }
        }
        return count;
    }
}
