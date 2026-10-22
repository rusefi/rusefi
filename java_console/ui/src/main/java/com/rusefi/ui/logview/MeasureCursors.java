package com.rusefi.ui.logview;

import java.util.ArrayList;
import java.util.List;

/**
 * The A and B measurement cursors of the log chart, as times in seconds. With both set the chart shows the
 * difference between them and computes its statistics between A and B instead of over the visible time.
 */
final class MeasureCursors {
    enum Which {
        A, B
    }

    private double a = Double.NaN;
    private double b = Double.NaN;
    private final List<Runnable> listeners = new ArrayList<>();

    void addListener(Runnable listener) {
        listeners.add(listener);
    }

    private void changed() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    boolean has(Which which) {
        return !Double.isNaN(time(which));
    }

    boolean isEmpty() {
        return !has(Which.A) && !has(Which.B);
    }

    /** Both cursors are set: the chart measures between them. */
    boolean isRange() {
        return has(Which.A) && has(Which.B);
    }

    /** A cursor's time, or NaN when it is not set. */
    double time(Which which) {
        return which == Which.A ? a : b;
    }

    /** The earlier of A and B, with both set. */
    double start() {
        return Math.min(a, b);
    }

    /** The later of A and B, with both set. */
    double end() {
        return Math.max(a, b);
    }

    void set(Which which, double time) {
        if (time == time(which)) {
            return;
        }
        if (which == Which.A) {
            a = time;
        } else {
            b = time;
        }
        changed();
    }

    /**
     * A measuring click: places A, then B; with both set it moves the one nearer to the click.
     *
     * @param distanceToA how far the click is from A, in any unit, and likewise for B
     */
    Which place(double time, double distanceToA, double distanceToB) {
        Which which = !has(Which.A) ? Which.A : !has(Which.B) ? Which.B
                : distanceToA <= distanceToB ? Which.A : Which.B;
        set(which, time);
        return which;
    }

    void clear() {
        if (isEmpty()) {
            return;
        }
        a = b = Double.NaN;
        changed();
    }
}
